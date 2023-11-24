// https://blog.naver.com/roboholic84/221186233842
// https://randomnerdtutorials.com/esp32-http-get-post-arduino/

#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>

char serialCommand = '3';

// http 통신 설정 코드
const char* ssid = "hello_world";
const char* password = "lemon123";
const char* serverName = "http://esp32.ranong.xyz/command";
int httpResponseCode;
unsigned long timerDelay = 1000;
unsigned long lastTime = 0;
String httpRequestData;
String payload;
String server_command;
String check_command = "\nstand";
int num_count = 1;

int check = 0;

// 초음파
#define echoPin 14
#define trigPin 15
long distance;
long duration;

// 서보 모터
#define servo_pin 12
Servo servo1;

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

  servo1.attach(servo_pin);
  // 초음파
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo1.write(0);
}

void loop() {
  server_command = sendData();
  if (server_command == "stand"){
    if (check == 1){
      sit();
      check = 0;
    }

  }
  else if(server_command == "sit"){
    if(check == 0){
      stand();
      check = 1;
    }

  }
}

void sit(){
  for(int i = 0; i < 101; i++){
    servo1.write(i);
    delay(10);
  }
}
void stand(){
  for(int i = 100; i > 0; i--){
    servo1.write(i);
    delay(10);
    if(getdistance() < 5)
      break;
  }
}
String sendData(){
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    httpRequestData = String(serverName);
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
    WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}
  return payload;
}
long getdistance(){
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 

  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  
  return distance;
}