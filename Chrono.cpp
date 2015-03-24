/*
 * Chrono.cpp
 *
 *  Created on: 16/ago/2014
 *      Author: Tongo
 */

#include "Chrono.h"

#define DEBUG_LOG_SETUP false
#define DEBUG_LOG false
#define GPS_BAUD_RATE 57600

Chrono::Chrono(ILI9341_due* lcdTft, Adafruit_GPS* gpsSensor, HardwareSerial *gpsSerial) {
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

	this->gps->begin(GPS_BAUD_RATE);
	#if DEBUG_LOG_SETUP
		Serial.println("GPS initialized");
	#endif

	gpsSerial->begin(9600);
	//this->gps->sendCommand(PMTK_SET_BAUD_9600);
	this->gps->sendCommand(PMTK_SET_BAUD_57600);
	gpsSerial->flush();
	delay(100);
	gpsSerial->end();
	delay(100);
	gpsSerial->begin(GPS_BAUD_RATE);
	
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

	chronoGui.updateTrackName(track->trackName);

	// SD-Card
	this->useSdCard = false;
	this->logFile = NULL;

	// TouchScreen
	this->touchScreenManager = NULL;

	chronoGui.updateGearCounter(0);
}

Chrono::~Chrono() {
	// TODO Auto-generated destructor stub
}

void Chrono::setLogSdCard(bool useSdCard) {
	this->useSdCard = useSdCard;
	if(logFile == NULL) {
		this->logFile = new SdFile();
		Serial.println("Log file initialization...");
		if (logFile->open("syslog.log", O_WRITE | O_CREAT | O_APPEND)) {
			Serial.println("Log file OK");
			logFile->println("*** LOGGING NEW SESSION ***");
			logFile->close();
		} else {
			Serial.println("Log file initialization ERROR");
		}
	}
}

void Chrono::setTouchScreen(TouchScreen* touchScreen) {
	this->touchScreenManager = new TouchScreenManager(touchScreen);
}

void Chrono::loopChrono(void) {

	if(this->touchScreenManager != NULL) {
		int operation = this->touchScreenManager->getChronoOperation();
		handleUserOperation(operation);
	}

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

	if (gps->newNMEAreceived()) {
		#if DEBUG_LOG
			Serial.println(gps->lastNMEA());
		#endif
		// NMEA parsing
		gps->parse(gps->lastNMEA());

		if (gps->fix) {
			// log
			#if DEBUG_LOG
				Serial.print("Location: ");
				Serial.print(gps->latitude, 5); Serial.print(gps->lat);
				Serial.print(", ");
				Serial.print(gps->longitude, 5); Serial.println(gps->lon);
				Serial.print("Satellites: "); Serial.println((int)gps->satellites);
			#endif
		}

		#if DEBUG_LOG
		Serial.print("Check new Position ...");
		#endif

		newPoint.updatePointDegree(gps->latitude, gps->longitude);

		if(!lastPoint.equalsGpsPoint(&newPoint) || simulateNewLap) {
			#if DEBUG_LOG
			Serial.print("NEW POSITION");
			#endif
			// Check new Lap
			GpsPoint* intersectionPoint = track->isFinishLinePassed(&lastPoint, &newPoint);
			if(intersectionPoint != NULL || simulateNewLap) {
				simulateNewLap = false;
				lapTimer.newLap();
				chronoGui.updateLapNumber(lapTimer.getLapNumber());

				// TODO manage BEST / LAST delay calculation
				chronoGui.updateLapDelay(lapTimer.getBestLapDelay());

				if(intersectionPoint != NULL) {
					delete intersectionPoint;
				}
			}
		}

		if(useSdCard && logFile != NULL) {
			logPositionToSdCard(&newPoint);
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

	if(gpsStelliteNumber != gps->satellites) {
		gpsStelliteNumber = gps->satellites;
		chronoGui.updateGpsSatelliteNumber(gps->satellites);
	}

	#if DEBUG_LOG
		Serial.print("Update GUI");
	#endif
	chronoGui.updateLapTime(lapTimer.getCurrentLapTime());
}

float knotsToKmH(float speed) {
	return speed * 1.852;
}

void Chrono::logPositionToSdCard(GpsPoint* intersectionPoint) {
	if (!logFile->open("syslog.log", O_WRITE | O_CREAT | O_APPEND)) {
		Serial.println("File syslog.log open ERROR");
	} else {
		logFile->print(gps->day);
		logFile->print("-");
		logFile->print(gps->month);
		logFile->print("-");
		logFile->print(gps->year);
		logFile->print(" ");

		logFile->print(gps->hour);
		logFile->print(":");
		logFile->print(gps->minute);
		logFile->print(":");
		logFile->print(gps->seconds);
		logFile->print(".");
		logFile->print(gps->milliseconds);
		logFile->print(";");

		//dataFile.print("NEW LAT: ");
		logFile->print(newPoint.latitude, 6);
		logFile->print(";");
		//dataFile.print("NEW LON: ");
		logFile->print(newPoint.longitude, 6);
		logFile->print(";");

		if(intersectionPoint != NULL) {
			logFile->print("1;");
			logFile->print(intersectionPoint->latitude, 6);
			logFile->print(";");
			logFile->print(intersectionPoint->longitude, 6);
			logFile->print(";");
		} else {
			logFile->print("0;;;");
		}
		logFile->println("");
		logFile->close();
	}
}

void Chrono::handleUserOperation(int operation) {
	if(operation == OPERATION_NO_OPERATION) { return; }
	else if(operation == OPERATION_NEXT_TRACK) { loadTrack(true); }
	else if(operation == OPERATION_PREV_TRACK) { loadTrack(false); }
}

void Chrono::loadTrack(bool nextTrack) {
	SdFile file;
	if(!file.open("tracks.txt", O_READ)) {
		Serial.println("File tracks.txt open ERROR");
	} else {
		uint32_t fileLength = file.fileSize();
		char tracksFileContent[fileLength];

		file.read(&tracksFileContent, fileLength);
		file.close(); //Close the file

		#if DEBUG_LOG
			Serial.println("FILE CONTENT: ");
			Serial.println(tracksFileContent);
		#endif
		
		DynamicJsonBuffer jsonBuffer;
		JsonArray& trackArray = jsonBuffer.parseArray(tracksFileContent);

		if (!trackArray.success()) {
			Serial.println("parseArray() failed");
			return;
		}

		#if DEBUG_LOG
			trackArray.printTo(Serial);
			Serial.println(""); Serial.println("");
		#endif

		int newTrackIndex = 0;
		if(nextTrack) {
			newTrackIndex = (track->trackIndex + 1);
		} else {
			newTrackIndex = (track->trackIndex - 1);
		}

		if(newTrackIndex > (trackArray.size() -1)) {
			newTrackIndex = 0;
		} else if(newTrackIndex < 0) {
			newTrackIndex = (trackArray.size() -1);
		}
		track->trackIndex = newTrackIndex;

		JsonObject& tracciato = trackArray[newTrackIndex];

		#if DEBUG_LOG
			Serial.print("Selected track: ");
			tracciato.printTo(Serial);
			Serial.println("");
		#endif

		String nome = tracciato["name"].asString();

		char temp[nome.length()];
		nome.toCharArray(temp, nome.length() + 1);
		track->trackName = temp;

		track->finishLinePoint1->latitude = tracciato["finishLine"][0]["lat"].as<float>();
		track->finishLinePoint1->longitude = tracciato["finishLine"][0]["lon"].as<float>();

		track->finishLinePoint2->latitude = tracciato["finishLine"][1]["lat"].as<float>();
		track->finishLinePoint2->longitude = tracciato["finishLine"][1]["lon"].as<float>();

		chronoGui.updateTrackName(track->trackName);
	}
}
