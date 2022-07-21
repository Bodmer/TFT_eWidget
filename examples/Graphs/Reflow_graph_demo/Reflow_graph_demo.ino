// Monitors a thermocuple connected to SPI MAX31855 device

// Requires widget library here:
// https://github.com/Bodmer/TFT_eWidget

#include <TFT_eSPI.h>
#include "Free_Fonts.h"

#include <Adafruit_MAX31855.h>

TFT_eSPI tft = TFT_eSPI();

// Chip select for default SPI bus
#define MAXCS   21

Adafruit_MAX31855 thermocouple(MAXCS);

#include <TFT_eWidget.h>             // Widget library
GraphWidget gr = GraphWidget(&tft);  // Graph widget
TraceWidget tr = TraceWidget(&gr);   // Graph trace tr with pointer to gr

void setup() {
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, tft.color565(5, 5, 5));
  tft.setFreeFont(FF18);
  tft.setTextPadding(tft.textWidth("888"));

  gr.createGraph(200, 150, tft.color565(5, 5, 5));
  gr.setGraphGrid(0.0, 60.0, 25.0, 25.0, tft.color565(15, 15, 15));  
  gr.setGraphScale(0.0, 300.0, 25.0, 225.0);
  gr.drawGraph(40, 40);

  // Plot the solder melt temperature line (183 deg. C)
  tr.startTrace(TFT_RED);
  tr.addPoint(0.0, 183.0);
  tr.addPoint(300.0, 183.0);

  // Plot the desired temperature profile
  tr.startTrace(tft.color565(100, 100, 100));
  tr.addPoint(0.0, 25.0);
  tr.addPoint(50.0, 120.0);
  tr.addPoint(140.0, 170.0);
  tr.addPoint(200.0, 225.0);
  tr.addPoint(220.0, 225.0);
  tr.addPoint(260.0, 150.0);

  tr.startTrace(TFT_WHITE);

  // wait for MAX chip to stabilize
  delay(500);

  Serial.print("Initializing sensor...");
  if (!thermocouple.begin()) {
    Serial.println("ERROR.");
    while (1) delay(10);
  }
  Serial.println("DONE.");

  float c = thermocouple.readCelsius();
  if (isnan(c)) {
    Serial.println("Something wrong with thermocouple!");
  }
}

void loop() {
  static uint32_t scanTime = millis();
  static float gx = 0.0, gy = 25.0;
  static bool cool = false;
  static uint16_t cnt = 0;

  // Sample periodically
  if (millis() - scanTime >= 1000) {
    float c = thermocouple.readCelsius();
    tft.drawNumber(c + 0.5, 40, 40);
    tr.addPoint(gx, c);
    gx += 1.0;

    if (gy > 225.0) {
      gy = 224.0;
      cool = true;
      cnt = 0;
    }

    if (cool) {
      if (cnt > 20) gy -= 1.5;
      cnt++;
      if (gy < 25.0) {
        cool = false;
        gr.drawGraph(40, 40);
        tr.startTrace(TFT_RED);
        tr.addPoint(0.0, 183.0);
        tr.addPoint(300.0, 183.0);

        tr.startTrace(tft.color565(100, 100, 100));
        tr.addPoint(0.0, 25.0);
        tr.addPoint(50.0, 120.0);
        tr.addPoint(140.0, 170.0);
        tr.addPoint(200.0, 225.0);
        tr.addPoint(220.0, 225.0);
        tr.addPoint(260.0, 150.0);

        tr.startTrace(TFT_GREEN);
        gx = 0.0;
      }
    }
    else {
      if (gy < 120 || gy > 170.0) gy += 1.5;
      else gy += 0.40;
    }

    scanTime = millis();
  }
}
