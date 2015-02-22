/*
 * Chrono.h
 *
 *  Created on: 16/ago/2014
 *      Author: Tongo
 */

#include "LapTimer.h"
#include "ChronoGui.h"
#include "Adafruit_GPS.h"
#include "Track.h"
#include "GpsPoint.h"
#include <ILI9341_due.h>

#ifndef CHRONO_H_
#define CHRONO_H_

class Chrono {
public:
	Chrono(ILI9341_due* lcdTft, Adafruit_GPS* gpsSensor, HardwareSerial *gpsSerial);
	virtual ~Chrono();
	void loopChrono(void);
	void setLogSdCard(bool useSdCard);
	void updateLapTimeGui();

	// TEST
	boolean simulateNewLap;
private:
	LapTimer lapTimer;
	ChronoGui chronoGui;
	Adafruit_GPS* gps;
	Track* track;

	// GPS
	boolean gpsFixState;
	long gpsFixStateLedTimestamp;
	int8_t gpsStelliteNumber;

	GpsPoint lastPoint;
	GpsPoint newPoint;

	bool useSdCard;
};

#endif /* CHRONO_H_ */
