/*
 * Track.h
 *
 *  Created on: 16/ago/2014
 *      Author: Tongo
 */

#ifndef TRACK_H_
#define TRACK_H_

#import "GpsPoint.h"

class Track {
public:
	Track();
	virtual ~Track();

	GpsPoint* finishLinePoint1;
	GpsPoint* finishLinePoint2;

	GpsPoint* isFinishLinePassed(GpsPoint* start, GpsPoint* finish);
};

#endif /* TRACK_H_ */
