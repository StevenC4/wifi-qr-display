#include "ui_utils.h"

LGFX_Sprite sprite(&M5.Lcd);
void initSprite() {
  sprite.setColorDepth(16);
  sprite.setPsram(true);
  sprite.createSprite(M5.Lcd.width(), M5.Lcd.height());
}

void drawQRCode(const String& qrText) {
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, ECC_LOW, qrText.c_str());

  int scale = 5;
  const int qrSize = qrcode.size * scale;
  const int offsetX = (sprite.width() - qrSize) / 2;
  const int offsetY = (sprite.height() - qrSize) / 2;

  for (int y = 0; y < qrcode.size; y++) {
    for (int x = 0; x < qrcode.size; x++) {
      int px = offsetX + x * scale;
      int py = offsetY + y * scale;
      bool isBlack = qrcode_getModule(&qrcode, x, y);
      sprite.fillRect(px, py, scale, scale, isBlack ? BLACK : WHITE);
    }
  }
}

void showWiFiInfo(const String& ssidText, const String& passText) {
  sprite.setTextColor(BLACK);
  sprite.setTextSize(2);
  sprite.setTextDatum(MC_DATUM);

  int padding = 12;  // vertical space between the two rows
  int lineHeight = sprite.fontHeight();
  int centerX = sprite.width() / 2;

  int currentY = (sprite.height() - 3 * lineHeight - padding) / 2;
  sprite.drawString(ssidText.c_str(), centerX, currentY);
  currentY += lineHeight + padding;

  int maxWidth = sprite.width() - 20;
  String line = "";
  for (int i = 0; i < passText.length(); ++i) {
    line += passText[i];
    if (sprite.textWidth(line) > maxWidth) {
      line.remove(line.length() - 1); // remove overflow char
      sprite.drawString(line, centerX, currentY);
      currentY += lineHeight;
      line = passText[i];
    }
  }
  if (line.length() > 0) {
    sprite.drawString(line, centerX, currentY);
  }
}

void drawBatteryIcon(int x, int y) {
  bool charging = M5.Power.isCharging();
  int level = M5.Power.getBatteryLevel();  // 0–100

  // Battery outline
  sprite.drawRect(x, y, 28, 14, BLACK);   // main battery body
  sprite.fillRect(x + 28, y + 4, 2, 6, BLACK); // terminal nub

  // Fill based on level
  int bars = map(level, 0, 100, 0, 4);
  for (int i = 0; i < bars; ++i) {
    sprite.fillRect(x + 3 + i * 6, y + 3, 5, 8, charging ? YELLOW : GREEN);
  }
}

void showError(const String& message) {
  sprite.fillScreen(RED);
  sprite.setTextColor(WHITE);
  sprite.setTextDatum(MC_DATUM);
  sprite.setTextSize(2);
  sprite.drawString(message, 160, 120);
}

void drawProgressBorder(unsigned long elapsed, unsigned long total, uint16_t color) {
  float progress = min((float)elapsed / total, 1.0f);
  int w = sprite.width();
  int h = sprite.height();
  int thickness = 6;

  int totalPixels = (w * 2 + h * 2);
  int progressPixels = totalPixels * progress;

  // Top edge
  int topLen = min(progressPixels, w);
  if (topLen > 0)
    sprite.fillRect(0, 0, topLen, thickness, color);
  progressPixels -= topLen;

  // Right edge
  int rightLen = min(progressPixels, h);
  if (rightLen > 0)
    sprite.fillRect(w - thickness, 0, thickness, rightLen, color);
  progressPixels -= rightLen;

  // Bottom edge
  int bottomLen = min(progressPixels, w);
  if (bottomLen > 0)
    sprite.fillRect(w - bottomLen, h - thickness, bottomLen, thickness, color);
  progressPixels -= bottomLen;

  // Left edge
  int leftLen = min(progressPixels, h);
  if (leftLen > 0)
    sprite.fillRect(0, h - leftLen, thickness, leftLen, color);
}

