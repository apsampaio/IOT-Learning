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
  StaticJsonDocument<200> jsonMQTT;
  DeserializationError error = deserializeJson(jsonMQTT, payload);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  const char* id = jsonMQTT["id"];
  String metric = jsonMQTT["metric"];
  long value = jsonMQTT["value"];

  Serial.println("==PACKAGE==");
  Serial.print("Id: ");
  Serial.println(id);
  Serial.print("Metric: ");
  Serial.println(metric);
  Serial.print("Value: ");
  Serial.println(value);  
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
    client.subscribe(SUB);

    oldConnection = millis();
    oldPublishing = millis();

}

void loop() {
  
  publishing();
  loopConnection();
  
}

void publishing() {
  //Publish a Random number every minute
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
    if(client.state() < 0 || client.state() >= 1) clientConnection();
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
