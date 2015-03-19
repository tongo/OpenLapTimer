
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

		Serial.print("X = "); Serial.print(lastPoint.x);
		Serial.print("\tY = "); Serial.print(lastPoint.y);
		Serial.print("\tPressure = "); Serial.println(p.z);
	}

	return operation;
}

int TouchScreenManager::checkChronoOperation() {
	if(lastPoint.x > 0 && lastPoint.x < 35 && lastPoint.y > 0 && lastPoint.y < 146) { return 1; }
	return 0;
}