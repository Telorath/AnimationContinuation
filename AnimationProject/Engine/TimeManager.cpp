#include "TimeManager.h"
#include "WindowsTimeManager.h"

TimeManager::~TimeManager()
{
}

TimeManager * TimeManager::CreateWindowsTimeManager()
{
	TimeManager* Return = new WindowsTimeManager();
	return Return;
}
