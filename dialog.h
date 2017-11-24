#if defined(__AVR__)
    #include <avr/pgmspace.h>
#elif defined(__PIC32MX__)
    #define PROGMEM
#elif defined(__arm__)
    #define PROGMEM
#endif

#ifndef DIALOG_H
#define DIALOG_H

//0 = K, 1 = Deckard (YOUNG), 2 = Custom Human, 3 = Custom Replicant, 4 = Custom Indeterminate
#define userSelect 2
//0 = WALLACE CORP REPLICANT, 1 = TYRELL CORP REPLICANT. Only used if User is Replicant.
#define replicantType 0

char const nameK[] PROGMEM = "K D6-3.7";
char const nameDeckard[] PROGMEM = "DECKARD, RICK";
char const nameCustom[] PROGMEM = "SHEFKI, ADAM";

const char* const userName[] PROGMEM = {nameK, nameDeckard, nameCustom, nameCustom, nameCustom};

char const userDescReplicant[] PROGMEM = "NEXUS 9 SRS";
char const userDescHuman[] PROGMEM = "HUMAN";
char const userDescIndeterminate[] PROGMEM = "REPLICANT";

const char* const userDesc[] PROGMEM = {userDescReplicant, userDescIndeterminate, userDescHuman, userDescReplicant, userDescIndeterminate};

char const securityCodeK[] PROGMEM = "25805-K-2973";
char const securityCodeDeckard[] PROGMEM = "91077-B-26354";

const char* const userSecCode[] PROGMEM = {securityCodeK, securityCodeDeckard};

char const access[] PROGMEM = "ACCESS";

char const profileAuthMessage[] PROGMEM =
"AUTHORIZATION GRANTED TO THE ABOVE USER TO DETAIN, IDENTIFY, AND/OR "
"RETIRE ANY INDIVIDUAL OR ENTITY SUSPECTED OF PRIOR VIOLATION OF THE "
"REPLICANT PROHIBITION ACT OF 2023 OR OTHERWISE IN BREACH OF ALLOWED "
"OPERATIONAL PARAMETERS.";

char const profileLicenseLine1[] PROGMEM = "PROPERTY OF LOS ANGELES";
char const profileLicenseLine2[] PROGMEM = "POLICE DEPT, SECTOR 5";

const char* const profileLicenseMessage[] PROGMEM = {profileLicenseLine1, profileLicenseLine2};

char const noUserImageFound[] PROGMEM = "NO USER IMAGE FOUND";

char const analysisWait[] PROGMEM = "AWAITING SCAN DATA...";

char const redactedText[] PROGMEM = "REDACTED";

char const notWanted[] PROGMEM = "N/A";

char const wanted[] PROGMEM = "WARRANT NO ";

char const match[] PROGMEM = "MATCH";

char const partial[] PROGMEM = "PARTIAL";

char const missing[] PROGMEM = "NO DATA";

char const secCode[] PROGMEM = "SEC CODE";

//  Garbage characters, used for intentionally garbled data.
char const garbage1 PROGMEM = 0xda;
char const garbage2 PROGMEM = 0xdb;
char const garbage3 PROGMEM = 0xdd;
char const garbage4 PROGMEM = 0xdc;
char const garbage5 PROGMEM = 0xb0;
char const garbage6 PROGMEM = 0xb1;
char const garbage7 PROGMEM = 0xa0;
char const garbage8 PROGMEM = 0xac;
char const garbage9 PROGMEM = 0xec;
char const garbage10 PROGMEM = ' ';

const char garbageTextArray [] PROGMEM = {
  garbage1, garbage2, garbage3, garbage4, garbage5, garbage6, garbage7, garbage8, garbage9, garbage10
};

//  Male first names, individually defined into PROGMEM (Flash memory, instead of RAM)
const char firstNameM01 [] PROGMEM = "ADAM";
const char firstNameM02 [] PROGMEM = "ADI";
const char firstNameM03 [] PROGMEM = "AJAX";
const char firstNameM04 [] PROGMEM = "ANDREW";
const char firstNameM05 [] PROGMEM = "BIG";
const char firstNameM06 [] PROGMEM = "CLAUDE";
const char firstNameM07 [] PROGMEM = "DARYL";
const char firstNameM08 [] PROGMEM = "DAVID";
const char firstNameM09 [] PROGMEM = "EDWARD";
const char firstNameM10 [] PROGMEM = "GORDO";
const char firstNameM11 [] PROGMEM = "GRAHAM";
const char firstNameM12 [] PROGMEM = "HARRY";
const char firstNameM13 [] PROGMEM = "JACK";
const char firstNameM14 [] PROGMEM = "JOHN";
const char firstNameM15 [] PROGMEM = "LEONARD";
const char firstNameM16 [] PROGMEM = "MARCUS";
const char firstNameM17 [] PROGMEM = "NIANDER";
const char firstNameM18 [] PROGMEM = "RICK";
const char firstNameM19 [] PROGMEM = "ROBERT";
const char firstNameM20 [] PROGMEM = "ROY";
const char firstNameM21 [] PROGMEM = "SAPPER";
const char firstNameM22 [] PROGMEM = "SPENCER";
const char firstNameM23 [] PROGMEM = "STEPHEN";

//  An array to hold the above-declared Male first names, also in PROGMEM
const char* const firstNameMale[23] PROGMEM = {
firstNameM01, firstNameM02, firstNameM03, firstNameM04, firstNameM05, firstNameM06, firstNameM07, firstNameM08,
firstNameM09, firstNameM10, firstNameM11, firstNameM12, firstNameM13, firstNameM14, firstNameM15, firstNameM16,
firstNameM17, firstNameM18, firstNameM19, firstNameM20, firstNameM21, firstNameM22, firstNameM23
};

//  Female first names, individually defined into PROGMEM (Flash memory, instead of RAM)
const char firstNameF01 [] PROGMEM = "ADA";
const char firstNameF02 [] PROGMEM = "ANNA";
const char firstNameF03 [] PROGMEM = "BETSY";
const char firstNameF04 [] PROGMEM = "DAISY";
const char firstNameF05 [] PROGMEM = "EISA";
const char firstNameF06 [] PROGMEM = "GIANA";
const char firstNameF07 [] PROGMEM = "GLORY";
const char firstNameF08 [] PROGMEM = "JADE";
const char firstNameF09 [] PROGMEM = "JANET";
const char firstNameF10 [] PROGMEM = "JEAN";
const char firstNameF11 [] PROGMEM = "LUCY";
const char firstNameF12 [] PROGMEM = "MARIA";
const char firstNameF13 [] PROGMEM = "MARYLIN";
const char firstNameF14 [] PROGMEM = "MOLLY";
const char firstNameF15 [] PROGMEM = "MOTOKO";
const char firstNameF16 [] PROGMEM = "NATALYA";
const char firstNameF17 [] PROGMEM = "PEARL";
const char firstNameF18 [] PROGMEM = "PRISCILLA";
const char firstNameF19 [] PROGMEM = "RACHAEL";
const char firstNameF20 [] PROGMEM = "SOPHIE";
const char firstNameF21 [] PROGMEM = "TAFFEY";
const char firstNameF22 [] PROGMEM = "TERA";
const char firstNameF23 [] PROGMEM = "VERA";

//  An array to hold the above-declared Female first names, also in PROGMEM
const char* const firstNameFemale[23] PROGMEM = {
    firstNameF01, firstNameF02, firstNameF03, firstNameF04, firstNameF05, firstNameF06, firstNameF07, firstNameF08,
    firstNameF09, firstNameF10, firstNameF11, firstNameF12, firstNameF13, firstNameF14, firstNameF15, firstNameF16,
    firstNameF17, firstNameF18, firstNameF19, firstNameF20, firstNameF21, firstNameF22, firstNameF23
    };

//  A pool of last names, individually defined into PROGMEM (Flash memory, instead of RAM)
const char lastName01 [] PROGMEM = "AMDAHL";
const char lastName02 [] PROGMEM = "BARKER";
const char lastName03 [] PROGMEM = "BATTY";
const char lastName04 [] PROGMEM = "BAXTER";
const char lastName05 [] PROGMEM = "BEZIER";
const char lastName06 [] PROGMEM = "BRYANT";
const char lastName07 [] PROGMEM = "BOVOR";
const char lastName08 [] PROGMEM = "CHEW";
const char lastName09 [] PROGMEM = "COLEMAN";
const char lastName10 [] PROGMEM = "COOPER";
const char lastName11 [] PROGMEM = "DECKARD";
const char lastName12 [] PROGMEM = "EDISON";
const char lastName13 [] PROGMEM = "ERLANG";
const char lastName14 [] PROGMEM = "FARADAY";
const char lastName15 [] PROGMEM = "GATES";
const char lastName16 [] PROGMEM = "GRANVILLE";
const char lastName17 [] PROGMEM = "GRAVES";
const char lastName18 [] PROGMEM = "GUZZA";
const char lastName19 [] PROGMEM = "HOLDEN";
const char lastName20 [] PROGMEM = "HOPPER";
const char lastName21 [] PROGMEM = "KELVIN";
const char lastName22 [] PROGMEM = "KOLVIG";
const char lastName23 [] PROGMEM = "KOWALSKI";
const char lastName24 [] PROGMEM = "KUSANAGI";
const char lastName25 [] PROGMEM = "LAMARR";
const char lastName26 [] PROGMEM = "LEE";
const char lastName27 [] PROGMEM = "LOVELACE";
const char lastName28 [] PROGMEM = "MCLARGEHUGE"; //lol shut up
const char lastName29 [] PROGMEM = "MCNULTY";
const char lastName30 [] PROGMEM = "MONADI";
const char lastName31 [] PROGMEM = "MORTON";
const char lastName32 [] PROGMEM = "NETT";
const char lastName33 [] PROGMEM = "NETZEN";
const char lastName34 [] PROGMEM = "NEWTON";
const char lastName35 [] PROGMEM = "PERLMAN";
const char lastName36 [] PROGMEM = "PIM";
const char lastName37 [] PROGMEM = "RHODES";
const char lastName38 [] PROGMEM = "ROWE";
const char lastName39 [] PROGMEM = "RYDER";
const char lastName40 [] PROGMEM = "SAVAGE";
const char lastName41 [] PROGMEM = "SCHOTTKY";
const char lastName42 [] PROGMEM = "SEBASTIAN";
const char lastName43 [] PROGMEM = "STANFORD";
const char lastName44 [] PROGMEM = "STEELE";
const char lastName45 [] PROGMEM = "STRATTON";
const char lastName46 [] PROGMEM = "THERON";
const char lastName47 [] PROGMEM = "TURING";
const char lastName48 [] PROGMEM = "TURNER";
const char lastName49 [] PROGMEM = "TYRELL";
const char lastName50 [] PROGMEM = "WALLACE";
const char lastName51 [] PROGMEM = "WEAVER";
const char lastName52 [] PROGMEM = "WILLIAMS";

//  An array to hold the above-declared last names, also in PROGMEM
const char* const lastName[52] PROGMEM = {
    lastName01, lastName02, lastName03, lastName04, lastName05, lastName06, lastName07, lastName08,
    lastName09, lastName10, lastName11, lastName12, lastName13, lastName14, lastName15, lastName16,
    lastName17, lastName18, lastName19, lastName20, lastName21, lastName22, lastName23, lastName24,
    lastName25, lastName26, lastName27, lastName28, lastName30, lastName31, lastName32, lastName33,
    lastName34, lastName35, lastName36, lastName37, lastName38, lastName39, lastName40, lastName41,
    lastName42, lastName43, lastName44, lastName45, lastName46, lastName47, lastName48, lastName49,
    lastName50, lastName51, lastName52
};


//  Replicant job descriptions, individually defined into PROGMEM (Flash memory, instead of RAM)
const char professionR01 [] PROGMEM = "MILITARY/ENGINEER";
const char professionR02 [] PROGMEM = "MILITARY/COMBAT";
const char professionR03 [] PROGMEM = "MILITARY/LEISURE";
const char professionR04 [] PROGMEM = "MILITARY/MEDICAL";
const char professionR05 [] PROGMEM = "MILITARY/INTEL";
const char professionR06 [] PROGMEM = "MILITARY/PILOT";
const char professionR07 [] PROGMEM = "HEAVY LOADER";
const char professionR08 [] PROGMEM = "PROTEIN FARMER";
const char professionR09 [] PROGMEM = "HAZ/CONTAINMENT";
const char professionR10 [] PROGMEM = "PROTEIN FARMER";
const char professionR11 [] PROGMEM = "EXEC/ASSISTANT";
const char professionR12 [] PROGMEM = "CIV/CHILDCARE";
const char professionR13 [] PROGMEM = "MERCENARY";
const char professionR14 [] PROGMEM = "MECH/FOUNDRY";
const char professionR15 [] PROGMEM = "SPINNER REPAIR";
const char professionR16 [] PROGMEM = "CRYPTOGRAPHER";
const char professionR17 [] PROGMEM = "EXPLOSIVES DISP.";
const char professionR18 [] PROGMEM = "BLADE RUNNER";
const char professionR19 [] PROGMEM = "SURVEILLANCE";
const char professionR20 [] PROGMEM = "CONSTRUCTION";
const char professionR21 [] PROGMEM = "OFFWORLD/COLONIST";

const char* const professionReplicant[21] PROGMEM = {
    professionR01, professionR02, professionR03, professionR04, professionR05,
    professionR06, professionR07, professionR08, professionR09, professionR10,
    professionR11, professionR12, professionR13, professionR14, professionR15,
    professionR16, professionR17, professionR18, professionR19, professionR20,
    professionR21
};

const char professionH01 [] PROGMEM = "ENGINEER";
const char professionH02 [] PROGMEM = "BLADE RUNNER";
const char professionH03 [] PROGMEM = "GENETICIST";
const char professionH04 [] PROGMEM = "POLITICIAN";
const char professionH05 [] PROGMEM = "LAW ENFORCEMENT";
const char professionH06 [] PROGMEM = "SCIENTIST";
const char professionH07 [] PROGMEM = "DOCTOR";
const char professionH08 [] PROGMEM = "TRADER";
const char professionH09 [] PROGMEM = "SCRAP DEALER";
const char professionH10 [] PROGMEM = "PRIVATE SECURITY";
const char professionH11 [] PROGMEM = "CORP/CLERK";
const char professionH12 [] PROGMEM = "ENTERTAINER";
const char professionH13 [] PROGMEM = "ENGINEER";
const char professionH14 [] PROGMEM = "ARCHIVIST";
const char professionH15 [] PROGMEM = "GUN RUNNER";
const char professionH16 [] PROGMEM = "NOODLE CHEF";
const char professionH17 [] PROGMEM = "VID-PHONE OP.";
const char professionH18 [] PROGMEM = "BOTANIST";
const char professionH19 [] PROGMEM = "CIV/PILOT";
const char professionH20 [] PROGMEM = "ANIMOID DESIGNER";
const char professionH21 [] PROGMEM = "PROGRAMMER";

const char* const professionHuman[21] PROGMEM = {
    professionH01, professionH02, professionH03, professionH04, professionH05,
    professionH06, professionH07, professionH08, professionH09, professionH10,
    professionH11, professionH12, professionH13, professionH14, professionH15,
    professionH16, professionH17, professionH18, professionH19, professionH20,
    professionH21
};

const char crime01 [] PROGMEM = "HOMICIDE";
const char crime02 [] PROGMEM = "ARSON";
const char crime03 [] PROGMEM = "BATTERY";
const char crime04 [] PROGMEM = "KIDNAPPING";
const char crime05 [] PROGMEM = "MANSLAUGHTER";
const char crime06 [] PROGMEM = "THEFT (DATA)";
const char crime07 [] PROGMEM = "BURGLARY";
const char crime08 [] PROGMEM = "DRUG MANUF";
const char crime09 [] PROGMEM = "THEFT (IDENT)";
const char crime10 [] PROGMEM = "PIRACY (DATA)";
const char crime11 [] PROGMEM = "TERRORISM";
const char crime12 [] PROGMEM = "FRAUD (CREDIT)";
const char crime13 [] PROGMEM = "COUNTERFEITING";
const char crime14 [] PROGMEM = "VANDALISM";
const char crime15 [] PROGMEM = "SOLICITATION";

const char* const crimeList[15] PROGMEM = {
    crime01, crime02, crime03, crime04, crime05,
    crime06, crime07, crime08, crime09, crime10,
    crime11, crime12, crime13, crime14, crime15
};

#endif
