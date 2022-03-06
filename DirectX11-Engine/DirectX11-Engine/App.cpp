#include "App.h"

#include "Window.h"

App::App()
	:
	wnd(1280, 720, L"DirectX11 Engine")
{}

int App::Process()
{
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		ProcessFrame();
	}

	// check if GetMessage call itself borked
	if (gResult == -1)
	{
		throw WIN32EXCEPTION(GetLastError());
	}

	// wParam here is the value passed to PostQuitMessage
	return msg.wParam;
}

void App::ProcessFrame()
{

}