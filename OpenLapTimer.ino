/* Open LapTimer */

#include <ILI_SdSpi.h>
#include <ILI_SdFatConfig.h>
#include <ILI9341_due_gText.h>
#include <ILI9341_due.h>
#include <SdFat.h>

//#include "fonts/Arial_bold_14.h"

//#include "SPI.h"
//#include "SD.h"
//#include "Adafruit_GFX.h"
//#include "Adafruit_ILI9341.h"
#include "Adafruit_GPS.h"
#include "Chrono.h"
//#include "MyAdafruit_ILI9341.h"

// For the Adafruit shield, these are the default.
#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10

// Button to simulate the new lap
#define FINISH_LINE_BUTTON 2
int prevTestButtonState = 0;

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
//MyAdafruit_ILI9341* tft = new MyAdafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

ILI9341_due* tft = new ILI9341_due(TFT_CS, TFT_DC, TFT_RST);

// SD-Card
#define SD_CS 7

SdFat* sd = new SdFat();

// GPS
#define gpsSerial Serial1
Adafruit_GPS* gps = new Adafruit_GPS(&gpsSerial);

Chrono* chrono;

long loopLenghtTimestamp;

void setup() {
  Serial.begin(115200);
  Serial.println("START SETUP Open LapTimer!");
  pinMode(FINISH_LINE_BUTTON, INPUT);
  
  // SD-Card - initialize it before Chrono
  bool useSd = false;
  if(sd->begin(SD_CS, SPI_HALF_SPEED)) {
    useSd = true;
    Serial.println("SD-Card initialized OK");
  } else {
    Serial.println("SD-Card NOT initialized");
  }
  
  Serial.println("Chrono setup");
  chrono = new Chrono(tft, gps, &gpsSerial);
  chrono->setLogSdCard(useSd);
  Serial.println("Chrono setup finish");
  
  Serial.println("FINE SETUP LapTimer!");
}


void loop(void) {
  long loopLenght = millis() - loopLenghtTimestamp;
  loopLenghtTimestamp = millis();
      
  // test button state
  int currentTestButtonState = digitalRead(FINISH_LINE_BUTTON);
  //Serial.println(currentTestButtonState);
  
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (currentTestButtonState == HIGH && prevTestButtonState == LOW) {
    Serial.print("ButtonTest Pressed");
    chrono->simulateNewLap = true;
    Serial.print("loop: ");
    Serial.println(loopLenght);
  }
  prevTestButtonState = currentTestButtonState;
  
  chrono->loopChrono();
}
