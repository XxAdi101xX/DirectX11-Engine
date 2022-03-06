#pragma once

#include "Window.h"
#include "Timer.h"

class App
{

public:
	App();

	int Process();
private:
	Window wnd;
	Timer timer;

	void ProcessFrame();
};
