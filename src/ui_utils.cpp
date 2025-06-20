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

  // Calculate total block height and start Y
  int totalHeight = lineHeight * 2 + padding;
  int startY = (sprite.height() - totalHeight) / 2;

  sprite.drawString(ssidText.c_str(), sprite.width() / 2, startY);
  sprite.drawString(passText.c_str(), sprite.width() / 2, startY + lineHeight + padding);
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

