/*
 * ChronoGui.cpp
 *
 *  Created on: 12/lug/2014
 *      Author: Tongo
 */

#include "ChronoGui.h"
#include "LapTimerFont.h"
#include "MainFont.h"

ChronoGui::ChronoGui() {
	this->tft = NULL;
	this->timeTextBox = NULL;
}

void ChronoGui::initTft(ILI9341_due* tft) {
	Serial.println("ChronoGui INIT - START");
	this->tft = tft;
	this->tft->setRotation(iliRotation270);
	//this->tft->setRotation(3); // Landscape right
	this->tft->fillScreen(ILI9341_BLACK);

	// Time text box initilization
	
	this->timeTextBox = new ILI9341_due_gText(this->tft);
	this->timeTextBox->defineArea(4, 104, 220, 130);
	this->timeTextBox->selectFont(Arial_bold_14);
	this->timeTextBox->setFontLetterSpacing(5);
	this->timeTextBox->setFontColor(ILI9341_WHITE, ILI9341_BLACK);
	this->timeTextBox->drawString("ii:ii:ii", gTextAlignMiddleCenter);

	Serial.println("ChronoGui INIT - FINISH");
}

void ChronoGui::updateLapTime(long lapTime) {
	Serial.println("UPDATE TIMER - START");
	/*
	uint8_t mins = 0;
	uint8_t secs = 0;
	uint16_t cents = 0;
	cents = lapTime / 10;
	secs = lapTime / 1000; //convect milliseconds to seconds
	mins = secs / 60; //convert seconds to minutes

	cents = cents - (secs * 100);
	secs = secs - (mins * 60); //subtract the coverted seconds to minutes in order to display 59 secs max

	this->tft->setCursor(4, 104);
	this->tft->setFont(lapTimeFont);
	this->tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
	this->tft->setTextSize(1);
	if(mins < 10) { this->tft->print(""); }
	this->tft->print(mins);
	this->tft->print("'");
	if(secs < 10) { this->tft->print("0"); }
	this->tft->print(secs);
	this->tft->print("\"");
	this->tft->print(cents);
*/
	this->timeTextBox = new ILI9341_due_gText(this->tft);
	this->timeTextBox->defineArea(4, 104, 220, 130);
	this->timeTextBox->selectFont(Arial_bold_14);
	this->timeTextBox->setFontLetterSpacing(5);
	this->timeTextBox->setFontColor(ILI9341_WHITE, ILI9341_BLACK);
	this->timeTextBox->drawString("xx:xx:xx", gTextAlignMiddleCenter);
	Serial.println("UPDATE TIMER - FINISH");
}

void ChronoGui::updateLapNumber(int lapCount) {
	/*
	this->tft->setCursor(10, 206);
	this->tft->setFont(mainFont);
	this->tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
	this->tft->setTextSize(1);
	this->tft->print(lapCount);
	this->tft->print(" lap");
	*/
}

void ChronoGui::updateGpsFixState(bool gpsState) {
	if(gpsState) {
		this->tft->fillCircle(68, 19, 10, ILI9341_GREEN);
	} else {
		this->tft->fillCircle(68, 19, 10, ILI9341_RED);
	}
	this->tft->drawCircle(68, 19, 10, ILI9341_WHITE);
}

void ChronoGui::updateGpsSatelliteNumber(int8_t satelliteNumber) {
	/*
	this->tft->setCursor(93, 8);
	this->tft->setFont(mainFont);
	this->tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
	this->tft->setTextSize(1);
	if(satelliteNumber > 0) {
		this->tft->print(satelliteNumber);
		this->tft->print(" sat");
	} else {
		this->tft->print("--");
	}
	*/
}

void ChronoGui::updateGearCounter(uint8_t gear) {
	// TODO
}

