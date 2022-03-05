#include <Windows.h>

#include "Window.h"

#include <string>
#include <sstream>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	try {
		Window wnd(1280, 720, L"DirectX11 Engine");

		MSG msg;
		BOOL gResult;
		while (gResult = GetMessage(&msg, nullptr, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (gResult == -1)
		{
			return -1;
		}

		return msg.wParam;
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