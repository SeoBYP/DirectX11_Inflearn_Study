#include "pch.h"
#include "Graphics.h"

Graphics::Graphics(HWND hwnd)
{
	_hwnd = hwnd; // 윈도우 핸들 저장

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewPort();
}

Graphics::~Graphics()
{
}

void Graphics::RenderBegin()
{
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr); // 렌더 타겟 뷰 설정
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor); // 렌더 타겟 뷰를 지정된 색으로 클리어
	_deviceContext->RSSetViewports(1, &_viewPort); // 뷰포트 설정
}

void Graphics::RenderEnd()
{
	_swapChain->Present(1, 0); // 스왑 체인을 통해 렌더링된 내용을 화면에 표시
}

void Graphics::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.BufferDesc.Width = GWinSizeX;
		desc.BufferDesc.Height = GWinSizeY;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;
		desc.OutputWindow = _hwnd;
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);
	CHECK(hr);
}

void Graphics::CreateRenderTargetView()
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr; // 백 버퍼를 가리키는 포인터

	// 스왑 체인에서 백 버퍼 가져오기
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	// 백 버퍼를 사용해 렌더 타겟 뷰 생성
	hr = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);
}

void Graphics::SetViewPort()
{
	_viewPort.TopLeftX = 0.0f; // 뷰포트의 왼쪽 상단 X 좌표
	_viewPort.TopLeftY = 0.0f; // 뷰포트의 왼쪽 상단 Y 좌표
	_viewPort.Width = static_cast<float>(GWinSizeX); // 뷰포트의 너비
	_viewPort.Height = static_cast<float>(GWinSizeY); // 뷰포트의 높이
	_viewPort.MinDepth = 0.0f; // 뷰포트의 최소 깊이
	_viewPort.MaxDepth = 1.0f; // 뷰포트의 최대 깊이
}
