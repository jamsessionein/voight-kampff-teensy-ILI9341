#include "Arduino.h"
#include "scan.h"
#include "dialog.h"
#include "graphics.c"

Scan::Scan(ILI9341_t3 &tftdev, UI &uidev)
{
  tft = &tftdev;
  ui = &uidev;
  sprintf(statLetters, "ABCSDF");
}

void Scan::generateInceptDate(){
  day = random(1,29);
  month = random(1,12);
  year = random(17,42);
}

//  Simple 0 or 1 function to set the boolean isReplicant. Uses analogRead on Pin 0 to get a random number without needing to call
//  random(), which took up more memory.
void Scan::generateHumanOrReplicant(){
  isReplicant = random(0,2); //  True or false.
  if(year>=36){ //  Nexus 6 had a 4-year lifespan, all dead by now. Nexus 7 was probably just Rachael? Nexus 8 were pre-blackout, and Nexus 9 started after 2036.
    nexusGen = 9;
  } else {
    nexusGen = 8;
  }
}

void Scan::generateSummaryGender(){
  isMale = random(0,2);   //  True or false.
}

void Scan::generateStateCharacters(){
  physState = randomStateCharacter();
  mentState = randomStateCharacter();
}

void Scan::generateThumbprintData(){
  leftPrintIndex = random(0,5);
  rightPrintIndex = random(0,5);
  while(rightPrintIndex == leftPrintIndex){    // We want to make sure we get two different thumbprint images, so index1 can't equal index2.
    rightPrintIndex = random(0,5);
  }
  if(random(0,100)>90){
    leftPrintIndex = 255;
  }
  if(random(0,100)>90){
    rightPrintIndex = 255;
  }
}

char Scan::randomStateCharacter(){
  if(random(0,100)<90){ //  Rough 90% chance filter here - A, B, or C will follow
    return statLetters[random(0,3)];
  } else { // Rough 10% chance of an S, D, F, or ?
    return statLetters[random(0,3)+3];
  }
}

void Scan::generateReplicantID(){
  char gender;
  if (isMale){
    gender = 'M';
  } else {
    gender = 'F';
  }
  sprintf(nexusID,"N%d-%c%c%c%d%02d%02d",nexusGen,gender,physState,mentState,month,day,year);
}

//  Derives a first name and last name from the tables in names.h, then combines them and sets the value of summaryName to the result.
void Scan::generateSummaryName(){
    byte first;
    if(isMale){
      first = random(0,23); //generate a number to pick a random first name from the array in names.h
      strcpy_P(summaryName, (char*)pgm_read_word(&(firstNameMale[first])));
    } else {
      first = random(0,23);
      strcpy_P(summaryName, (char*)pgm_read_word(&(firstNameFemale[first])));
    }
    strcat(summaryName, " ");
    byte last = random(0,48); //  generate a number to pick a random last name from the array in names.h
    strcat_P(summaryName,(char*)pgm_read_word(&(lastName[last])));
    strcat(summaryName,'\0'); //  summaryName has now been assembled into [Firstname][space][Lastname]
    Serial.println(summaryName);
}

//  Simple 0 or 1 function to set the boolean isCriminal.
void Scan::generateCriminalStatus(){
  isCriminal = random(0,2);   //  True or false.
}
//////////////////////////////////
//  WRITING FUNCTIONS
//////////////////////////////////
void Scan::writeNexusID(int xPos, int yPos, uint16_t color){
  tft->setTextColor(color);
  tft->setCursor(xPos, yPos);
  tft->print(nexusID);
}

//  Parent function that fills the Summary field with text about the scanned target.
void Scan::writeSummary(int xPos,int yPos,int summaryWidth, uint16_t color){
  tft->setTextColor(color);
  writeName(xPos, yPos, summaryWidth);
  yPos = stepDownYPosition(yPos);
  writeIncept(xPos, yPos, summaryWidth);
  yPos = stepDownYPosition(yPos);
  writeFunction(xPos, yPos, summaryWidth);
  yPos = stepDownYPosition(yPos);
  writePhysState(xPos, yPos, summaryWidth);
  yPos = stepDownYPosition(yPos);
  writeMentState(xPos, yPos, summaryWidth);
  yPos = stepDownYPosition(yPos);
  writeArrestStatus(xPos, yPos, summaryWidth);
  yPos = stepDownYPosition(yPos);
  if(isCriminal){
    writeCrimes(xPos, yPos, summaryWidth);
  }
}

//  A function to move the writing line down 15 pixels between lines, to ensure proper spacing.
int Scan::stepDownYPosition(int yPos){
  return yPos+15;
}

//  Draws a line above the current line of text, assuming it will be underlining text above.
void Scan::drawLineAboveText(int xPos, int yPos, int summaryWidth){
    tft->drawFastHLine(xPos, yPos-4,summaryWidth, DARKBLUEUI);
}

//  Writes the scanned target's generated name.
void Scan::writeName(int xPos, int yPos, int summaryWidth){
  tft->setCursor(xPos, yPos);
  tft->print(F("NAME"));
  //  calculating our new X position for the right-aligned text based on text length.
  byte textX = xPos+summaryWidth-(strlen(summaryName)*6);
  tft->setCursor(textX, yPos);
  tft->print(summaryName);
}

//  Writes the scanned target's Incept (if Replicant) or Birth (if Human) date. Generated during this function and not saved.
void Scan::writeIncept(int xPos, int yPos, int summaryWidth){
  //  Add a separation line between this text and the text above it.
  drawLineAboveText(xPos, yPos, summaryWidth);
  tft->setCursor(xPos, yPos);
  //  Print a variable field name depending on if the target is human or replicant.
  if(isReplicant){
    tft->print(F("INCEPT DATE"));
  } else {
    tft->print(F("DATE OF BIRTH"));
  }
  //  Makes a random date of birth or incept date in MM/DD/YYYY format 90% of the time. The other 10% of the time, it will display
  //  the text "REDACTED". Uses analogRead on Pin 0 in place of random() to save a bit of memory. Because I'm lazy, the month field
  //  can only display numbers from 01 through 09, as I'd waste memory storing and checking one-digit versus two-digit month numbers
  //  for formatting purposes. Nobody but you and I, faithful reader, will ever know!

  if(random(0,100)<90){                 //  90% chance filter for normal return
    char buffer[10];
    sprintf(buffer,"%02d/%02d/20%02d", month, day, year);
    byte textX = xPos+summaryWidth-60;  //  MM/DD/YYYY is 10 characters, times 6 pixel character width = 60
    tft->setCursor(textX, yPos);
    tft->print(buffer);
  } else {
    //  Look at the text in the char array 'redactedText' in names.h, determine width, offset X from right accordingly.
    char buffer[30];
    strcpy_P(buffer, redactedText);
    byte textX = xPos+summaryWidth-(strlen(buffer)*6);
    tft->setCursor(textX, yPos);
    tft->print(buffer);
  }
}

//  Writes the scanned target's Function (if Replicant) or Profession (if Human)
void Scan::writeFunction(int xPos, int yPos, int summaryWidth){
  //  Add a separation line between this text and the text above it.
  drawLineAboveText(xPos, yPos, summaryWidth);
  tft->setCursor(xPos, yPos);
  char buffer[30];
  //  Generate a number to use as an index to read the functions[] char array in names.h
  byte functionIndex = random(0,21);
  //  Print a variable field name depending on if the target is human or replicant.
  if(isReplicant){
    tft->print(F("FUNCTION"));
    strcpy_P(buffer,(char*)pgm_read_word(&(professionReplicant[functionIndex])));
    byte textX = xPos+summaryWidth-strlen(buffer)*6;
    tft->setCursor(textX, yPos);
    tft->print(buffer);
  } else {
    tft->print(F("PROFESSION"));
    strcpy_P(buffer,(char*)pgm_read_word(&(professionHuman[functionIndex])));
    byte textX = xPos+summaryWidth-strlen(buffer)*6;
    tft->setCursor(textX, yPos);
    tft->print(buffer);
  }
}

//  Writes the scanned target's Physical state.
void Scan::writePhysState(int xPos, int yPos, int summaryWidth){
//  Add a separation line between this text and the text above it.
  drawLineAboveText(xPos, yPos, summaryWidth);
  tft->setCursor(xPos, yPos);
  tft->print(F("PHYSICAL STATE"));
  //  If the target is a replicant, we'll give a 'LEV X' value. Otherwise, we'll show 'N/A', on the assumption it can't read humans.
  if(isReplicant){
     writeLevelData(xPos, yPos, summaryWidth, true);
  } else {
     writeLevelNA(xPos, yPos, summaryWidth);
  }
}

//  Writes the scanned target's Mental state.
void Scan::writeMentState(int xPos, int yPos, int summaryWidth){
  //  Add a separation line between this text and the text above it.
  drawLineAboveText(xPos, yPos, summaryWidth);
  tft->setCursor(xPos, yPos);
  tft->print(F("MENTAL STATE"));
  //  If the target is a replicant, we'll give a 'LEV X' value. Otherwise, we'll show 'N/A', on the assumption it can't read humans.
  if(isReplicant){
     writeLevelData(xPos, yPos, summaryWidth, false);
  } else {
     writeLevelNA(xPos, yPos, summaryWidth);
  }
}


//  Used only for Replicants - generates 'LEV X' value and displays. Value is not saved.
void Scan::writeLevelData(int xPos, int yPos, int summaryWidth, bool phys){
     byte textX = xPos+summaryWidth-30; //"LEV _" is 5 characters, times 6 pixel character width = 30
     tft->setCursor(textX, yPos);
     tft->print(F("LEV "));
     if(phys){
     tft->print(physState);
   } else {
     tft->print(mentState);
   }
}

//  Used only for Humans - assumes the scanner can't pick this information up about humans.
void Scan::writeLevelNA(int xPos, int yPos, int summaryWidth){
    byte textX = xPos+summaryWidth-18; //"N/A" is 3 characters, times 6 pixel character width = 18
    tft->setCursor(textX, yPos);
    tft->print(F("N/A"));
}

void Scan::writeArrestStatus(int xPos, int yPos, int summaryWidth){
  //  Add a separation line between this text and the text above it.
  drawLineAboveText(xPos, yPos, summaryWidth);
  tft->setCursor(xPos, yPos);
  tft->print(F("ARREST STATUS"));
  if(isCriminal){
    //  Set the X based on the wanted text char array in names.h plus 5 more characters for a random warrant number.
    char buffer[30];
    strcpy_P(buffer, wanted);
    byte textX = xPos+summaryWidth-((strlen(buffer)+5)*6);
    tft->setCursor(textX, yPos);
    tft->setTextColor(REDUI);
    tft->print(buffer);
    //  Generate a fake warrant number, five digits, each between 1 and 9.
    for(byte i = 0; i<5; i++){
      tft->print(random(1,10));
    }
  } else {
    char buffer[30];
    strcpy_P(buffer, notWanted);
    byte textX = xPos+summaryWidth-(strlen(buffer)*6);
    tft->setCursor(textX, yPos);
    tft->print(buffer);
  }
}

void Scan::writeCrimes(int xPos, int yPos, int summaryWidth){
  //  Add a separation line between this text and the text above it.
  drawLineAboveText(xPos, yPos, summaryWidth);
  byte crimeCount = analogRead(0)%2+1; //(1-2)
  byte crimeIndex1 = random(0,15);
  byte crimeIndex2 = random(0,15);
  while(crimeIndex2 == crimeIndex1){
    crimeIndex2 = random(0,15);
  }
  //By this point, we should have three unique indicies for the crimes.
  char crimeBuffer[39];
  strcpy_P(crimeBuffer, (char*)pgm_read_word(&(crimeList[crimeIndex1])));
  if(crimeCount > 1){
    strcat(crimeBuffer, " / ");
    strcat_P(crimeBuffer,(char*)pgm_read_word(&(crimeList[crimeIndex2])));
  }
  strcat(crimeBuffer, '\0');
  //  Full crime list string generated. Calculate width and right-align.
  byte textX = xPos+summaryWidth-(strlen(crimeBuffer)*6);
  tft->setCursor(textX, yPos);
  tft->print(crimeBuffer);
}

void Scan::writeRecordArea(){
  int topLeftX = ui->leftMargin+5;
  int topLeftY = ui->gridBottomY+5;
  tft->setCursor(topLeftX, topLeftY);
  tft->setTextColor(MEDBLUEUI);
  if(isReplicant){
    tft->print(F("IDENT CONFIRM"));
    writeNexusID(topLeftX+1, topLeftY+12, LIGHTBLUEUI);
  } else {
    tft->print(F("NO SYNTHETIC"));
  }
  tft->setCursor(topLeftX, ui->summaryFieldTop-13);
  tft->setTextColor(MEDBLUEUI);
  tft->print(F("LOC RECORD"));
  ui->drawRecordBox(ui->vertGutterLeft - 15, ui->summaryFieldTop-15, true, MEDBLUEUI);
}
