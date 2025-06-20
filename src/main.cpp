#include <M5Unified.h>
#include "driver/adc.h"
#include <WiFi.h>
#include "secrets.h"
#include "net.h"
#include "ui_utils.h"

#ifdef LOCAL_TEST_MODE
const unsigned long REFRESH_INTERVAL_MS = 15UL * 1000UL;  // 15 seconds
#else
const unsigned long REFRESH_INTERVAL_MS = 6UL * 60UL * 60UL * 1000UL;  // 6 hours
#endif
const unsigned long SCREEN_TIMEOUT_MS = 5UL * 60UL * 1000UL;  // 5 min
const unsigned long SLEEP_TIMEOUT_MS = 15UL * 60UL * 1000UL;  // 15 min

unsigned long lastFetch = 0;
unsigned long lastActivity = 0;
bool screenOn = true;
bool showQRCodeView = true; // Start in QR-only mode
WifiData currentData;

void renderCurrentView() {
  sprite.fillScreen(WHITE);
  if (showQRCodeView) {
    drawQRCode(currentData.getQr());
  } else {
    showWiFiInfo(currentData.ssid, currentData.password);
  }

  // Border progress
  unsigned long elapsed = millis() - lastFetch;
  drawProgressBorder(elapsed, REFRESH_INTERVAL_MS, GREEN);

  sprite.pushSprite(0, 0);
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  initSprite();

  sprite.setRotation(0);
  sprite.setTextSize(2);
  sprite.setTextColor(WHITE);
  sprite.setTextDatum(MC_DATUM);
  sprite.fillScreen(WHITE);

#ifndef LOCAL_TEST_MODE
  M5.Lcd.drawString("Connecting to Wi-Fi...", 160, 120);

  configTime(0, 0, "pool.ntp.org");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(500);
  }

  if (WiFi.status() != WL_CONNECTED) {
    M5.Lcd.fillScreen(RED);
    M5.Lcd.drawString("Wi-Fi failed!", 160, 120);
    return;
  }

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.drawString("Fetching QR code...", 160, 120);
#endif

  currentData = fetchWifiData();
  lastFetch = millis();
  renderCurrentView();
}

void loop() {
  M5.update();
  unsigned long now = millis();

  // --- Handle Button A Press ---
  auto touch = M5.Touch.getDetail();
  if (M5.BtnA.wasPressed() || touch.wasPressed()) {
    lastActivity = now;

    if (!screenOn) {
      M5.Lcd.wakeup();
      screenOn = true;
      showQRCodeView = true;
      renderCurrentView();
    } else {
      showQRCodeView = !showQRCodeView;
      renderCurrentView();
    }
  }

  // --- Periodic Refresh ---
  if (now - lastFetch >= REFRESH_INTERVAL_MS) {
    currentData = fetchWifiData();
    lastFetch = now;
    lastActivity = now;
  }

  // --- Auto Screen Off ---
  if (screenOn && (now - lastActivity >= SCREEN_TIMEOUT_MS)) {
    M5.Lcd.sleep();
    screenOn = false;
  }

  // --- Auto Deep Sleep ---
  if (now - lastActivity >= SLEEP_TIMEOUT_MS) {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.drawString("Sleeping...", 160, 120);
    delay(1000);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_39, 0); // Wake on Btn A
    esp_deep_sleep_start();
  }

  if (screenOn) {
    renderCurrentView();
  }

  delay(screenOn ? 33 : 200);
}
