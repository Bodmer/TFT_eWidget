/***************************************************************************************

***************************************************************************************/

#include "GraphWidget.h"


/***************************************************************************************
** Function name:           GraphWidget
** Description:             Constructor with pointers to TFT and sprite instances
***************************************************************************************/
GraphWidget::GraphWidget(TFT_eSPI *tft)
{
  _tft = tft;
}

/***************************************************************************************
** Function name:           createGraph
** Description:             Create graph with parameters
***************************************************************************************/
bool GraphWidget::createGraph(uint16_t graphWidth, uint16_t graphHeight, uint16_t bgColor)
{
  _width   = graphWidth;
  _height  = graphHeight;
  _bgColor = bgColor;

  return 0;
}

/***************************************************************************************
** Function name:           setGraphScale
** Description:             Set slider scale range with movement delay in us per pixel
***************************************************************************************/
void GraphWidget::setGraphScale(float xmin, float xmax, float ymin, float ymax)
{
  _xMin = xmin;
  _xMax = xmax;
  _yMin = ymin;
  _yMax = ymax;
}

/***************************************************************************************
** Function name:           setGraphScale
** Description:             Set slider scale range with movement delay in us per pixel
***************************************************************************************/
void GraphWidget::setGraphGrid(float xsval, float xinc, float ysval, float yinc, uint16_t gridColor)
{
  _xGridStart = xsval;
  _xGridInc   = xinc;
  _yGridStart = ysval;
  _yGridInc   = yinc;
  _gridColor  = gridColor;
}

/***************************************************************************************
** Function name:           drawGraph
** Description:             drawGraph to TFT screen with set parameters
***************************************************************************************/
void GraphWidget::drawGraph(uint16_t x, uint16_t y)
{
  _tft->fillRect(x, y, _width, _height, _bgColor);

  for (float px = _xGridStart; px <= _xMax; px += _xGridInc)
  {
    uint16_t gx = map(px, _xMin, _xMax, 0, _width);
    _tft->drawFastVLine(x + gx, y, _height, _gridColor);
  }

  for (float py = _yGridStart; py <= _yMax; py += _yGridInc)
  {
    uint16_t gy = map(py, _yMin, _yMax, _height, 0);
    _tft->drawFastHLine(x, y + gy, _width, _gridColor);
  }

  _xpos = x;
  _ypos = y;
}

/***************************************************************************************
** Function name:           setGraphPosition
** Description:             Set the graph top left corner position without drawing it
***************************************************************************************/
void GraphWidget::setGraphPosition(uint16_t x, uint16_t y)
{
  _xpos = x;
  _ypos = y;
}

/***************************************************************************************
** Function name:           getGraphPosition
** Description:             Get the graph top left corner position
***************************************************************************************/
void GraphWidget::getGraphPosition(uint16_t *x, uint16_t *y)
{
  *x = _xpos;
  *y = _ypos;
}

/***************************************************************************************
** Function name:           getBoundingBox
** Description:             Return the bounding box as coordinates
***************************************************************************************/
void GraphWidget::getBoundingBox(int16_t *xs, int16_t *ys, int16_t *xe, int16_t *ye)
{
  // Bounds already corrected for Sprite wipe action
  *xs = _xpos;
  *ys = _ypos;
  *xe = _xpos + _width;
  *ye = _ypos + _height;
}

/***************************************************************************************
** Function name:           getBoundingRect
** Description:             Return outside bounding box rectangle x,y and width,height
***************************************************************************************/
void GraphWidget::getBoundingRect(int16_t *x, int16_t *y, uint16_t *w, uint16_t *h)
{
  *x = _xpos;
  *y = _ypos;
  *w = _width;
  *h = _height;
}

/***************************************************************************************
** Function name:           getPointX
** Description:             Get x pixel coordinates of a position on display
***************************************************************************************/
int16_t GraphWidget::getPointX(float xval)
{
  int16_t gx = 0.5 + mapFloat(xval, _xMin, _xMax, 0, _width);

  return _xpos + gx;
}

/***************************************************************************************
** Function name:           getPointY
** Description:             Get y pixel coordinates of a position on display
***************************************************************************************/
int16_t GraphWidget::getPointY(float yval)
{
  int16_t gy = 0.5 + mapFloat(yval, _yMin, _yMax, _height, 0);

  return _ypos + gy; 
}

/***************************************************************************************
** Function name:           addLine
** Description:             Add new line segment on graph
***************************************************************************************/
bool GraphWidget::addLine(float xs, float ys, float xe, float ye, uint16_t col)
{
  if (clipTrace(&xs, &ys, &xe, &ye))
  {
    // All or part of line is in graph area
    int16_t cxs = _xpos + 0.5 + mapFloat(xs, _xMin, _xMax, 0, _width);
    int16_t cys = _ypos + 0.5 + mapFloat(ys, _yMin, _yMax, _height, 0);
    int16_t cxe = _xpos + 0.5 + mapFloat(xe, _xMin, _xMax, 0, _width);
    int16_t cye = _ypos + 0.5 + mapFloat(ye, _yMin, _yMax, _height, 0);
    _tft->drawLine(cxs, cys, cxe, cye, col);
    return true;
  }
  
  // No part of line was in graph area
  return false;
}


/***************************************************************************************
** Function name:           regionCode
** Description:             Compute region code for a point(x, y)
***************************************************************************************/
uint16_t GraphWidget::regionCode(float x, float y)
{
	// Assume inside
	int code = INSIDE;

	if (x < _xMin) // to the left of rectangle
		code |= LEFT;
	else if (x > _xMax) // to the right of rectangle
		code |= RIGHT;
	if (y < _yMin) // below the rectangle
		code |= BOTTOM;
	else if (y > _yMax) // above the rectangle
		code |= TOP;

// Reported 4 bit code values
//
// 	        left 	central  right
// top 	    1001 	 1000 	 1010
// central 	0001 	 0000 	 0010
// bottom 	0101 	 0100 	 0110 

	return code;
}

/***************************************************************************************
** Function name:           clipTrace
** Description:             Implement Cohen-Sutherland clipping algorithm
***************************************************************************************/
// https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
bool GraphWidget::clipTrace(float *xs, float *ys, float *xe, float *ye)
{
  float x1 = *xs;
  float y1 = *ys;
  float x2 = *xe;
  float y2 = *ye;

	// Compute region codes
	uint16_t code1 = regionCode(x1, y1);
	uint16_t code2 = regionCode(x2, y2);
	bool inside = false;

	while (true) {
		if ((code1 == 0) && (code2 == 0)) {
			// bitwise OR is 0: both points inside window; trivially accept and exit loop
			inside = true;
			break;
		}
		else if (code1 & code2) {
			// bitwise AND is not 0: both points share an outside zone (LEFT, RIGHT, TOP,
			// or BOTTOM), so both must be outside window; exit loop (accept is false)
			break;
		}
		else {
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			uint16_t code_out;
			float x = 0, y = 0;

			// At least one endpoint is outside the clip rectangle; pick it.
			if (code1 != 0)
				code_out = code1;
			else
				code_out = code2;

			// Now find the intersection point;
			// use formulas:
			//   slope = (y1 - y0) / (x1 - x0)
			//   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
			//   y = y0 + slope * (xm - x0), where xm is xmin or xmax
			// No need to worry about divide-by-zero because, in each case, the
			// code_out bit being tested guarantees the denominator is non-zero
			if (code_out & TOP) {
				// point is above the clip rectangle
				x = x1 + (x2 - x1) * (_yMax - y1) / (y2 - y1);
				y = _yMax;
			}
			else if (code_out & BOTTOM) {
				// point is below the rectangle
				x = x1 + (x2 - x1) * (_yMin - y1) / (y2 - y1);
				y = _yMin;
			}
			else if (code_out & RIGHT) {
				// point is to the right of rectangle
				y = y1 + (y2 - y1) * (_xMax - x1) / (x2 - x1);
				x = _xMax;
			}
			else if (code_out & LEFT) {
				// point is to the left of rectangle
				y = y1 + (y2 - y1) * (_xMin - x1) / (x2 - x1);
				x = _xMin;
			}

			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (code_out == code1) {
				x1 = x;
				y1 = y;
				code1 = regionCode(x1, y1);
			}
			else {
				x2 = x;
				y2 = y;
				code2 = regionCode(x2, y2);
			}
		}
	}

  // Return the clipped coordinates
  if (inside)
  {
    *xs = x1;
    *ys = y1;
    *xe = x2;
    *ye = y2;
  }

  // Return true if a line segment need to be drawn in rectangle
  return inside;
}
