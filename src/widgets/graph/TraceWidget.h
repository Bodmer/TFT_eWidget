/***************************************************************************************

***************************************************************************************/
#ifndef _TraceWidgetH_
#define _TraceWidgetH_

//Standard support
#include <Arduino.h>

#include <TFT_eSPI.h>
#include "GraphWidget.h"

// Created by Bodmer from widget sketch functions

class TraceWidget : public TFT_eSPI {

 public:

  TraceWidget(GraphWidget *gw);

  void startTrace(uint16_t ptColor);
  bool addPoint(float xval, float yval);

  uint16_t getLastPointX(void);
  uint16_t getLastPointY(void);

 private:

  // Map a float to a new range
  float mapFloat(float ip, float ipmin, float ipmax, float tomin, float tomax)
  {
    return tomin + (((tomax - tomin) * (ip - ipmin))/ (ipmax - ipmin));
  }

  uint16_t regionCode(float x, float y);
  bool clipTrace(float *xs, float *ys, float *xe, float *ye);

  // trace
  bool _newTrace = true;
  uint16_t _ptColor = TFT_WHITE;
  uint16_t _xpt = 0;
  uint16_t _ypt = 0;
  float _xval = 0;
  float _yval = 0;

  GraphWidget *_gw;
};

#endif