#include "net.h"
#include "crypto_utils.h"
#include "ui_utils.h"
#include "secrets.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#ifdef LOCAL_TEST_MODE
const char* WORDS[] = {
  "cloud", "river", "stone", "ember", "fox", "hill", "leaf", "oak", "star", "wind",
  "echo", "fire", "glow", "haze", "ice", "jazz", "kite", "lava", "moon", "nova"
};
const int WORD_COUNT = sizeof(WORDS) / sizeof(WORDS[0]);

String generatePassphrase(int wordCount = 3) {
  String result;
  for (int i = 0; i < wordCount; ++i) {
    if (i > 0) result += "-";
    result += WORDS[random(WORD_COUNT)];
  }
  return result;
}
#endif

WifiData currentWifiData;

WifiData fetchWifiData() {
#ifdef LOCAL_TEST_MODE
  String ssid = "Cruzeiro do Sul";
  String pass = generatePassphrase(4);
  WifiData data(ssid, pass);
  data.fetchedAt = millis();
  data.valid = true;
  return data;
#else
  HTTPClient http;
  http.begin(SERVER_URL);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();
    StaticJsonDocument<768> doc;
    DeserializationError err = deserializeJson(doc, payload);

    if (err) {
      showError("JSON parse failed");
      return WifiData();
    }

    String ssid = doc["ssid"];
    String pass = doc["password"];
    String qr = doc["qr"];
    String tsStr = doc["timestamp"];
    String sig = doc["signature"];

    unsigned long ts = tsStr.toInt();

    if (!isTimestampValid(ts)) {
      showError("Timestamp invalid");
      return WifiData();
    }

    String msg = buildSigningString(ssid, pass, qr, tsStr);
    String expectedSig = computeHMAC(msg, SHARED_SECRET);

    if (expectedSig != sig) {
      showError("Invalid signature");
      return WifiData();
    }

    WifiData data(ssid, pass);
    data.fetchedAt = millis();
    data.valid = true;
    return data;
  } else {
    showError("HTTP Error " + String(httpCode));
    return WifiData();
  }

  http.end();
#endif
}
