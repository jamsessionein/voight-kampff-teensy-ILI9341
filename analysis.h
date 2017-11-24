#ifndef ANALYSIS_h
#define ANALYSIS_h

#include "Arduino.h"
#include "ILI9341_t3.h"

class Analysis
{
    public:
    Analysis(int x, int y, int fieldWidth, int fieldHeight, int columnsValue, int rowsValue,
       ILI9341_t3 &tftdev, int blipWidth = 11, int blipHeight = 6);
    ILI9341_t3 *tft;
    int xPos;
    int yPos;
    int width;
    int height;
    int columns;
    int rows;
    int blipWidth;
    int blipHeight;
    int redIndex1;
    int redIndex2;
    struct resultsArray {
      int x;
      int y;
      uint16_t color;
      bool visible;
    }results[48]; //  Putting in a fixed value here was easier than fighting with it.
                  //  48 represents columns * rows. Vectors/templates seemed too hard to bother.
    void generate();
    void populateBlip(int index, int x, int y, uint16_t color);
    void generateRedBlips();
    void drawArray();
};

#endif
