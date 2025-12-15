// Description: Header file for Mapi class to interface with Mapi sensors
// Author: Roberto Figueiredo
// License: MIT]

#ifndef MAPI_H
#define MAPI_H

#include "Arduino.h"
#include <tuple>

// This library targets the Arduino UNO R4 WiFi which provides the WiFiS3 API.
// Include the board-specific WiFi header directly.
#include <WiFiS3.h>
#include <ArduinoMqttClient.h>

class DHT11; // forward declaration to avoid requiring DHT11 in header

class Mapi
{
public:
  // Constructor takes analog/digital pin numbers for sensors
  Mapi(int lightSensorPin, int tempHumiditySensorPin, int carbonMonoxideSensorPin);
  ~Mapi();

  // Initialize pins and sensor instances (call in setup())
  void begin();

  // -- WiFi helpers (optional)
  // Attempt to connect to the given SSID/pass. Returns WiFi status (WL_CONNECTED on success).
  // timeoutMs: maximum milliseconds to wait for connection (default 20000)
  int wifiBegin(const char *ssid, const char *pass, unsigned long timeoutMs = 20000);

  // Return current WiFi status (same codes as WiFi.status())
  int wifiStatus();

  // Return local IP address (valid if connected)
  IPAddress wifiLocalIP();

  // Print basic WiFi info to Serial (IP, MAC, SSID, RSSI)
  void wifiPrintInfo();

  // Fill provided 6-byte array with MAC address
  void wifiMacAddress(byte mac[]);

  // -- MQTT helpers (optional)
  // Initialize MQTT client with broker info. Call after WiFi is connected.
  // broker: IP address or hostname of MQTT broker
  // port: MQTT port (default 1883)
  // clientId: unique client identifier (if empty, uses MAC address)
  void mqttBegin(const char *broker, int port = 1883, const char *clientId = "");

  // Set MQTT credentials (optional, for brokers requiring authentication)
  void mqttSetCredentials(const char *username, const char *password);

  // Connect to MQTT broker. Returns true if successful.
  bool mqttConnect();

  // Check if MQTT client is connected
  bool mqttConnected();

  // Reconnect to MQTT broker if disconnected. Returns true if connected.
  bool mqttReconnect();

  // Maintain MQTT connection (call in loop())
  void mqttLoop();

  // Publish sensor data as JSON to a topic
  // topic: MQTT topic (e.g., "home/sensors/room1")
  // includeCO: whether to include carbon monoxide reading
  bool mqttPublishSensorData(const char *topic, bool includeCO = true);

  // Publish a single value to a topic
  bool mqttPublish(const char *topic, const char *payload);

  // Read light level as percentage (0-100)
  float lightSensor();

  // Read temp & humidity from DHT11. Returns tuple {resultCode, temperature, humidity}
  // resultCode: 0 = OK (depends on DHT11 library), non-zero indicates error
  std::tuple<int, int, int> readTempAndHumidity();

  // Read carbon monoxide sensor as percentage (0-100)
  int readCarbonMonoxide();

private:
  int _lightSensorPin;
  int _tempHumiditySensorPin;
  int _carbonMonoxideSensorPin;

  // DHT11 instance created in begin()
  DHT11 *_dht;

  // MQTT client and configuration
  WiFiClient *_wifiClient;
  MqttClient *_mqttClient;
  String _mqttBroker;
  int _mqttPort;
  String _mqttClientId;
  String _mqttUsername;
  String _mqttPassword;
};

#endif
