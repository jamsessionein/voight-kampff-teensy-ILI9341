#include "Arduino.h"
#include "analysis.h"
#include "graphics.c"

Analysis::Analysis(int x, int y, int fieldWidth, int fieldHeight, int columnsValue, int rowsValue,
  ILI9341_t3 &tftdev, int blipWidthValue, int blipHeightValue){
    tft = &tftdev;
    xPos = x;
    yPos = y;
    width = fieldWidth;
    height = fieldHeight;
    columns = columnsValue;
    rows = rowsValue;
    blipWidth = width/columns;//blipWidthValue;
    blipHeight = height/rows;//blipHeightValue;
}

void Analysis::generate(){
//  A pair of variables used to provide the relative coordinates for each blip in the array.
int x = 0;
int y = 0;
for (int i = 0; i < columns*rows; i++) {
  //  initial test for blip visibility, 50/50 odds.
  if(random(0,2)){
    // This blip will show! Lets fill it in.
    populateBlip(i, x*blipWidth+2, y*blipHeight, (uint16_t)pgm_read_word(&(analysisColors[random(0,3)])));
  } else {
    //  This blip will not show, so we won't worry about it.
    results[i].visible = false;
    //  analysisResults[i].x = 11*xPos;
    //  analysisResults[i].y = 6*yPos;
  }
  x++;
  if(x == columns){
    //  We have reached the rightmost blip and need to step down a row when generating X and Y coordinates.
    x = 0;
    y++;
  }
}
}

void Analysis::generateRedBlips(){
  //  Initial analysis pass is done - lets come up with indicies for two red 'detail' blips.
  //  We want this to be somewhere near the top of the rows, and would prefer it be somewhere
  //  on the right end of the array, for aesthetics sake
  redIndex1 = random(0,2) * rows + random(3,7);
  //  We want to generate two unique red blips, but on separate rows so their analysis lines into the picture don't overlap.
  //  Easiest way is to add a fixed amount to the first Index so that we guarantee it moves down a row (8 slots).
  //  As a result, redIndex2 will always be below redIndex1 on the chart.
  redIndex2 = redIndex1 + random(8,16);
  //  Populate the data for the red blip positions.
  populateBlip(redIndex1, (redIndex1%columns)*blipWidth+2, (redIndex1/columns)*blipHeight, REDUI);
  populateBlip(redIndex2, (redIndex2%columns)*blipWidth+2, (redIndex2/columns)*blipHeight, REDUI);
}

void Analysis::populateBlip(int index, int x, int y, uint16_t color){
  results[index].x = x;
  results[index].y = y;
  results[index].visible = true;
  results[index].color = color;
}

void Analysis::drawArray(){
  tft->fillRect(xPos-1,yPos-1,width-3, height-3,DARKBLUEUI);
  for(int i = 0; i<columns*rows; i++){
    if(results[i].visible){
      tft->fillRect(xPos+results[i].x, yPos+results[i].y, 8, 4, results[i].color);
    }
  }
  //generateEyeTargets(0);
  //generateEyeTargets(1);
  //drawAnalysisLine(redIndex1);
  //drawAnalysisLine(redIndex2);
}

/*void drawAnalysisLine(int index){
  int anchorX = analysisResults[index].x+ui.leftMargin+11;
  int anchorY = analysisResults[index].y+4+ui.topGutterY+ui.topLeftHeight;
  tft.drawFastHLine(anchorX, anchorY, ui.vertGutterRight-anchorX , REDUI);
  tft.drawFastHLine(anchorX, anchorY+1, ui.vertGutterRight-anchorX, BACKGROUND);
}*/

/*void generateEyeTargets(int eyeValue){
  eye[eyeValue].targetX = random(eye[eyeValue].x, eye[eyeValue].x+eye[eyeValue].w);
  eye[eyeValue].targetY = random(eye[eyeValue].y, eye[eyeValue].y+eye[eyeValue].h);
  Serial.print((String)"Target values are X: "+eye[eyeValue].targetX+" and Y: "+eye[eyeValue].targetY);
}*/
