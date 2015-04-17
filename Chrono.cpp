/*
 * Chrono.cpp
 *
 *  Created on: 16/ago/2014
 *      Author: Tongo
 */

#include "Chrono.h"

#define DEBUG_LOG_SETUP false
#define DEBUG_LOG_GPS_NMEA_SD false
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
	isTimerRunning = false;
	lapTimerDelayBest = true;

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
	gps->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	//gps->sendCommand(PMTK_SET_NMEA_OUTPUT_ALLDATA);
	// NMEA in Output frequency
	#if DEBUG_LOG_SETUP
		Serial.println("GPS init OutputData selected");
	#endif
	
	gps->sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);
	//gps->sendCommand(PMTK_SET_NMEA_UPDATE_10HZ);
	// 5Hz GPS Fix
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

	chronoGui.updateLapDelayState(lapTimerDelayBest);
	chronoGui.updateGearCounter(0);
}

Chrono::~Chrono() {
	// TODO Auto-generated destructor stub
}

void Chrono::setLogSdCard(SdFat* sd) {
	if(sd == NULL) {
		this->useSdCard = false;
		return;
	}
	this->useSdCard = true;
	sd->chdir("/", true);
	SdFile file;
	char dirName[14];
	
	int maxFolderName = 0;

	while (file.openNext(sd->vwd(), O_READ)) {
		if(!file.isDir()) {
			file.close();
			continue;
		}
		
		file.getName(dirName, 14);
		int folderNumber = atoi(dirName);
		if(folderNumber > maxFolderName) {
			maxFolderName = folderNumber;
		}
		
		Serial.print(F("file: ")); Serial.println(dirName);
		file.close();
	}
	maxFolderName ++;

	sprintf(dirName, "/%d", maxFolderName);
	this->logFileDirName = dirName;
	sd->mkdir(this->logFileDirName, true);

	char lofFileName[20];
	sprintf(lofFileName, this->logFileDirName);
	strcat (lofFileName, "/syslog.log");

	Serial.print("Log file: "); Serial.println(lofFileName);
	if(logFile == NULL) {
		this->logFile = new SdFile();
		Serial.println("Log file initialization...");
		if (logFile->open(lofFileName, O_WRITE | O_CREAT | O_APPEND)) {
			Serial.println("Log file OK");
			logFile->println("*** LOGGING NEW SESSION ***");
			logFile->sync();
			//logFile->close();
		} else {
			Serial.println("Log file initialization ERROR");
		}
	}

	if(logNmeaFile == NULL) {
		this->logNmeaFile = new SdFile();
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

		#if DEBUG_LOG_GPS_NMEA_SD
			logNmeaToSdCard(gps->lastNMEA());
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
			// Check new Lap
			/*
			Serial.println("NEW POSITION");
			Serial.println(gps->lastNMEA());

			Serial.print("GPS float [latitude= "); Serial.print(gps->latitude, 6); Serial.print(", ");
			Serial.print("longitude= "); Serial.print(gps->longitude, 6); Serial.println("]");

			Serial.print("GPS long [latitude= "); Serial.print(gps->latitude_fixed); Serial.print(", ");
			Serial.print("longitude= "); Serial.print(gps->longitude_fixed); Serial.println("]");

			Serial.print("newPoint [latitude= "); Serial.print(newPoint.latitude, 6); Serial.print(", ");
			Serial.print("longitude= "); Serial.print(newPoint.longitude, 6); Serial.println("]");
			*/
			
			GpsPoint* intersectionPoint = track->isFinishLinePassed(&lastPoint, &newPoint);

			/*
			if(intersectionPoint == NULL) {
				Serial.println("");
				Serial.println("NO INTERSECTION");
			} else {
				Serial.print("IntersectionPoint [latitude= "); Serial.print(intersectionPoint->latitude, 6); Serial.print(", ");
				Serial.print("longitude= "); Serial.print(intersectionPoint->longitude, 6); Serial.println("]");
			}

			Serial.println("");
			Serial.println("*******************************************************");
			Serial.println("");
			*/

			if((intersectionPoint != NULL || simulateNewLap) && isTimerRunning) {
				simulateNewLap = false;
				lapTimer.newLap();

				logLapToSdCard();

				chronoGui.updateLapNumber(lapTimer.getLapNumber());

				updateLapDelay();

				if(intersectionPoint != NULL) {
					delete intersectionPoint;
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

	if(gpsStelliteNumber != gps->satellites) {
		gpsStelliteNumber = gps->satellites;
		chronoGui.updateGpsSatelliteNumber(gps->satellites);
	}

	#if DEBUG_LOG
		Serial.print("Update GUI");
	#endif
	if(isTimerRunning) {
		chronoGui.updateLapTime(lapTimer.getCurrentLapTime());
	} else {
		chronoGui.updateLapTime(0);
	}
	if(useSdCard && logFile != NULL) {
		logPositionToSdCard(NULL);
	}
}

void Chrono::updateLapDelay() {
	if(lapTimerDelayBest) {
		chronoGui.updateLapDelay(lapTimer.getBestLapDelay());
	} else {
		chronoGui.updateLapDelay(lapTimer.getLastLapDelay());
	}
}

float knotsToKmH(float speed) {
	return speed * 1.852;
}

void Chrono::logPositionToSdCard(GpsPoint* intersectionPoint) {
	//long startTimestamp = millis();
	if(!logFile->isOpen()) {
		char lofFileName[20];
		sprintf(lofFileName, this->logFileDirName);
		strcat (lofFileName, "/syslog.log");
		if(!logFile->open(lofFileName, O_WRITE | O_CREAT | O_APPEND)) {
			Serial.println("File syslog.log open ERROR");
			return;
		}
	}
	logFile->print("POINT;");
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
	logFile->sync();
	//logFile->close();

	//long sdTime = millis() - startTimestamp;
	//Serial.println(sdTime);
}

void Chrono::logLapToSdCard() {
	if(!logFile->isOpen()) {
		char lofFileName[20];
		sprintf(lofFileName, this->logFileDirName);
		strcat (lofFileName, "/syslog.log");
		if(!logFile->open(lofFileName, O_WRITE | O_CREAT | O_APPEND)) {
			Serial.println("File syslog.log open ERROR");
			return;
		}
	}
	logFile->print("LAP;");
	logFile->print(lapTimer.getLastLapTime());
	logFile->print(";");
	logFile->print(lapTimer.getLapNumber());
	logFile->println("");
	logFile->sync();
	//logFile->close();
}

void Chrono::logNmeaToSdCard(char* nmea) {
	if(!logNmeaFile->isOpen()) {
		char lofFileName[20];
		sprintf(lofFileName, this->logFileDirName);
		strcat (lofFileName, "/nmea.log");
		if(!logNmeaFile->open(lofFileName, O_WRITE | O_CREAT | O_APPEND)) {
			Serial.println("File nmea.log open ERROR");
			return;
		}
	}
	logFile->print(nmea);
	logFile->sync();
}

void Chrono::handleUserOperation(int operation) {
	if(operation == OPERATION_NO_OPERATION) { return; }
	else if(operation == OPERATION_NEXT_TRACK) { loadTrack(true); }
	else if(operation == OPERATION_PREV_TRACK) { loadTrack(false); }
	else if(operation == OPERATION_CHANGE_TIMER_STATE) { changeTimerState(!isTimerRunning); }
	else if(operation == OPERATION_CHANGE_LAP_DELAY_STATE) { changeLapDelayState(); }
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


			Serial.print("Selected track: ");
			tracciato.printTo(Serial);
			Serial.println("");


		String nome = tracciato["name"].asString();

		char temp[nome.length()];
		nome.toCharArray(temp, nome.length() + 1);
		track->trackName = temp;

		JsonArray& finishLineArray = tracciato["finishLine"];
		JsonObject& finishLine1 = finishLineArray[0];
		JsonObject& finishLine2 = finishLineArray[1];

		Serial.print("FinishLineArray: ");
		finishLineArray.printTo(Serial);
		Serial.println("");

		Serial.print("FinishLine1: ");
		finishLine1.printTo(Serial);
		Serial.println("");

		Serial.print("FinishLine2: ");
		finishLine2.printTo(Serial);
		Serial.println("");

		track->finishLinePoint1->latitude = finishLine1["lat"].as<float>();
		track->finishLinePoint1->longitude = finishLine1["lon"].as<float>();

		track->finishLinePoint2->latitude = finishLine2["lat"].as<float>();
		track->finishLinePoint2->longitude = finishLine2["lon"].as<float>();

		Serial.print("track 1 LAT= "); Serial.print(track->finishLinePoint1->latitude, 6);
		Serial.print("track 1 LON= "); Serial.print(track->finishLinePoint1->longitude, 6);
		Serial.print("track 2 LAT= "); Serial.print(track->finishLinePoint2->latitude, 6);
		Serial.print("track 2 LON= "); Serial.print(track->finishLinePoint2->longitude, 6);

		chronoGui.updateTrackName(track->trackName);
	}
}

void Chrono::changeTimerState (bool state) {
	isTimerRunning = state;
	if(state) {
		lapTimer.startTimer();
	} else {
		lapTimer.stopTimer();
	}
}

void Chrono::changeLapDelayState () {
	// TODO update GUI with new delay STATE and delay TIME
	lapTimerDelayBest = !lapTimerDelayBest;
	chronoGui.updateLapDelayState(lapTimerDelayBest);
	updateLapDelay();
}
