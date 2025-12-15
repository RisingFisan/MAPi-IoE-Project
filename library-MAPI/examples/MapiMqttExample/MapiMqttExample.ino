/**
 * MAPI MQTT Example
 *
 * This example demonstrates how to use the MAPI library to read sensor data
 * and publish it to an MQTT broker for Node-RED integration.
 *
 * Required libraries:
 * - MAPI (this library)
 * - DHT11
 * - ArduinoMqttClient
 *
 * Hardware:
 * - Arduino UNO R4 WiFi
 * - Light sensor on A0
 * - DHT11 temperature/humidity sensor on D2
 * - CO sensor on A1 (optional)
 *
 * Author: Roberto Figueiredo
 */

#include <Arduino.h>
#include "Mapi.h"

// ==================== WiFi Configuration ====================
// Update these with your WiFi credentials
#define SECRET_SSID "MAPi2025"
#define SECRET_PASS "mapi2025"

// ==================== MQTT Configuration ====================
// Update these with your MQTT broker details
#define MQTT_BROKER "mapi.local"        // IP address or hostname of your MQTT broker
#define MQTT_PORT 1883                  // Default MQTT port
#define MQTT_TOPIC "home/sensors/room1" // Topic to publish sensor data

// Optional: Set these if your broker requires authentication
// #define MQTT_USERNAME "your_username"
// #define MQTT_PASSWORD "your_password"

// ==================== Sensor Pin Configuration ====================
const int LIGHT_PIN = A0; // analog pin for light sensor
const int DHT_PIN = 2;    // digital pin for DHT11 data
const int CO_PIN = A1;    // analog pin for CO sensor

// ==================== Timing Configuration ====================
const unsigned long PUBLISH_INTERVAL = 5000; // Publish every 5 seconds (in milliseconds)
unsigned long lastPublishTime = 0;

// Create MAPI sensor object
Mapi sensors(LIGHT_PIN, DHT_PIN, CO_PIN);

// Set to true to publish synthetic sensor readings without hardware.
bool DUMMY = true;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ; // Wait for serial port to connect (needed for native USB)
    }

    Serial.println("=================================");
    Serial.println("MAPI MQTT Example Started");
    Serial.println("=================================");

    // Initialize sensors
    sensors.begin();
    delay(1000);

    // ==================== Connect to WiFi ====================
    Serial.println("\n[WiFi] Connecting...");
    Serial.print("[WiFi] SSID: ");
    Serial.println(SECRET_SSID);

    int wifiStatus = sensors.wifiBegin(SECRET_SSID, SECRET_PASS, 15000);

    if (wifiStatus == WL_CONNECTED)
    {
        Serial.println("[WiFi] Connected successfully!");
        sensors.wifiPrintInfo();
    }
    else
    {
        Serial.print("[WiFi] Connection failed! Status: ");
        Serial.println(wifiStatus);
        Serial.println("[ERROR] Cannot proceed without WiFi. Please check credentials.");
        while (1)
        {
            delay(1000); // Halt execution
        }
    }

    // ==================== Initialize MQTT ====================
    Serial.println("\n[MQTT] Initializing...");
    sensors.mqttBegin(MQTT_BROKER, MQTT_PORT);

    // Set MQTT credentials if needed
#ifdef MQTT_USERNAME
    sensors.mqttSetCredentials(MQTT_USERNAME, MQTT_PASSWORD);
#endif

    // Connect to MQTT broker
    if (sensors.mqttConnect())
    {
        Serial.println("[MQTT] Connected to broker!");
    }
    else
    {
        Serial.println("[MQTT] Initial connection failed. Will retry in loop.");
    }

    Serial.println("\n=================================");
    Serial.println("Setup Complete - Starting Loop");
    Serial.println("=================================\n");
}

void loop()
{
    // Maintain MQTT connection
    sensors.mqttLoop();

    // Check if we need to reconnect to MQTT
    if (!sensors.mqttConnected())
    {
        Serial.println("[MQTT] Disconnected! Attempting to reconnect...");
        if (sensors.mqttReconnect())
        {
            Serial.println("[MQTT] Reconnected successfully!");
        }
        else
        {
            Serial.println("[MQTT] Reconnection failed. Will retry next loop.");
            delay(5000); // Wait before retrying
            return;
        }
    }

    // Publish sensor data at regular intervals
    unsigned long currentTime = millis();
    if (currentTime - lastPublishTime >= PUBLISH_INTERVAL)
    {
        lastPublishTime = currentTime;

        Serial.println("\n--- Reading Sensors ---");

        float lightPct = 0.0f;
        int temperature = 0;
        int humidity = 0;
        int coPct = 0;
        int result = 0;

        if (DUMMY)
        {
            unsigned long tick = (millis() / 1000UL);
            lightPct = 40.0f + (tick % 25); // 40-64%
            if (lightPct > 100.0f)
            {
                lightPct = 100.0f;
            }
            temperature = 22 + (tick % 6); // 22-27 °C
            humidity = 45 + (tick % 15);   // 45-59 %
            coPct = 5 + (tick % 8);        // 5-12 %
        }
        else
        {
            // Read and display light sensor
            lightPct = sensors.lightSensor();

            // Read and display temperature/humidity
            auto thResult = sensors.readTempAndHumidity();
            result = std::get<0>(thResult);
            temperature = std::get<1>(thResult);
            humidity = std::get<2>(thResult);

            // Read and display CO sensor
            coPct = sensors.readCarbonMonoxide();
        }

        Serial.print("Light: ");
        Serial.print(lightPct);
        Serial.println(" %");

        if (result == 0)
        {
            Serial.print("Temperature: ");
            Serial.print(temperature);
            Serial.println(" °C");

            Serial.print("Humidity: ");
            Serial.print(humidity);
            Serial.println(" %");
        }
        else
        {
            Serial.print("DHT read error: ");
            Serial.println(result);
        }

        Serial.print("CO (approx): ");
        Serial.print(coPct);
        Serial.println(" %");

        // Publish all sensor data as JSON to MQTT
        Serial.print("\n[MQTT] Publishing to topic: ");
        Serial.println(MQTT_TOPIC);

        bool published = false;
        if (DUMMY)
        {
            String payload = "{";
            payload += "\"light\":";
            payload += String(lightPct, 2);
            payload += ",\"temperature\":";
            payload += String(temperature);
            payload += ",\"humidity\":";
            payload += String(humidity);
            payload += ",\"co\":";
            payload += String(coPct);
            payload += ",\"timestamp\":";
            payload += String(millis());
            payload += "}";
            published = sensors.mqttPublish(MQTT_TOPIC, payload.c_str());
        }
        else
        {
            published = sensors.mqttPublishSensorData(MQTT_TOPIC, true);
        }

        if (published)
        {
            Serial.println("[MQTT] Data published successfully!");
        }
        else
        {
            Serial.println("[MQTT] Failed to publish data!");
        }

        Serial.println("------------------------");
    }

    delay(100); // Small delay to prevent overwhelming the loop
}
