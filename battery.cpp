#include "Arduino.h"
#include "battery.h"
#include "graphics.c"

Battery::Battery(int x, int y, uint16_t colorParam, ILI9341_t3 &tftdev, int percent)
{
  xPos = x;
  yPos = y;
  color = colorParam;
  tft = &tftdev;
  batteryPercent = percent;
  batteryColors[0] = REDUI;
  batteryColors[1] = color;
  batteryColors[2] = color;
  batteryColors[3] = color;
}

void Battery::drawBattery()
{
  drawFrame();
  drawBlips();
}

void Battery::drawFrame()
{
  tft->drawRect(xPos, yPos, 23, 9, color);
  tft->fillRect(xPos + 23, yPos + 3, 2, 3, color);
}

void Battery::drawBlips()
{
    //  We're using the same X and Y reference as the battery frame, but we want to move inside it, so we're going to shift here.
  int xRef = xPos + 2;
  int yRef = yPos + 2;
  //  4 blips, times the percentage of battery power remaining, rounded upwards by the +1.
  byte batteryState = (4*batteryPercent/100)+1;
  for (int i = 0; i < batteryState; i++) {
    //  Last 25% = REDUI at the moment, anything about 25% will be the normal MEDBLUEUI color.
    tft->fillRect(xRef + (5 * i), yRef, 4, 5, (batteryColors[batteryState-1]));
  }
}
