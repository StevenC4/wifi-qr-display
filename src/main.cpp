#include <M5Core2.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <qrcode.h>
#include <utility/qrcode.h>

const char* ssid = "YourNetworkName";
const char* password = "YourNetworkPassword";

const char* serverUrl = "http://mozart.local/wifi-info";  // Your server endpoint

void drawQRCode(const String& qrText) {
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, ECC_LOW, qrText.c_str());

  int scale = 5;
  int offsetX = 20;
  int offsetY = 30;

  for (int y = 0; y < qrcode.size; y++) {
    for (int x = 0; x < qrcode.size; x++) {
      int px = offsetX + x * scale;
      int py = offsetY + y * scale;
      if (qrcode_getModule(&qrcode, x, y)) {
        M5.Lcd.fillRect(px, py, scale, scale, BLACK);
      } else {
        M5.Lcd.fillRect(px, py, scale, scale, WHITE);
      }
    }
  }
}

void showWiFiInfo(const String& ssidText, const String& passText) {
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.setCursor(10, 150);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("SSID: %s", ssidText.c_str());

  M5.Lcd.setCursor(10, 180);
  M5.Lcd.printf("Pass: %s", passText.c_str());
}

void fetchAndDisplayQR() {
  HTTPClient http;
  http.begin(serverUrl);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();
    StaticJsonDocument<512> doc;
    DeserializationError err = deserializeJson(doc, payload);

    if (err) {
      Serial.println("JSON parse failed");
      return;
    }

    String ssid = doc["ssid"];
    String pass = doc["password"];
    String qr = doc["qr"];

    showWiFiInfo(ssid, pass);
    drawQRCode(qr);
  } else {
    Serial.printf("HTTP Error: %d\n", httpCode);
  }

  http.end();
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextDatum(MC_DATUM);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.drawString("Connecting to Wi-Fi...", 160, 120);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.drawString("Fetching QR code...", 160, 120);

  fetchAndDisplayQR();
}

void loop() {
  // Device could enter deep sleep here if desired
}
