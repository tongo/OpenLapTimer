
/**
 * Classe che modella il cronometro
 */

#ifndef LAPTIMER_H_
#define LAPTIMER_H_

class LapTimer {
	public:
		LapTimer();
		int getLapNumber();
		long getCurrentLapTime();
		void startTimer();
		void stopTimer();
		void newLap();
		void sector();
		long getBestLapDelay();
		long getLastLapDelay();
		long getLastLapTime();
	private:
		int lapCounter;
		long startLapTimestamp;
		long finishLapTimestamp;

		long bestLapTime;
		long bestLapTimeDelay;
		long previousLapTime;
		long previousLapTimeDelay;
};

#endif /* LAPTIMER_H_ */