#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "DESKTOP-N75BSJP 8172";
const char* password = "2@6A6o73";

const char* serverUrl = "http://minimalist.iptime.org:14523/device/plz";  // Flask 서버 URL 입력

#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"

void setup() {
  pinMode(4, OUTPUT);
  analogWrite(4, 30);
  delay(10);
  analogWrite(4, LOW);
  delay(100);
  analogWrite(4, 30);
  delay(10);
  analogWrite(4, LOW);

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  // 카메라 초기화
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // 카메라 초기화 및 실행
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera initialization failed with error 0x%x", err);
    return;
  }
}

void loop() {
  // 아두이노 코드에서 다른 부분이 있을 수 있으므로 이 부분은 채워주세요.
  // 예를 들어 변수 값을 업데이트하거나 다른 동작을 수행하는 등의 코드가 들어갈 수 있습니다.
  if (Serial.available() && Serial.read() == '1'){
    digitalWrite(4, HIGH);
    captureAndSend();
    digitalWrite(4, LOW);
  }
}

void captureAndSend() {
  camera_fb_t *fb = NULL;
  // 카메라로부터 이미지를 캡처합니다.
  digitalWrite(4, HIGH);
  Serial.println("flash");
  
  fb = esp_camera_fb_get();
  // Serial.printf("%u\n", fb);

  if (!fb) {
    Serial.println("Failed to capture image");
    return;
  }
  delay(10);
  digitalWrite(4, LOW);

  // HTTPClient 객체 생성
  HTTPClient http;

  // 사진 전송 준비
  http.begin(serverUrl);
  http.addHeader("Content-Type", "image/jpeg");

  // 사진 데이터를 서버로 전송
  // int httpResponseCode = http.POST(fb->buf, fb->len);
  int httpResponseCode = http.POST("hello world");

  // HTTP 응답을 확인하고 처리
  if (httpResponseCode > 0) {
    Serial.printf("HTTP Response code: %d\n", httpResponseCode);
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.printf("Error on sending POST request: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  // HTTP 연결 해제
  http.end();

  // 사진 데이터 메모리 해제
  esp_camera_fb_return(fb);
}
