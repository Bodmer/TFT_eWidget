
#ifndef Meter_h
#define Meter_h

#include "Arduino.h"
#include "TFT_eSPI.h"

// Meter boarder colour
#define TFT_GREY 0x5AEB

// Meter class
class MeterWidget
{
  public:
    MeterWidget(TFT_eSPI* tft);

    // Set red, orange, yellow and green start+end zones as a percentage of full scale
    void setZones(uint16_t rs, uint16_t re, uint16_t os, uint16_t oe, uint16_t ys, uint16_t ye, uint16_t gs, uint16_t ge);
    // Draw meter meter at x, y and define full scale range plus the scale labels
    void analogMeter(uint16_t x, uint16_t y, float fullScale, const char *units, const char *s0, const char *s1, const char *s2, const char *s3, const char *s4);
    // Draw meter meter at x, y and define full scale range plus the scale labels
    void analogMeter(uint16_t x, uint16_t y, float startScale, float endScale, const char *units, const char *s0, const char *s1, const char *s2, const char *s3, const char *s4);
    // Move needle to new position
    void updateNeedle(float value, uint32_t ms_delay);

  private:
    // Pointer to TFT_eSPI class functions
    TFT_eSPI* ntft;

    float ltx;         // x delta of needle start
    uint16_t osx, osy; // Saved x & y coords of needle end
    int old_analog;    // Value last displayed
    int old_digital;   // Value last displayed

    // x, y coord of top left corner of meter graphic
    uint16_t mx;
    uint16_t my;

    // Scale factor
    float factor;
    float scaleStart;

    // Scale label
    char     mlabel[9];

    // Scale values
    char     ms0[5];
    char     ms1[5];
    char     ms2[5];
    char     ms3[5];
    char     ms4[5];

    // Scale colour zone start end end values
    int16_t redStart;
    int16_t redEnd;
    int16_t orangeStart;
    int16_t orangeEnd;
    int16_t yellowStart;
    int16_t yellowEnd;
    int16_t greenStart;
    int16_t greenEnd;
};

#endif
