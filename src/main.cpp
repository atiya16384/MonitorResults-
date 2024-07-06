#include <WiFi.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <FS.h>
#include <SPIFFS.h>
#include "ui.h"  // Include the UI header
#include <XPT2046_Touchscreen.h>  // Include touch library

// Replace with your network credentials
const char* ssid = "VM5653743-EXT";
const char* password = "jd3Lkxfstyhx";

// Define the TOUCH_CS pin
#define TOUCH_CS 15

// Create an instance of the display
TFT_eSPI tft = TFT_eSPI();
XPT2046_Touchscreen ts(TOUCH_CS);  // Create instance of touch screen with CS pin

// Current page number
int currentPage = 0;

// Total number of pages
const int totalPages = 3;

// Function to handle page swiping
void handleSwipe() {
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    if (p.x > tft.width() / 2) {
      currentPage = (currentPage + 1) % totalPages;
    } else {
      currentPage = (currentPage - 1 + totalPages) % totalPages;
    }

    switch (currentPage) {
      case 0:
        drawPage(tft, TFT_RED);  // Page 1: Red
        break;
      case 1:
        drawChart(tft);  // Page 2: Chart
        break;
      case 2:
        drawQRCode(tft, "https://www.example.com");  // Page 3: QR Code
        break;
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize the display
  tft.init();
  tft.setRotation(1);  // Set the rotation if necessary

  // Initialize touch screen
  ts.begin();
  ts.setRotation(1);

  // Initialize SPIFFS for loading images
  if (!SPIFFS.begin()) {
    Serial.println("An error occurred while mounting SPIFFS");
    return;
  }

  // Draw the initial page
  drawPage(tft, TFT_RED);
}

void loop() {
  // Handle swipe to change pages
  handleSwipe();
  delay(200);  // Small delay to debounce touch input
}
