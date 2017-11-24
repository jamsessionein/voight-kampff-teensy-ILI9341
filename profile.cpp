#include "Arduino.h"
#include "profile.h"
#include "graphics.c"
#include "dialog.h"

Profile::Profile(ILI9341_t3 &tftdev, UI &uidev){
  tft = &tftdev;
  ui = &uidev;
  profileImageX = ui->leftMargin+ui->profileGridPadding;
  profileImageY = ui->profileNameFieldY+ui->profileGridPadding;
  passwordLength = random(7,10);
  generateSecurityCode();
}

void Profile::drawProfileImage(){
  tft->fillRect(profileImageX+1, profileImageY+1, ui->profileGridWidth-2, ui->profileGridHeight-2, EYECOLOR1);
  if(userSelect == 0){
    //  Profile belongs to K.
    tft->drawBitmap(profileImageX, profileImageY, kBitmapLow, 120, 120, EYECOLOR2);
    tft->drawBitmap(profileImageX, profileImageY, kBitmapMed, 120, 120, EYECOLOR3);
    tft->drawBitmap(profileImageX, profileImageY, kBitmapHigh, 120, 120, EYECOLOR4);
    ui->drawProfileImageFrame(MEDBLUEUI);
  } else if (userSelect == 1){
    //  Profile belongs to Deckard (Young)
    tft->drawBitmap(profileImageX, profileImageY, deckardBitmapLow, 120, 120, EYECOLOR2);
    tft->drawBitmap(profileImageX, profileImageY, deckardBitmapMed, 120, 120, EYECOLOR3);
    tft->drawBitmap(profileImageX, profileImageY, deckardBitmapHigh, 120, 120, EYECOLOR4);
    ui->drawProfileImageFrame(MEDBLUEUI);
  } else {
    //  Custom user profile images.
    if(strlen_P(customUserBitmapLow) == 0){
      //  No profile picture was set for the custom user. Let's default to a placeholder.
      byte markWidth = 30;
      byte markHeight = 30;
      ui->drawXMark((ui->profileGridWidth/2-markWidth/2)+ui->leftMargin+ui->profileGridPadding, (ui->profileGridHeight/2-markHeight/2)+ui->profileNameFieldY+ui->profileGridPadding, markWidth, markHeight, 3, ANALYSIS1);
      char buffer[20];
      strcpy_P(buffer, noUserImageFound);
      byte textWidth = strlen(buffer)*6;
      tft->setCursor((ui->profileGridWidth/2 - textWidth/2)+ui->leftMargin+ui->profileGridPadding, ui->profileSummaryFieldY - 14);
      tft->setTextColor(ANALYSIS1);
      tft->print(buffer);
    } else {
      tft->drawBitmap(profileImageX, profileImageY, customUserBitmapLow, 120, 120, EYECOLOR2);
      tft->drawBitmap(profileImageX, profileImageY, customUserBitmapMed, 120, 120, EYECOLOR3);
      tft->drawBitmap(profileImageX, profileImageY, customUserBitmapHigh, 120, 120, EYECOLOR4);
      ui->drawProfileImageFrame(MEDBLUEUI);
    }
  }
}

void Profile::generateSecurityCode(){
  if(userSelect == 0 || userSelect == 1){
    //  This is either the profile for K or Deckard, which has been set up in advance, so use those.
    strcpy_P(securityCode, userSecCode[userSelect]);
  } else {
    //  This is a custom user profile. Let's generate a security code for them.
    char buffer[13];
    char tempUserName[25];
    strcpy_P(tempUserName, userName[userSelect]);
    randomSeed(hash((unsigned char*)tempUserName)); //  This should make it so it generates the same numbers consistently?
    sprintf(buffer,"%d%d%d%d%d-%c-%d%d%d%d", random(0,10), random(0,10), random(0,10), random(0,10), random(0,10), tempUserName[0], random(0,10), random(0,10), random(0,10), random(0,10));
    strcpy(securityCode, buffer);
  }
}

unsigned long Profile::hash(unsigned char *str){
  unsigned long hash = 5381;
  int c;
  while (c = *str++)
      hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash;
}

void Profile::writeUserData(uint16_t color){
  writeProfileName(color);
  writeAccessCode(color);
  if(userSelect != 1 && userSelect !=4){
    ui->drawProfileCheckbox(MEDBLUEUI, true);
  }
  writeUserDetails(color);
  writeAuthorizationText(color);
  writeLicenseText(color);
  drawLAPDEagles(MEDBLUEUI, 100);
}

void Profile::writeProfileName(uint16_t color){
  int leftTextX = ui->leftMargin+4;
  int leftTextY = ui->topGutterY+4;
  tft->setCursor(leftTextX, leftTextY);
  tft->setTextColor(color);
  tft->print(F("OFFICER"));
  tft->setCursor(leftTextX, leftTextY+12);
  tft->setTextColor(LIGHTBLUEUI);
  tft->setTextSize(2);
  tft->print((char*)pgm_read_word(&(userName[userSelect])));
  tft->setTextSize(1);
}

void Profile::writeAccessCode(uint16_t color){
  byte textX = ui->rightMargin-4-6*6;  //  6 here for the 6 characters in "ACCESS"
  tft->setCursor(textX, ui->topGutterY+4);
  tft->setTextColor(color);
  tft->print(F("ACCESS"));
  char buffer[passwordLength];
  for(int i=0; i<passwordLength; i++){
    buffer[i] = '*';
  }
  tft->setCursor(ui->rightMargin-4-passwordLength*6, ui->topGutterY+16);
  tft->setTextColor(color);
  tft->print(buffer);
}

void Profile::writeUserDetails(uint16_t color){
  //  Start by writing the user Description - Human, Replicant, or somewhere in between!
  byte textX = ui->profileUserDescX+8;
  byte textY = ui->profileNameFieldY+8;
  tft->setCursor(textX, textY);
  tft->setTextColor(color);
  char userDescBuffer[25];
  strcpy_P(userDescBuffer, userDesc[userSelect]);
  if(userSelect != 1 && userSelect != 4){
    //  Human or Replicant type is known; print user description as usual.
    tft->print(userDescBuffer);
  } else {
    //  We have some kind of indeterminate type of user, so print garbage.
    byte randomLength = random(3,7);
    for(int i=0; i<randomLength; i++){
      tft->print(userDescBuffer[i]);
    }
    tft->print(" ");
    tft->print(F("HUMAN"));
    tft->setTextColor(ANALYSIS1);
    tft->setCursor(textX+(randomLength-3)*6, textY+2);
    randomLength = random(4,8);
    for(int i = 0; i<randomLength; i++){
      tft->print(garbageTextArray[random(0,11)]);
    }
  }
  //  Description done. Now, let's write the User's Security Code.
  textY += 15;
  tft->setTextColor(color);
  tft->setCursor(textX, textY);
  char buffer[15];
  strcpy_P(buffer, secCode);
  tft->print(buffer);
  //  Security Code Field Name done - print the code below this.
  textY+=12;
  tft->setCursor(textX, textY);
  tft->print(securityCode);
  textY += 14;
  //  Security Code done. Now, print the Blade Runner text.
  tft->drawBitmap(textX, textY, blade_runner_japanese, 88, 11, LIGHTBLUEUI);
  textY += 15;
  tft->setCursor(textX, textY);
  tft->setTextColor(LIGHTBLUEUI);
  char bladeRunnerText[] = "BLADE RUNNER";
  byte tempX = textX;
  for(int i = 0; i<strlen(bladeRunnerText); i++){
    tft->print(bladeRunnerText[i]);
    tempX += 7;
    tft->setCursor(tempX, textY);
  }
  textY += 12;
  tft->drawBitmap(textX, textY, blade_runner_chinese, 56, 11, LIGHTBLUEUI);
  //  Blade Runner text done. Now, print a manufacturer brand if applicable (Replicant)
  textY +=19;
  if(userSelect == 0 || userSelect == 3){
    if(replicantType == 0){
      //  WALLACE CORP LOGO - This is a Wallace Replicant.
      tft->drawBitmap(textX, textY, wallaceCorp, 88, 26, MEDBLUEUI);
    } else {
      //  TYRELL CORP LOGO - How weird! This is a Tyrell Replicant.
      tft->drawBitmap(textX, textY, tyrellCorp, 80, 28, MEDBLUEUI);
    }
  }
}

void Profile::writeAuthorizationText(uint16_t color){
  //  Set up some useful variables for referencing specific X/Y coordinates during this print.
  int textMargin = 8;
  int textFieldWidth = ui->rightMargin-ui->leftMargin-textMargin*2;
  int textXStart = ui->leftMargin+textMargin;
  int textXEnd = textXStart+textFieldWidth;
  int currentTextX = textXStart;
  int textYStart = ui->profileSummaryFieldY+textMargin;
  int currentTextY = textYStart;
  tft->setTextColor(color);
  tft->setCursor(textXStart, textYStart);
  //  We use strtok to tokenize a string, but that string needs to be editable, as strtok inserts a NULL in wherever
  //  it finds a delimiter. Our dialog.h strings are not editable in their current format, so we need to make a local
  //  copy for the purpose of this function. We're doing that here, and calling it "authMessage"
  char authMessage[strlen(profileAuthMessage)];
  strcpy_P(authMessage, profileAuthMessage);
  int wordCount = 0;
  //  Now we need to tokenize it. The below array assumes a 50-word maximum. My default in dialog.h is 35.
  char* tokens[50];
  for(char* p=strtok(authMessage, " "); p; p = strtok(NULL, " ")){
    tokens[wordCount++] = p;
  }
  //  Tokens is now established as an array of pointers to each individual word in authMessage.
  for(int word = 0; word<wordCount; word++){
    byte textWidth = (strlen(tokens[word])+1)*6;
    if(!strcmp(tokens[word],"DETAIN,") || !strcmp(tokens[word],"IDENTIFY,") || !strcmp(tokens[word],"RETIRE")){
      tft->setTextColor(ANALYSIS2);
    }
    if(textWidth+currentTextX < textXEnd){
      //  We're within the summary field boundaries; OK to print text.
      tft->setCursor(currentTextX, currentTextY);
      tft->print(tokens[word]);
    } else {
      //  Not enough room to print the word in question; drop down a line instead.
      currentTextY += 12;
      currentTextX = textXStart;
      tft->setCursor(currentTextX, currentTextY);
      tft->print(tokens[word]);
    }
    tft->setTextColor(color);
    currentTextX += textWidth;
  }
}

void Profile::writeLicenseText(uint16_t color){
  tft->setTextColor(color);
  byte fieldWidth = ui->rightMargin-ui->leftMargin;
  int textY = ui->profileLicenseFieldY+5;
  for(int i=0; i<2; i++){
    char buffer[30];
    strcpy_P(buffer, profileLicenseMessage[i]);
    byte textWidth = strlen(buffer)*6;
    byte textX = (fieldWidth/2-textWidth/2)+ui->leftMargin;
    textY += i*10;
    tft->setCursor(textX, textY);
    tft->print(buffer);
  }
}

void Profile::drawLAPDEagles(uint16_t color, int opacity){
  byte imageWidth = 32;
  byte imageHeight = 19;
  byte imageXMargin = 5;
  byte fieldHeight = ui->bottomMargin - ui->profileLicenseFieldY-1;
  int imageY = fieldHeight/2 - imageHeight/2 + ui->profileLicenseFieldY;
  tft->drawBitmap(ui->leftMargin+imageXMargin, imageY, lapdEagle, imageWidth, imageHeight, color, opacity);
  tft->drawBitmap(ui->rightMargin-imageXMargin-imageWidth, imageY, lapdEagle, imageWidth, imageHeight, color, opacity);
}
