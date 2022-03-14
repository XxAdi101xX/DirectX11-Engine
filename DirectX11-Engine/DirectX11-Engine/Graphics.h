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
	void DrawTestTriangle(float angle, float x, float z);
	void EndFrame();
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr <IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> pDeviceContext;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> pRenderTargetView;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView> pDepthStencilView;
};