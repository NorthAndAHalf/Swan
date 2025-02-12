#pragma once

class TimeManager
{
public:
	TimeManager();

	void update_time();

	double get_delta_time();
	double get_time();
private:
	double currentFrameTime;
	double lastFrameTime;
};
