
#include <WiFi.h>
#include <HTTPClient.h>

#define WIFI_INFO
#include "wifi_info.h"

// http 통신 설정 코드
const char* ssid =     WIFI_ID_INFO;
const char* password = WIFI_PW_INFO;
const char* serverName = SERVER_ADDRESS;
int httpResponseCode;
unsigned long timerDelay = 1000;
unsigned long lastTime = 0;
String httpRequestData;
String payload;

int brightness = 0;
int bright_sen_pin = 16;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  pinMode(4, OUTPUT);
}

void loop() {
  if((millis() - lastTime) > timerDelay){
    analogWrite(4, 1);
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      brightness = analogRead(bright_sen_pin);
      Serial.print("birghtness : ");
      Serial.println(brightness);
      httpRequestData = String(serverName) + "?light=" + String(brightness);
      http.begin(httpRequestData.c_str());
      httpResponseCode = http.GET();
      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString();
        Serial.println(payload);
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
      delay(1000);
    }
    lastTime = millis();
    analogWrite(4, 0);
  }
}
