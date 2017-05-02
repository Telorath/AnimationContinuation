#pragma once
//Used to make a window with no functionality. Derive to create additional functionality.
class BaseApp
{
protected:
	void SetActiveApplication();
public:
	static BaseApp* GetApplication();
	BaseApp();
	~BaseApp();
	virtual void init() { };
	virtual void run() { };
	virtual void shutdown() { };
};

