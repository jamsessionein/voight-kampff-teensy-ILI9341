#ifndef PROFILE_h
#define PROFILE_h

#include "Arduino.h"
#include "ILI9341_t3.h"
#include "ui.h"
#include "dialog.h"
#include "graphics.c"

class Profile
{
    public:
        Profile(ILI9341_t3 &tftdev, UI &uidev);
        ILI9341_t3 *tft;
        UI *ui;
        int profileImageX;
        int profileImageY;
        int passwordLength;
        char securityCode[13];

        void drawProfileImage();
        void generateSecurityCode();
        void writeUserData(uint16_t color);
        void writeProfileName(uint16_t color);
        void writeAccessCode(uint16_t color);
        void writeUserDetails(uint16_t color);
        void writeAuthorizationText(uint16_t color);
        void writeLicenseText(uint16_t color);
        void drawLAPDEagles(uint16_t color, int opacity=100);
        unsigned long hash(unsigned char *str);
};

#endif
