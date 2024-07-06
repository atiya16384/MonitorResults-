#ifndef UI_H
#define UI_H

#include <TFT_eSPI.h>
#include <FS.h>

// Function declarations
void drawPage(TFT_eSPI &tft, uint16_t color);
void drawChart(TFT_eSPI &tft);
void drawBmp(TFT_eSPI &tft, const char *filename, int16_t x, int16_t y);
void drawQRCode(TFT_eSPI &tft, const char *text);

uint16_t read16(fs::File &f);
uint32_t read32(fs::File &f);

#endif
