#ifndef BATTERY_h
#define BATTERY_h

#include "Arduino.h"
#include "ILI9341_t3.h"

class Battery
{
  public:
    Battery(int x, int y, uint16_t colorParam, ILI9341_t3 &tftdev, int percent = 99);
    void drawBattery();
    void drawFrame();
    void drawBlips();
    int xPos;
    int yPos;
    uint16_t color;
    int batteryPercent;
    ILI9341_t3 *tft;
    uint16_t batteryColors[4];
};

#endif
