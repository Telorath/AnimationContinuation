#pragma once
#include <Windows.h>
#include <ctime>
#include "Rendering.h"

class Camera;
class BaseObject;
class TimeManager;

class GameEngine
{
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;
	Rendering::DXManager*			directXManager;
	//Temporary home of the main camera.
	Camera* MainCamera;
	TimeManager* GameTimer;
	std::vector<BaseObject*> BaseObjects;

	void WindowSetup();

public:
	GameEngine(HINSTANCE hinst, WNDPROC proc);
	bool Run();
	bool Shutdown();
};

