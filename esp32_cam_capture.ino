#include "esp_camera.h"
#include "FS.h"
#include "SD_MMC.h"

// Camera model
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

const int SHUTTER_PIN = 13;   // ← changed to GPIO13
bool lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

bool initSD() {
  if (!SD_MMC.begin("/sdcard", true)) {
    Serial.println("SD Card Mount Failed");
    return false;
  }
  return true;
}

String nextNumericFilename() {
  for (int i = 1; i < 10000; ++i) {
    char buf[32];
    sprintf(buf, "/%d.jpg", i);
    if (!SD_MMC.exists(buf)) return String(buf);
  }
  char buf[32];
  sprintf(buf, "/IMG_%lu.jpg", millis());
  return String(buf);
}

void takePhotoOnce() {
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  String path = nextNumericFilename();
  delay(400);

  File file;
  const int maxAttempts = 6;
  int attempt = 0;
  while (attempt < maxAttempts) {
    file = SD_MMC.open(path.c_str(), FILE_WRITE);
    if (file) break;
    attempt++;
    Serial.printf("Retry open file %s attempt %d\n", path.c_str(), attempt);
    delay(250);
  }

  if (!file) {
    Serial.printf("Failed to open file %s after %d tries\n", path.c_str(), maxAttempts);
    esp_camera_fb_return(fb);
    delay(600);
    return;
  }

  size_t written = file.write(fb->buf, fb->len);
  file.close();
  esp_camera_fb_return(fb);
  Serial.printf("Photo saved as %s (%u bytes)\n", path.c_str(), (unsigned)written);

  delay(1200);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting...");

  pinMode(SHUTTER_PIN, INPUT_PULLUP);

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
  config.frame_size = FRAMESIZE_QQVGA; // 160x120
  config.jpeg_quality = 50;            // small files
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed 0x%x\n", err);
    return;
  }

  if (!initSD()) return;
  Serial.println("Setup complete. Waiting for button press...");
}

void loop() {
  int reading = digitalRead(SHUTTER_PIN);

  static int lastButtonStateLocal = HIGH;
  if (reading != lastButtonStateLocal) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && lastButtonStateLocal == HIGH) {
      Serial.println("Button pressed - capturing");
      takePhotoOnce();
    }
  }
  lastButtonStateLocal = reading;
  delay(10);
}
