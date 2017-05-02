#pragma once
#include "..\Engine\BaseApp.h"
class GameApp : public BaseApp
{

public:
	GameApp();
	~GameApp();
	void init() override;
	void run() override;
	void shutdown() override;
};