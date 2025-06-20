#include "base64_utils.h"

namespace base64 {
  const char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  String encodeBase64Safe(const uint8_t* input, size_t length) {
    String out;
    int val = 0, valb = -6;
    for (size_t i = 0; i < length; ++i) {
      val = (val << 8) + input[i];
      valb += 8;
      while (valb >= 0) {
        out += alphabet[(val >> valb) & 0x3F];
        valb -= 6;
      }
    }
    if (valb > -6) out += alphabet[((val << 8) >> (valb + 8)) & 0x3F];
    while (out.length() % 4) out += '=';
    return out;
  }
}
