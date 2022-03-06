#pragma once
#include "Window.h"

class App
{

public:
	App();

	int Process();
private:
	Window wnd;

	void ProcessFrame();
};
