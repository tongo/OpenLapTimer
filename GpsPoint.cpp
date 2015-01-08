/*
 * Track.cpp
 *
 *  Created on: 16/ago/2014
 *      Author: Tongo
 */

#include "GpsPoint.h"

GpsPoint::GpsPoint() {
	this->latitude = 0.0;
	this->longitude = 0.0;
}

GpsPoint::GpsPoint(float latitude, float longitude) {
	this->latitude = latitude;
	this->longitude = longitude;
}

bool GpsPoint::equalsGpsPoint(GpsPoint* point) {
	if(point->latitude == latitude && point->longitude == longitude) { return true; }
	return false;
}

void GpsPoint::updatePointDecimal(float latitude, float longitude) {
	this->latitude = latitude;
	this->longitude = longitude;
}

void GpsPoint::updatePointDegree(float latitude, float longitude) {
	this->latitude = convertDegMinToDecDeg(latitude);
	this->longitude = convertDegMinToDecDeg(longitude);
}

GpsPoint::~GpsPoint() { }


// converts lat/long from Adafruit
// degree-minute format to decimal-degrees
double GpsPoint::convertDegMinToDecDeg (float degMin) {
  double min = 0.0;
  double decDeg = 0.0;

  //get the minutes, fmod() requires double
  min = fmod((double)degMin, 100.0);

  //rebuild coordinates in decimal degrees
  degMin = (int) ( degMin / 100 );
  decDeg = degMin + ( min / 60 );

  return decDeg;
}
