#include <Arduino.h>
#include "SPI.h"
#include "ILI9341_t3.h"
#include "graphics.c"
#include "battery.h"
#include "dialog.h"
#include "scan.h"
#include "ui.h"
#include "analysis.h"
#include "profile.h"
#include "elapsedMillis.h"

#define ENABLE_ILI9341_FRAMEBUFFER

#define TFT_DC 9
#define TFT_CS 10
#define TFT_MOSI 11
#define TFT_MISO 12
#define TFT_RST 8
#define TFT_CLK 13 //Arduino SCK
#define TFT_LCD 6

#define TONE_PIN 3

//  We need to construct the TFT screen object from the library in order to start, you know, doing stuff to it. Better late than never.
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_CLK, TFT_MISO, TFT_LCD);

//  Construct a UI object that points at the TFT so it knows how to draw.
UI ui(240, 320, 4, MEDBLUEUI, tft);

//  Constructs the battery object, gives it origin data, and points it at the tft object above so it can be drawn.
Battery bat(ui.batteryOriginX, ui.batteryOriginY, MEDBLUEUI, tft);

//  Constructs the Scan object, points it at the TFT object so it can draw.
Scan scan(tft, ui);

//  Constructs the Profile object, contains device user data.
Profile user(tft, ui);

//  Constructs the analysis object, used to handle some target display graphics.
Analysis blips(ui.leftMargin+3, ui.topLeftHeight+3, ui.vertGutterLeft-ui.leftMargin, ui.gridBottomY-ui.topLeftHeight, 8, 6, tft);

bool showStartScreen = true;
elapsedMillis logoStartFadeTimer;
int screenBrightness = 0;

uint8_t use_fb = 1;
uint8_t use_clip_rect = 0;
uint8_t use_set_origin = 0;

#define ORIGIN_TEST_X 0
#define ORIGIN_TEST_Y 0

void setup() {
  Serial.begin(115200);
  //  Don't do anything until the Serial communication is established. Remove this when finalizing code.
  while(!Serial);
  Serial.println("Talking");
  //  Randomize the seed
  randomSeed(analogRead(0));
  //tft.setBrightness(0);
  tft.begin();
  initializeFlashEffect();
  SetupOrClearClipRectAndOffsets();
  tft.fillScreen(BACKGROUND);
  tft.setBrightness(255);
  for(int i = 0; i<=ui.screenHeight/2; i+=20){
    tft.setClipRect(0,ui.screenHeight/2-i, ui.screenWidth, i*2);
    ui.startLAPDScreen();
  }
  //startUpWhineSound();
  //tft.setBrightness(255);
  //tft.begin();
  //delay(500);
  //tft.setBrightness(0);
  //tft.fillScreen(BACKGROUND);
  //ui.startLAPDScreen();
  //delay(1000);
  //tft.fillScreen(BACKGROUND);
  //tft.setBrightness(120);
  //ui.drawMainUIGrid();
  //bat.drawBattery();
  //ui.drawAwaitingText(MEDBLUEUI);  //  Text to show in the bottom field while idle
  //delay(5000);  //  Temporary delay until fake "Scan"
  //scanTarget(); //  Runs collection of functions following the completion of a scan
  //delay(10000);
  //showUserProfile();
}

void loop() {
  /*if(showStartScreen){
    //  We're entering the loop on our first go through. Lets fade the start screen up.
    if(logoStartFadeTimer > 5 && screenBrightness <= 255){
      tft.setBrightness(screenBrightness);
      screenBrightness+=5;
      logoStartFadeTimer = 0;
    }
  }*/
}

void initializeFlashEffect(){
  //  Audio and brightness timers for the startup animation
  elapsedMillis freqTimer, brightnessTimer;
  int startFreq, currentFreq = 3000;
  int endFreq = 22000;
  int freqStep = (endFreq-startFreq)/255;
  int currentBrightness = 255;
  bool initialized = false;
  while(initialized == false){
    //  Audio stuff
    if(currentFreq<endFreq){
      //  The frequency hasn't reached our max yet, so we can still check to see if it's worth changing it.
      if(freqTimer > 2){
        //  Take a step with the tone frequency
        tone(TONE_PIN, currentFreq);
        freqTimer = 0;
        currentFreq += freqStep;
      }
    } else {
      noTone(TONE_PIN);
      initialized = true;
    }
    //  End Audio stuff
    //  Brightness/flash stuff
    if(currentBrightness>=0){
      if(brightnessTimer > 3){
        //  Take a step down with brightness
        tft.setBrightness(currentBrightness);
        brightnessTimer = 0;
        currentBrightness -= 5;
      }
    }
  }
}

/*void drawGlitch(){
  int w = random(20,80);
  int h = random(10,40);
  int x = random(0,screenWidth-80);
  int y = random(0,screenHeight-40);
  uint16_t pixelColorArray[(3 * w * h)+3];
  tft.readRect(x, y, w, h, pixelColorArray);
  tft.fillRect(x,y,w,h-1,REDUI);
  tft.writeRect(x,y,w,h-1,pixelColorArray);
}*/

void showUserProfile(){
  tft.fillScreen(BACKGROUND);
  ui.drawProfileUI(MEDBLUEUI);
  bat.drawBattery();
  user.drawProfileImage();
  user.writeUserData(MEDBLUEUI);
}


void scanTarget(){
  generateScanData();     //  Generates necessary Scan data in object 'scan' for further work
  blips.generate();    //  Populates results[] in Analysis object 'blips' with information for later drawing.
  if(scan.isReplicant){
    blips.generateRedBlips();  //  Adds red blips to the analysis array.
  }
  tft.setBrightness(255);
  renderScanGraphics();   //  Draws scan images onto UI
  ui.drawAwaitingText(BACKGROUND);  //  Time to hide the idle text
  scan.writeSummary(ui.leftMargin+5, ui.summaryFieldTop+8, ui.width-10, MEDBLUEUI);   //  Writes out summary text in bottom region
  scan.writeRecordArea();
  ui.drawDNASamples(ui.vertGutterRight+4, ui.summaryFieldTop-33, scan.isReplicant);
  blips.drawArray();  //  Draws an infographic based on analysisResults[].
}

void generateScanData(){
  scan.generateInceptDate();
  scan.generateHumanOrReplicant();   //  Will affect the titles of the summary fields (i.e. 'date of birth' v 'incept date'), and some further generation parameters.
  scan.generateSummaryGender();      //  Calculating this here so I can figure out which letter to append into a Replicant's serial number if needed.
  scan.generateThumbprintData();
  scan.generateStateCharacters();    //  Calculating this here so I can figure out which letter to append into a Replicant's serial number if needed.
  if(scan.isReplicant){
    scan.generateReplicantID();
  }
  scan.generateSummaryName();        //  Produces a string based on firstNameMale[] or firstNameFemale[] plus lastName[] tables in names.h
  scan.generateCriminalStatus();      //  Determine if this individual is flagged as a criminal or not.
}

void renderScanGraphics(){
  ui.drawGenderBox(ui.vertGutterLeft, ui.horizPrintBar+4, LIGHTBLUEUI, scan.isMale);  // Highlights the correct gender box with brighter color
  ui.drawThumbprintSideTags(LIGHTBLUEUI);  //  Labels the thumbprint boxes "L" and "R"
  ui.drawThumbprints(scan.leftPrintIndex, scan.rightPrintIndex);  //  Draws a pair of random thumbprint graphics in the box spaces
  drawEye(ui.gridOriginX, ui.gridOriginY); // Draws the eye graphics, then redraws grid overtop
  ui.drawFocalText(MEDBLUEUI);
}


/////////////////////////////
// EYE GRAPHIC DRAW FUNCTIONS
/////////////////////////////

//void drawEye1(int xPos, int yPos) {
  //tft.fillRect(xPos + 1, yPos + 1, 119, 119, EYECOLOR1);
  //drawEyeGrid(xPos, yPos, MEDBLUEUI);
  //tft.drawBitmap(xPos + 1, yPos + 1, eye1Bitmap1, 120, 109, EYECOLOR2);
  //drawEyeGrid(xPos, yPos, MEDBLUEUI);
  //tft.drawBitmap(xPos + 1, yPos + 1, eye1Bitmap2, 120, 120, EYECOLOR3);
  //drawEyeGrid(xPos, yPos, MEDBLUEUI);
  //tft.drawBitmap(xPos + 1, yPos + 1, eye1Bitmap3, 120, 120, EYECOLOR4);
  //drawEyeGrid(MEDBLUEUI);
//}

void drawEye(int xPos, int yPos){
  tft.fillRect(xPos+1, yPos+1, 119, 119, EYECOLOR1);
  tft.drawBitmap(xPos+1, yPos+1, eye2Bitmap1, 120, 120, EYECOLOR2);
  tft.drawBitmap(xPos+1, yPos+1, eye2Bitmap2, 120, 120, EYECOLOR3);
  tft.drawBitmap(xPos+1, yPos+1, eye2Bitmap3, 120, 120, EYECOLOR4);
  drawEyeGrid(MEDBLUEUI);
}

void drawEyeGrid(uint16_t color) {
  ui.drawVGridLines(ui.gridOriginX, ui.gridOriginY, ui.gridSide, ui.gridSide, 6, color);
  ui.drawHGridLines(ui.gridOriginX, ui.gridOriginY, ui.gridSide, ui.gridSide, 6, color);
}


void SetupOrClearClipRectAndOffsets() {
  if (use_clip_rect) {
    tft.setClipRect();  // make sure we clear the whole screen
    tft.setOrigin();    // make sure none are set yet

    tft.fillScreen(ILI9341_LIGHTGREY);

    // Now lets set origin.
    if (use_set_origin)
      tft.setOrigin(ORIGIN_TEST_X, ORIGIN_TEST_Y);
    int x = tft.width() / 4;
    int y = tft.height() / 4;
    int w = tft.width() / 2;
    int h = tft.height() / 2;
    tft.drawRect(x, y, w, h, ILI9341_ORANGE);
    tft.updateScreen();
    tft.setClipRect(x + 1, y + 1, w - 2, h - 2);
    delay(250);

  } else {
    tft.setClipRect();
    if (use_set_origin)
      tft.setOrigin(ORIGIN_TEST_X, ORIGIN_TEST_Y);
    else
      tft.setOrigin();
  }
}
