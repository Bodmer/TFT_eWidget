/***************************************************************************************

  TFT display Widget library comprising following classes:
    1. ButtonWidget - button library
    2. SliderWidget - control sliders

***************************************************************************************/
#ifndef _TFT_eWidgetH_
#define _TFT_eWidgetH_

//Standard support
#include <Arduino.h>

// LittleFS is required for touch calibration in TFT_eSPI sketches
#if defined (ESP8266) || defined (ESP32)
  #include <pgmspace.h>
  #include <FS.h>
  #include <LittleFS.h>
#endif

#include <TFT_eSPI.h>

#include "widgets/button/ButtonWidget.h"
#include "widgets/slider/SliderWidget.h"
#include "widgets/meter/Meter.h"
#include "widgets/graph/GraphWidget.h"
#include "widgets/graph/TraceWidget.h"
#endif