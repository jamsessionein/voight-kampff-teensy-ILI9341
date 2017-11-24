#include "Arduino.h"
#include "ui.h"
#include "graphics.c"
#include "dialog.h"

UI::UI(int w, int h, int marginValue, uint16_t colorParam, ILI9341_t3 &tftdev){
  screenWidth = w;
  screenHeight = h;
  margin = marginValue;
  width = screenWidth - margin*2;
  height = screenHeight - margin*2;
  topMargin = margin;
  leftMargin = margin;
  bottomMargin = screenHeight - margin-1;
  rightMargin = screenWidth - margin-1;
  topGutterY = topMargin+18;
  horizPrintBar = topGutterY+62;
  summaryFieldTop = margin+188;
  topLeftHeight = topGutterY+81;
  gridBottomY = margin+138;
  gridSide = 121;
  vertGutterRight = rightMargin-gridSide;
  vertGutterLeft = vertGutterRight-9;
  vertPrintBar = (vertGutterLeft-leftMargin)/2+leftMargin;
  gridOriginX = vertGutterRight;
  gridOriginY = topGutterY;
  genderLeftX = vertGutterLeft-35;
  batteryOriginX = rightMargin - 30;
  batteryOriginY = topMargin + 5;
  profileNameFieldY = topGutterY+35;
  profileGridPadding = 3;
  profileSummaryFieldY = profileGridPadding*2+profileNameFieldY+gridSide;
  profileUserDescX = profileGridPadding*2+leftMargin+gridSide;
  profileLicenseFieldY = bottomMargin - 27;
  profileGridHeight = profileSummaryFieldY-profileNameFieldY-profileGridPadding*2;
  profileGridWidth = profileUserDescX-leftMargin-profileGridPadding*2;
  color = colorParam;
  tft = &tftdev;
  awaitingTextIsVisible = false;
  awaitingTextIsChinese = false;
}

void UI::startLAPDScreen(){
  drawOuterUIBorder();
  drawInnerFieldGlow(12, margin, margin, width-1, height-1, GLOWEDGECOLOR, BACKGROUND);
  drawScanLines(26);
  drawLAPDStartGraphics();
}

void UI::drawMainUIGrid() {
  drawOuterUIBorder();
  //  Multiple individual dividing lines
  tft->drawFastHLine(leftMargin, topGutterY, width, color); //Horizontal line beneath LAPD Text
  tft->drawFastHLine(leftMargin, gridBottomY, width, color); //Horizontal line even with bottom of grid
  tft->drawFastHLine(leftMargin, topLeftHeight, vertGutterLeft - margin, color); //Horizontal line 2/3 down eye grid
  tft->drawFastHLine(leftMargin, summaryFieldTop, width, color); //Horizontal line above text summary field
  tft->drawFastVLine(vertGutterLeft, topGutterY, summaryFieldTop - topGutterY, color); //Left vertical gutter line
  tft->drawFastVLine(vertGutterRight, topGutterY, summaryFieldTop - topGutterY, color); //Right vertical gutter line, should be even with grid edge.
  tft->drawFastVLine(vertPrintBar, topGutterY, horizPrintBar-topGutterY, color); //Dividing line down left top to separate the fingerprint region.
  tft->drawFastHLine(leftMargin, horizPrintBar, vertGutterLeft-margin, color); //Horizontal line under the fingerprint area.
  tft->drawFastVLine(genderLeftX,horizPrintBar,topLeftHeight-horizPrintBar, color);
  tft->drawRect(gridOriginX, gridOriginY, gridSide, gridSide, color); // Rectangular outer border for eye scan area.
  //  Draws the region for the "L" and "R" fingerprint tag letters
  drawFingerprintTagBox(leftMargin, topGutterY, color);
  drawFingerprintTagBox(vertPrintBar, topGutterY, color);
  //  Renders a backlight glow on the bottom Summary field
  drawInnerFieldGlow(15, leftMargin, summaryFieldTop, rightMargin-leftMargin, bottomMargin-summaryFieldTop, GLOWEDGECOLOR, BACKGROUND);
  //  Fills a grid pattern in on the eye scan area
  drawEyeGrid(color);
  //  Corner dots on main UI elements for aesthetics
  drawCornerUIDots(WHITE);
  //  LAPD logo and chinese text in the top left
  drawLAPDTopLeft();
  //  "Greyed-out" gender boxes - barely visible until scan is done, to light one or the other of them up.
  drawGenderUIBoxes();
  drawRecordBox(vertGutterLeft - 15, summaryFieldTop-15, false, DARKBLUEUI);
}

void UI::drawOuterUIBorder(){
  tft->drawRect(leftMargin, topMargin, width, height, color); //outer UI borders
}

//  Draws the rectangular regions for "L" and "R" fingerprint tags.
void UI::drawFingerprintTagBox(int xPos, int yPos, uint16_t color){
  tft->drawRect(xPos, yPos, 11, 13, color);
}

//  Creates a glow in the designated area, fading from edgeColor to bgColor over Radius pixels.
//  Used for Main UI Summary Field and Start Screen.
void UI::drawInnerFieldGlow(int radius, int xPos, int yPos, int fieldWidth, int fieldHeight, uint16_t edgeColor, uint16_t bgColor){
  RGB color1 = tft->color565toRGB(edgeColor);
  RGB color2 = tft->color565toRGB(bgColor);
  RGB deltaColor = {uint8_t((color1.r-color2.r)/radius), uint8_t((color1.g-color2.g)/radius), uint8_t((color1.b-color2.b)/radius)};
  for(int i = 1; i<radius; i++){
    tft->drawFastVLine(xPos+i, yPos+1, fieldHeight-1, tft->color565(color1.r-deltaColor.r*i, color1.g-deltaColor.g*i,color1.b-deltaColor.b*i));
    tft->drawFastVLine(xPos+fieldWidth-i, yPos+1, fieldHeight-1, tft->color565(color1.r-deltaColor.r*i, color1.g-deltaColor.g*i,color1.b-deltaColor.b*i));
  }
}

void UI::drawEyeGrid(uint16_t color){
  drawVGridLines(gridOriginX, gridOriginY, gridSide, gridSide, 6, color);
  drawHGridLines(gridOriginX, gridOriginY, gridSide, gridSide, 6, color);
}

//  Used for generating vertical grid lines on certain areas (Eye grid, background behind logos on start screen)
void UI::drawVGridLines(int xPos, int yPos, int gridWidth, int gridHeight, int numberOfLines, uint16_t color){
  for(int x = xPos; x<= xPos+gridWidth; x+=gridWidth/numberOfLines){
    tft->drawFastVLine(x, yPos, gridHeight, color);
  }
}

//  Used for generating horizontal grid lines on certain areas (Eye grid, background behind logos on start screen)
void UI::drawHGridLines(int xPos, int yPos, int gridWidth, int gridHeight, int numberOfLines, uint16_t color){
  for(int y = yPos; y<= yPos+gridHeight; y+=gridHeight/numberOfLines){
    tft->drawFastHLine(xPos, y, gridWidth, color);
  }
}

//  Draws dots in the intersections of main UI element corners, mostly just because I thought it looked good
void UI::drawCornerUIDots(uint16_t color){
  tft->drawPixel(leftMargin, topMargin, color);
  tft->drawPixel(leftMargin, bottomMargin, color);
  tft->drawPixel(rightMargin, topMargin, color);
  tft->drawPixel(rightMargin, bottomMargin, color);
  //primary corners done
  drawUIDotsWithGutter(topGutterY, color);
  //beneath top gutter done
  tft->drawPixel(leftMargin,topLeftHeight, color);
  tft->drawPixel(vertGutterLeft, topLeftHeight, color);
  //2/3 down to grid done
  drawUIDotsWithGutter(gridBottomY, color);
  //beneath grid done
  drawUIDotsWithGutter(summaryFieldTop, color);
  //Top of summary field done
  //tft->drawPixel(leftMargin,uiSummaryHeaderHeight, WHITE);
  //tft->drawPixel(uiRightMargin, uiSummaryHeaderHeight, WHITE);
}

//  Some of the UI dots are in repeatable X-positions, but vary based on Y, so a quick function to do them all with variable Y saves memory.
void UI::drawUIDotsWithGutter(int yPos, uint16_t color){
  tft->drawPixel(leftMargin,yPos, color);
  tft->drawPixel(vertGutterLeft, yPos, color);
  tft->drawPixel(vertGutterRight, yPos, color);
  tft->drawPixel(rightMargin, yPos, color);
}

//  Draws the chinese characters for "POLICE" and the custom LAPD logo font in the top left of the Main UI
void UI::drawLAPDTopLeft(){
  tft->drawBitmap(leftMargin+4, topMargin+4, iconLAPDChinese, 24, 11, LIGHTBLUEUI);
  tft->drawBitmap(leftMargin+28, topMargin+4, iconLAPDText, 40, 11, LIGHTBLUEUI);
}

//  The initial call for the gender boxes, which draws them in a dark 'greyed-out' color. Order is [F] [M] on the screen.
void UI::drawGenderUIBoxes(){
  drawGenderBox(vertGutterLeft, horizPrintBar+4, DARKBLUEUI, true);
  drawGenderBox(vertGutterLeft, horizPrintBar+4, DARKBLUEUI, false);
}

//  Used to set up the initial gender boxes in a dark color, then again once gender is generated on a scan to color one in.
//  Assumes order of [F] [M] on the screen to make my life easier.
void UI::drawGenderBox(int xPos, int yPos,uint16_t color, bool genderM){
  char genderChar;
  if(genderM){
    //  The boolean indicates gender of M, so we change the character to draw and offset X to line up the correct box.
    genderChar = 'M';
    xPos -= 15;
  } else {
    //  The boolean indicates gender of F, so we change the character to draw and offset X to line up on the correct box.
    genderChar = 'F';
    xPos -= 30;
  }
  tft->setTextColor(color);
  tft->drawRect(xPos, yPos, 11, 11, color);
  tft->setCursor(xPos+3, yPos+2);
  tft->print(genderChar);
}

void UI::drawAwaitingText(uint16_t color){
  if(awaitingTextIsVisible == false){
    //  No text already showing, so we are OK to pick a language before display.
    int languageChance = random(0,100);
    if(languageChance>60){
      //  40% chance to display English
      awaitingTextIsChinese = false;
      drawAwaitingTextEnglish(color);
      awaitingTextIsVisible = true;
    } else {
      //  60% chance to display Chinese
      awaitingTextIsChinese = true;
      //  The following is just to make it look glitchy! Feel free to delete between the comments if you want normal.
          if(!random(0,3)){  //  Basically 2/3 of the 60% chance, so 40% of the time.
            drawAwaitingTextGlitch(color);
          }
      //
      drawAwaitingTextChinese(color);
      awaitingTextIsVisible = true;
    }
  } else {
    //  Awaiting text is already showing, so we must now undo it with the same as what's drawn.
    if(awaitingTextIsChinese){
      //  The text already on the screen is in Chinese, so we need to use the same function as before.
      drawAwaitingTextChinese(color);
      awaitingTextIsVisible = false;
    } else {
      //  The text on the screen is in English, so we use the same function as before.
      drawAwaitingTextEnglish(color);
      awaitingTextIsVisible = false;
    }
  }
}

void UI::drawAwaitingTextEnglish(uint16_t color){
  char buffer[30];
  strcpy_P(buffer, analysisWait);
  //  Get the length of the string in characters, multiply it by 6 based on pixel width for total text width
  byte textWidth = strlen(buffer)*6;
  //  Set the cursor so the text draws in the center of the summary field region based on the length of the string.
  tft->setCursor(screenWidth/2-textWidth/2, summaryFieldTop+(bottomMargin - summaryFieldTop)/2-7);
  tft->setTextColor(color);
  tft->print(buffer);
}

void UI::drawAwaitingTextChinese(uint16_t color){
  int xPos = screenWidth/2 - 32; //32 here is the width of the text, 64, divided by two.
  int yPos = summaryFieldTop+(bottomMargin-summaryFieldTop)/2-8;
  tft->drawBitmap(xPos, yPos, awaiting_data_chinese, 64, 11, color);
}

void UI::drawAwaitingTextGlitch(uint16_t color){
  drawAwaitingTextEnglish(color);
  char buffer[30];
  strcpy_P(buffer, analysisWait);
  byte textWidth = strlen(buffer)*6;
  delay(150);
  tft->setCursor(screenWidth/2-textWidth/2, summaryFieldTop+(bottomMargin - summaryFieldTop)/2-7);
  tft->setTextColor(BACKGROUND);
  for(int i = 0; i<strlen(buffer); i++){
    tft->print(garbageTextArray[random(0,11)]);
    delay(10);
  }
  delay(400);
  tft->fillRect(screenWidth/2-textWidth/2-1, summaryFieldTop+10, textWidth, (bottomMargin-summaryFieldTop)/2, BACKGROUND);
  delay(50);
}

void UI::drawScanLines(int lines){
  int numberOfLines = lines;
  int lineHeight = (screenHeight-margin*2)/numberOfLines;
  for(int y = margin+lineHeight; y < screenHeight-margin; y += lineHeight){
    tft->drawFastHLine(leftMargin, y, width, MEDBLUEUI);
  }
}

void UI::drawLAPDStartGraphics(){
  //  We've set up the general UI, now to draw the extras.
  graphic LAPD = {0,100,184,58,7}; // Didn't set initial X position; will calculate that based on graphic width.
  LAPD.x = (screenWidth-LAPD.w)/2;
  graphic bgRect = {(screenWidth-LAPD.w)/2-LAPD.pad, LAPD.y-LAPD.pad, LAPD.w+LAPD.pad*2, LAPD.h+LAPD.pad*2, 0};
  graphic subText = {0, bgRect.y+bgRect.h, 24, 11, 7}; // Didn't set initial X position; will calculate that based on graphic width.
  subText.x = screenWidth/2-subText.w/2;
  graphic vkLogo = {0,277,160,16,6}; // Didn't set initial X position; will calculate that based on graphic width.
  vkLogo.x = screenWidth/2-vkLogo.w/2;
  graphic vkRect = {vkLogo.x-vkLogo.pad, vkLogo.y-vkLogo.pad/2, vkLogo.w+vkLogo.pad*2, vkLogo.h+vkLogo.pad, 0};
  //  Draw background colors to cover scan line effect in certain areas.
  tft->fillRect(bgRect.x, bgRect.y, bgRect.w, bgRect.h, BACKGROUND);
  tft->fillRect(subText.x-subText.pad, subText.y-subText.pad/2, subText.w+subText.pad*2, subText.h+subText.pad, BACKGROUND); //  vertical padding on this will only be half the horizontal, for aesthetics.
  tft->fillRect(vkRect.x, vkRect.y, vkRect.w, vkRect.h, BACKGROUND);         //  vertical padding on this will only be half the horizontal, for aesthetics.
  //  Draw grid lines behind LAPD
  drawVGridLines(bgRect.x, bgRect.y, bgRect.w, bgRect.h, 20, DARKBLUEUI);
  drawHGridLines(bgRect.x, bgRect.y, bgRect.w, bgRect.h, 8, DARKBLUEUI);
  //  Draw grid lines behind VK Logo
  drawVGridLines(vkRect.x, vkRect.y, vkRect.w, vkRect.h, 18, DARKBLUEUI);
  drawHGridLines(vkRect.x, vkRect.y, vkRect.w, vkRect.h, 2, DARKBLUEUI);
  //  Draw all necessary images - LAPD logo, Chinese subtext, VK logo at footer
  tft->drawBitmap(subText.x, subText.y+1, iconLAPDChinese, subText.w, subText.h, LIGHTBLUEUI);
  tft->drawBitmap(vkLogo.x, vkLogo.y, vk_logo, vkLogo.w, vkLogo.h, LIGHTBLUEUI);
  tft->drawBitmap((screenWidth-LAPD.w)/2,LAPD.y, lapdLogo, LAPD.w, LAPD.h, LIGHTBLUEUI);
  //tft->drawBitmap((screenWidth-LAPD.w)/2-4, LAPD.y-3, lapdGlow1, LAPD.w+8, LAPD.h+6, LIGHTBLUEUI, 40);
  //tft->drawBitmap((screenWidth-LAPD.w)/2-4, LAPD.y-3, lapdGlow2, LAPD.w+8, LAPD.h+6, LIGHTBLUEUI, 30);
  //tft->drawBitmap((screenWidth-LAPD.w)/2-4, LAPD.y-3, lapdGlow3, LAPD.w+8, LAPD.h+6, LIGHTBLUEUI, 20);
}

void UI::drawRecordBox(int x, int y, bool strike, uint16_t color){
  byte width = 11;
  byte height = 11;
  tft->drawRect(x, y, width, height, color);
  if(strike){
    tft->drawLine(x,y+height, x+width, y, LIGHTBLUEUI);
  }
}

void UI::drawThumbprints(byte leftPrintIndex, byte rightPrintIndex){
  if(rightPrintIndex != 255){
    tft->drawBitmap((vertGutterLeft-vertPrintBar)/2+vertPrintBar-16, topGutterY+12, (uint8_t*)pgm_read_word(&(thumbImages[rightPrintIndex])), 32, 44, MEDBLUEUI);
  } else {
    drawXMark((vertGutterLeft-vertPrintBar)/2+vertPrintBar-5, (horizPrintBar-topGutterY)/2+topGutterY-5, 10, 10, 2, MEDBLUEUI);
  }
  if(leftPrintIndex != 255){
    tft->drawBitmap((vertPrintBar-leftMargin)/2+leftMargin-16, topGutterY+12, (uint8_t*)pgm_read_word(&(thumbImages[leftPrintIndex])), 32, 44, MEDBLUEUI);
  } else {
    drawXMark((vertPrintBar-leftMargin)/2+leftMargin-5, (horizPrintBar-topGutterY)/2+topGutterY-5, 10, 10, 2, MEDBLUEUI);
  }
  drawThumbprintMatch(leftPrintIndex, rightPrintIndex);
}

void UI::drawThumbprintSideTags(uint16_t color){
  tft->setTextColor(color);
  tft->setCursor(leftMargin+3, topGutterY+3);
  tft->print("L");
  tft->setCursor(vertPrintBar+3, topGutterY+3);
  tft->print("R");
}

void UI::drawThumbprintMatch(byte leftPrintIndex, byte rightPrintIndex){
  int x = leftMargin+3;
  int y = horizPrintBar+3;
  int w = genderLeftX-x-2;
  int h = topLeftHeight - y - 2;
  //tft->fillRect(x,y,w,h,DARKBLUEUI);
  //  Write text in 'match' from dialog.h (probably "MATCH")
  int languageRandomizer = random(0,100);
  if(languageRandomizer>40){
    //  This is the routine if we are going to display english. 60% of the time.
    char buffer[10];
    if((leftPrintIndex == 255 && rightPrintIndex != 255) || (rightPrintIndex == 255 && leftPrintIndex != 255)){
      //  Matched one thumbprint, but not both. Write partial.
      strcpy_P(buffer, partial);
    } else if (leftPrintIndex == 255 && rightPrintIndex == 255){
      //  Missing both thumbprints - pretty rare! Write the 'missing' text.
      strcpy_P(buffer, missing);
    } else {
      //  Both thumbprints present - write the normal match text.
      strcpy_P(buffer, match);
    }
    byte textX = w/2+x-((strlen(buffer)*6)/2);
    byte textY = h/2+y-4;
    tft->setCursor(textX,textY);
    tft->setTextColor(ANALYSIS1);
    tft->print(buffer);
  } else {
    //  Hey, we're writing in Chinese this time! Lets set some variables up for that.
    byte drawX = w/2+x-12; //12 here is half of 24, the width of the image.
    byte drawY = h/2+y -6; // 6 here is half of 11, the width of the image, rounded up.
    if((leftPrintIndex == 255 && rightPrintIndex != 255) || (rightPrintIndex == 255 && leftPrintIndex != 255)){
      //  Matched one thumbprint, but not both. Write the characters for 'partial'.
      tft->drawBitmap(drawX,drawY,match_partial_chinese, 24, 11, ANALYSIS1);
    } else if (leftPrintIndex == 255 && rightPrintIndex == 255) {
      //  Missing both thumbprints AND in Chinese - a truly rare chance! draw the characters for "NO DATA".
      drawX = drawX - 12; //  This graphic is 48 pixels wide, instead of 24, so we double our offset.
      tft->drawBitmap(drawX,drawY,match_no_data_chinese, 48, 11, ANALYSIS1);
    } else {
      //  Both thumbprints present - draw the chinese characters for 'match'.
      tft->drawBitmap(drawX, drawY, match_chinese, 24, 11, ANALYSIS1);
    }
    //  This is the routine if we are going to display Chinese. 40% of the time.

  }
}

void UI::drawWallaceLogo(int baseX, int baseY, int xSpacing){
  tft->fillRoundRect(baseX,baseY,5,26,2,REDUI);
  tft->fillRoundRect(baseX+xSpacing,baseY+8,5,18,2,REDUI);
  tft->fillRoundRect(baseX+xSpacing*2,baseY+8,5,18,2,REDUI);
}

void UI::drawXMark(int xPos, int yPos, int markWidth, int markHeight, int lineWidth, uint16_t color){
  for(int i = 0; i<lineWidth; i++){
    tft->drawLine(xPos+i,yPos,xPos+markWidth+i,yPos+markHeight,color);
    tft->drawLine(xPos+i,yPos+markHeight,xPos+markWidth+i,yPos,color);
  }
}

void UI::drawDNASamples(int xPos, int yPos, bool isReplicant){
  //tft.fillRect(xPos, yPos-23, 115, 20, DARKBLUEUI);   //  Background area for the DNA field
  //tft->drawBitmap(xPos-2, yPos-23, dnaStrand, 120, 20, MEDBLUEUI);   //  Draws DNA Bitmap
  tft->drawBitmap(xPos, yPos, GCAT, 8 , 30, ANALYSIS1);  //  Draws super-small GCAT letters. Bitmap was the most data efficient way.
  xPos = xPos+8;
  for(int i = 0; i<4; i++){
    drawDNABar(xPos, yPos+i*8, 105, 6); // drawn out a horizontal bar next to the GCAT text, now to do the lines.
    for(int x = xPos+2; x<xPos+103; x++){
      if(random(0,5) == 0){         //  simple one in five chance we draw a line
        if(random(0,20) == 0 && isReplicant){      //  simple one in fifteen chance of that line being red
          tft->drawFastVLine(x, yPos+i*8, 6, REDUI);
          tft->drawFastVLine(x-1, yPos+i*8, 6, REDUI);   //  We draw two lines here to make the red areas more visible.
        } else {
          tft->drawFastVLine(x, yPos+i*8, 6, LIGHTBLUEUI);
        }
      }
    }
  }
}

void UI::drawDNABar(int xPos, int yPos, byte barWidth, byte barHeight){
  tft->fillRect(xPos, yPos, barWidth, barHeight, ANALYSIS1);
}

void UI::drawFocalText(uint16_t color){
  byte firstNum = random(0,255);
  byte secondNum = random(0,255);
  byte thirdNum = random(0,10);
  char buffer[20];
  sprintf(buffer, "DE %d MM %d DR %d", firstNum, secondNum, thirdNum);
  byte textWidth = strlen(buffer)*6;
  tft->setCursor(((rightMargin-gridOriginX)/2+gridOriginX)-textWidth/2, gridBottomY+5);
  tft->setTextColor(color);
  tft->print(buffer);
}

void UI::drawProfileUI(uint16_t color){
  drawOuterUIBorder();
  tft->fillRect(leftMargin+1, topGutterY, width-2, profileNameFieldY-topGutterY, DARKBLUEUI);
  tft->drawFastHLine(leftMargin, topGutterY, width, color); // Horizontal line beneath LAPD Text
  tft->drawFastHLine(leftMargin, profileNameFieldY, width, color); // Horizontal line beneath the Profile page name field.
  tft->drawFastHLine(leftMargin, profileSummaryFieldY, width, color); // Horizontal line beneath profile image, above large text field.
  tft->drawFastHLine(leftMargin, profileLicenseFieldY, width, color); //  Horizontal line above LAPD property text
  tft->fillRect(leftMargin+1, profileLicenseFieldY+1, width-2, bottomMargin-profileLicenseFieldY-2, DARKBLUEUI);
  tft->drawFastVLine(profileUserDescX, profileNameFieldY, profileSummaryFieldY-profileNameFieldY, color); //  Vertical line between image and text on right
  drawLAPDTopLeft();  //  LAPD logo and Chinese text in the top left.
  drawInnerFieldGlow(15, leftMargin, profileSummaryFieldY, rightMargin-leftMargin, profileLicenseFieldY-profileSummaryFieldY, GLOWEDGECOLOR, BACKGROUND);
  drawProfileCornerDots(WHITE); //  White dots in the corners and intersections for aesthetics.
  drawProfileCheckbox(DARKBLUEUI, false);
  drawProfileImageFrame(MEDBLUEUI); //  Sets up area for profile image.
}

void UI::drawProfileImageFrame(uint16_t color){
  tft->drawRect(leftMargin+profileGridPadding, profileNameFieldY+profileGridPadding, profileGridWidth, profileGridHeight, color);
}

void UI::drawProfileCornerDots(uint16_t color){
  drawProfileBorderDots(color, topMargin);
  drawProfileBorderDots(color, topGutterY);
  drawProfileBorderDots(color, profileNameFieldY);
  drawProfileBorderDots(color, profileSummaryFieldY);
  drawProfileBorderDots(color, profileLicenseFieldY);
  drawProfileBorderDots(color, bottomMargin);
  tft->drawPixel(profileUserDescX, profileNameFieldY, color);
  tft->drawPixel(profileUserDescX, profileSummaryFieldY, color);
}

void UI::drawProfileBorderDots(uint16_t color, int y){
  tft->drawPixel(leftMargin, y, color);
  tft->drawPixel(rightMargin, y, color);
}

void UI::drawProfileCheckbox(uint16_t color, bool checked){
  byte xPos = rightMargin-16;
  byte yPos = profileNameFieldY+6;
  tft->drawRect(xPos, yPos, 11, 11, color);
  if(checked){
    tft->drawLine(xPos, yPos+11, xPos+11, yPos, LIGHTBLUEUI);
  }
}
