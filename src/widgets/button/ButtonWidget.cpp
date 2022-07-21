#include "ButtonWidget.h"

/***************************************************************************************
** Code for the GFX button UI element
** Grabbed from Adafruit_tft library and enhanced to handle any label font
***************************************************************************************/
ButtonWidget::ButtonWidget(TFT_eSPI *tft) {
  _tft       = tft;
  _xd        = 0;
  _yd        = 0;
  _textdatum = MC_DATUM;
  _label[9]  = '\0';
  _currstate  = false;
  _laststate  = false;
  _inverted  = false;
}

void ButtonWidget::setPressAction(actionCallback action)
{
  pressAction = action;
}

void ButtonWidget::setReleaseAction(actionCallback action)
{
  releaseAction = action;
}

// Classic initButton() function: pass center & size
void ButtonWidget::initButton(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill, uint16_t textcolor, char *label, uint8_t textsize)
{
  // Tweak arguments and pass to the newer initButtonUL() function...
  initButtonUL(x - (w / 2), y - (h / 2), w, h, outline, fill, textcolor, label, textsize);
}

// Newer function instead accepts upper-left corner & size
void ButtonWidget::initButtonUL(int16_t x1, int16_t y1, uint16_t w, uint16_t h, uint16_t outline, uint16_t fill, uint16_t textcolor, char *label, uint8_t textsize)
{
  _x1           = x1;
  _y1           = y1;
  _w            = w;
  _h            = h;
  _outlinecolor = outline;
  _outlinewidth = 2;
  _fillcolor    = fill;
  _textcolor    = textcolor;
  _textsize     = textsize;
  strncpy(_label, label, 9);
  _pressTime    = 0xFFFFFFFF;
  _releaseTime  = 0xFFFFFFFF;
}

// Adjust text datum and x, y deltas
void ButtonWidget::setLabelDatum(int16_t x_delta, int16_t y_delta, uint8_t datum)
{
  _xd        = x_delta;
  _yd        = y_delta;
  _textdatum = datum;
}

void ButtonWidget::drawButton(bool inverted, String long_name) {
  uint16_t fill, outline, text;

  _inverted  = inverted;

  if(!inverted) {
    fill    = _fillcolor;
    outline = _outlinecolor;
    text    = _textcolor;
  } else {
    fill    = _textcolor;
    outline = _outlinecolor;
    text    = _fillcolor;
  }

  uint8_t r = min(_w, _h) / 4; // Corner radius
  _tft->fillRoundRect(_x1, _y1, _w, _h, r, fill);
  _tft->drawRoundRect(_x1, _y1, _w, _h, r, outline);

  if (_tft->textfont == 255) {
    _tft->setCursor(_x1 + (_w / 8),
                    _y1 + (_h / 4));
    _tft->setTextColor(text);
    _tft->setTextSize(_textsize);
    _tft->print(_label);
  }
  else {
    _tft->setTextColor(text, fill);
    _tft->setTextSize(_textsize);

    uint8_t tempdatum = _tft->getTextDatum();
    _tft->setTextDatum(_textdatum);
    uint16_t tempPadding = _tft->getTextPadding();
    _tft->setTextPadding(0);

    if (long_name == "")
      _tft->drawString(_label, _x1 + (_w/2) + _xd, _y1 + (_h/2) - 4 + _yd);
    else
      _tft->drawString(long_name, _x1 + (_w/2) + _xd, _y1 + (_h/2) - 4 + _yd);

    _tft->setTextDatum(tempdatum);
    _tft->setTextPadding(tempPadding);
  }
}

void ButtonWidget::drawSmoothButton(bool inverted, int16_t outlinewidth, uint32_t bgcolor, String long_name) {
  uint16_t fill, outline, text;
  if (bgcolor != 0x00FFFFFF) _bgcolor = bgcolor;
  if (outlinewidth >=0) _outlinewidth = outlinewidth;
  _inverted  = inverted;

  if(!inverted) {
    fill    = _fillcolor;
    outline = _outlinecolor;
    text    = _textcolor;
  } else {
    fill    = _textcolor;
    outline = _outlinecolor;
    text    = _fillcolor;
  }

  uint8_t r = min(_w, _h) / 4; // Corner radius
  if (outlinewidth > 0) _tft->fillSmoothRoundRect(_x1, _y1, _w, _h, r, outline, _bgcolor);
  _tft->fillSmoothRoundRect(_x1+_outlinewidth, _y1+_outlinewidth, _w-(2*_outlinewidth), _h-(2*_outlinewidth), r-_outlinewidth, fill, outline);

  if (_tft->textfont == 255) {
    _tft->setCursor(_x1 + (_w / 8),
                    _y1 + (_h / 4));
    _tft->setTextColor(text);
    _tft->setTextSize(_textsize);
    _tft->print(_label);
  }
  else {
    _tft->setTextColor(text, fill);
    _tft->setTextSize(_textsize);

    uint8_t tempdatum = _tft->getTextDatum();
    _tft->setTextDatum(_textdatum);
    uint16_t tempPadding = _tft->getTextPadding();
    _tft->setTextPadding(0);

    if (long_name == "")
      _tft->drawString(_label, _x1 + (_w/2) + _xd, _y1 + (_h/2) - 4 + _yd);
    else
      _tft->drawString(long_name, _x1 + (_w/2) + _xd, _y1 + (_h/2) - 4 + _yd);

    _tft->setTextDatum(tempdatum);
    _tft->setTextPadding(tempPadding);
  }
}

bool ButtonWidget::contains(int16_t x, int16_t y) {
  return ((x >= _x1) && (x < (_x1 + _w)) &&
          (y >= _y1) && (y < (_y1 + _h)));
}

void ButtonWidget::press(bool p) {
  _laststate = _currstate;
  _currstate = p;
}

bool ButtonWidget::isPressed()    { return _currstate; }
bool ButtonWidget::justPressed()  { return (_currstate && !_laststate); }
bool ButtonWidget::justReleased() { return (!_currstate && _laststate); }
