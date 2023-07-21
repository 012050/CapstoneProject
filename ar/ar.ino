
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
// String count = "0";
int count = 0;
String requestURL = "";
int num;

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

void setup() {
  Serial.begin(115200); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(speaker, OUTPUT);
  myservo1.attach(22);
  myservo2.attach(23);
  wificonnect();
}

void loop() {
  // if (getdistance() <= 10){
  //   if (!isWithinRange){
  //     isWithinRange = true;
  //     startTime = millis();
  //   }
  //   if (millis() - startTime >= 2000)
  //   state = 1;
  // }
  // else {
  //   isWithinRange = false;
  //   state = 0;
  // }

  // if (prev_state == 0 and state == 1){
  //   Serial.println(state);
  //   c_Value = analogRead(cell);
  //   digitalWrite(LED, HIGH); // LED를 켬
  //   delay(1000);
  //   digitalWrite(LED, LOW); 
  // }
  // else if (prev_state == 1 and state == 0)
  //   Serial.println(state);

  // prev_state = state;

  // if (Serial.available()) {
  //   result = Serial.read(); 
  //   if(result =='1'){ 
  //     //tone(speaker, 1000, 500); 
  //     myservo1.write(0);
  //     myservo2.write(180);
  //     // delay (3000);
  //     while (true){
  //       if (getdistance() > 30) {
  //         // delay(1000);
  //         myservo1.write(80);
  //         myservo2.write(100);
  //         break;
  //       }
  //     }
        
  //   }

  // }



  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
// -------------------------------------------------------------

      requestURL = serverURL + String(count);
      count++;

      http.begin(requestURL.c_str());

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