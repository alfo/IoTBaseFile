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
 * 
 */

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

char deviceTopic[40];
char willTopic[40];

void mqttCallback(char* topic, byte* payload, unsigned int length) {

  
}

// This runs once from setup() in the main sketch
// It just takes the config (which has been loaded by this point)
void setupMQTT() {

  sprintf(willTopic,"%s/mqtt", hostname);
  sprintf(deviceTopic,"%s/in", hostname);
  
  // We store the port as a char[6] so need to convert
  mqttClient.setServer(mqtt_server, atoi(mqtt_port));
  mqttClient.setCallback(mqttCallback);
}

void mqttConnect() {

  Serial.print("=== MQTT connecting to ");
  Serial.print(mqtt_server);
  Serial.println(atoi(mqtt_port));
  
  // Use hostname as our client ID
  // Use our will topic to broadcast a "disconnected" message when device goes down
  if (mqttClient.connect(hostname, mqtt_username, mqtt_password, willTopic, 0, 1, "disconnected")) {
    
    mqttClient.publish(willTopic, "connected");
    mqttClient.subscribe(deviceTopic);
  } else {

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
