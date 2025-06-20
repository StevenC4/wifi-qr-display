#pragma once
#include <Arduino.h>

String buildSigningString(const String& ssid, const String& password, const String& qr, const String& timestamp);
String computeHMAC(const String& message, const String& key);
bool isTimestampValid(unsigned long timestamp);
