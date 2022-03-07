#pragma once
#include "Window.h"
#include <d3d11.h>

#include <vector>

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics &) = delete;
	Graphics &operator=(const Graphics &) = delete;
	~Graphics();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void EndFrame();
private:
	ID3D11Device *pDevice = nullptr;
	IDXGISwapChain *pSwapChain = nullptr;
	ID3D11DeviceContext *pDeviceContext = nullptr;
	ID3D11RenderTargetView *pRenderTargetView = nullptr;
};