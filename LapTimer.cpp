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
	
	bestLapTime = 0;
	bestLapTimeDelay = 0;
	previousLapTime = 0;
	previousLapTimeDelay = 0;
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
	long lapTime = finishLapTimestamp - startLapTimestamp;
	// calculating delay
	bestLapTimeDelay = lapTime - bestLapTime;
	previousLapTimeDelay = lapTime - previousLapTime;

	previousLapTime = lapTime;
	if(lapTime < bestLapTime || bestLapTime == 0) {
		bestLapTime = lapTime;
	}
	startTimer();
	lapCounter ++;
}
void LapTimer::sector() { }

long LapTimer::getBestLapDelay() {
	return bestLapTimeDelay;
}
long LapTimer::getLastLapDelay() {
	return previousLapTimeDelay;
}
long LapTimer::getLastLapTime() {
	return previousLapTime;
}
