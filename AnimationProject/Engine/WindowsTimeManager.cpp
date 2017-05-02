#include "WindowsTimeManager.h"
#include <Windows.h>

WindowsTimeManager::WindowsTimeManager()
{
}


WindowsTimeManager::~WindowsTimeManager()
{
	return;
}

void WindowsTimeManager::init()
{
//	QueryPerformanceCounter(&TrueTimer);
//	QueryPerformanceFrequency(&PerformanceFrequency);
	DeltaTime = 0;
//	TicksToSeconds = 1;// / 10000000.0f;
//	TicksToSeconds = TicksToSeconds / PerformanceFrequency.QuadPart;
	OldTime = (unsigned int)GetTickCount64();
}

void WindowsTimeManager::ping()
{
	/*
	LARGE_INTEGER Oldtime = TrueTimer;
	QueryPerformanceCounter(&TrueTimer);
	LARGE_INTEGER Delta;
	Delta.QuadPart = TrueTimer.QuadPart - Oldtime.QuadPart;
//	Delta.u = TrueTimer.u;
	DeltaTime = (float)(Delta.QuadPart * TicksToSeconds);
//	Delta.QuadPart /= 10000;
//	Delta.QuadPart /= PerformanceFrequency.QuadPart;
//	DeltaTime = Delta.QuadPart;*/
	unsigned int NewTime =(unsigned int)GetTickCount64();
	DeltaTime = (NewTime - OldTime) / 1000.0f;
	OldTime = NewTime;
}

float WindowsTimeManager::GetDeltaTime()
{
	return DeltaTime;
}