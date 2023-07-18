
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>

#define echoPin 19
#define trigPin 18

// 원본 아두이노 핀 번호
Servo myservo1;
Servo myservo2;
int cell = A0; //조도센서
int c_Value;
int s_Value;
int LED = 21;
int speaker = 8;
long duration, distance;
unsigned long startTime = 0;    // 거리가 30cm 이하인 상태의 시작 시간
bool isWithinRange = false;     // 거리가 30cm 이하인 상태 여부
char result;
int state = 0;
int prev_state = 0;

const char* ssid = "DESKTOP-N75BSJP 8172";
const char* password = "2@6A6o73";

String serverURL = "http://minimalist.iptime.org:14523/device/esp32board";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200); 
  wificonnect();
}

void loop() {
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      // String serverPath = serverName + "?temperature=24.37";
      
      http.begin(serverURL.c_str());
      
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.print("payload: ");
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
      wificonnect();
    }
    lastTime = millis();
  }
}

void wificonnect(){
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

