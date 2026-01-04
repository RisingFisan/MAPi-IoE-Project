#include <Arduino.h>
#include "Mapi.h"

// If you keep secrets in the Arduino "Secret" tab, create an "arduino_secrets.h" with
#define SECRET_SSID "MAPi2025"
#define SECRET_PASS "mapi2025"

// WiFi headers and WL_CONNECTED handling moved into the library header (Mapi.h)

// Example pins - adjust to match your wiring
const int LIGHT_PIN = A0; // analog pin for light sensor
const int DHT_PIN = 2;    // digital pin for DHT11 data
const int CO_PIN = A1;    // analog pin for CO sensor

Mapi sensors(LIGHT_PIN, DHT_PIN, CO_PIN);

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600); // Initialize Serial1 for people count sensor
    sensors.begin();
    sensors.beginPeopleCount(&Serial1); // Register Serial1 for people count
    delay(1000);
    Serial.println("Mapi example started");

    // Attempt WiFi connection using secrets (if defined)
#ifdef SECRET_SSID
    Serial.print("Connecting to WiFi SSID: ");
    Serial.println(SECRET_SSID);
    int st = sensors.wifiBegin(SECRET_SSID, SECRET_PASS, 15000);
    if (st == WL_CONNECTED)
    {
        Serial.println("WiFi connected");
        sensors.wifiPrintInfo();
    }
    else
    {
        Serial.print("WiFi failed, status=");
        Serial.println(st);
        while (true)
        {
            delay(1000);
        }
    }
#else
    Serial.println("SECRET_SSID not defined — skipping WiFi connect. Add arduino_secrets.h to use WiFi.");
#endif
}

void loop()
{
    // Read light
    float lightPct = sensors.lightSensor();
    Serial.print("Light: ");
    Serial.print(lightPct);
    Serial.println(" %");

    // Read DHT11
    auto rh = sensors.readTempAndHumidity();
    int result = std::get<0>(rh);
    int temperature = std::get<1>(rh);
    int humidity = std::get<2>(rh);

    if (result == 0)
    {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.println(" C");

        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");
    }
    else
    {
        Serial.print("DHT read error: ");
        Serial.println(result);
    }

    // Read CO
    int coPct = sensors.readCarbonMonoxide();
    Serial.print("CO (approx): ");
    Serial.print(coPct);
    Serial.println(" %");

    // Read People Count
    int people = sensors.readPeopleCount();
    if (people >= 0)
    {
        Serial.print("People Count: ");
        Serial.println(people);
    }

    Serial.println("----");
    delay(2000);
}
