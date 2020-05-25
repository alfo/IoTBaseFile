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
#include <WiFiManager.h>   // https://github.com/tzapu/WiFiManager ~v2.0.0
#include <ArduinoJson.h>   // https://github.com/bblanchon/ArduinoJson ~v6.x.x
#include <PubSubClient.h>  // https://github.com/knolleary/pubsubclient ~v2.7.0
#include <WiFiUdp.h>       // For the below
#include <ArduinoOTA.h>    // Included with core

#ifdef ESP32
  #include <SPIFFS.h>
#endif

// Uncomment to reset device memory
//#define START_ANEW

// Which room in the house is this project
char room[30] = "living_room";

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

  // Start ArduinoOTA service
  ArduinoOTA.begin();

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

  // All-purpose way to keep track of the time
  long now = millis();

  // Loop our network services
  mqttLoop(now);          // Non-blocking MQTT connect/re-connect
  ArduinoOTA.handle();    // In case we want to upload a new sketch

  // Only check MQTT connection and messages every two seconds
  if (now - lastMsg > 5000) {

    // Create a debug message
    lastMsg = now;
    snprintf (msg, 50, "hello world #%ld", ++value);

    // Send a debug message
    Serial.print("Publish message: ");
    Serial.println(msg);

    mqttClient.publish(deviceTopic, msg);

    // Flash the built in LED
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
  }

}
