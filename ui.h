#ifndef UI_h
#define UI_h

#include "Arduino.h"
#include "ILI9341_t3.h"

class UI
{
  public:
    UI(int screenWidth, int screenHeight, int margin, uint16_t colorParam, ILI9341_t3 &tftdev);

    void startLAPDScreen();
    void drawMainUIGrid();
    void drawOuterUIBorder();
    void drawScanLines(int lines);
    void drawFingerprintTagBox(int xPos, int yPos, uint16_t color);
    void drawInnerFieldGlow(int radius, int xPos, int yPos,
    int fieldWidth, int fieldHeight, uint16_t edgeColor, uint16_t bgColor);
    void drawEyeGrid(uint16_t color);
    void drawVGridLines(int xPos, int yPos, int gridWidth, int gridHeight, int numberOfLines, uint16_t color);
    void drawHGridLines(int xPos, int yPos, int gridWidth, int gridHeight, int numberOfLines, uint16_t color);
    void drawCornerUIDots(uint16_t color);
    void drawUIDotsWithGutter(int yPos, uint16_t color);
    void drawLAPDTopLeft();
    void drawGenderUIBoxes();
    void drawGenderBox(int xPos, int yPos, uint16_t color, bool genderM);
    void drawAwaitingText(uint16_t color);
    void drawAwaitingTextEnglish(uint16_t color);
    void drawAwaitingTextChinese(uint16_t color);
    void drawAwaitingTextGlitch(uint16_t color);
    void drawLAPDStartGraphics();
    void drawWallaceLogo(int baseX, int baseY, int xSpacing = 7);
    void drawRecordBox(int x, int y, bool strike, uint16_t color);
    void drawThumbprints(byte leftPrintIndex, byte rightPrintIndex);
    void drawThumbprintSideTags(uint16_t color);
    void drawThumbprintMatch(byte leftPrintIndex, byte rightPrintIndex);
    void drawXMark(int xPos, int yPos, int markWidth, int markHeight, int lineWidth, uint16_t color);
    void drawDNASamples(int xPos, int yPos, bool isReplicant);
    void drawDNABar(int xPos, int yPos, byte barWidth, byte barHeight);
    void drawFocalText(uint16_t color);
    void drawProfileUI(uint16_t color);
    void drawProfileImageFrame(uint16_t color);
    void drawProfileBorderDots(uint16_t color, int y);
    void drawProfileCornerDots(uint16_t color);
    void drawProfileCheckbox(uint16_t color, bool checked);

    bool awaitingTextIsVisible;
    bool awaitingTextIsChinese;

    ILI9341_t3 *tft;
    int screenWidth;
    int screenHeight;
    int margin;
    int width;
    int height;
    int topMargin;
    int leftMargin;
    int bottomMargin;
    int rightMargin;
    int topGutterY;
    int vertGutterRight;
    int vertGutterLeft;
    int vertPrintBar;
    int horizPrintBar;
    int summaryFieldTop;
    int topLeftHeight;
    int gridBottomY;
    int gridSide;
    int gridOriginX;
    int gridOriginY;
    int batteryOriginX;
    int batteryOriginY;
    int genderLeftX;
    int profileNameFieldY;
    int profileGridPadding;
    int profileSummaryFieldY;
    int profileUserDescX;
    int profileLicenseFieldY;
    int profileGridHeight;
    int profileGridWidth;
    uint16_t color;

    struct graphic{int x;  int y;  int w;  int h;  int pad;};
};

#endif
