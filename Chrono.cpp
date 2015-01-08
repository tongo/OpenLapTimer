/*
 * Chrono.cpp
 *
 *  Created on: 16/ago/2014
 *      Author: Tongo
 */

#include "Chrono.h"
#define DEBUG_LOG_SETUP false
#define DEBUG_LOG false
  
Chrono::Chrono(MyAdafruit_ILI9341* lcdTft, Adafruit_GPS* gpsSensor, HardwareSerial *gpsSerial) {
	lcdTft->begin();
	#if DEBUG_LOG_SETUP
	Serial.println("LCD init finish");
	#endif
	chronoGui = ChronoGui();
	#if DEBUG_LOG_SETUP
	Serial.println("ChronoGui init finish");
	#endif
	chronoGui.initTft(lcdTft);
	lapTimer = LapTimer();
	#if DEBUG_LOG_SETUP
	Serial.println("LapTimer init finish");
	#endif

	#if DEBUG_LOG_SETUP
	Serial.println("GPS init...");
	#endif
	this->gps = gpsSensor;
	
	#if DEBUG_LOG_SETUP
	Serial.println("GPS assigned");
	#endif

	this->gps->begin(9600);
	#if DEBUG_LOG_SETUP
	Serial.println("GPS initialized");
	#endif

	gpsSerial->begin(9600);

	this->gps->sendCommand(PMTK_SET_BAUD_9600);
	#if DEBUG_LOG_SETUP
	Serial.println("GPS init baudRate selected");
	#endif

	//gps->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
	//GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    gps->sendCommand(PMTK_SET_NMEA_OUTPUT_ALLDATA);
	// NMEA in Output frequency
	#if DEBUG_LOG_SETUP
    Serial.println("GPS init OutputData selected");
	#endif
    gps->sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
	// 1Hz GPS Fix
	gps->sendCommand(PMTK_API_SET_FIX_CTL_5HZ);
	#if DEBUG_LOG_SETUP
	Serial.println("GPS init FixRate selected");
	#endif
	gps->sendCommand(PGCMD_NOANTENNA);
	//GPS.sendCommand(PGCMD_ANTENNA);
	#if DEBUG_LOG_SETUP
	Serial.println("GPS init Antenna selected");
	#endif
	//gpsSerial.println(PMTK_Q_RELEASE);

	#if DEBUG_LOG_SETUP
	Serial.println("LapTimer init finish");
	#endif

	gpsFixState = false;
	gpsFixStateLedTimestamp = 0;
	gpsStelliteNumber = 0;
	chronoGui.updateGpsFixState(gps->fix);

	#if DEBUG_LOG_SETUP
	Serial.println("GPS init finish");
	#endif

	chronoGui.updateLapNumber(lapTimer.getLapNumber());
	chronoGui.updateGpsSatelliteNumber(gpsStelliteNumber);

	simulateNewLap = false;
	//lastPoint = new GpsPoint(0.0, 0.0);
	//newPoint = new GpsPoint(0.0, 0.0);

	track = new Track();

	// SD-Card
	this->useSdCard = false;

	chronoGui.updateGearCounter(0);
}

Chrono::~Chrono() {
	// TODO Auto-generated destructor stub
}

void Chrono::setLogSdCard(bool useSdCard) {
	this->useSdCard = useSdCard;
}

void Chrono::loopChrono(void) {
	#if DEBUG_LOG
	Serial.println("Reading gpsSerial...");
	#endif
	while(char c = gps->read()) {
		#if DEBUG_LOG
		Serial.print(c);
		#endif
	}
	#if DEBUG_LOG
	Serial.println("gpsSerial red");
	#endif

	//Serial.println("");
	if (gps->newNMEAreceived()) {
		#if DEBUG_LOG
		Serial.println(gps->lastNMEA());
		#endif
		// NMEA parsing
		gps->parse(gps->lastNMEA());

		if (gps->fix) {
			// log
			Serial.print("Location: ");
			Serial.print(gps->latitude, 4); Serial.print(gps->lat);
			Serial.print(", ");
			Serial.print(gps->longitude, 4); Serial.println(gps->lon);
			Serial.print("Satellites: "); Serial.println((int)gps->satellites);

			if(gpsStelliteNumber != gps->satellites) {
				gpsStelliteNumber = gps->satellites;
				chronoGui.updateGpsSatelliteNumber(gps->satellites);
			}
		}


		#if DEBUG_LOG
		Serial.print("Check new Position ...");
		#endif

		newPoint.updatePointDegree(gps->latitude, gps->longitude);
		if(!lastPoint.equalsGpsPoint(&newPoint) || simulateNewLap) {
			// Check new Lap
			GpsPoint* intersectionPoint = track->isFinishLinePassed(&lastPoint, &newPoint);
			if(intersectionPoint != NULL || simulateNewLap) {
				simulateNewLap = false;
				lapTimer.newLap();
				chronoGui.updateLapNumber(lapTimer.getLapNumber());
				if(intersectionPoint != NULL) {
					delete intersectionPoint;
				}
			}

			// SD-Card logging (only on new Position change)
			if(useSdCard) {
				File dataFile = SD.open("track.txt", FILE_WRITE);
				if (dataFile) {
					dataFile.print(gps->day);
					dataFile.print("-");
					dataFile.print(gps->month);
					dataFile.print("-");
					dataFile.print(gps->year);
					dataFile.print(" ");

					dataFile.print(gps->hour);
					dataFile.print(":");
					dataFile.print(gps->minute);
					dataFile.print(":");
					dataFile.print(gps->seconds);
					dataFile.print(".");
					dataFile.print(gps->milliseconds);
					dataFile.print(";");

					//dataFile.print("LAST LAT: ");
					dataFile.print(lastPoint.latitude);
					dataFile.print(";");
					//dataFile.print("LAST LON: ");
					dataFile.print(lastPoint.longitude);
					dataFile.print(";");
					//dataFile.print("NEW LAT: ");
					dataFile.print(newPoint.latitude);
					dataFile.print(";");
					//dataFile.print("NEW LON: ");
					dataFile.print(newPoint.longitude);
					dataFile.print(";");

					if(intersectionPoint != NULL) {
						dataFile.print("1;");
						dataFile.print(intersectionPoint->latitude);
						dataFile.print(";");
						dataFile.print(intersectionPoint->longitude);
						dataFile.print(";");
					} else {
						dataFile.print("0;;;");
					}
					dataFile.println("");
					dataFile.close();
				}
			}
		}

		lastPoint.updatePointDecimal(newPoint.latitude, newPoint.longitude);
	}
	#if DEBUG_LOG
	Serial.print("Check GPS Fix ...");
	#endif
	if(gpsFixState != gps->fix) {
		chronoGui.updateGpsFixState(gps->fix);
		gpsFixState = gps->fix;
	}

	chronoGui.updateLapTime(lapTimer.getCurrentLapTime());
}

void Chrono::updateLapTimeGui() {
	chronoGui.updateLapTime(lapTimer.getCurrentLapTime());
}

float knotsToKmH(float speed) {
	return speed * 1.852;
}
