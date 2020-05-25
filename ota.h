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
 ota.h
 *
 */

// We don't use any of these, but good to have them
ArduinoOTA.onStart([]() {
  //
});

ArduinoOTA.onEnd([]() {
  //
});

ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
  Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
});

 void startOTA() {

  // So not just anyone can re-flash our devices
  ArduinoOTA.setPassword("chvFSEebm9X4GtpY");
  
  ArduinoOTA.begin();
  
 }
