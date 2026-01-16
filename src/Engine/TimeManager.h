#pragma once

class TimeManager
{
public:
	TimeManager();

	void UpdateTime();

	double GetDeltaTime();
	double GetTime();
private:
	double m_CurrentFrameTime;
	double m_LastFrameTime;
};
