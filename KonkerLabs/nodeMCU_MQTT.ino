// Based on:https://github.com/KonkerLabs/examples 
// But with ArduinoJson.h 6.0

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> 

const char* SSID = "WIFI_SSID";
const char* PASSWORD = "WIFI_PASSWORD";
const char* mqtt_client = "MQTT_CLIENT";
const char* PUB = "PUB_LINK";
const char* SUB = "SUB_LINK";
const char* KonkerUser = "DEVICE_USER";
const char* KonkerPass = "DEVICE_PASS";

char *data;
char *jsonMqttData(const char *id, const char *metric, float value);

int currentConnection;
int oldConnection;
int currentPublishing;
int oldPublishing;

void publishing();
void loopConnection();
void clientConnection();

//Callback function
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

//Creating objects of connection with network and MQTT server
WiFiClient espClient;
PubSubClient client(mqtt_client, 1883, callback, espClient);

void setup() {

    Serial.begin(115200);
    WiFi.begin(SSID, PASSWORD);
    Serial.print("Connecting...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
    clientConnection();
    oldConnection = millis();
    oldPublishing = millis();
    client.loop();
}

void loop() {
  
  publishing();
  loopConnection();
  
}

void publishing() {
  //Publish a Random number
  currentPublishing = millis();
  if((currentPublishing - oldPublishing) >= 60000) { 
    
    data = jsonMqttData("NODEMCU1", "Random", random(100));
    Serial.println(data);
    client.publish(PUB,data);
    oldPublishing = millis();
  }
}

void loopConnection(){
  // Loop connection every 5 seconds to keep connection alive
  // and reconnect if connection had a timeout
   
  currentConnection = millis();
  if((currentConnection - oldConnection) >= 5000) { 
    client.loop();
    Serial.println("Connection Loop!");
    Serial.print("MQTT Connection: ");
      
    switch(client.state()){
      case -4: Serial.println("CONNECTION_TIMEOUT");
                clientConnection();
      break;
      case -3: Serial.println("CONNECTION_LOST");
      break;
      case -2: Serial.println("CONNECT_FAILED");
      break;
      case -1: Serial.println("DISCONNECTED");
      break;
      case 0: Serial.println("CONNECTED");
      break;
      case 1: Serial.println("CONNECT_BAD_PROTOCOL");
      break;
      case 2: Serial.println("CONNECT_BAD_CLIENT_ID");
      break;
      case 3: Serial.println("CONNECT_UNAVAILABLE");
      break;
      case 4: Serial.println("CONNECT_BAD_CREDENTIALS");
      break;
      case 5: Serial.println("CONNECT_UNAUTHORIZED");
    }

    oldConnection = millis();
  }
}

void clientConnection() {
    Serial.print("Connecting to MQTT...");
    while(!client.connected()) { 
      if (client.connect("DEVICE_NAME", "DEVICE_USER", "DEVICE_PASS")) {
        Serial.println("Connected to MQTT!");
        client.subscribe(SUB);
      } else {
      delay(500);
      Serial.print(".");
      }
    }

}

char *jsonMqttData(const char *id, const char *metric, float value) {
  
  char bufferJ[256];
  StaticJsonDocument<200> jsonMQTT;
  jsonMQTT["id"] = id;
  jsonMQTT["metric"] = metric;
  jsonMQTT["value"] = value;
  serializeJson(jsonMQTT, bufferJ);
  return bufferJ;
  
}
