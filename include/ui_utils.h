#pragma once

#include <Arduino.h>
#include <M5Unified.h>
#include <qrcode.h>

extern LGFX_Sprite sprite;

void initSprite();
void drawQRCode(const String& qrText);
void showWiFiInfo(const String& ssidText, const String& passText);
void drawBatteryIcon(int x, int y);
void showError(const String& message);
void drawProgressBorder(unsigned long elapsed, unsigned long total, uint16_t color);
