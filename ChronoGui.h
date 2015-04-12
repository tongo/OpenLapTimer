/*
 * ChronoGui.h
 *
 *  Created on: 12/lug/2014
 *      Author: Tongo
 */

#include <ILI_SdSpi.h>
#include <ILI_SdFatConfig.h>
#include <ILI9341_due_gText.h>
#include <ILI9341_due.h>

class ChronoGui {
public:
	ChronoGui();
	void initTft(ILI9341_due* tft);
	void updateLapTime(long lapTime);
	void updateLapNumber(int lapCount);
	void updateGpsFixState(bool gpsState);
	void updateGpsSatelliteNumber(int8_t satelliteNumber);
	void updateTrackName(char* trackName);
	void updateLapDelay(long delay);
	void updateLapDelayState(bool bestLapDelay);
	void updateGearCounter(uint8_t gear);
private:
	ILI9341_due* tft;
	ILI9341_due_gText * timeTextBox;
	ILI9341_due_gText * gpsSatTextBox;
	ILI9341_due_gText * trackNameTextBox;
	ILI9341_due_gText * lapCountTextBox;
	ILI9341_due_gText * lapDelayTextBox;
	ILI9341_due_gText * lapDelayStateTextBox;
};

