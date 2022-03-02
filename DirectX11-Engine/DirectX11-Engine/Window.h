#include <Windows.h>

class Window
{
private:
	// Singleton
	class WindowClass
	{
	public:
		static const wchar_t *GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();

		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;

		static WindowClass windowClass;
		static constexpr const wchar_t *wndClassName = L"DirectX11 Engine Window";
		HINSTANCE hInst;
	};
public:
	Window(int width, int height, const wchar_t* name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
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
};