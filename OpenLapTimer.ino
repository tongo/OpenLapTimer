/* Open LapTimer */

#include <ILI_SdSpi.h>
#include <ILI_SdFatConfig.h>
#include <ILI9341_due_gText.h>
#include <ILI9341_due.h>
#include <SdFat.h>
#include <TouchScreen.h>
#include <Adafruit_GPS.h>
#include <ArduinoJson.h>
#include "Chrono.h"

// TFT SPI pin
#define TFT_RST 8
#define TFT_DC 9
#define TFT_CS 10

// Touchscreen pin
#define YP A2 // must be an analog pin, use "An" notation!
#define XM A3 // must be an analog pin, use "An" notation!
#define YM 5  // can be a digital pin
#define XP 6  // can be a digital pin
// Touchscreen X+ / X- resistance (for better pressure precision)
#define TOUCH_XP_XM_OHM 300

// Button to simulate the new lap
#define FINISH_LINE_BUTTON 2
int prevTestButtonState = 0;

// SD-Card
#define SD_CS 7

// GPS
#define gpsSerial Serial1

ILI9341_due* tft = new ILI9341_due(TFT_CS, TFT_DC, TFT_RST);
TouchScreen* touchScreen = new TouchScreen(XP, YP, XM, YM, TOUCH_XP_XM_OHM);
SdFat* sd;
Adafruit_GPS* gps = new Adafruit_GPS(&gpsSerial);
Chrono* chrono;
/*
long loopLenghtTimestamp;
long loopLenghtMax;
long loopCount;
*/
void setup() {
  Serial.begin(115200);
  Serial.println("START SETUP Open LapTimer!");
  pinMode(FINISH_LINE_BUTTON, INPUT);
  
  // SD-Card - initialize it before Chrono
  sd = new SdFat();
  if(sd->begin(SD_CS, SPI_HALF_SPEED)) {
    Serial.println("SD-Card initialized OK");
  } else {
    sd = NULL;
    Serial.println("SD-Card NOT initialized");
  }
  
  Serial.println("Chrono setup");
  chrono = new Chrono(tft, gps, &gpsSerial);
  chrono->setLogSdCard(sd);
  chrono->setTouchScreen(touchScreen);
  Serial.println("Chrono setup finish");
  
  Serial.println("FINE SETUP LapTimer!");
}

void loop(void) {
  /*
  loopCount ++;
  long loopLenght = millis() - loopLenghtTimestamp;
  loopLenghtTimestamp = millis();

  if(loopLenght > loopLenghtMax) {
    loopLenghtMax = loopLenght;
  }
  long average = (millis() / loopCount);

  Serial.print(loopLenght); Serial.print(" - ");
  Serial.print(average); Serial.print(" - "); Serial.println(loopLenghtMax);
  */
  // test button state
  int currentTestButtonState = digitalRead(FINISH_LINE_BUTTON);
  //Serial.println(currentTestButtonState);
  
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (currentTestButtonState == HIGH && prevTestButtonState == LOW) {
    Serial.print("ButtonTest Pressed");
    chrono->simulateNewLap = true;
    Serial.print("loop: ");
    //Serial.println(loopLenght);
  }
  prevTestButtonState = currentTestButtonState;
  
  chrono->loopChrono();
}
