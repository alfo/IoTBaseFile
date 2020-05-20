/*
 *
 *
 *         _             ______
     /\   | |           |  ____|
    /  \  | | _____  __ | |__ ___  _ __ ___ _   _
   / /\ \ | |/ _ \ \/ / |  __/ _ \| '__/ _ \ | | |
  / ____ \| |  __/>  <  | | | (_) | | |  __/ |_| |
 /_/    \_\_|\___/_/\_\ |_|  \___/|_|  \___|\__, |
                                             __/ |
 Internet of Things Base Project            |___/
 For ESP8266 / ESP32
 IoTBaseFile.ino
 *
 */

 // Include Libraries
#include <FS.h>            // this needs to be first, or it all crashes and burns...
#include <WiFiManager.h>   // https://github.com/tzapu/WiFiManager v0.1.5
#include <ArduinoJson.h>   // https://github.com/bblanchon/ArduinoJson ~v5.x.x
#include <PubSubClient.h>
#ifdef ESP32
  #include <SPIFFS.h>
#endif

// Uncomment to reset device memory
//#define START_ANEW

// Include the other sketch files
#include "settings.h"
#include "wifi.h"
#include "mqtt.h"

void setup() {

  // Start serial
  Serial.begin(115200);
  Serial.println();

  // Load config from the file system
  setupStorage();

  // Start up WiFi and config parameters etc.
  startWiFiManagerWithParameters();

  // Instantiate MQTT
  setupMQTT();

  // Usefull for all sketches really
  pinMode(LED_BUILTIN, OUTPUT);
}

// Variables just for the example code below
long lastMsg = 0;
int value = 0;
char msg[50];

void loop() {

  long now = millis();

  // Only check MQTT connection and messages every two seconds
  if (now - lastMsg > 2000) {
    mqttLoop();

    lastMsg = now;
    ++value;
    snprintf (msg, 50, "hello world #%ld", value);

    Serial.print("Publish message: ");
    Serial.println(msg);

    mqttClient.publish(deviceTopic, msg);

    // Flash the built in LED
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
  }

}
