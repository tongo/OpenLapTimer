/*
 * ChronoGui.cpp
 *
 *  Created on: 12/lug/2014
 *      Author: Tongo
 */

#include "ChronoGui.h"
#include "MainFont.h"
#include "LapTimeFont.h"
#include "fonts/Arial_bold_14.h"

#define TOP_ROW_X 0
#define TOP_ROW_Y 30
#define TOP_ROW_L 320

#define TOP_ROW_SEPARATOR_X 110
#define TOP_ROW_SEPARATOR_Y 0
#define TOP_ROW_SEPARATOR_H 30

#define BOTTOM_ROW_X 0
#define BOTTOM_ROW_Y 210
#define BOTTOM_ROW_L 320

#define BOTTOM_ROW_SEPARATOR_1_X 80
#define BOTTOM_ROW_SEPARATOR_1_Y 210
#define BOTTOM_ROW_SEPARATOR_1_H 30

#define BOTTOM_ROW_SEPARATOR_2_X 250
#define BOTTOM_ROW_SEPARATOR_2_Y 210
#define BOTTOM_ROW_SEPARATOR_2_H 30

#define GPS_INDICATOR_X 15
#define GPS_INDICATOR_Y 15
#define GPS_INDICATOR_R 10

#define GPS_SAT_TEXTBOX_X 50
#define GPS_SAT_TEXTBOX_Y 4
#define GPS_SAT_TEXTBOX_COL 85
#define GPS_SAT_TEXTBOX_ROW 26

#define LAP_TIME_TEXTBOX_X 24
#define LAP_TIME_TEXTBOX_Y 104
#define LAP_TIME_TEXTBOX_COL 215
#define LAP_TIME_TEXTBOX_ROW 145

#define LAP_COUNT_TEXTBOX_X 10
#define LAP_COUNT_TEXTBOX_Y 220
#define LAP_COUNT_TEXTBOX_COL 75
#define LAP_COUNT_TEXTBOX_ROW 235

#define LAP_DELAY_TEXTBOX_X 85
#define LAP_DELAY_TEXTBOX_Y 220
#define LAP_DELAY_TEXTBOX_COL 245
#define LAP_DELAY_TEXTBOX_ROW 235

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

	// TOP SEPARATOR ROW
	this->tft->ILI9341_due::drawFastHLine(TOP_ROW_X, TOP_ROW_Y, TOP_ROW_L, ILI9341_WHITE);
	this->tft->ILI9341_due::drawFastVLine(TOP_ROW_SEPARATOR_X, TOP_ROW_SEPARATOR_Y, TOP_ROW_SEPARATOR_H, ILI9341_WHITE);

	this->tft->ILI9341_due::drawFastHLine(BOTTOM_ROW_X, BOTTOM_ROW_Y, BOTTOM_ROW_L, ILI9341_WHITE);
	this->tft->ILI9341_due::drawFastVLine(BOTTOM_ROW_SEPARATOR_1_X, BOTTOM_ROW_SEPARATOR_1_Y, BOTTOM_ROW_SEPARATOR_1_H, ILI9341_WHITE);
	this->tft->ILI9341_due::drawFastVLine(BOTTOM_ROW_SEPARATOR_2_X, BOTTOM_ROW_SEPARATOR_2_Y, BOTTOM_ROW_SEPARATOR_2_H, ILI9341_WHITE);

	// GPS satellite number
	this->gpsSatTextBox = new ILI9341_due_gText(this->tft);
	this->gpsSatTextBox->defineArea(GPS_SAT_TEXTBOX_X, GPS_SAT_TEXTBOX_Y, GPS_SAT_TEXTBOX_COL, GPS_SAT_TEXTBOX_ROW);
	this->gpsSatTextBox->selectFont(Arial_bold_14);
	this->gpsSatTextBox->setFontLetterSpacing(5);
	this->gpsSatTextBox->setFontColor(ILI9341_WHITE, ILI9341_BLACK);

	// Time box
	this->timeTextBox = new ILI9341_due_gText(this->tft);
	this->timeTextBox->defineArea(LAP_TIME_TEXTBOX_X, LAP_TIME_TEXTBOX_Y, LAP_TIME_TEXTBOX_COL, LAP_TIME_TEXTBOX_ROW);
	this->timeTextBox->selectFont(lapTimeFont);
	this->timeTextBox->setFontLetterSpacing(5);
	this->timeTextBox->setFontColor(ILI9341_WHITE, ILI9341_BLACK);
	
	// Lap counter
	this->lapCountTextBox = new ILI9341_due_gText(this->tft);
	this->lapCountTextBox->defineArea(LAP_COUNT_TEXTBOX_X, LAP_COUNT_TEXTBOX_Y, LAP_COUNT_TEXTBOX_COL, LAP_COUNT_TEXTBOX_ROW);
	this->lapCountTextBox->selectFont(Arial_bold_14);
	this->lapCountTextBox->setFontLetterSpacing(5);
	this->lapCountTextBox->setFontColor(ILI9341_WHITE, ILI9341_BLACK);
	
	// Lap delay BEST / LAST
	this->lapDelayTextBox = new ILI9341_due_gText(this->tft);
	this->lapDelayTextBox->defineArea(LAP_DELAY_TEXTBOX_X, LAP_DELAY_TEXTBOX_Y, LAP_DELAY_TEXTBOX_COL, LAP_DELAY_TEXTBOX_ROW);
	this->lapDelayTextBox->selectFont(Arial_bold_14);
	this->lapDelayTextBox->setFontLetterSpacing(5);

	Serial.println("ChronoGui INIT - FINISH");
}

void ChronoGui::updateLapTime(long lapTime) {
	uint16_t mins = 0;
	uint16_t secs = 0;
	uint16_t cents = 0;
	cents = lapTime / 10;
	secs = lapTime / 1000; //convect milliseconds to seconds
	mins = secs / 60; //convert seconds to minutes

	cents = cents - (secs * 100);
	secs = secs - (mins * 60); //subtract the coverted seconds to minutes in order to display 59 secs max
	cents = cents / 10;

	char textBuff[20];
	char secBuffer[2];
	char centsBuffer[2];

	sprintf(textBuff, "%d", mins);

	strcat (textBuff, "\'");

	sprintf(secBuffer, "%d", secs);
	if(secs < 10) {
		strcat (textBuff, "0");
	}
	strcat (textBuff, secBuffer);

	strcat (textBuff, "\"");

	sprintf(centsBuffer, "%d", cents);
	strcat (textBuff, centsBuffer);

	this->timeTextBox->drawString(textBuff, gTextAlignTopLeft);
}

void ChronoGui::updateLapNumber(int lapCount) {
	this->lapCountTextBox->clearArea(ILI9341_BLACK);
	if(lapCount > 0) {
		char textBuff[6];
		sprintf(textBuff, "%d lap", lapCount);
		this->lapCountTextBox->drawString(textBuff, gTextAlignMiddleRight);
	} else {
		this->lapCountTextBox->drawString("--", gTextAlignMiddleRight);
	}
}

void ChronoGui::updateGpsFixState(bool gpsState) {
	if(gpsState) {
		this->tft->fillCircle(GPS_INDICATOR_X, GPS_INDICATOR_Y, GPS_INDICATOR_R, ILI9341_GREEN);
	} else {
		this->tft->fillCircle(GPS_INDICATOR_X, GPS_INDICATOR_Y, GPS_INDICATOR_R, ILI9341_RED);
	}
	this->tft->drawCircle(GPS_INDICATOR_X, GPS_INDICATOR_Y, GPS_INDICATOR_R, ILI9341_WHITE);
}

void ChronoGui::updateGpsSatelliteNumber(int8_t satelliteNumber) {
	this->gpsSatTextBox->clearArea(ILI9341_BLACK);
	if(satelliteNumber > 0) {
		char textBuff[7];
		sprintf(textBuff, "%d sat", satelliteNumber);
		this->gpsSatTextBox->drawString(textBuff, gTextAlignMiddleRight);
	} else {
		this->gpsSatTextBox->drawString(":(", gTextAlignMiddleRight);
	}
}

void ChronoGui::updateLapDelay(long delay) {
	bool positive = (delay > 0);
	delay = abs(delay);
	
	uint16_t secs = 0;
	uint16_t cents = 0;
	secs = delay / 1000;
	cents = delay / 10;
	cents = cents - (secs * 100);

	char textBuff[10];
	char centsBuffer[2];

	if(positive) {
		this->lapDelayTextBox->setFontColor(ILI9341_RED, ILI9341_BLACK);
		sprintf(textBuff, "+ %d.", secs);
	} else {
		this->lapDelayTextBox->setFontColor(ILI9341_GREEN, ILI9341_BLACK);
		sprintf(textBuff, "- %d.", secs);
	}

	sprintf(centsBuffer, "%d", cents);
	if(cents < 10) {
		strcat (textBuff, "0");
	}
	strcat (textBuff, centsBuffer);

	this->lapDelayTextBox->clearArea(ILI9341_BLACK);
	this->lapDelayTextBox->drawString(textBuff, gTextAlignMiddleCenter);
}

void ChronoGui::updateGearCounter(uint8_t gear) {
	// TODO
}

