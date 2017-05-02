#pragma once
class __declspec(novtable) TimeManager
{
public:
	TimeManager() {};
	~TimeManager();
	//Resets the deltatime to the current time = 0ms
	virtual void init() = 0;
	//Determines the deltatime since the last call of ping.
	virtual void ping() = 0;
	//Returns the deltatime between the last two calls of ping()
	virtual float GetDeltaTime() = 0;
	static TimeManager* CreateWindowsTimeManager();
};