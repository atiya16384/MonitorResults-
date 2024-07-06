#include <TFT_eSPI.h>
#include <SPIFFS.h>
#include <qrcode.h>
#include "ui.h"

// Function to draw a page with a specified color
void drawPage(TFT_eSPI &tft, uint16_t color) {
  tft.fillScreen(color);
  tft.setTextColor(TFT_WHITE, color);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("Page");
}

// Function to draw a simple chart
void drawChart(TFT_eSPI &tft) {
  tft.fillScreen(TFT_BLACK);
  tft.drawRect(10, 10, 220, 220, TFT_WHITE); // Draw chart border
  for (int i = 0; i < 10; i++) {
    tft.drawLine(20 + i*20, 200, 20 + (i+1)*20, 200 - (i*10), TFT_GREEN); // Sample chart line
  }
}

// Function to display a BMP image from SPIFFS
void drawBmp(TFT_eSPI &tft, const char *filename, int16_t x, int16_t y) {
  if ((x >= tft.width()) || (y >= tft.height())) return;

  fs::File bmpFile = SPIFFS.open(filename, "r");
  if (!bmpFile) {
    Serial.print("File not found");
    return;
  }

  uint32_t seekOffset;
  uint16_t w, h;
  uint8_t r, g, b;

  if (read16(bmpFile) == 0x4D42) {
    read32(bmpFile);
    seekOffset = read32(bmpFile);
    read32(bmpFile);
    w = read32(bmpFile);
    h = read32(bmpFile);

    if ((read16(bmpFile) == 1) && (read16(bmpFile) == 24) && (read32(bmpFile) == 0)) {
      y += h - 1;
      tft.setSwapBytes(true);
      bmpFile.seek(seekOffset);

      uint16_t padding = (4 - ((w * 3) & 3)) & 3;
      uint8_t lineBuffer[w * 3];

      for (uint16_t row = 0; row < h; row++) {
        bmpFile.read(lineBuffer, sizeof(lineBuffer));
        uint8_t* bptr = lineBuffer;
        for (uint16_t col = 0; col < w; col++) {
          b = *bptr++;
          g = *bptr++;
          r = *bptr++;
          tft.drawPixel(x + col, y, tft.color565(r, g, b));
        }
        y--;
        bmpFile.read(lineBuffer, padding);
      }
    }
  }
  bmpFile.close();
}

uint16_t read16(fs::File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read();
  ((uint8_t *)&result)[1] = f.read();
  return result;
}

uint32_t read32(fs::File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read();
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read();
  return result;
}

// Function to generate and display a QR code
void drawQRCode(TFT_eSPI &tft, const char *text) {
  tft.fillScreen(TFT_WHITE);
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, 0, text);

  for (int y = 0; y < qrcode.size; y++) {
    for (int x = 0; x < qrcode.size; x++) {
      tft.drawRect(10 + x * 4, 10 + y * 4, 4, 4, qrcode_getModule(&qrcode, x, y) ? TFT_BLACK : TFT_WHITE);
    }
  }
}
