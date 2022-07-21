/***************************************************************************************

***************************************************************************************/
#ifndef _GraphWidgetH_
#define _GraphWidgetH_

//Standard support
#include <Arduino.h>

#include <TFT_eSPI.h>
// Created by Bodmer from widget sketch functions

class GraphWidget : public TFT_eSPI {

 public:

  GraphWidget(TFT_eSPI *tft);

  bool createGraph(uint16_t graphWidth, uint16_t graphHeight, uint16_t bgColor);
  void setGraphGrid(float xsval, float xinc, float ysval, float yinc, uint16_t gridColor);

  void setGraphPosition(uint16_t x, uint16_t y);
  void getGraphPosition(uint16_t *x, uint16_t *y);

  void drawGraph(uint16_t x, uint16_t y);

  void setGraphScale(float xmin, float xmax, float ymin, float ymax);

  void getBoundingBox(int16_t *xs, int16_t *ys, int16_t *xe, int16_t *ye);
  void getBoundingRect(int16_t *x, int16_t *y, uint16_t *w, uint16_t *h);

  int16_t getPointX(float xval);
  int16_t getPointY(float yval);

  bool addLine(float xs, float ys, float xe, float ye, uint16_t col);

  // createGraph
  uint16_t _width;
  uint16_t _height;

  // setGraphScale
  float _xMin = 0.0;
  float _xMax = 100.0;
  float _yMin = 0.0;
  float _yMax = 100.0;

  // drawGraph
  uint16_t _xpos = 0;
  uint16_t _ypos = 0;

 private:

  // Map a float to a new range
  float mapFloat(float ip, float ipmin, float ipmax, float tomin, float tomax)
  {
    return tomin + (((tomax - tomin) * (ip - ipmin))/ (ipmax - ipmin));
  }

  uint16_t regionCode(float x, float y);
  bool clipTrace(float *xs, float *ys, float *xe, float *ye);

  uint16_t _gridColor;
  uint16_t _bgColor;

  // setGraphGrid
  float _xGridStart = 0.0;
  float _xGridInc = 10.0;
  float _yGridStart = 0.0;
  float _yGridInc = 10.0;

  // Defining region codes
  const uint16_t INSIDE = 0x0; // 0000
  const uint16_t LEFT =   0x1; // 0001
  const uint16_t RIGHT =  0x2; // 0010
  const uint16_t BOTTOM = 0x4; // 0100
  const uint16_t TOP =    0x8; // 1000

  TFT_eSPI *_tft;
};

#endif