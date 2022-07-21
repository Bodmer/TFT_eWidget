/***************************************************************************************

***************************************************************************************/

#include "SliderWidget.h"

#define H_SLIDER true
#define V_SLIDER false

// Swap any type
template <typename T> static inline void
swap_val(T& a, T& b) { T t = a; a = b; b = t; }

/***************************************************************************************
** Function name:           SliderWidget
** Description:             Constructor with pointers to TFT and sprite instances
***************************************************************************************/
SliderWidget::SliderWidget(TFT_eSPI *tft, TFT_eSprite *spr) {
  _tft = tft;
  _spr = spr;
}

/***************************************************************************************
** Function name:           createSlider
** Description:             Create slider with slot parameters
***************************************************************************************/
bool SliderWidget::createSlider(uint16_t slotWidth, uint16_t slotLength, uint16_t slotColor, uint16_t bgColor, bool orientation) {
  _slotWidth   = slotWidth;
  _slotLength  = slotLength;
  _slotColor   = slotColor;
  _slotBgColor     = bgColor;
  _horiz = orientation;
  _kposPrev = slotLength/2;
  return 0;
}

/***************************************************************************************
** Function name:           createKnob
** Description:             Create the slider control knob with parameters
***************************************************************************************/
void SliderWidget::createKnob(uint16_t kwidth, uint16_t kheight, uint16_t kradius, uint16_t kcolor1, uint16_t kcolor2) {
  _kwidth  = kwidth;
  _kheight = kheight;
  _kradius = kradius;
  _kcolor1 = kcolor1;
  _kcolor2 = kcolor2;

  _sliderPos =  _slotLength/2;
}


/***************************************************************************************
** Function name:           setSliderScale
** Description:             Set slider scale range with movement delay in us per pixel
***************************************************************************************/
void SliderWidget::setSliderScale(int16_t min, int16_t max, uint16_t usdelay) {
  setSliderScale(min, max);
  _usdelay = usdelay;
}

/***************************************************************************************
** Function name:           setSliderScale
** Description:             Set slider scale range (no movement delay)
***************************************************************************************/
void SliderWidget::setSliderScale(int16_t min, int16_t max) {

  if (min > max) {
    _invert = true;
    swap_val(min, max);
  }
  else {
    _invert = false;
  }
  _sliderMin = min;
  _sliderMax = max;
  _sliderPos = min;
}

/***************************************************************************************
** Function name:           setSliderPosition
** Description:             Set slider position to a value in the set scale range
***************************************************************************************/
void SliderWidget::setSliderPosition(int16_t val)
{
  moveTo(_invert ? _sliderMax - val : val);
}

/***************************************************************************************
** Function name:           getSliderPosition
** Description:             Get the current slider value in set scale range
***************************************************************************************/
int16_t SliderWidget::getSliderPosition(void) {
  return _invert ? _sliderMax - _sliderPos : _sliderPos;
}

/***************************************************************************************
** Function name:           checkTouch
** Description:             Check is touch x, are inside slider box, if so move slider
***************************************************************************************/
bool SliderWidget::checkTouch(uint16_t tx, uint16_t ty)
{
  if (tx >= _sxs && tx <= _sxe && ty >= _sys && ty <= _sye) {
    uint16_t tp, kd;
    if(_horiz) {
      tp = tx - _sxs;
      kd = _kwidth;
    }
    else {
      tp = ty - _sys;
      kd = _kheight;
    }
    int16_t tv = map(tp, _slotWidth/2 + kd/2 + 1, _slotLength - _slotWidth/2 - kd/2 - 1, _sliderMin, _sliderMax);
    tv = constrain( tv, _sliderMin, _sliderMax);
    moveTo(tv);
    return true;
  }
  return false;
}

/***************************************************************************************
** Function name:           drawSlider
** Description:             drawSlider to TFT screen with set parameters
***************************************************************************************/
void SliderWidget::drawSlider(uint16_t x, uint16_t y, slider_t param)
{
  // createSlider
  _slotWidth = param.slotWidth;
  _slotLength = param.slotLength;
  _slotColor = param.slotColor;
  _slotBgColor = param.slotBgColor;
  _horiz = param.orientation;

  // createKnob
  _kwidth = param.knobWidth;
  _kheight = param.knobHeight;
  _kradius = param.knobRadius;
  _kcolor1 = param.knobColor;
  _kcolor2 = param.knobLineColor;

  // setSliderScale
  setSliderScale(param.sliderLT, param.sliderRB);

  _sliderPos = param.startPosition;
  _usdelay = param.sliderDelay;


  _kposPrev = _slotLength/2;

  drawSlider(x, y);
}


/***************************************************************************************
** Function name:           drawSlider
** Description:             drawSlider to TFT screen and set slider position value
***************************************************************************************/
void SliderWidget::drawSlider(uint16_t x, uint16_t y)
{
  if(_horiz) {
    _xpos = x + 2;
    _ypos = y + _kheight/2 + 2;
    //_tft->drawWideLine(_xpos, _ypos, _xpos + _slotLength, _ypos_ypos,_slotWidth, _slotColor, _slotBgColor);
    _tft->fillSmoothRoundRect(_xpos, _ypos - _slotWidth/2, _slotLength, _slotWidth, _slotWidth/2, _slotColor, _slotBgColor);
    _sxs = _xpos - 1;
    _sys = _ypos - _kheight/2 - 1;
    _sxe = _xpos + _slotLength + 2;
    _sye = _ypos + _kheight/2 + 3;
  }
  else  {
    _xpos = x + _kwidth/2 + 2;
    _ypos = y + 2;
    _tft->fillSmoothRoundRect(_xpos - _slotWidth/2, _ypos, _slotWidth, _slotLength, _slotWidth/2, _slotColor, _slotBgColor);
    _sxs = _xpos - _kwidth/2 - 1;
    _sys = _ypos - 1;
    _sxe = _xpos + _kwidth/2 + 3;
    _sye = _ypos + _slotLength + 2;
  }
  uint16_t kd = (_horiz ? _kwidth : _kheight);
  _kposPrev = map(_sliderPos, _sliderMin, _sliderMax, _slotWidth/2 + 1, _slotLength - _slotWidth/2 - kd - 1);
  setSliderPosition(_sliderPos);
}

/***************************************************************************************
** Function name:           moveTo (private fn)
** Description:             Move the slider to a new value in set range
***************************************************************************************/
void SliderWidget::moveTo(int16_t val)
{
  val = constrain( val, _sliderMin, _sliderMax );

  //if (val == _sliderPos) return;

  _sliderPos = val;

  uint16_t kd = (_horiz ? _kwidth : _kheight);
  uint16_t kpos = map(val, _sliderMin, _sliderMax, _slotWidth/2 + 1, _slotLength - _slotWidth/2 - kd - 1);

  _spr->createSprite(_kwidth + 2, _kheight + 2);

  if (_usdelay == 0 && abs(kpos - _kposPrev) > kd + 1) {
    drawKnob(kpos);
    _kposPrev = kpos;
  }
  else {
    int8_t dp = 1;
    if (kpos < _kposPrev) dp = -1;
    while (kpos != _kposPrev) {_kposPrev += dp;  drawKnob(_kposPrev); delayMicroseconds(_usdelay);}
  }

  _spr->deleteSprite();
}

/***************************************************************************************
** Function name:           drawKnob (private fn)
** Description:             Draw the slider control knob at pixel kpos position
***************************************************************************************/
void SliderWidget::drawKnob(uint16_t kpos)
{
  uint16_t x, y;

  if(_horiz) {
    x = _xpos + kpos - 1;
    y = _ypos - _kheight/2 - 1;
    _spr->fillRect(0, _kheight/2 - _slotWidth/2 + 1, _kwidth + 2, _slotWidth, _slotColor);
  }
  else  {
    x = _xpos - _kwidth/2 - 1;
    y = _ypos + kpos - 1;
    _spr->fillRect(_kwidth/2 - _slotWidth/2 + 1, 0, _slotWidth, _kheight + 2, _slotColor);
  }

  if (_usdelay == 0 && abs(kpos - _kposPrev) > (_horiz ? _kwidth : _kheight) + 1) {
    if(_horiz) _spr->pushSprite(_xpos + _kposPrev, y);
    else _spr->pushSprite(_xpos, y + _kposPrev);
  }

  // Draw slider outline
  _spr->fillSmoothRoundRect(1, 1, _kwidth, _kheight, _kradius, _kcolor1, _slotBgColor);

  // Draw marker stripe
  if(_kcolor1 != _kcolor2) {
    if(_horiz) {
      _spr->drawFastVLine(_kwidth/2 + 1, 1, _kheight, _kcolor2);
    }
    else  {
      _spr->drawFastHLine(1, _kheight/2 + 1, _kwidth, _kcolor2);
    }
  }
  _spr->pushSprite(x,y);
}

/***************************************************************************************
** Function name:           getBoundingBox
** Description:             Return the bounding box as coordinates
***************************************************************************************/
void SliderWidget::getBoundingBox(int16_t *xs, int16_t *ys, int16_t *xe, int16_t *ye)
{
  // Bounds already corrected for Sprite wipe action
  *xs = _sxs;
  *ys = _sys;
  *xe = _sxe;
  *ye = _sye;
}


/***************************************************************************************
** Function name:           getBoundingRect
** Description:             Return outside bounding box rectangle x,y and width,height
***************************************************************************************/
void SliderWidget::getBoundingRect(int16_t *x, int16_t *y, uint16_t *w, uint16_t *h)
{
  // Corrected to be outside slider draw zone
  *x = _sxs - 1;
  *y = _sys - 1;
  *w = _sxe - _sxs + 1;
  *h = _sye - _sys + 1;
}
