#include "App.h"

#include "Window.h"

#include <sstream>
#include <iomanip>
#include "Graphics.h"

App::App() : wnd(1280, 720, L"DirectX11 Engine")
{}

int App::Process()
{
	timer.start();

	// TODO remove once issues regarding freezes are resolved
#if 0
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
#endif

	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const std::optional<int> ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		ProcessFrame();
	}
}

void App::ProcessFrame()
{
	const double t = timer.elapsed();
	std::wstringstream oss;
	oss << L"Time elapsed: " << std::setprecision(1) << std::fixed << t << "s";
	wnd.SetTitle(oss.str());

	wnd.GetGraphics().ClearBuffer(1.0f, 0.5f, 0.0f);
	wnd.GetGraphics().EndFrame();
}