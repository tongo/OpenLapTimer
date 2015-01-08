/*
 * Chrono.cpp
 *
 *  Created on: 10/lug/2014
 *      Author: Tongo
 */

#include "Arduino.h"
#include "LapTimer.h"

LapTimer::LapTimer() {
	lapCounter = 0;
	startLapTimestamp = 0;
	finishLapTimestamp = 0;
}

int LapTimer::getLapNumber() {
	return lapCounter;
}
long LapTimer::getCurrentLapTime() {
	return millis() - startLapTimestamp;
}
void LapTimer::startTimer() {
	startLapTimestamp = millis();
}
void LapTimer::stopTimer() {
	finishLapTimestamp = millis();
}
void LapTimer::newLap() {
	stopTimer();
	startTimer();
	lapCounter ++;
}
void LapTimer::sector() { }
