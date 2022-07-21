/***************************************************************************************

***************************************************************************************/
#ifndef _SliderWidgetH_
#define _SliderWidgetH_

//Standard support
#include <Arduino.h>

#include <TFT_eSPI.h>
// Created by Bodmer from widget sketch functions

#define H_SLIDER true
#define V_SLIDER false

typedef struct slider_t {
  // createSlider
  uint16_t slotWidth = 5;
  uint16_t slotLength = 100;
  uint16_t slotColor = TFT_GREEN;
  uint16_t slotBgColor = TFT_BLACK;
  bool orientation = true;

  // createKnob
  uint16_t knobWidth = 21;
  uint16_t knobHeight =21;
  uint16_t knobRadius = 5;
  uint16_t knobColor = TFT_WHITE;
  uint16_t knobLineColor = TFT_RED;

  // setSliderScale
  int16_t sliderLT = 0.0;
  int16_t sliderRB = 100.0;
  int16_t startPosition = 50;
  uint16_t sliderDelay = 2000;
  
} slider_param;


class SliderWidget : public TFT_eSPI {

 public:

  SliderWidget(TFT_eSPI *tft, TFT_eSprite *spr);

  void drawSlider(uint16_t x, uint16_t y);
  void drawSlider(uint16_t x, uint16_t y, slider_t param);

  bool createSlider(uint16_t slotWidth, uint16_t slotLength, uint16_t slotColor, uint16_t bgColor, bool orientation);

  void createKnob(uint16_t kwidth, uint16_t kheight, uint16_t kradius, uint16_t kcolor1, uint16_t kcolor2);

  void setSliderScale(int16_t min, int16_t max, uint16_t usdelay);
  void setSliderScale(int16_t min, int16_t max);

  void setSliderPosition(int16_t val);
  int16_t getSliderPosition(void);

  void getBoundingBox(int16_t *xs, int16_t *ys, int16_t *xe, int16_t *ye);
  void getBoundingRect(int16_t *x, int16_t *y, uint16_t *w, uint16_t *h);

  bool checkTouch(uint16_t tx, uint16_t ty);

 private:
  void moveTo(int16_t val);
  void drawKnob(uint16_t kpos);

  // createSlider
  uint16_t _slotWidth;
  uint16_t _slotLength;
  uint16_t _slotColor;
  uint16_t _slotBgColor;
  bool _horiz;

  // createKnob
  uint16_t _kwidth;
  uint16_t _kheight;
  uint16_t _kradius;
  uint16_t _kcolor1;
  uint16_t _kcolor2;

  // setSliderScale
  int16_t _sliderMin = 0.0;
  int16_t _sliderMax = 100.0;
  uint16_t _usdelay = 0;

  // drawSlider
  uint16_t _xpos = 0;
  uint16_t _ypos = 0;

  bool _invert = false;

  // moveTo
  int16_t _sliderPos;
  uint16_t _kposPrev;


  // checkTouch
  uint16_t _sxs;
  uint16_t _sys;
  uint16_t _sxe;
  uint16_t _sye;

  TFT_eSprite *_spr;
  TFT_eSPI *_tft;
};

#endif