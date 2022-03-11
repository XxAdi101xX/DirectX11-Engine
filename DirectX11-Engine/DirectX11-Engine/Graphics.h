#pragma once
#include "Window.h"
#include <d3d11.h>
#include "wrl.h"

#include <d3dcompiler.h>
#include <vector>

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics &) = delete;
	Graphics &operator=(const Graphics &) = delete;
	~Graphics() = default;
	void ClearBuffer(float red, float green, float blue) noexcept;
	void DrawTestTriangle()
	{
		struct Vertex
		{
			float x;
			float y;
			float z;
			float w;
		};

		// create vertex buffer (1 2d triangle at center of screen)
		const Vertex vertices[] =
		{
			{ 0.0f,0.5f, 0.0f, 1.0f },
			{ 0.5f,-0.5f, 0.0f, 1.0f },
			{ -0.5f, -0.5f, 0.0f, 1.0f },
		};
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
		D3D11_BUFFER_DESC vertexShaderBufferDesc = {};
		vertexShaderBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexShaderBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexShaderBufferDesc.CPUAccessFlags = 0u;
		vertexShaderBufferDesc.MiscFlags = 0u;
		vertexShaderBufferDesc.ByteWidth = sizeof(vertices);
		vertexShaderBufferDesc.StructureByteStride = sizeof(Vertex);
		D3D11_SUBRESOURCE_DATA subresourceData = {};
		subresourceData.pSysMem = vertices;
		CHECK_HRESULT(pDevice->CreateBuffer(&vertexShaderBufferDesc, &subresourceData, &pVertexBuffer));

		// set primative topology
		pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// bind vertex buffer to pipeline
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;
		pDeviceContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		// create vertex shader
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
		CHECK_HRESULT(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
		CHECK_HRESULT(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

		// bind vertex shader
		pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

		// input layout
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
		const D3D11_INPUT_ELEMENT_DESC inputElementDescriptor[] =
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		CHECK_HRESULT(pDevice->CreateInputLayout(inputElementDescriptor, static_cast<UINT>(std::size(inputElementDescriptor)), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));
		pDeviceContext->IASetInputLayout(pInputLayout.Get());

		// create pixel shader
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
		CHECK_HRESULT(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
		CHECK_HRESULT(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

		// bind vertex shader
		pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

		// bind render target
		pDeviceContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), nullptr);

		// configure viewport
		D3D11_VIEWPORT viewport;
		viewport.Width = 1280;
		viewport.Height = 720;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		pDeviceContext->RSSetViewports(1u, &viewport);

		pDeviceContext->Draw(static_cast<UINT>(std::size(vertices)), 0u);
	}
	void EndFrame();
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr <IDXGISwapChain> pSwapChain = nullptr;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> pDeviceContext = nullptr;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> pRenderTargetView = nullptr;
};