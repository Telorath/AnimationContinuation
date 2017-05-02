#include "BaseApp.h"

BaseApp* ApplicationInstance = nullptr;

void BaseApp::SetActiveApplication()
{
	ApplicationInstance = this;
}

BaseApp * BaseApp::GetApplication()
{
	return ApplicationInstance;
}

BaseApp::BaseApp()
{
}


BaseApp::~BaseApp()
{
}
