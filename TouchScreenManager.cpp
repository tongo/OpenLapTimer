
#include "Arduino.h"
#include "TouchScreenManager.h"

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

#define TFT_HEIGHT 240
#define TFT_WIDTH 320

#define TOUCH_MIN_DURATION 100 // millisecond

#define NEXT_TRACK_AREA_XM 0
#define NEXT_TRACK_AREA_XP 35
#define NEXT_TRACK_AREA_YM 0
#define NEXT_TRACK_AREA_YP 70

#define PREV_TRACK_AREA_XM 0
#define PREV_TRACK_AREA_XP 35
#define PREV_TRACK_AREA_YM 76
#define PREV_TRACK_AREA_YP 146

#define DEBUG_TOUCH false

TouchScreenManager::TouchScreenManager(TouchScreen* ts) {
	this->touchScreen = ts;
	lastPoint = Point(0, 0, 0);
}

TouchScreenManager::~TouchScreenManager() { }

int TouchScreenManager::getChronoOperation() {
	int operation = 0;
	Point p = this->touchScreen->getPoint();  
	// we have some minimum pressure we consider 'valid'
	// pressure of 0 means no pressing!
	if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
		if(touchStarted && (millis() - touchStartedTimestamp) > TOUCH_MIN_DURATION) {
			touchStarted = false;
			operation = checkChronoOperation();
		}
	} else {
		touchStarted = true;
		touchStartedTimestamp = millis();

		// Scale from ~0->1000 to tft.width using the calibration #'s
		lastPoint.x = map(p.x, TS_MINX, TS_MAXX, 0, TFT_WIDTH);
		lastPoint.y = map(p.y, TS_MINY, TS_MAXY, 0, TFT_HEIGHT);

		#if DEBUG_TOUCH
			Serial.print("X = "); Serial.print(lastPoint.x);
			Serial.print("\tY = "); Serial.print(lastPoint.y);
			Serial.print("\tPressure = "); Serial.println(p.z);
		#endif
	}

	return operation;
}

int TouchScreenManager::checkChronoOperation() {
	// Next Track
	if(lastPoint.x > NEXT_TRACK_AREA_XM && lastPoint.x < NEXT_TRACK_AREA_XP && lastPoint.y > NEXT_TRACK_AREA_YM && lastPoint.y < NEXT_TRACK_AREA_YP) { return OPERATION_NEXT_TRACK; }
	// Prev Track
	if(lastPoint.x > PREV_TRACK_AREA_XM && lastPoint.x < PREV_TRACK_AREA_XP && lastPoint.y > PREV_TRACK_AREA_YM && lastPoint.y < PREV_TRACK_AREA_YP) { return OPERATION_PREV_TRACK; }

	return OPERATION_NO_OPERATION;
}