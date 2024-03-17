#pragma once
class Graphics
{
public:
	Graphics(HWND hwnd);
	~Graphics();

	void RenderBegin(); // 렌더링 시작 전에 필요한 설정을 하는 함수
	void RenderEnd(); // 렌더링 후 처리를 하는 함수

	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }

private:
	void CreateDeviceAndSwapChain(); // Direct3D 디바이스와 스왑 체인 생성
	void CreateRenderTargetView(); // 렌더 타겟 뷰 생성
	void SetViewPort(); // 뷰포트 설정

private:
	HWND _hwnd; // 윈도우 핸들

private:
	// DirectX 관련 멤버 변수
	ComPtr<ID3D11Device> _device = nullptr; // Direct3D 디바이스
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr; // 디바이스 컨텍스트
	ComPtr<IDXGISwapChain> _swapChain = nullptr; // 스왑 체인

	/// RTV
	ComPtr<ID3D11RenderTargetView> _renderTargetView; // 렌더 타겟 뷰

	// 기타
	D3D11_VIEWPORT _viewPort = { 0 }; // 뷰포트

	float _clearColor[4] = { 0.f,0.f,0.f,0.f }; // 렌더링 시 배경색
};

