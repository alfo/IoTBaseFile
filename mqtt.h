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
 settings.ho
 *r
 */

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

char deviceTopic[40];
char inTopic[40];
char willTopic[40];
char roomStateTopic[40];

void mqttCallback(char* topic, byte* payload, unsigned int length) {

  Serial.print("!!! Received: ");
  Serial.print(topic);
  Serial.print(" ");
  Serial.println((const char *)payload);
  Serial.println();

}

// This runs once from setup() in the main sketch
// It just takes the config (which has been loaded by this point)
void setupMQTT() {

  // Format the topics to include the hostname
  sprintf(willTopic,"%s/mqtt", hostname);
  sprintf(inTopic,"%s/in", hostname);
  sprintf(deviceTopic,"%s/out", hostname);
  sprintf(roomStateTopic,"house/%s/state", room);

  // We store the port as a char[6] so need to convert
  mqttClient.setServer(mqtt_server, atoi(mqtt_port));
  mqttClient.setCallback(mqttCallback);
}

void mqttConnect() {

  Serial.print("=== MQTT connecting to ");
  Serial.print(mqtt_server);
  Serial.print(":");
  Serial.println(atoi(mqtt_port));

  // Use hostname as our client ID
  // Use our will topic to broadcast a "disconnected" message when device goes down
  if (mqttClient.connect(hostname, mqtt_username, mqtt_password, willTopic, 0, 1, "disconnected")) {

    // Announce the connection, and make our subscriptions
    mqttClient.publish(willTopic, "connected");
    mqttClient.subscribe(inTopic);

    // This one is so we can set room-wide settings (like party mode etc.)
    mqttClient.subscribe(roomStateTopic);

  } else {

    // Sadness and tears. Check your MQTT params in portal
    Serial.println("%%% Failed to connect to MQTT");

  }

}

void mqttLoop() {

  // Maintain connection
  // This is also how we connect for the first time
  if(!mqttClient.connected()) {
    mqttConnect();
  }

  // Check for incoming messages
  mqttClient.loop();

}
