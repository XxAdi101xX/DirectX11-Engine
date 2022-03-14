#include "Graphics.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// create device and front/back buffers, and swap chain and rendering context
	CHECK_HRESULT(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pDeviceContext
	));

	// Get Texture subresource in swapchain
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRenderTargetView);
}

void Graphics::DrawTestTriangle(float angle)
{
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		} position;

		struct
		{
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
	};

	// create vertex buffer
	const Vertex vertices[] =
	{
		{ 0.0f, 0.5f, 0.0f, 1.0f,	255, 0, 0, 0 },
		{ 0.5f,-0.5f, 0.0f, 1.0f,	0, 255, 0, 0 },
		{ -0.5f, -0.5f, 0.0f, 1.0f,	0, 0, 255, 0 },
		{ -0.3f, 0.3f, 0.0f, 1.0f,	0, 255, 0, 0 },
		{ 0.3f, 0.3f, 0.0f, 1.0f,	0, 0, 255, 0 },
		{ 0.0f, -0.8f, 0.0f, 1.0f,	255, 0, 0, 0 },
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.CPUAccessFlags = 0u;
	vertexBufferDesc.MiscFlags = 0u;
	vertexBufferDesc.ByteWidth = sizeof(vertices);
	vertexBufferDesc.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA vertexBufferSubresourceData = {};
	vertexBufferSubresourceData.pSysMem = vertices;
	CHECK_HRESULT(pDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferSubresourceData, &pVertexBuffer));

	// set primative topology
	pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pDeviceContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// create index buffer
	const unsigned short indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 1,
		2, 1, 5
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA indexBufferSubresourceData = {};
	indexBufferSubresourceData.pSysMem = indices;
	CHECK_HRESULT(pDevice->CreateBuffer(&indexBufferDesc, &indexBufferSubresourceData, &pIndexBuffer));

	// bind index buffer
	pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

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
		{ "Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // D3D11_APPEND_ALIGNED_ELEMENT should be 16 since the position has 4 * 4-byte values
	};
	CHECK_HRESULT(pDevice->CreateInputLayout(inputElementDescriptor, static_cast<UINT>(std::size(inputElementDescriptor)), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));
	pDeviceContext->IASetInputLayout(pInputLayout.Get());

	// create constant buffer for the transformation matrix
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transformation;
	};

	// we device the width by height because of ndc (normalized device coordinate)
	// which treats the viewport like a box with equal width and height so we have to compensate
	const ConstantBuffer transformationBuffer =
	{
		{
			 DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationZ(angle) * DirectX::XMMatrixScaling(720.0f / 1280.0f, 1.0f, 1.0f))
		}
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> pTransformationBuffer;
	D3D11_BUFFER_DESC transformationBufferDesc = {};
	transformationBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	transformationBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	transformationBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	transformationBufferDesc.MiscFlags = 0u;
	transformationBufferDesc.ByteWidth = sizeof(transformationBuffer);
	transformationBufferDesc.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA constantBufferSubresourceData = {};
	constantBufferSubresourceData.pSysMem = &transformationBuffer;
	CHECK_HRESULT(pDevice->CreateBuffer(&transformationBufferDesc, &constantBufferSubresourceData, &pTransformationBuffer));

	// bind transformation buffer
	pDeviceContext->VSSetConstantBuffers(0u, 1u, pTransformationBuffer.GetAddressOf());

	// create pixel shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	CHECK_HRESULT(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
	CHECK_HRESULT(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	// bind pixel shader
	pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	// bind render target
	pDeviceContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), nullptr);

	// configure viewport
	D3D11_VIEWPORT viewport;
	viewport.Width = 1280.0f;
	viewport.Height = 720.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	pDeviceContext->RSSetViewports(1u, &viewport);

	pDeviceContext->DrawIndexed(static_cast<UINT>(std::size(indices)), 0u, 0u);
}

void Graphics::EndFrame()
{
	pSwapChain->Present(1u, 0u);
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red, green, blue, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
}
