/*
 * Chrono.h
 *
 *  Created on: 16/ago/2014
 *      Author: Tongo
 */

#ifndef CHRONO_H_
#define CHRONO_H_

#include <ILI9341_due.h>
#include <SdFat.h>
#include <TouchScreen.h>
#include <ArduinoJson.h>
#include "LapTimer.h"
#include "ChronoGui.h"
#include "Adafruit_GPS.h"
#include "Track.h"
#include "GpsPoint.h"
#include "TouchScreenManager.h"

class Chrono {
public:
	Chrono(ILI9341_due* lcdTft, Adafruit_GPS* gpsSensor, HardwareSerial *gpsSerial);
	virtual ~Chrono();
	void loopChrono(void);
	void setLogSdCard(SdFat* sd);
	void setTouchScreen(TouchScreen* touchScreen);

	// TEST
	boolean simulateNewLap;
private:
	void logPositionToSdCard(GpsPoint* intersectionPoint);
	void logLapToSdCard();
	void logNmeaToSdCard(char* nmea);
	void handleUserOperation(int operation);
	void loadTrack(bool nextTrack);
	void changeTimerState (bool state);
	void changeLapDelayState();
	void updateLapDelay();

	LapTimer lapTimer;
	bool isTimerRunning;
	bool lapTimerDelayBest;
	
	ChronoGui chronoGui;
	Adafruit_GPS* gps;
	Track* track;

	// GPS
	boolean gpsFixState;
	long gpsFixStateLedTimestamp;
	int8_t gpsStelliteNumber;

	GpsPoint lastPoint;
	GpsPoint newPoint;

	SdFat* sd;
	bool useSdCard;
	char* logFileDirName;
	SdFile* logFile;
	SdFile* logNmeaFile;

	// TouchScreen
	TouchScreenManager* touchScreenManager;
};

#endif /* CHRONO_H_ */
