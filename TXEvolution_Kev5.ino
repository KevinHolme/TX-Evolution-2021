/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
 * This is the code I am using for a one transmitter with multiple recievers controller based on adafruit feathers. Specifically Adafruit Feather M0 RFM69HCW Packet Radio 
 * It is a work in progress based on John Parks Remote effects Trigger Box here https://learn.adafruit.com/remote-effects-trigger/programing-the-remote
 * This is all adafruit hardware so use them to hget all the library's setup, follow the trigger box instructions,skip the OLED and Trellis then go to the learning page for the 3.5 inch TFT
 */


#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "Adafruit_HX8357.h"
#include <Adafruit_STMPE610.h>
#include <RH_RF69.h>
//#include <Wire.h>
//#include <Adafruit_SSD1306.h>
#include <RHReliableDatagram.h>
#include <RH_ASK.h>
#include <Encoder.h>

//-----------------------------------------------------------------------------------------------------
//---------------------------------------------------Here is where you put your pages of commands------------------------
//---------------------the trasmitter will constantly send a 2 or 3 digit number , We ignore all ending in zero-------
//---------------------any number ending in 1 to 8 can be used to trigger something. This first page is 10 when Idle---------
//---------------------and will send 11 to 18 when a button is pressed and held for a second. 

 // This is the labels for page 1, commands 11 to 18
#define Page1Header (" Random Sounds ")
#define Page1But1 (" R Whistle ")
#define Page1But2 (" R Chat ")
#define Page1But3 (" R Razz ")
#define Page1But4 (" R Scream ")
#define Page1But5 (" R Ack ")
#define Page1But6 (" R Sad ")
#define Page1But7 (" R Alarm ")
#define Page1But8 (" R Hum ")

 // This is the labels for page 2, commands 21 to 28
#define Page2Header (" Quick Fun ")
#define Page2But1 (" U Arm Open ")
#define Page2But2 (" Wave Left ")
#define Page2But3 ("    ")
#define Page2But4 ("    ")
#define Page2But5 (" U Arm Closed ")
#define Page2But6 (" Wave Right ")
#define Page2But7 ("   ")
#define Page2But8 ("   ")

 // This is the labels for page 3, commands 31 to 38
#define Page3Header (" Periscope ")
#define Page3But1 (" Random Fast")
#define Page3But2 (" Up ")
#define Page3But3 (" Search CCW ")
#define Page3But4 ("    ")
#define Page3But5 (" Down ")
#define Page3But6 (" Degobah ")
#define Page3But7 (" Search CW ")
#define Page3But8 (" Random Slow ")

// This is the labels for page 4, commands 41 to 48
#define Page4Header (" Life Force Scanner ")
#define Page4But1 (" Mid ")
#define Page4But2 (" Scan 1 ")
#define Page4But3 ("    ")
#define Page4But4 ("   ")
#define Page4But5 (" Up ")
#define Page4But6 (" Scan 2 ")
#define Page4But7 ("    ")
#define Page4But8 (" Down ")

// This is the labels for page 5, commands 51 to 58
#define Page5Header (" Dome Gadgets ")
#define Page5But1 (" Speaker  ")
#define Page5But2 (" Saber  ")
#define Page5But3 (" Saber Light ")
#define Page5But4 (" Fire ")
#define Page5But5 ("  ")
#define Page5But6 ("  ")
#define Page5But7 ("  ")
#define Page5But8 (" Dome Smoke ")

// This is the labels for page 6, commands 61 to 68
#define Page6Header (" Body Gadgets 1 ")
#define Page6But1 (" Rockets Open ")
#define Page6But2 (" Rocket LED ")
#define Page6But3 (" Rocket Smoke ")
#define Page6But4 ("   ")
#define Page6But5 (" Closed ")
#define Page6But6 (" LED off ")
#define Page6But7 ("   ")
#define Page6But8 ("  ")

// This is the labels for page 7, commands 71 to 78
#define Page7Header (" Body Gadgets 2 ")
#define Page7But1 (" CPU Open ")
#define Page7But2 (" CPU Ext ")
#define Page7But3 (" Zap Open ")
#define Page7But4 (" Zap Zap ")
#define Page7But5 (" CPU Closed ")
#define Page7But6 ("   ")
#define Page7But7 (" Zap Closed ")
#define Page7But8 ("   ")

// This is the labels for page 8, commands 81 to 88
#define Page8Header (" Body Gadgets 3 ")
#define Page8But1 (" Welder Open ")
#define Page8But2 (" Weld ")
#define Page8But3 (" Hyper Open ")
#define Page8But4 ("   ")
#define Page8But5 (" Weld Closed ")
#define Page8But6 ("   ")
#define Page8But7 (" Hyper Closed ")
#define Page8But8 ("   ")

// This is the labels for page 9, commands 91 to 98
#define Page9Header (" Shows 1 ")
#define Page9But1 (" Rocket Man ")
#define Page9But2 (" Leia Holo ")
#define Page9But3 (" EverythgAwe ")
#define Page9But4 (" Matcho ")
#define Page9But5 (" Fav Things ")
#define Page9But6 (" Cantina ")
#define Page9But7 (" Low Rider ")
#define Page9But8 ("   ")

// This is the labels for page 10, commands 101 to 108
#define Page10Header (" Shows 2")
#define Page10But1 (" Happy BDay ")
#define Page10But2 ("  ")
#define Page10But3 ("  ")
#define Page10But4 ("  ")
#define Page10But5 ("  ")
#define Page10But6 ("  ")
#define Page10But7 ("  ")
#define Page10But8 ("  ")

// This is the labels for page 11, commands 111 to 118
#define Page11Header (" Shows 3 ")
#define Page11But1 ("  ")
#define Page11But2 ("  ")
#define Page11But3 ("  ")
#define Page11But4 ("  ")
#define Page11But5 ("  ")
#define Page11But6 ("  ")
#define Page11But7 ("  ")
#define Page11But8 ("  ")

// This is the labels for page 12, commands 121 to 128
#define Page12Header (" Shows 4 ")
#define Page12But1 ("   ")
#define Page12But2 ("   ")
#define Page12But3 ("   ")
#define Page12But4 ("   ")
#define Page12But5 ("   ")
#define Page12But6 ("   ")
#define Page12But7 ("   ")
#define Page12But8 ("   ")

// This is the labels for page 13, commands 131 to 138
#define Page13Header ("  Songs 1 ")
#define Page13But1 (" Main Th ")
#define Page13But2 (" Leias Th ")
#define Page13But3 ("  ")
#define Page13But4 ("  ")
#define Page13But5 (" Vaders Th ")
#define Page13But6 (" Cantina ")
#define Page13But7 ("  ")
#define Page13But8 ("  ")

// This is the labels for page 14, commands 141 to 148
#define Page14Header (" Songs 2 ")
#define Page14But1 (" ManaMana ")
#define Page14But2 (" Harlam Sh ")
#define Page14But3 (" Staying Alive ")
#define Page14But4 (" Gangham ")
#define Page14But5 (" PBJ Time ")
#define Page14But6 (" 24 Carrat ")
#define Page14But7 (" R2 Rock It ")
#define Page14But8 (" Disco SW ")

// This is the labels for page 15, commands 151 to 158
#define Page15Header (" Feet Colors ")
#define Page15But1 (" Blue")
#define Page15But2 (" Green ")
#define Page15But3 (" Red ")
#define Page15But4 (" R,W.B  ")
#define Page15But5 (" White ")
#define Page15But6 (" Magents ")
#define Page15But7 (" Purple ")
#define Page15But8 (" Black ")

// This is the labels for page 16, commands 161 to 168
#define Page16Header (" Stance ")
#define Page16But1 (" 2 Legs")
#define Page16But2 ("   ")
#define Page16But3 ("   ")
#define Page16But4 (" 3 Legs ")
#define Page16But5 (" Look Up ")
#define Page16But6 (" Look Up Max ")
#define Page16But7 ("Look Dn ")
#define Page16But8 (" Look Dn Max ")

// This is the labels for page 17, commands 171 to 178
#define Page17Header (" All Doors ")
#define Page17But1 (" Open All ")
#define Page17But2 (" Wave 1 ")
#define Page17But3 (" Wave 3 ")
#define Page17But4 (" Alt 1 ")
#define Page17But5 (" Close All ")
#define Page17But6 (" Wave 2 ")
#define Page17But7 (" Wave 4 ")
#define Page17But8 (" Alt 2 ")

// This is the labels for page 18, commands 181 to 188
#define Page18Header (" Just Dome ")
#define Page18But1 (" Open All ")
#define Page18But2 (" Wave 1 ")
#define Page18But3 (" Wave 3 ")
#define Page18But4 (" Alt 1 ")
#define Page18But5 (" Close All ")
#define Page18But6 (" Wave 2 ")
#define Page18But7 (" Wave 4 ")
#define Page18But8 (" Alt 2 ")

// This is the labels for page 19, commands 191 to 198
#define Page19Header (" Just Front Skin ")
#define Page19But1 (" Open All ")
#define Page19But2 (" Wave 1 ")
#define Page19But3 (" Wave 3 ")
#define Page19But4 (" Alt 1 ")
#define Page19But5 (" Close All ")
#define Page19But6 (" Wave 2 ")
#define Page19But7 (" Wave 4 ")
#define Page19But8 (" Alt 2 ")

// This is the labels for page 20, commands 201 to 208
#define Page20Header (" Just Rear Skin ")
#define Page20But1 (" Open All ")
#define Page20But2 (" Wave 1 ")
#define Page20But3 (" Wave 3 ")
#define Page20But4 (" Alt 1 ")
#define Page20But5 (" Close All ")
#define Page20But6 (" Wave 2 ")
#define Page20But7 (" Wave 4 ")
#define Page20But8 (" Alt 2 ") 

// This is the labels for page 21, commands 211 to 218
#define Page21Header (" Doors 1 ")
#define Page21But1 (" Pie 1 ")
#define Page21But2 (" Pie 3 ")
#define Page21But3 (" 1 ")
#define Page21But4 (" 3 ")
#define Page21But5 (" Pie 2 ")
#define Page21But6 (" Pie 4 ")
#define Page21But7 (" 2 ")
#define Page21But8 (" 4 ") 

// This is the labels for page 22, commands 221 to 228
#define Page22Header (" Doors 2 ")
#define Page22But1 (" 7 ")
#define Page22But2 (" 11 ")
#define Page22But3 (" F1 ")
#define Page22But4 (" Leia ")
#define Page22But5 (" 10 ")
#define Page22But6 (" 13 ")
#define Page22But7 (" Charge Bay ")
#define Page22But8 (" Data ") 

// This is the labels for page 23, commands 231 to 238
#define Page23Header (" Doors 3 ")
#define Page23But1 (" F5 ")
#define Page23But2 (" FL2 ")
#define Page23But3 (" FL4")
#define Page23But4 (" Big Top ")
#define Page23But5 (" FL1 ")
#define Page23But6 (" FL3 ")
#define Page23But7 (" R1 ")
#define Page23But8 (" R3 ") 

// This is the labels for page 24, commands 241 to 248
#define Page24Header (" Doors 4 ")
#define Page24But1 (" Winch ")
#define Page24But2 (" RL1 ")
#define Page24But3 (" 1RL3")
#define Page24But4 ("   ")
#define Page24But5 (" R5 ")
#define Page24But6 (" RL2 ")
#define Page24But7 (" RL4 ")
#define Page24But8 ("  ") 


//********* Encoder Setup ***************
//#define PIN_ENCODER_SWITCH 13   // pin for the pushbutton switch in the encoder, pin 13 is the led, but I am not using the led, and the feather is pin short. I am also not using the button, but it is here. 
Encoder knob(11, 12);   //pins for the encoder input
uint8_t activeRow = 0;
long pos = -999;
long newpos;
int prevButtonState = HIGH;
bool needsRefresh = true;
bool advanced = false;
unsigned long startTime;


//-------------------------------------------TFT Screen-------------------------
//the tft screen is made to be attached to the feather, we cant do that, so using the tft pinout guide, solder jumpers in place with the controller open and laying flat. 

#define STMPE_CS 6
#define TFT_CS   9
#define TFT_DC   10
#define SD_CS    -1
#define TFT_RST -1
Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 3800
#define TS_MAXX 100
#define TS_MINY 100
#define TS_MAXY 3750

// Redefine original colors, add additional colors to match those available with the ILI9341 library
#define HX8357_BLACK       0x0000  ///<   0,   0,   0
#define HX8357_NAVY        0x000F  ///<   0,   0, 123
#define HX8357_DARKGREEN   0x03E0  ///<   0, 125,   0
#define HX8357_DARKCYAN    0x03EF  ///<   0, 125, 123
#define HX8357_MAROON      0x7800  ///< 123,   0,   0
#define HX8357_PURPLE      0x780F  ///< 123,   0, 123
#define HX8357_OLIVE       0x7BE0  ///< 123, 125,   0
#define HX8357_LIGHTGREY   0xC618  ///< 198, 195, 198
#define HX8357_DARKGREY    0x7BEF  ///< 123, 125, 123
#define HX8357_BLUE        0x001F  ///<   0,   0, 255
#define HX8357_GREEN       0x07E0  ///<   0, 255,   0
#define HX8357_CYAN        0x07FF  ///<   0, 255, 255
#define HX8357_RED         0xF800  ///< 255,   0,   0
#define HX8357_MAGENTA     0xF81F  ///< 255,   0, 255
#define HX8357_YELLOW      0xFFE0  ///< 255, 255,   0
#define HX8357_WHITE       0xFFFF  ///< 255, 255, 255
#define HX8357_ORANGE      0xFD20  ///< 255, 165,   0
#define HX8357_GREENYELLOW 0xAFE5  ///< 173, 255,  41
#define HX8357_PINK        0xFC18  ///< 255, 130, 198

#define BUTTON1_X 30
#define BUTTON1_Y 30
#define BUTTON1_W 47
#define BUTTON1_H 50
#define BUTTON1_TEXTSIZE 1
#define DISPLAY_XOFFSET 80
#define DISPLAY_TEXTOFFSET 90
#define DISPLAY_YOFFSET 0


#define MENU1_BTN_CNT 10
Adafruit_GFX_Button Menu1Buttons[MENU1_BTN_CNT];
char Menu1Labels1[MENU1_BTN_CNT][10] = {"-","RS", "QF", "Per", "LFS", "D Gad", "B Gad1", "B Gad2", "B Gad3", "+"};
char Menu1Labels2[MENU1_BTN_CNT][10] = {"-", "Show1", "Show2", "Show3", "Show4", "Song1", "Song2", "FeetC", "Stance", "+"};
char Menu1Labels3[MENU1_BTN_CNT][10] = {"-", "All D", "Dome", "F Skin ", "R Skin", "Doors1", "Doors2", "Doors3", "Doors4", "+"};
uint16_t Menu1Colors[MENU1_BTN_CNT] = {HX8357_DARKGREY,HX8357_BLUE, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY};
uint16_t Menu1Colors1[MENU1_BTN_CNT] = {HX8357_DARKGREY,HX8357_BLUE, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY};
uint16_t Menu1Colors2[MENU1_BTN_CNT] = {HX8357_DARKGREY,HX8357_DARKGREY, HX8357_BLUE, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY};
uint16_t Menu1Colors3[MENU1_BTN_CNT] = {HX8357_DARKGREY,HX8357_DARKGREY, HX8357_DARKGREY, HX8357_BLUE, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY};
uint16_t Menu1Colors4[MENU1_BTN_CNT] = {HX8357_DARKGREY,HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_BLUE, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY};
uint16_t Menu1Colors5[MENU1_BTN_CNT] = {HX8357_DARKGREY,HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_BLUE, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY};
uint16_t Menu1Colors6[MENU1_BTN_CNT] = {HX8357_DARKGREY,HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_BLUE, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY};
uint16_t Menu1Colors7[MENU1_BTN_CNT] = {HX8357_DARKGREY,HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_BLUE, HX8357_DARKGREY, HX8357_DARKGREY};
uint16_t Menu1Colors8[MENU1_BTN_CNT] = {HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_DARKGREY, HX8357_BLUE, HX8357_DARKGREY};
char  MenuLabel[MENU1_BTN_CNT][10];
uint16_t MenuColor[MENU1_BTN_CNT];

int ButtonRow = 1;
int ButtonRowHist = 1;
//-------------------------------------------------------------------------
bool initializeButtons(
            Adafruit_GFX_Button menu1Buttons[], uint16_t Menu1Colors1[], char menu1Labels1[][10], int menu1ButtonCount) {
        for (uint8_t col=0; col<menu1ButtonCount; col++) {
        menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors1[col], HX8357_WHITE,Menu1Labels1[col], BUTTON1_TEXTSIZE); 
            menu1Buttons[col].drawButton();
 }
    return true;
            }
//----------------------------------------------Radio ---------------------------
  #define RF69_FREQ 919.0
  #define RFM69_CS      8
  #define RFM69_INT     3
  #define RFM69_RST     4
  
// Where to send packets to! It is possible to send out commands to specific radios, and then have them all check in, it added complexity, and the recieve failures were so low, I quit using it. 255 is send to all. 
#define DEST_ADDRESS 255


// change addresses for each client board, any number :)
#define MY_ADDRESS 1
struct dataStruct{  // defines the radio packet n1 and n2 are for screen number, n3 is the pushbutton, this will limit you to 99 screens of 8 choices per, for a total of 792 individual commands. 
byte n1;
byte n2;
byte n3;
}RadioPacket;

 byte buf[sizeof(RadioPacket)] = {0};

 // Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram rf69_manager(rf69, MY_ADDRESS);

//int16_t packetnum = 0;  // packet counter, we increment per xmission
//----------------------------------------------constants---------------
//assign the board pins to the buttons 
const int Button1 = 13;
const int Button2 = 5;
const int Button3 = 18;
const int Button4 = 19;
const int Button5 = 0;
const int Button6 = 1;
const int Button7 = 20;
const int Button8 = 21;
const int Tog1 = A0;
const int Tog2 = A1;
const int Tog3 = A2;
const int Tog4 = A3;


//variable for reading the pushbutton status 
int Button1state = 0;
int Button2state = 0;
int Button3state = 0;
int Button4state = 0;
int Button5state = 0;
int Button6state = 0;
int Button7state = 0;
int Button8state = 0;
int Tog1State = 0;
int Tog1StateA = 0;
int Tog1StateH = 0;
int Tog2State = 0;
int Tog2StateA = 0;
int Tog2StateH = 0;
int Tog3State = 0;
int Tog3StateA = 0;
int Tog3StateH = 0;
int Tog4State = 0;
int Tog4StateA = 0;
int Tog4StateH = 0;
int HomeS = 0;
int HomeSHist = 0;
int SwitchIntHist = 0;

//int Send1;
byte val1;
byte val2;
byte val3;

String menuTitle;
String menu1;
String menu2;
String menu3;
String menu4;
String menu5;
String menu6;
String menu7;
String menu8;

String Word1;
String Word2;
String Switches;
String Switch1;
String Switch2;
String Switch3;
String Switch4;
String Switch5;
String Buttons;
String Butt1;
String Butt2;
String Butt3;
String Butt4;
String Butt5;
String Butt6;
String Butt7;
String Butt8;
int SwitchInt;
int ButtonsInt;
int TxTimes = 0;
int diff;
int btnHist = 1;
int pgeHist = 1;
int pge = 1;
//*********************************************************Timeing*******************************************
/*Based on the blink without delay sketch. these timeings can be used to fine toon your sketch. This is the data used by the time triggers in the loop section. 
 * For instance, if the serial display as going so fast you cant read it, set it to only refresh every so often. this works like this.
 *1. the very first line in loop is "currentMillis = millis();"  this capures the current time in milliseconds and names it Millis()
 *2. each thing you wish to control the time of gets a placeholder these are the unsigned long variables , our serial screen is previousDisplayMillis
 *3. how long the delay you wish is stored below in the const int these are also milliseconds so 500 is 1/2 second. 
so we do not have to scroll back and forth, I borrowed this from loop

if (millis() - previousDisplayMillis >= DisplayTime){
previousDisplayMillis = currentMillis;  
PrintValues();
}

this is the time check function the first line is asking if the current time in ms, minus the last stored value is greater or equal to the time desired. 
if the answer to that math problem is NO the program jumps over the the bracketed section, if the answer is YES then the program runs the bracket section.it does two things. 
1. makes previousDisplayMillis equal to the time right now.
2. Runs the subroutine called PrintValues();

This is such a useful tool. Arduinos are fast, but we dont need or want the display to update 200 times each second. by having it done just twice a second we reduced the load on the arduino. so it can do other things.
pretty much you want to use this and if you can help it, eliminate the use of delay in your sketches. 

*/
const int SensorTime = 500;
const int DisplayTime = 1000;
const int LoopTime = 500;
const int SendTime = 200;
unsigned long currentMillis = millis();      // stores the value of millis() in each iteration of loop()
unsigned long previousSensorMillis = millis();
unsigned long previousDisplayMillis = millis();
unsigned long previousDisplay2Millis = millis();
unsigned long previousSwitchMillis = millis();
unsigned long previousAddMillis = millis();
unsigned long previousSwitch1Millis = millis();
unsigned long previousSendMillis = millis();

//*************************************************Encoder Setup*******************************************


int menuList[24]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24}; //for rotary encoder choices
int m = 1; //variable to increment through menu list


//-------------------------------------------------Setup______________________________
void setup() {

  Serial.begin(115200);
  delay(10);
//  pinMode(8, INPUT_PULLUP);
// digitalWrite(8, HIGH);
 analogReference(AR_DEFAULT);
 pinMode(Button1, INPUT_PULLUP);
 pinMode(Button2, INPUT_PULLUP);
 pinMode(Button3, INPUT_PULLUP);
 pinMode(Button4, INPUT_PULLUP);
 pinMode(Button5, INPUT_PULLUP);
 pinMode(Button6, INPUT_PULLUP);
 pinMode(Button7, INPUT_PULLUP);
 pinMode(Button8, INPUT_PULLUP);
 pinMode(Tog1, INPUT);
 pinMode(Tog2, INPUT);
 pinMode(Tog3, INPUT);
 pinMode(Tog4, INPUT);

 digitalPinToInterrupt(11); //on M0, Encoder library doesn't auto set these as interrupts
 digitalPinToInterrupt(12);
  

//---------------------------------------------Radio------------------------  
  
  
 
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
  
  if (!rf69.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
  }
  Serial.println("RFM69 radio init OK!");
  
  
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(20, true);

  // The encryption key has to be the same as the one in the server.      BUT DIFFERENT FROM OTHER DROIDS IN THE ROOM
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x07, 0x08, 0x09};
  rf69.setEncryptionKey(key);
  
//  pinMode(LED, OUTPUT);

  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");
  
 // give the radio packet a temp value 
RadioPacket.n1 = 0;
RadioPacket.n2 = 0;
RadioPacket.n3 = 0;


 delay(500); //yea, I know, a delay, but it is in the setup section, Just runs once. 

//---------------------------------------Display--------------------------------
 if (!ts.begin()) {
    Serial.println("Couldn't start touchscreen controller");
    while (1);
  }
  Serial.println("Touchscreen started");
  
 //tft.begin();
 tft.begin(HX8357D);
Serial.println("HX8357D Test!");
 tft.setRotation(1);

  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0,0);
  tft.setTextSize(2);
  tft.println("RFM69 @ ");
  tft.print((int)RF69_FREQ);
  tft.println(" MHz");
  //tft.display();
  delay(1200); //pause to let freq message be read by a human
  
//this displays your name and phone number at start up. I still think that most of the world is honest, thay just need the oportunity
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0,0);
  tft.setTextSize(3);
  tft.println(" Kevin Holme");
  tft.println("  ");
  tft.println(" 909.239.0663 ");
  delay(3000); //pause to let message be read by a human
  tft.fillScreen(HX8357_BLACK);


  
}

// Dont put this on the stack:
//uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
//uint8_t data[] = "  OK";

//------------------------------------------------------------------------------------------
int  tftButtonRelease(Adafruit_GFX_Button menu1Buttons[], int menu1ButtonCount) {
    int btn = -1;
    TS_Point p;
    
    if (ts.bufferSize()) 
    {
        p = ts.getPoint(); 
    } 
    else 
    {
        // this is our way of tracking touch 'release'!
        p.x = p.y = p.z = -1;
    }
  
    // Scale from ~0->4000 to  tft.width using the calibration #'s
    if (p.z != -1) 
    {
        int16_t px = p.x;
        int16_t py = p.y;
        p.x = map(py, TS_MINY, TS_MAXY, 0,  tft.width());
        p.y = map(px, TS_MINX, TS_MAXX, 0,  tft.height()); 
    }

    // go thru all the buttons, checking if they were pressed
    for (uint8_t b=0; b<menu1ButtonCount; b++) 
    {
        if (menu1Buttons[b].contains(p.x, p.y)) 
        {
          Serial.print("Pressing: "); Serial.println(b);
          menu1Buttons[b].press(true);  // tell the button it is pressed
        } 
        else 
        {
          menu1Buttons[b].press(false);  // tell the button it is NOT pressed
        }
    }
    
    // now we can ask the buttons if their state has changed
    for (uint8_t b=0; b<menu1ButtonCount; b++) 
    {
        if (menu1Buttons[b].justReleased()) 
        {
          Serial.print("Released: "); Serial.println(b);
          menu1Buttons[b].drawButton();  // draw normal
          btn = b;
        }

        if (menu1Buttons[b].justPressed()) 
        {
            menu1Buttons[b].drawButton(true);  // draw invert!
            delay(500); // UI debouncing
        }
    }
    return btn;
    
}

//--------------------------------------------------------------------------------
void processMenu1() {
  
    String msg = "";
    
    int btn =  tftButtonRelease(Menu1Buttons, MENU1_BTN_CNT);
    if (btn >= 0 && btn < MENU1_BTN_CNT)
    {
        Serial.print("btn = "); Serial.println(btn);
          Serial.print(" pge = "); Serial.println(pge);
          Serial.print("  btnHist = "); Serial.println( btnHist);
        Serial.print("m = "); Serial.println(m);
        Serial.print("pHist = "); Serial.println(pgeHist);
        Serial.print("SwitchIntHist = "); Serial.println(SwitchIntHist);
         Serial.print("ButtonRowHist  = "); Serial.println(ButtonRowHist );
        
      pge = btn;
    }
 if (pge != btnHist){

if (ButtonRowHist == 1){
 
     switch (pge)
    {
       case 0:
        m = 24;
        pgeHist = 24;
        btnHist = 0;
        ButtonRowHist = 3;
        break;
        
      case 1:
        m = 1;
        pgeHist = 1;
        btnHist = 1;
        break;
        
      case 2:
        m = 2;
        pgeHist = 2;
        btnHist = 2;
        break;
        
       case 3:
        m = 3;
        pgeHist = 3;
        btnHist = 3;
         break;
        
       case 4:
        m = 4;
         pgeHist = 4;
        btnHist = 4;
         break;
            
        case 5:
        m = 5;
         pgeHist = 5;
        btnHist = 5;
        break;
        
       case 6:
        m = 6;
         pgeHist = 6;
        btnHist = 6;
         break;
        
      case 7:
        m = 7;
         pgeHist = 7;
        btnHist = 7;
        break;
        
     case 8:
        m = 8;
         pgeHist = 8;
        btnHist = 8;
        break;

      case 9:
        m = 9;
        pgeHist = 9;
        btnHist = 9;
        ButtonRowHist = 2;
         break;
    } }   
         
  if (ButtonRowHist == 2){
    
      switch (pge){ 
         case 0:
        m = 8;
         pgeHist = 8;
        btnHist = 0;
        ButtonRowHist = 1;
         break; 
            
       case 1:
        m = 9;
         pgeHist = 9;
        btnHist = 1;
         break;
        
      case 2:
        m = 10;
         pgeHist = 10;
        btnHist = 2;
        break;
        
      case 3:
        m = 11;
         pgeHist = 11;
        btnHist = 3;
        break;
        
       case 4:
        m = 12;
         pgeHist = 12;
        btnHist = 4;
         break;
        
       case 5:
        m = 13;
         pgeHist = 13;
        btnHist = 5;
         break;
        
        
        case 6:
        m = 14;
         pgeHist = 14;
        btnHist = 6;
         break;
        
        case 7:
        m = 15;
         pgeHist = 15;
        btnHist = 7;
        break;
        
         case 8:
        m = 16;
         pgeHist = 16;
        btnHist = 8;
        break;
        
        case 9:
        m = 17;
         pgeHist = 17;
        btnHist = 9;
        ButtonRowHist = 3;
        break; 
        }
}
 if (ButtonRowHist == 3){
    
      switch (pge){ 
         case 0:
        m = 16;
         pgeHist = 16;
        btnHist = 0;
        ButtonRowHist = 2;
         break; 
            
       case 1:
        m = 17;
         pgeHist = 17;
        btnHist = 1;
         break;
        
      case 2:
        m = 18;
         pgeHist = 18;
        btnHist = 2;
        break;
        
      case 3:
        m = 19;
         pgeHist = 19;
        btnHist = 3;
        break;
        
       case 4:
        m = 20;
         pgeHist = 20;
        btnHist = 4;
         break;
        
       case 5:
        m = 21;
         pgeHist = 21;
        btnHist = 5;
         break;
        
        
        case 6:
        m = 22;
         pgeHist = 22;
        btnHist = 6;
         break;
        
        case 7:
        m = 23;
         pgeHist = 23;
        btnHist = 7;
        break;
        
         case 8:
        m = 24;
         pgeHist = 24;
        btnHist = 8;
        break;
        
        case 9:
        m = 1;
         pgeHist = 1;
        btnHist = 9;
        ButtonRowHist = 1;
        break; 
        }

}}}


//*******************************************************Read Buttons**************************
/*
 * this section is reading the buttons the first bit is the encoder m is the encoder pos and i am using this to turn that position into a 2 digit value
 */
void ReadButtons(){

 Tog1State = analogRead(Tog1);
if (Tog1State > 800){
  HomeS = 5;
  m = 5;
}
if (Tog1State < 200){
  HomeS = 1;
  m = 1;
}
Tog2State = analogRead(Tog2);
if (Tog2State > 800){
  HomeS = 13;
  m = 13;
}
if (Tog2State < 200){
  HomeS = 9;
  m = 9;
}

Tog3State = analogRead(Tog3);
if (Tog3State > 800){
  HomeS = 17;
  m = 17;
}

if (Tog3State < 200){
  HomeS = 21;
  m = 21;
}
Tog4State = analogRead(Tog4);
if (Tog4State > 800){
  HomeS = 1;
  m = 1;
}
if (Tog4State < 200){
  HomeS = 24;
  m = 24;
}

switch (m){
   case 0:
        val1 = 0;
        val2 = 1;
    case 1:
        val1 = 0;
        val2 = 1;
    break;
      case 2:
        val1 = 0;
        val2 = 2;
    break;
     case 3:
        val1 = 0;
        val2 = 3;
    break;
     case 4:
        val1 = 0;
        val2 = 4;
    break;
    case 5:
        val1 = 0;
        val2 = 5;
    break;
      case 6:
        val1 = 0;
        val2 = 6;
    break;
     case 7:
        val1 = 0;
        val2 = 7;
    break;
     case 8:
        val1 = 0;
        val2 = 8;
    break;
    case 9:
        val1 = 0;
        val2 = 9;
    break;
      case 10:
        val1 = 1;
        val2 = 0;
    break;
     case 11:
        val1 = 1;
        val2 = 1;
    break;
     case 12:
        val1 = 1;
        val2 = 2;
    break;
    case 13:
        val1 = 1;
        val2 = 3;
    break;
      case 14:
        val1 = 1;
        val2 = 4;
    break;
     case 15:
        val1 = 1;
        val2 = 5;
    break;
     case 16:
        val1 = 1;
        val2 = 6;
    break;
        case 17:
        val1 = 1;
        val2 = 7;
    break;
      case 18:
        val1 = 1;
        val2 = 8;
    break;
     case 19:
        val1 = 1;
        val2 = 9;
    break;
     case 20:
        val1 = 2;
        val2 = 0;
    break;
    case 21:
        val1 = 2;
        val2 = 1;
    break;
      case 22:
        val1 = 2;
        val2 = 2;
    break;
     case 23:
        val1 = 2;
        val2 = 3;
    break;
     case 24:
        val1 = 2;
        val2 = 4;
    break;
}    
       // * here we read the buttons
        
Button1state = digitalRead(Button1);
Button2state = digitalRead(Button2);
Button3state = digitalRead(Button3);
Button4state = digitalRead(Button4);
Button5state = digitalRead(Button5);
Button6state = digitalRead(Button6);
Button7state = digitalRead(Button7);
Button8state = digitalRead(Button8);

Tog1State = analogRead(Tog1);
Tog2State = analogRead(Tog2);
Tog3State = analogRead(Tog3);
Tog4State = analogRead(Tog4);

//turn the readings into strings

Butt1 = String(Button1state);
Butt2 = String(Button2state);
Butt3 = String(Button3state);
Butt4 = String(Button4state);
Butt5 = String(Button5state);
Butt6 = String(Button6state);
Butt7 = String(Button7state);
Butt8 = String(Button8state);

//strings because then they can be strung together and then turned into a single integer. 
//now we have all 8 buttons expressed as a single integer. then by using switch case I also eliminate the possibility of multiple button presses. 

Buttons = Butt1 + Butt2 + Butt3 + Butt4 + Butt5 + Butt6 + Butt7 + Butt8;
ButtonsInt=(Buttons.toInt());
switch (ButtonsInt){
  case 1111111:
  val3 = 1;
  break;
  case 10111111:
  val3 = 2;
  break;
  case 11011111:
  val3 = 3;
  break;
  case 11101111:
  val3 = 4;
  break;
  case 11110111:
  val3 = 5;
  break;
  case 11111011:
  val3 = 6;
  break;
  case 11111101:
  val3 = 7;
  break;
  case 11111110:
  val3 = 8;
  break;
  default:
  val3 = 0;
  break;
}
// and finally with one more set of strings  we create a single intiger that represents the encoder position and button press in one number
// because it is an integer, the leading 0 is dropped. so 010 becomes 10.
Switch1 = String(val1);
Switch2 = String(val2);
Switch3 = String(val3);

Switches = Switch1 + Switch2 + Switch3;
SwitchInt=(Switches.toInt());

}



//*********************************************************Print Values*************************
// run this to help troubleshoot. serail print the switch values.
void PrintValues() {

  Serial.print (" Rotary: ");
  Serial.print (val1);
  Serial.print (val2);
  Serial.print (" Buttons: ");
  Serial.println (val3);
   Serial.print (" Tog 1: ");
  Serial.print (Tog1State);
    Serial.print (" Tog 2: ");
  Serial.println (Tog2State);
    Serial.print (" Tog 3: ");
  Serial.print (Tog3State);
    Serial.print (" Tog 4: ");
  Serial.println (Tog4State);
 


 
}

//*************************************************************Display*********************
// in the display subroutine we have the seperate menu pages (later) This next section. and it is long, is the data that is dislayed when a button is pressed. 

void Display(){
  
if ((SwitchInt)!= (SwitchIntHist)){     // this will only let the screen refresh if the switch has changed. 
 tft.fillScreen(HX8357_BLACK);
} 

 switch (SwitchInt) {   //use switchInt to pick the screen display
  case 11:
Select1();              // Run Select1() 
SwitchIntHist = 11;     // store that 11 was the last switch press
delay(200);             // needed
   break;               // exit out of switch case after finding match.
case 12:
Select2();
SwitchIntHist = 12;
delay(200);  
   break;
case 13:
Select3();
SwitchIntHist = 13;
delay(200);  
   break;
case 14:
Select4();
SwitchIntHist = 14;
delay(200); 
   break;
case 15:
Select5();
SwitchIntHist = 15;
delay(200);  
   break;
case 16:
Select6();
SwitchIntHist = 16;
delay(200);  
   break;
case 17:
Select7();
SwitchIntHist = 17;
delay(200);  
   break;
case 18:
Select8();
SwitchIntHist = 18;
delay(200);  
   break;
  case 21:
Select1();
SwitchIntHist = 21;
delay(200);   
   break;
case 22:
Select2();
SwitchIntHist = 22;
delay(200);  
   break;
case 23:
Select3();
SwitchIntHist = 23;
delay(200);  
   break;
case 24:
Select4();
SwitchIntHist = 24;
delay(200);
  break;
case 25:
Select5();
SwitchIntHist = 25;
delay(200);
   break;
case 26:
Select6();
SwitchIntHist = 26;
delay(200);  
   break;
case 27:
Select7();
SwitchIntHist = 27;
delay(200);  
   break;
case 28:
Select8();
SwitchIntHist = 28;
delay(200);  
   break;
 case 31:
Select1();
SwitchIntHist = 31;
delay(200);   
   break;
case 32:
 Select2();
SwitchIntHist = 32;
   break;
case 33:
Select3();
SwitchIntHist = 33;
   break;
case 34:
Select4();
SwitchIntHist = 34;
   break;
case 35:
Select5();
SwitchIntHist = 35;
   break;
case 36:
Select6();
SwitchIntHist = 36;
   break;
case 37:
Select7();
SwitchIntHist = 37;
   break;
case 38:
Select8();
SwitchIntHist = 38;
   break;       
 case 41:
Select1();
SwitchIntHist = 41;
delay(200);   
   break;
case 42:
Select2();
SwitchIntHist = 42;
   break;
case 43:
Select3();
SwitchIntHist = 43;
   break;
case 44:
Select4();
SwitchIntHist = 44;
   break;
case 45:
Select5();
SwitchIntHist = 45;
   break;
case 46:
Select6();
SwitchIntHist = 46;
   break;
case 47:
Select7();
SwitchIntHist = 47;
   break;
case 48:
Select8();
SwitchIntHist = 48;
   break;       
 case 51:
Select1();
SwitchIntHist = 51;
delay(200);   
   break;
case 52:
Select2();
SwitchIntHist = 52;
delay(200);  
   break;
case 53:
Select3();
SwitchIntHist = 53;
delay(200);  
   break;
case 54:
Select4();
SwitchIntHist = 54;
delay(200); 
   break;
case 55:
Select5();
SwitchIntHist = 55;
delay(200);  
   break;
case 56:
Select6();
SwitchIntHist = 56;
delay(200);  
   break;
case 57:
Select7();
SwitchIntHist = 57;
delay(200);  
   break;
case 58:
Select8();
SwitchIntHist = 58;
delay(200);  
   break;
    case 61:
Select1();
SwitchIntHist = 61;
delay(200);   
   break;
case 62:
Select2();
SwitchIntHist = 62;
delay(200);  
   break;
case 63:
Select3();
SwitchIntHist = 63;
delay(200);  
   break;
case 64:
Select4();
SwitchIntHist = 64;
delay(200); 
   break;
case 65:
Select5();
SwitchIntHist = 65;
delay(200);  
   break;
case 66:
Select6();
SwitchIntHist = 66;
delay(200);  
   break;
case 67:
Select7();
SwitchIntHist = 67;
delay(200);  
   break;
case 68:
Select8();
SwitchIntHist = 68;
delay(200);  
   break;
 case 71:
Select1();
SwitchIntHist = 71;
delay(200);   
   break;
case 72:
Select2();
SwitchIntHist = 72;
delay(200);  
   break;
case 73:
Select3();
SwitchIntHist = 73;
delay(200);  
   break;
case 74:
Select4();
SwitchIntHist = 74;
delay(200); 
   break;
case 75:
Select5();
SwitchIntHist = 75;
delay(200);  
   break;
case 76:
Select6();
SwitchIntHist = 76;
delay(200);  
   break;
case 77:
Select7();
SwitchIntHist = 77;
delay(200);  
   break;
case 78:
Select8();
SwitchIntHist = 78;
delay(200);  
   break;
    case 81:
Select1();
SwitchIntHist = 81;
delay(200);   
   break;
case 82:
Select2();
SwitchIntHist = 82;
delay(200);  
   break;
case 83:
Select3();
SwitchIntHist = 83;
delay(200);  
   break;
case 84:
Select4();
SwitchIntHist = 84;
delay(200); 
   break;
case 85:
Select5();
SwitchIntHist = 85;
delay(200);  
   break;
case 86:
Select6();
SwitchIntHist = 86;
delay(200);  
   break;
case 87:
Select7();
SwitchIntHist = 87;
delay(200);  
   break;
case 88:
Select8();
SwitchIntHist = 88;
delay(200);  
   break;

case 91:
Select1();
SwitchIntHist = 91;
    break;
case 92:
 Select2();
SwitchIntHist = 92;
   break;
case 93:
Select3();
SwitchIntHist = 93;
   break;
case 94:
Select4();
SwitchIntHist = 94;
   break;
case 95:
Select5();
SwitchIntHist = 95;
   break;
case 96:
Select6();
SwitchIntHist = 96;
   break;
case 97:
Select7();
SwitchIntHist = 97;
   break;
case 98:
Select8();
SwitchIntHist = 98;
   break; 
  case 101:
Select1();
SwitchIntHist = 101;
    break;
case 102:
Select2();
SwitchIntHist = 102;
   break;
case 103:
Select3();
SwitchIntHist = 103;
   break;
case 104:
Select4();
SwitchIntHist = 104;
   break;
case 105:
Select5();
SwitchIntHist = 105;
   break;
case 106:
Select6();
SwitchIntHist = 106;
   break;
case 107:
Select7();
SwitchIntHist =107;
   break;
case 108:
Select8();
SwitchIntHist = 108;
   break; 
 case 111:
Select1();
SwitchIntHist = 111;
break;
case 112:
Select2();
SwitchIntHist = 112;
   break;
case 113:
Select3();
SwitchIntHist = 113;
   break;
case 114:
Select4();
SwitchIntHist = 114;
   break;
case 115:
Select5();
SwitchIntHist = 115;
   break;
case 116:
Select6();
SwitchIntHist = 116;
   break;
case 117:
Select7();
SwitchIntHist = 117;
   break;
case 118:
Select8();
SwitchIntHist = 118;
   break;
 case 121:
Select1();
SwitchIntHist = 121;
delay(200);   
   break;
case 122:
Select2();
SwitchIntHist = 122;
delay(200);  
   break;
case 123:
Select3();
SwitchIntHist = 123;
delay(200);  
   break;
case 124:
Select4();
SwitchIntHist = 124;
delay(200);
  break;
case 125:
Select5();
SwitchIntHist = 125;
delay(200);
   break;
case 126:
Select6();
SwitchIntHist = 126;
delay(200);  
   break;
case 127:
Select7();
SwitchIntHist = 127;
delay(200);  
   break;
case 128:
Select8();
SwitchIntHist = 128;
delay(200);  
   break;
case 131:
Select1();
SwitchIntHist = 131;
delay(200);   
   break;
case 132:
Select2();
SwitchIntHist = 132;
   break;
case 133:
Select3();
SwitchIntHist = 133;
   break;
case 134:
Select4();
SwitchIntHist = 134;
   break;
case 135:
Select5();
SwitchIntHist = 135;
   break;
case 136:
Select6();
SwitchIntHist = 136;
   break;
case 137:
Select7();
SwitchIntHist = 137;
   break;
case 138:
Select8();
SwitchIntHist = 138;
   break;
   case 141:
Select1();
SwitchIntHist = 141;
delay(200);   
   break;
case 142:
Select2();
SwitchIntHist = 142;
   break;
case 143:
Select3();
SwitchIntHist = 143;
   break;
case 144:
Select4();
SwitchIntHist = 144;
   break;
case 145:
Select5();
SwitchIntHist = 145;
   break;
case 146:
Select6();
SwitchIntHist = 146;
   break;
case 147:
Select7();
SwitchIntHist = 147;
   break;
case 148:
Select8();
SwitchIntHist = 148;
   break;
case 151:
Select1();
SwitchIntHist = 151;
delay(200);   
   break;
case 152:
Select2();
SwitchIntHist = 152;
   break;
case 153:
Select3();
SwitchIntHist = 153;
   break;
case 154:
Select4();
SwitchIntHist = 154;
   break;
case 155:
Select5();
SwitchIntHist = 155;
   break;
case 156:
Select6();
SwitchIntHist = 156;
   break;
case 157:
Select7();
SwitchIntHist = 157;
   break;
case 158:
Select8();
SwitchIntHist = 158;
   break;
   case 161:
Select1();
SwitchIntHist = 161;
delay(200);   
   break;
case 162:
Select2();
SwitchIntHist = 162;
   break;
case 163:
Select3();
SwitchIntHist = 163;
   break;
case 164:
Select4();
SwitchIntHist = 164;
   break;
case 165:
Select5();
SwitchIntHist = 165;
   break;
case 166:
Select6();
SwitchIntHist = 166;
   break;
case 167:
Select7();
SwitchIntHist = 167;
   break;
case 168:
Select8();
SwitchIntHist = 168;
   break;
      case 171:
Select1();
SwitchIntHist = 171;
delay(200);   
   break;
case 172:
Select2();
SwitchIntHist = 172;
   break;
case 173:
Select3();
SwitchIntHist = 173;
   break;
case 174:
Select4();
SwitchIntHist = 174;
   break;
case 175:
Select5();
SwitchIntHist = 175;
   break;
case 176:
Select6();
SwitchIntHist = 176;
   break;
case 177:
Select7();
SwitchIntHist = 177;
   break;
case 178:
Select8();
SwitchIntHist = 178;
   break;
      case 181:
Select1();
SwitchIntHist = 181;
delay(200);   
   break;
case 182:
Select2();
SwitchIntHist = 182;
   break;
case 183:
Select3();
SwitchIntHist = 183;
   break;
case 184:
Select4();
SwitchIntHist = 184;
   break;
case 185:
Select5();
SwitchIntHist = 185;
   break;
case 186:
Select6();
SwitchIntHist = 186;
   break;
case 187:
Select7();
SwitchIntHist = 187;
   break;
case 188:
Select8();
SwitchIntHist = 188;
   break;
      case 191:
Select1();
SwitchIntHist = 191;
delay(200);   
   break;
case 192:
Select2();
SwitchIntHist = 192;
   break;
case 193:
Select3();
SwitchIntHist = 193;
   break;
case 194:
Select4();
SwitchIntHist = 194;
   break;
case 195:
Select5();
SwitchIntHist = 195;
   break;
case 196:
Select6();
SwitchIntHist = 196;
   break;
case 197:
Select7();
SwitchIntHist = 197;
   break;
case 198:
Select8();
SwitchIntHist = 198;
   break;
      case 201:
Select1();
SwitchIntHist = 201;
delay(200);   
   break;
case 202:
Select2();
SwitchIntHist = 202;
   break;
case 203:
Select3();
SwitchIntHist = 203;
   break;
case 204:
Select4();
SwitchIntHist = 204;
   break;
case 205:
Select5();
SwitchIntHist = 205;
   break;
case 206:
Select6();
SwitchIntHist = 206;
   break;
case 207:
Select7();
SwitchIntHist = 207;
   break;
case 208:
Select8();
SwitchIntHist = 208;
   break;
case 211:
Select1();
SwitchIntHist = 211;
delay(200);   
   break;
case 212:
Select2();
SwitchIntHist = 212;
   break;
case 213:
Select3();
SwitchIntHist = 213;
   break;
case 214:
Select4();
SwitchIntHist = 214;
   break;
case 215:
Select5();
SwitchIntHist = 215;
   break;
case 216:
Select6();
SwitchIntHist = 216;
   break;
case 217:
Select7();
SwitchIntHist = 217;
   break;
case 218:
Select8();
SwitchIntHist = 218;
   break;
         case 221:
Select1();
SwitchIntHist = 221;
delay(200);   
   break;
case 222:
Select2();
SwitchIntHist = 222;
   break;
case 2023:
Select3();
SwitchIntHist = 223;
   break;
case 224:
Select4();
SwitchIntHist = 224;
   break;
case 225:
Select5();
SwitchIntHist = 225;
   break;
case 226:
Select6();
SwitchIntHist = 226;
   break;
case 227:
Select7();
SwitchIntHist = 227;
   break;
case 228:
Select8();
SwitchIntHist = 228;
   break;
   case 231:
Select1();
SwitchIntHist = 231;
delay(200);   
   break;
case 232:
Select2();
SwitchIntHist = 232;
   break;
case 233:
Select3();
SwitchIntHist = 233;
   break;
case 234:
Select4();
SwitchIntHist = 234;
   break;
case 235:
Select5();
SwitchIntHist = 235;
   break;
case 236:
Select6();
SwitchIntHist = 236;
   break;
case 237:
Select7();
SwitchIntHist = 237;
   break;
case 238:
Select8();
SwitchIntHist = 238;
   break;
   case 241:
Select1();
SwitchIntHist = 241;
delay(200);   
   break;
case 242:
Select2();
SwitchIntHist = 242;
   break;
case 243:
Select3();
SwitchIntHist = 243;
   break;
case 244:
Select4();
SwitchIntHist = 244;
   break;
case 245:
Select5();
SwitchIntHist = 245;
   break;
case 246:
Select6();
SwitchIntHist = 246;
   break;
case 247:
Select7();
SwitchIntHist = 247;
   break;
case 248:
Select8();
SwitchIntHist = 248;
   break;
//_______________________________________________________________________________Page menu Data__________________________________________________________
// by using the switch integer, any time that a button is not being pressed the end of that number is 0.
//when no button is pressed, than we want the menu to show. 
   

case 20:
menuTitle = Page2Header;
menu1 = Page2But1;
menu2 = Page2But2;
menu3 = Page2But3;
menu4 = Page2But4;
menu5 = Page2But5;
menu6 = Page2But6;
menu7 = Page2But7;
menu8 = Page2But8;
ButtonRowHist = 1;
SwitchIntHist = 20;
Menu();
break;
case 30:
menuTitle = Page3Header;
menu1 = Page3But1;
menu2 = Page3But2;
menu3 = Page3But3;
menu4 = Page3But4;
menu5 = Page3But5;
menu6 = Page3But6;
menu7 = Page3But7;
menu8 = Page3But8;
ButtonRowHist = 1;
SwitchIntHist = 30;
Menu();
break;
case 40:
menuTitle = Page4Header;
menu1 = Page4But1;
menu2 = Page4But2;
menu3 = Page4But3;
menu4 = Page4But4;
menu5 = Page4But5;
menu6 = Page4But6;
menu7 = Page4But7;
menu8 = Page4But8;
ButtonRowHist = 1;
SwitchIntHist = 40;
Menu();
break;

case 50:                          
menuTitle = Page5Header;
menu1 = Page5But1;
menu2 = Page5But2;
menu3 = Page5But3;
menu4 = Page5But4;
menu5 = Page5But5;
menu6 = Page5But6;
menu7 = Page5But7;
menu8 = Page5But8;
ButtonRowHist = 1;
SwitchIntHist = 50;
Menu();
break;                            

case 60:                          
menuTitle = Page6Header;
menu1 = Page6But1;
menu2 = Page6But2;
menu3 = Page6But3;
menu4 = Page6But4;
menu5 = Page6But5;
menu6 = Page6But6;
menu7 = Page6But7;
menu8 = Page6But8;
ButtonRowHist = 1;
SwitchIntHist = 60;
Menu();
break;                           
case 70:                          
menuTitle = Page7Header;
menu1 = Page7But1;
menu2 = Page7But2;
menu3 = Page7But3;
menu4 = Page7But4;
menu5 = Page7But5;
menu6 = Page7But6;
menu7 = Page7But7;
menu8 = Page7But8;
ButtonRowHist = 1;
SwitchIntHist = 70;
Menu();
break;                           
case 80:                          
menuTitle = Page8Header;
menu1 = Page8But1;
menu2 = Page8But2;
menu3 = Page8But3;
menu4 = Page8But4;
menu5 = Page8But5;
menu6 = Page8But6;
menu7 = Page8But7;
menu8 = Page8But8;
ButtonRowHist = 1;
SwitchIntHist = 80;
Menu();
break;                            
case 90:
menuTitle = Page9Header;
menu1 = Page9But1;
menu2 = Page9But2;
menu3 = Page9But3;
menu4 = Page9But4;
menu5 = Page9But5;
menu6 = Page9But6;
menu7 = Page9But7;
menu8 = Page9But8;
ButtonRowHist = 2;
SwitchIntHist = 90;
Menu();
break;                
case 100:
menuTitle = Page10Header;
menu1 = Page10But1;
menu2 = Page10But2;
menu3 = Page10But3;
menu4 = Page10But4;
menu5 = Page10But5;
menu6 = Page10But6;
menu7 = Page10But7;
menu8 = Page10But8;
ButtonRowHist = 2;
SwitchIntHist = 100;
Menu();
break;
case 110:
menuTitle = Page11Header;
menu1 = Page11But1;
menu2 = Page11But2;
menu3 = Page11But3;
menu4 = Page11But4;
menu5 = Page11But5;
menu6 = Page11But6;
menu7 = Page11But7;
menu8 = Page11But8;
ButtonRowHist = 2;
SwitchIntHist = 110;
Menu();
break;
case 120:
menuTitle = Page12Header;
menu1 = Page12But1;
menu2 = Page12But2;
menu3 = Page12But3;
menu4 = Page12But4;
menu5 = Page12But5;
menu6 = Page12But6;
menu7 = Page12But7;
menu8 = Page12But8;
ButtonRowHist = 2;
SwitchIntHist = 120;
Menu();
break;
case 130:
menuTitle = Page13Header;
menu1 = Page13But1;
menu2 = Page13But2;
menu3 = Page13But3;
menu4 = Page13But4;
menu5 = Page13But5;
menu6 = Page13But6;
menu7 = Page13But7;
menu8 = Page13But8;
ButtonRowHist = 2;
SwitchIntHist = 130;
Menu();
break;
case 140:
menuTitle = Page14Header;
menu1 = Page14But1;
menu2 = Page14But2;
menu3 = Page14But3;
menu4 = Page14But4;
menu5 = Page14But5;
menu6 = Page14But6;
menu7 = Page14But7;
menu8 = Page14But8;
ButtonRowHist = 2;
SwitchIntHist = 140;
Menu();
break;
case 150:
menuTitle = Page15Header;
menu1 = Page15But1;
menu2 = Page15But2;
menu3 = Page15But3;
menu4 = Page15But4;
menu5 = Page15But5;
menu6 = Page15But6;
menu7 = Page15But7;
menu8 = Page15But8;
ButtonRowHist = 2;
SwitchIntHist = 150;
Menu();
break;
case 160:
menuTitle = Page16Header;
menu1 = Page16But1;
menu2 = Page16But2;
menu3 = Page16But3;
menu4 = Page16But4;
menu5 = Page16But5;
menu6 = Page16But6;
menu7 = Page16But7;
menu8 = Page16But8;
ButtonRowHist = 2;
SwitchIntHist = 160;
Menu();
break;
case 170:
menuTitle = Page17Header;
menu1 = Page17But1;
menu2 = Page17But2;
menu3 = Page17But3;
menu4 = Page17But4;
menu5 = Page17But5;
menu6 = Page17But6;
menu7 = Page17But7;
menu8 = Page17But8;
ButtonRowHist = 3;
SwitchIntHist = 170;
Menu();
break;
case 180:
menuTitle = Page18Header;
menu1 = Page18But1;
menu2 = Page18But2;
menu3 = Page18But3;
menu4 = Page18But4;
menu5 = Page18But5;
menu6 = Page18But6;
menu7 = Page18But7;
menu8 = Page18But8;
ButtonRowHist = 3;
SwitchIntHist = 180;
Menu();
break;
case 190:
menuTitle = Page19Header;
menu1 = Page19But1;
menu2 = Page19But2;
menu3 = Page19But3;
menu4 = Page19But4;
menu5 = Page19But5;
menu6 = Page19But6;
menu7 = Page19But7;
menu8 = Page19But8;
ButtonRowHist = 3;
SwitchIntHist = 190;
Menu();
break;
case 200:
menuTitle = Page20Header;
menu1 = Page20But1;
menu2 = Page20But2;
menu3 = Page20But3;
menu4 = Page20But4;
menu5 = Page20But5;
menu6 = Page20But6;
menu7 = Page20But7;
menu8 = Page20But8;
ButtonRowHist = 3;
SwitchIntHist = 200;
Menu();
break;
case 210:
menuTitle = Page21Header;
menu1 = Page21But1;
menu2 = Page21But2;
menu3 = Page21But3;
menu4 = Page21But4;
menu5 = Page21But5;
menu6 = Page21But6;
menu7 = Page21But7;
menu8 = Page21But8;
ButtonRowHist = 3;
SwitchIntHist = 210;
Menu();
break;
case 220:
menuTitle = Page22Header;
menu1 = Page22But1;
menu2 = Page22But2;
menu3 = Page22But3;
menu4 = Page22But4;
menu5 = Page22But5;
menu6 = Page22But6;
menu7 = Page22But7;
menu8 = Page22But8;
ButtonRowHist = 3;
SwitchIntHist = 220;
Menu();
break;
case 230:
menuTitle = Page23Header;
menu1 = Page23But1;
menu2 = Page23But2;
menu3 = Page23But3;
menu4 = Page23But4;
menu5 = Page23But5;
menu6 = Page23But6;
menu7 = Page23But7;
menu8 = Page23But8;
ButtonRowHist = 3;
SwitchIntHist = 230;
Menu();
break;
case 240:
menuTitle = Page24Header;
menu1 = Page24But1;
menu2 = Page24But2;
menu3 = Page24But3;
menu4 = Page24But4;
menu5 = Page24But5;
menu6 = Page24But6;
menu7 = Page24But7;
menu8 = Page24But8;
ButtonRowHist = 3;
SwitchIntHist = 240;
Menu();
break;
default: 
case 10:
menuTitle = Page1Header;
menu1 = Page1But1;
menu2 = Page1But2;
menu3 = Page1But3;
menu4 = Page1But4;
menu5 = Page1But5;
menu6 = Page1But6;
menu7 = Page1But7;
menu8 = Page1But8;
Menu();
SwitchIntHist = 10;
break;

 }

}

// the color,  text size and spacing of the menu pages is set here
unsigned long Menu() {
      switch (m){
        case 1:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors1[col], HX8357_WHITE,Menu1Labels1[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
         case 2:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors2[col], HX8357_WHITE,Menu1Labels1[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
         case 3:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors3[col], HX8357_WHITE,Menu1Labels1[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
        case 4:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors4[col], HX8357_WHITE,Menu1Labels1[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
          case 5:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors5[col], HX8357_WHITE,Menu1Labels1[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
          case 6:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors6[col], HX8357_WHITE,Menu1Labels1[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
          case 7:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors7[col], HX8357_WHITE,Menu1Labels1[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
          case 8:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors8[col], HX8357_WHITE, Menu1Labels1[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
         case 9:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors1[col], HX8357_WHITE, Menu1Labels2[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
         case 10:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors2[col], HX8357_WHITE,Menu1Labels2[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
         case 11:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors3[col], HX8357_WHITE,Menu1Labels2[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
        case 12:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors4[col], HX8357_WHITE,Menu1Labels2[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
          case 13:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors5[col], HX8357_WHITE,Menu1Labels2[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
          case 14:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors6[col], HX8357_WHITE,Menu1Labels2[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
          case 15:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors7[col], HX8357_WHITE,Menu1Labels2[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
          case 16:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors8[col], HX8357_WHITE,Menu1Labels2[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
         case 17:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors1[col], HX8357_WHITE, Menu1Labels3[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
         case 18:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors2[col], HX8357_WHITE,Menu1Labels3[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
         case 19:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors3[col], HX8357_WHITE,Menu1Labels3[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
        case 20:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors4[col], HX8357_WHITE,Menu1Labels3[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
          case 21:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors5[col], HX8357_WHITE,Menu1Labels3[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
          case 22:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors6[col], HX8357_WHITE,Menu1Labels3[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
          case 23:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors7[col], HX8357_WHITE,Menu1Labels3[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
          case 24:
        for (uint8_t col=0; col<10; col++) {
        Menu1Buttons[col].initButton(& tft, BUTTON1_X + col *(BUTTON1_W), BUTTON1_Y, BUTTON1_W, BUTTON1_H,HX8357_BLACK,
            Menu1Colors8[col], HX8357_WHITE,Menu1Labels3[col], BUTTON1_TEXTSIZE); 
            Menu1Buttons[col].drawButton();
        }
        break;
        
      }

  
  //  tft.fillScreen(HX8357_BLACK);

// Adafruit_GFX_Button menu1Buttons[], uint16_t Menu1Colors3[], char menu1Labels1[][10], int menu1ButtonCount) {

    tft.setTextSize(3);
     tft.setCursor(100, 80);
    tft.setTextColor(HX8357_WHITE);
     tft.println(menuTitle);
     tft.setCursor(10, 130);
    tft.setTextColor(HX8357_BLUE);
     tft.println(menu1);
     tft.setCursor(10, 180);
    tft.setTextColor(HX8357_RED);
     tft.println(menu2);
     tft.setCursor(10, 230);
    tft.setTextColor(HX8357_GREEN);
     tft.println(menu3);
     tft.setCursor(10, 280);
    tft.setTextColor(HX8357_YELLOW);
     tft.println(menu4);
     tft.setCursor(250, 130);
    tft.setTextColor(HX8357_BLUE);
     tft.println(menu5);
     tft.setCursor(250, 180);
    tft.setTextColor(HX8357_RED);   
     tft.println(menu6);
     tft.setCursor(250, 230);
    tft.setTextColor(HX8357_GREEN);
     tft.println(menu7);
      tft.setCursor(250, 280);
    tft.setTextColor(HX8357_YELLOW);
     tft.println(menu8);
   
  
}
//  here is select 1 through 8 so that the chosen button is diplayed in the proper color.

unsigned long Select1() {
   tft.setTextColor(HX8357_BLUE);
   tft.setTextSize(4);
   tft.setCursor(100, 100);
   tft.println(menu1) ;
   }

unsigned long Select2() {
   tft.setTextColor(HX8357_RED);
   tft.setTextSize(4);
   tft.setCursor(100, 100);
   tft.println(menu2);
   }
unsigned long Select3() {
   tft.setTextColor(HX8357_GREEN);
   tft.setTextSize(4);
   tft.setCursor(100, 100);
   tft.println(menu3);
  }
unsigned long Select4() {
   tft.setTextColor(HX8357_YELLOW);
   tft.setTextSize(4);
   tft.setCursor(100, 100);
   tft.println(menu4);
  }
unsigned long Select5() {
   tft.setTextColor(HX8357_BLUE);
   tft.setTextSize(4);
   tft.setCursor(100, 100);
   tft.println(menu5) ;
 }

unsigned long Select6() {
   tft.setTextColor(HX8357_RED);
   tft.setTextSize(4);
   tft.setCursor(100, 100);
   tft.println(menu6);
  }
unsigned long Select7() {
   tft.setTextColor(HX8357_GREEN);
   tft.setTextSize(4);
   tft.setCursor(100, 100);
   tft.println(menu7);
}
unsigned long Select8() {
   tft.setTextColor(HX8357_YELLOW);
   tft.setTextSize(4);
   tft.setCursor(100, 100);
   tft.println(menu8);
}





//----------------------------------------------------------Send------------------

//here is where we send our three val's out to the world. The radio is constatnly transmitting and if no button is pressed the val3 will be zero.  this can be used in the recievers to trigger new actions. 
void Send(){
 RadioPacket.n1 = val1;
  RadioPacket.n2 = val2;
  RadioPacket.n3 = val3;
  
   byte zize=sizeof(RadioPacket);
  memcpy (buf, &RadioPacket, zize); 
 
 //  Serial.print("Sending "); Serial.println(SwitchInt);

   if (rf69_manager.sendtoWait(buf, zize, DEST_ADDRESS)) {
   
    }}
  

//---------------------------------------------------Debug Radio
// running this will help diagnose radio issues, normally comment it out in loop.
void DebugRadio(){
 
     Serial.print("RSSI: ");
      Serial.println(rf69.lastRssi(), DEC);
 Serial.print("Sending "); Serial.println(SwitchInt);
   
 byte zize=sizeof(RadioPacket);
    if (rf69_manager.sendto(buf, zize, DEST_ADDRESS)) {
      Serial.println("Message sent");   
    }
    else{
      Serial.println("sendto failed");
    }     
      
//      Serial.print("Got message from unit: ");
         
    
//      Serial.print(IncomingMsg);  
 //     Serial.print("Incoming Int   ");
 //     Serial.println(IncomingInt);
     
    
}
 //====================================================Debug Encoder
 // running this will help diagnose encoder issues, normally comment it out in loop.
void DebugEncoder(){
   
     Serial.print("pos=");
     Serial.print(pos);
     Serial.print(", newpos=");
     Serial.println(newpos);
    
      Serial.print("Diff = ");
      Serial.print(diff);
      Serial.print("  pos= ");
      Serial.print(pos);
      Serial.print(", newpos=");
      Serial.println(newpos);
      Serial.println(menuList[m]);
     

       Serial.print("m is: ");
      Serial.println(m);


      
    
}
           
//--------------------------------------------Loop--------------------------
// I have found that the encoder loop likes living here better than in a subrouteen. 
// ReadButtons is here without any time delay
//

void loop(void) {

currentMillis = millis();

  //check the encoder knob, set the current position as origin
    long newpos = knob.read() / 5;//divide for encoder detents
    if(newpos != pos){
      int diff = newpos - pos;//check the different between old and new position
      if(diff>=1){
        m--; 
        m = (m+25) % 25;//modulo to roll over the m variable through the list size
       }
     if(diff==-1){ //rotating backwards
         m++;
         m = (m+25) % 25;
       }
      pos = newpos; 
    }


// ReadSwitches();
ReadButtons();
 processMenu1(); 


if (millis() - previousDisplayMillis >= DisplayTime){
previousDisplayMillis = currentMillis;  
PrintValues();
}
if (millis() - previousDisplay2Millis >= DisplayTime){
previousDisplay2Millis = currentMillis;   
Display();
DebugRadio();
DebugEncoder();
}
if (millis() - previousSendMillis >= SendTime){
previousSendMillis = currentMillis;
Send();
 
}
 
}
