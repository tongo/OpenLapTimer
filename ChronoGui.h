/*
 * ChronoGui.h
 *
 *  Created on: 12/lug/2014
 *      Author: Tongo
 */

//#include "MyAdafruit_ILI9341.h"
#include <ILI_SdSpi.h>
#include <ILI_SdFatConfig.h>
#include <ILI9341_due_gText.h>
#include <ILI9341_due.h>

#include "fonts/Arial_bold_14.h"
 
class ChronoGui {
public:
	ChronoGui();
	void initTft(ILI9341_due* tft);
	void updateLapTime(long lapTime);
	void updateLapNumber(int lapCount);
	void updateGpsFixState(bool gpsState);
	void updateGpsSatelliteNumber(int8_t satelliteNumber);
	void updateGearCounter(uint8_t gear);
private:
	ILI9341_due* tft;
	ILI9341_due_gText * timeTextBox;
};

