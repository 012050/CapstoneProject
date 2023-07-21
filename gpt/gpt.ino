#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>

#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"

const char* ssid = "DESKTOP-N75BSJP 8172";
const char* password = "2@6A6o73";
camera_fb_t * fb = NULL;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  
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
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }




  WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print('.');
  }
  Serial.println("Connected to WiFi");
  takeAndUploadPhoto();
}

void takeAndUploadPhoto() {
  fb = esp_camera_fb_get();  
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }
  
  WiFiClient client;
  HTTPClient http;
  
  // Change the IP address and port to match your Flask server's address
  const char* serverIP = "minimalist.iptime.org";  // Replace with your Flask server IP address
  int serverPort = 14523;
  String path = "/upload";  // The route on the server to handle image uploads
  
  if (http.begin(client, "http://" + String(serverIP) + ":" + String(serverPort) + path)) {
    http.addHeader("Content-Type", "image/jpeg");
    int httpResponseCode = http.POST((uint8_t*)fb->buf, fb->len);  // Send the image data as the request body
    
    if (httpResponseCode == 200) {
      Serial.println("Photo uploaded successfully");
    } else {
      Serial.println("Photo upload failed");
      Serial.print("httpResponseCode: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("HTTP connection failed");
  }
  
  esp_camera_fb_return(fb);
}

void loop() {
  // Your other code here
}
