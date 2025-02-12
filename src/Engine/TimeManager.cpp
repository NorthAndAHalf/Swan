#include "TimeManager.h"
#include "GLFW/glfw3.h"

TimeManager::TimeManager()
	: currentFrameTime(0), lastFrameTime(0)
{
}

void TimeManager::update_time()
{
	currentFrameTime = lastFrameTime;
	lastFrameTime = glfwGetTime();
}

// TODO: Implement moving average calculation
double TimeManager::get_delta_time()
{
	return currentFrameTime - lastFrameTime;
}

double TimeManager::get_time()
{
	return glfwGetTime();
}