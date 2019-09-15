#define _DEBUG_
#define _DISABLE_TLS_

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>

#define SSID "YOUR_SSID"
#define PASSWORD "YOUR_PASSWORD"

ThingerESP8266 thing("USER", "NODEMCU", "TOKEN");

int temperature;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.begin(115200);
  thing.add_wifi(SSID, PASSWORD);

}

void loop() {

thing["temperature"] << inputValue(temperature);
thing["builtinLED"] << digitalPin(LED_BUILTIN);
thing["ledBUILTIN"] >> outputValue(digitalRead(LED_BUILTIN));
Serial.print("Temperature: ");
Serial.println(temperature);
thing.handle();
  
  
}
