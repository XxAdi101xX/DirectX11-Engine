#pragma once
#include "Window.h"
#include <d3d11.h>
#include "wrl.h"

#include <d3dcompiler.h>
#include <vector>
#include <cmath>
#include <DirectXMath.h>

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics &) = delete;
	Graphics &operator=(const Graphics &) = delete;
	~Graphics() = default;
	void ClearBuffer(float red, float green, float blue) noexcept;
	void DrawTestTriangle(float angle);
	void EndFrame();
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr <IDXGISwapChain> pSwapChain = nullptr;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> pDeviceContext = nullptr;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> pRenderTargetView = nullptr;
};