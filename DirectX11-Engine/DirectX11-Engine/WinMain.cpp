#include <Windows.h>

#include "App.h"

#include <string>
#include <sstream>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	try {
		return App{}.Process();
	}
	catch (const BaseException &e)
	{
		MessageBox(nullptr, e.What(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception &e)
	{
		MessageBox(nullptr, ConvertUtf8ToWide(e.what()).c_str(), L"StandardException", MB_OK | MB_ICONEXCLAMATION);
	}
}