#pragma once

#include <Windows.h>
#include <string>

#include "BaseException.h"
#include "Keyboard.h"
#include "Mouse.h"

class Window
{

public:
	Window(int width, int height, const wchar_t* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	Keyboard keyboard;
	Mouse mouse;

	void SetTitle(const std::wstring &title) const;

	static std::wstring TranslateErrorCode(HRESULT hr) noexcept;

	class Win32Exception : public BaseException
	{
	public:
		Win32Exception(int line, const char *file, HRESULT hResult) noexcept;
		virtual const wchar_t *What() const noexcept override;
		virtual const wchar_t *GetType() const noexcept override;

		HRESULT GetHResult() const noexcept;
	private:
		HRESULT hResult;
	};
private:
	int width;
	int height;
	HWND hWnd;

	// Note: we are making the following two methods static so that it is compatible with windows api calls as these functions can't be member functions; core logic will still reside on the HandleMsg member function
	// Setup the win32 side pointers to the HandleMsgMain and hWnd so that they can later invoke the HandleMsg member function
	static LRESULT WINAPI HandleMsgInit(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	// Obtain the hWnd from USERDATA and invoke HandleMessage
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	// Singleton WindowClass
	class WindowClass
	{
	public:
		static const wchar_t *GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();

		WindowClass(const WindowClass &) = delete;
		WindowClass &operator=(const WindowClass &) = delete;

		static WindowClass windowClass;
		static constexpr const wchar_t *wndClassName = L"DirectX11 Engine Window";
		HINSTANCE hInst;
	};
};

std::string ConvertWideToUtf8(const std::wstring &wstr);

std::wstring ConvertUtf8ToWide(const std::string &str);

#define WIN32EXCEPTION(hResult) Window::Win32Exception( __LINE__,__FILE__, hResult)


