#include "Arduino.h"
#include "Mapi.h"
#include <DHT11.h>
#include <tuple>
// Target UNO R4 WiFi: include the WiFiS3 header directly
#include <WiFiS3.h>
#include <ArduinoMqttClient.h>

using std::tuple;

Mapi::Mapi(int lightSensorPin, int tempHumSensorPin, int carbonMonoxideSensorPin)
{
    _lightSensorPin = lightSensorPin;
    _tempHumiditySensorPin = tempHumSensorPin;
    _carbonMonoxideSensorPin = carbonMonoxideSensorPin;
    _dht = nullptr;
    _wifiClient = nullptr;
    _mqttClient = nullptr;
    _mqttPort = 1883;
    _peopleCountSerial = nullptr;
    _lastPeopleCount = -1;
}

// -------------------- WiFi helper implementations --------------------

int Mapi::wifiBegin(const char *ssid, const char *pass, unsigned long timeoutMs)
{
    // Start connection
    int sts = WiFi.begin(ssid, pass);

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        if (millis() - start > timeoutMs)
        {
            break;
        }
        Serial.println(WiFi.status());
        delay(200);
    }
    return WiFi.status();
}

int Mapi::wifiStatus()
{
    return WiFi.status();
}

IPAddress Mapi::wifiLocalIP()
{
    return WiFi.localIP();
}

void Mapi::wifiPrintInfo()
{
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // BSSID / router MAC
    byte bssid[6];
    WiFi.BSSID(bssid);
    Serial.print("BSSID: ");
    for (int i = 0; i < 6; i++)
    {
        if (i)
            Serial.print(":");
        if (bssid[i] < 16)
            Serial.print("0");
        Serial.print(bssid[i], HEX);
    }
    Serial.println();

    Serial.print("signal strength (RSSI):");
    Serial.println(WiFi.RSSI());

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    byte mac[6];
    WiFi.macAddress(mac);
    Serial.print("MAC address: ");
    for (int i = 0; i < 6; i++)
    {
        if (i)
            Serial.print(":");
        if (mac[i] < 16)
            Serial.print("0");
        Serial.print(mac[i], HEX);
    }
    Serial.println();
}

void Mapi::wifiMacAddress(byte mac[])
{
    WiFi.macAddress(mac);
}

Mapi::~Mapi()
{
    if (_dht)
    {
        delete _dht;
        _dht = nullptr;
    }
    if (_mqttClient)
    {
        if (_mqttClient->connected())
        {
            _mqttClient->stop();
        }
        delete _mqttClient;
        _mqttClient = nullptr;
    }
    if (_wifiClient)
    {
        delete _wifiClient;
        _wifiClient = nullptr;
    }
}

void Mapi::begin()
{
    // Configure pins
    pinMode(_lightSensorPin, INPUT);
    // DHT11 library often expects a digital pin for data; treat temp/hum pin as input
    pinMode(_tempHumiditySensorPin, INPUT);
    pinMode(_carbonMonoxideSensorPin, INPUT);

    // Create DHT11 instance bound to the temp/humidity data pin
    if (!_dht)
    {
        _dht = new DHT11(_tempHumiditySensorPin);
    }
}

float Mapi::lightSensor()
{
    // map analog read (0..1023) to percentage 0..100
    float reading = map(analogRead(_lightSensorPin), 0, 1023, 0, 100);
    return reading;
}

tuple<int, int, int> Mapi::readTempAndHumidity()
{

    int temperature = 0;
    int humidity = 0;

    if (!_dht)
    {
        // Not initialized; indicate error with -1
        return {-1, 0, 0};
    }

    // Attempt to read the temperature and humidity values from the DHT11 sensor.
    int result = _dht->readTemperatureHumidity(temperature, humidity);

    return {result, temperature, humidity};
}

int Mapi::readCarbonMonoxide()
{
    // For a simple analog CO sensor, return mapped percentage 0..100
    int raw = analogRead(_carbonMonoxideSensorPin);
    int pct = map(raw, 0, 1023, 0, 100);
    return pct;
}

void Mapi::beginPeopleCount(Stream *serial)
{
    _peopleCountSerial = serial;
}

int Mapi::readPeopleCount()
{
    if (!_peopleCountSerial)
        return -1;

    while (_peopleCountSerial->available())
    {
        String line = _peopleCountSerial->readStringUntil('\n');
        line.trim();
        if (line.startsWith("DATA:"))
        {
            String countStr = line.substring(5);
            _lastPeopleCount = countStr.toInt();
        }
    }
    return _lastPeopleCount;
}

// -------------------- MQTT helper implementations --------------------

void Mapi::mqttBegin(const char *broker, int port, const char *clientId)
{
    _mqttBroker = String(broker);
    _mqttPort = port;

    // If no clientId provided, generate one from MAC address
    if (clientId && strlen(clientId) > 0)
    {
        _mqttClientId = String(clientId);
    }
    else
    {
        byte mac[6];
        WiFi.macAddress(mac);
        _mqttClientId = "mapi-";
        for (int i = 0; i < 6; i++)
        {
            if (mac[i] < 16)
                _mqttClientId += "0";
            _mqttClientId += String(mac[i], HEX);
        }
    }

    // Create WiFi client and MQTT client
    if (!_wifiClient)
    {
        _wifiClient = new WiFiClient();
    }
    if (!_mqttClient)
    {
        _mqttClient = new MqttClient(*_wifiClient);
    }
}

void Mapi::mqttSetCredentials(const char *username, const char *password)
{
    _mqttUsername = String(username);
    _mqttPassword = String(password);
}

bool Mapi::mqttConnect()
{
    if (!_mqttClient)
    {
        return false;
    }

    // Set username and password if provided
    if (_mqttUsername.length() > 0)
    {
        _mqttClient->setUsernamePassword(_mqttUsername.c_str(), _mqttPassword.c_str());
    }

    // Set client ID
    _mqttClient->setId(_mqttClientId.c_str());

    // Connect to broker
    Serial.print("Connecting to MQTT broker: ");
    Serial.print(_mqttBroker);
    Serial.print(":");
    Serial.println(_mqttPort);

    if (!_mqttClient->connect(_mqttBroker.c_str(), _mqttPort))
    {
        Serial.print("MQTT connection failed! Error code: ");
        Serial.println(_mqttClient->connectError());
        return false;
    }

    Serial.println("Connected to MQTT broker!");
    Serial.print("Client ID: ");
    Serial.println(_mqttClientId);
    return true;
}

bool Mapi::mqttConnected()
{
    if (!_mqttClient)
    {
        return false;
    }
    return _mqttClient->connected();
}

bool Mapi::mqttReconnect()
{
    if (mqttConnected())
    {
        return true;
    }

    Serial.println("Attempting MQTT reconnection...");
    return mqttConnect();
}

void Mapi::mqttLoop()
{
    if (_mqttClient)
    {
        _mqttClient->poll();
    }
}

bool Mapi::mqttPublish(const char *topic, const char *payload)
{
    if (!mqttConnected())
    {
        Serial.println("MQTT not connected, cannot publish");
        return false;
    }

    _mqttClient->beginMessage(topic);
    _mqttClient->print(payload);
    _mqttClient->endMessage();

    return true;
}

bool Mapi::mqttPublishSensorData(const char *topic, bool includeCO)
{
    if (!mqttConnected())
    {
        return false;
    }

    // Read all sensors
    float light = lightSensor();
    auto thResult = readTempAndHumidity();
    int tempResult = std::get<0>(thResult);
    int temperature = std::get<1>(thResult);
    int humidity = std::get<2>(thResult);
    int co = includeCO ? readCarbonMonoxide() : 0;
    int peopleCount = readPeopleCount();

    // Build JSON payload
    String payload = "{";
    payload += "\"light\":";
    payload += String(light, 2);

    if (tempResult == 0)
    {
        payload += ",\"temperature\":";
        payload += String(temperature);
        payload += ",\"humidity\":";
        payload += String(humidity);
    }

    if (includeCO)
    {
        payload += ",\"co\":";
        payload += String(co);
    }

    if (peopleCount >= 0)
    {
        payload += ",\"people_count\":";
        payload += String(peopleCount);
    }

    // Add timestamp (millis since boot)
    payload += ",\"timestamp\":";
    payload += String(millis());

    payload += "}";

    // Publish
    return mqttPublish(topic, payload.c_str());
}
