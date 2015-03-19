
#include <TouchScreen.h>

class TouchScreenManager {
public:
	TouchScreenManager(TouchScreen* ts);
	virtual ~TouchScreenManager();
	int getChronoOperation();
private:
	TouchScreen* touchScreen;
	Point lastPoint;
	bool touchStarted;
	long touchStartedTimestamp;

	int checkChronoOperation();
};
