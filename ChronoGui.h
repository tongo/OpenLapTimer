/*
 * ChronoGui.h
 *
 *  Created on: 12/lug/2014
 *      Author: Tongo
 */

#include "MyAdafruit_ILI9341.h"

class ChronoGui {
public:
	ChronoGui();
	void initTft(MyAdafruit_ILI9341* tft);
	void updateLapTime(long lapTime);
	void updateLapNumber(int lapCount);
	void updateGpsFixState(bool gpsState);
	void updateGpsSatelliteNumber(int8_t satelliteNumber);
	void updateGearCounter(uint8_t gear);
private:
	MyAdafruit_ILI9341* tft;
};

