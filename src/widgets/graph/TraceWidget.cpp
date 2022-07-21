/***************************************************************************************

***************************************************************************************/

#include "TraceWidget.h"

/***************************************************************************************
** Function name:           TraceWidget
** Description:             Constructor with pointers to TFT and sprite instances
***************************************************************************************/
TraceWidget::TraceWidget(GraphWidget *gw) {
  _gw = gw;
}

/***************************************************************************************
** Function name:           startTrace
** Description:             Start a new trace
***************************************************************************************/
void TraceWidget::startTrace(uint16_t ptColor)
{
  _newTrace = true;
  _ptColor = ptColor;
  _xpt = 0;
  _ypt = 0;
}

/***************************************************************************************
** Function name:           addPoint
** Description:             Add new point on graph
***************************************************************************************/
bool TraceWidget::addPoint(float xval, float yval)
{
  // Map point to display pixel coordinate
  _xpt = 0.5 + _gw->getPointX(xval);
  _ypt = 0.5 + _gw->getPointY(yval);

  // If it is a new trace then a single pixel is drawn
  if (_newTrace)
  {
    _newTrace = false;
    _xval = xval;
    _yval = yval;
  }

  // Draw the line segment on graph (gets clipped to graph area)
  bool updated = _gw->addLine(_xval, _yval, xval, yval, _ptColor);

  // Store last graph scale point for this trace
  _xval = xval;
  _yval = yval;

  // Returns true if part of resulting line drawn in graph area
  return updated;
}

/***************************************************************************************
** Function name:           getLastPointX
** Description:             Get x pixel coordinates of last point plotted
***************************************************************************************/
uint16_t TraceWidget::getLastPointX(void)
{
  return _xpt;
}

/***************************************************************************************
** Function name:           getLastPointX
** Description:             Get y pixel coordinates of last point plotted
***************************************************************************************/
uint16_t TraceWidget::getLastPointY(void)
{
  return _ypt;
}
