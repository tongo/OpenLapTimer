/*
 * Track.cpp
 *
 *  Created on: 16/ago/2014
 *      Author: Tongo
 */

#include "Track.h"
#include "Arduino.h"

#define DEBUG_LOG_INTERSECTION false

Track::Track() {
	finishLinePoint1 = new GpsPoint(0, 0);
	finishLinePoint2 = new GpsPoint(0, 0);

	char textBuff[4];
	sprintf(textBuff, "none");
	trackName = textBuff;
	
	trackIndex = 0;
}

Track::~Track() { }

GpsPoint* Track::isFinishLinePassed(GpsPoint* start, GpsPoint* finish) {
	// TODO calcolo la retta dei due punti GPS
	// Calcolo l'intersezione (se c'e)
	// Verifico che l'intersezione sia su una delle 2 rette

	// TODO calculating the straight line of the two GPS points
	// Calculate the intersection (if any)
	// I verify that the intersection is on one of the two straight lines

	// LATITUDE = Y
	// LONGITUDE = X
	
	// retta1: 1-2
	// retta2: 3-4
	
	#if DEBUG_LOG_INTERSECTION
		Serial.println("check interseption");
		Serial.print("x1= "); Serial.print(start->longitude, 6); 			Serial.print(", y1= "); Serial.println(start->latitude, 6);
		Serial.print("x2= "); Serial.print(finish->longitude, 6); 			Serial.print(", y2= "); Serial.println(finish->latitude, 6);
		Serial.print("x3= "); Serial.print(finishLinePoint1->longitude, 6); Serial.print(", y3= "); Serial.println(finishLinePoint2->latitude, 6);
		Serial.print("x4= "); Serial.print(finishLinePoint2->longitude, 6); Serial.print(", y4= "); Serial.println(finishLinePoint1->latitude, 6);
	#endif

	// delta0 = (y4-y3)*(x2-x1) - (x4-x3)*(y2-y1)
	double delta0 = (finishLinePoint1->latitude - finishLinePoint2->latitude) * (finish->longitude - start->longitude) -
			(finishLinePoint1->longitude - finishLinePoint2->longitude) * (finish->latitude - start->latitude);

	#if DEBUG_LOG_INTERSECTION
		Serial.print("delta0= "); Serial.print(delta0, 6); Serial.print(", ");
	#endif

	if(delta0 == 0.0 || delta0 == -0.0) {
		return 0;
	}

	// delta1 = (x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)
	double delta1 = (finishLinePoint1->longitude - finishLinePoint2->longitude) * (start->latitude - finishLinePoint2->latitude) -
			(finishLinePoint1->latitude - finishLinePoint2->latitude) * (start->longitude - finishLinePoint2->longitude);
	
	#if DEBUG_LOG_INTERSECTION
		Serial.print("delta1= "); Serial.print(delta1, 6); Serial.print(", ");
	#endif

	// delta2 = (x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)
	double delta2 = (finish->longitude - start->longitude) * (start->latitude - finishLinePoint2->latitude) -
			(finish->latitude - start->latitude) * (start->longitude - finishLinePoint2->longitude);

	#if DEBUG_LOG_INTERSECTION
		Serial.print("delta2= "); Serial.print(delta2, 6); Serial.print(", ");
	#endif

	// ka = Delta1/Delta0
	double ka = delta1 / delta0;
	// kb = Delta2/Delta0
	double kb = delta2 / delta0;

	#if DEBUG_LOG_INTERSECTION
		Serial.print("ka= "); Serial.print(ka, 6); Serial.print(", ");
		Serial.print("kb= "); Serial.print(kb, 6); Serial.print(", ");
	#endif

	if(ka < 0 || ka > 1 || kb < 0 || kb > 1) { return 0; }

	// x= x1+ ka*(x2-x1)
	float longitude = start->longitude + ka * (finish->longitude - start->longitude);

	// y= y1+ ka*(y2-y1)
	float latitude = start->latitude + ka * (finish->latitude - start->latitude);

	GpsPoint* intersection = new GpsPoint(latitude, longitude);
	return intersection;
}

/*
struct point { int x, y; };
struct line  { struct point p1, p2; };

int ccw (struct point p0, struct point p1, struct point p2) {
   int dx1,dx2,dy1,dy2;

   dx1 = p1.x-p0.x;
   dy1 = p1.y-p0.y;
   dx2 = p2.x-p0.x;
   dy2 = p2.y-p0.y;

   if (dx1*dy2 > dy1*dx2) return +1;
   if (dx1*dy2 < dy1*dx2) return -1;
   if ((dx1*dx2 < 0) || (dy1*dy2 < 0)) return -1;
   if ((dx1*dx1+dy1*dy1) < (dx2*dx2+dy2*dy2)) return +1;
   return 0;
}

int intersect(struct line l1, struct line l2) {
   return ((ccw(l1.p1,l1.p2,l2.p1)*ccw(l1.p1,l1.p2,l2.p2)) <=0) &&
          ((ccw(l2.p1,l2.p2,l1.p1)*ccw(l2.p1,l2.p2,l1.p2)) <=0);
}
*/
