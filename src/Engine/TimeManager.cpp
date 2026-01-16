#include "TimeManager.h"
#include "GLFW/glfw3.h"

TimeManager::TimeManager()
	: m_CurrentFrameTime(0), m_LastFrameTime(0)
{
}

void TimeManager::UpdateTime()
{
	m_CurrentFrameTime = m_LastFrameTime;
	m_LastFrameTime = glfwGetTime();
}

// TODO: Implement moving average calculation
double TimeManager::GetDeltaTime()
{
	return m_CurrentFrameTime - m_LastFrameTime;
}

double TimeManager::GetTime()
{
	return glfwGetTime();
}