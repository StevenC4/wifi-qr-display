#pragma once
#include <Arduino.h>

namespace base64 {
  String encodeBase64Safe(const uint8_t* input, size_t length);
}
