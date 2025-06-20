#include "crypto_utils.h"
#include <Crypto.h>
#include <SHA256.h>
#include <base64_utils.h>


const unsigned long ALLOWED_TIME_DRIFT = 10 * 60; // 10 minutes

String buildSigningString(const String& ssid, const String& password, const String& qr, const String& timestamp) {
  return ssid + "|" + password + "|" + qr + "|" + timestamp;
}

String computeHMAC(const String& message, const String& key) {
  SHA256 hasher;
  uint8_t hmac[32];
  hasher.resetHMAC((const uint8_t*)key.c_str(), key.length());
  hasher.update((const uint8_t*)message.c_str(), message.length());
  hasher.finalizeHMAC((const uint8_t*)key.c_str(), key.length(), hmac, sizeof(hmac));
  return base64::encodeBase64Safe(hmac, 32);
}

bool isTimestampValid(unsigned long timestamp) {
  unsigned long now = time(nullptr);
  return (timestamp > now - ALLOWED_TIME_DRIFT && timestamp < now + ALLOWED_TIME_DRIFT);
}
