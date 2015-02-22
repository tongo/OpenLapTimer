
/**
 * Classe che modella il cronometro
 */

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
	private:
		int lapCounter;
		long startLapTimestamp;
		long finishLapTimestamp;

		long bestLapTime;
		long bestLapTimeDelay;
		long previousLapTime;
		long previousLapTimeDelay;
};
