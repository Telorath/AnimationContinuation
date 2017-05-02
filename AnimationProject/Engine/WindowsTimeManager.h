#pragma once
#include "TimeManager.h"
#include <Windows.h>
class WindowsTimeManager : public TimeManager
{
//	LARGE_INTEGER TrueTimer;
//	LARGE_INTEGER PerformanceFrequency;
//	float TicksToSeconds;
	unsigned int OldTime;
	float DeltaTime;
public:
	WindowsTimeManager();
	~WindowsTimeManager();
	void init();
	void ping();
	float GetDeltaTime();
};

