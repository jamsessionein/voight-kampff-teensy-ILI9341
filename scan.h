#ifndef SCAN_h
#define SCAN_h

#include "Arduino.h"
#include "ILI9341_t3.h"
#include "ui.h"

class Scan
{
  public:
    Scan(ILI9341_t3 &tftdev, UI &uidev);
    char summaryName[22];
    bool isReplicant;
    bool isMale;
    bool isCriminal;
    ILI9341_t3 *tft;
    UI *ui;
    byte nexusGen;
    char nexusID[12];
    char physState;
    char mentState;
    char statLetters[6];
    byte day;
    byte month;
    byte year;
    byte leftPrintIndex;
    byte rightPrintIndex;

    void generateInceptDate();
    void generateHumanOrReplicant();
    void generateSummaryGender();
    void generateStateCharacters();
    void generateReplicantID();
    void generateSummaryName();
    void generateCriminalStatus();
    void generateThumbprintData();
    void writeNexusID(int xPos, int yPos, uint16_t color);
    void writeSummary(int xPos,int yPos,int summaryWidth, uint16_t color);
    void writeName(int xPos, int yPos, int summaryWidth);
    void writeIncept(int xPos, int yPos, int summaryWidth);
    void writeFunction(int xPos, int yPos, int summaryWidth);
    void writePhysState(int xPos, int yPos, int summaryWidth);
    void writeMentState(int xPos, int yPos, int summaryWidth);
    void writeLevelData(int xPos, int yPos, int summaryWidth, bool phys);
    void writeLevelNA(int xPos, int yPos, int summaryWidth);
    void writeArrestStatus(int xPos, int yPos, int summaryWidth);
    void writeCrimes(int xPos, int yPos, int summaryWidth);
    void drawLineAboveText(int xPos, int yPos, int summaryWidth);
    void writeRecordArea();

  private:
    char randomStateCharacter();
    int stepDownYPosition(int yPos);
};

#endif
