#pragma once // 현재 파일이 프로그램 내 다른 소스 파일에 의해 여러 번 include 되는 것을 방지
#include <string>

class Game
{
public:
	Game(); // 생성자
	~Game(); // 소멸자

public:
	void Init(HWND hwnd); // 게임 초기화 함수, 윈도우 핸들을 받음
	void Update(); // 게임 로직 업데이트 함수
	void Render(); // 렌더링 함수

private:
	void RenderBegin(); // 렌더링 시작 전에 필요한 설정을 하는 함수
	void RenderEnd(); // 렌더링 후 처리를 하는 함수

private:
	void CreateDeviceAndSwapChain(); // Direct3D 디바이스와 스왑 체인 생성
	void CreateRenderTargetView(); // 렌더 타겟 뷰 생성
	void SetViewPort(); // 뷰포트 설정

private:
	void CreateGeometry(); // 기하 구조(정점 데이터) 생성
	void CreateInputLayout(); // 입력 레이아웃 생성

	void CreateVS(); // 정점 쉐이더 생성
	void CreatePS(); // 픽셀 쉐이더 생성

	void CreateRasterizerState();
	void CreateSamplerState();
	void CreateBlenderState();
	void CreateSRV();

	void CreateConstantBuffer();

	//Shader 파일 로드
	void LoadShaderFromFile(const wstring& path, const string& name,
		const string& version, ComPtr<ID3DBlob>& blob);

private:
	HWND _hwnd; // 윈도우 핸들
	uint32 _width = 0; // 윈도우 너비
	uint32 _height = 0; // 윈도우 높이

private:
	// DirectX 관련 멤버 변수
	ComPtr<ID3D11Device> _device = nullptr; // Direct3D 디바이스
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr; // 디바이스 컨텍스트
	ComPtr<IDXGISwapChain> _swapChain = nullptr; // 스왑 체인

	/// RTV
	ComPtr<ID3D11RenderTargetView> _renderTargeView; // 렌더 타겟 뷰

	// 기타
	D3D11_VIEWPORT _viewPort = { 0 }; // 뷰포트

	float _clearColor[4] = { 0.f,0.f,0.f,0.f }; // 렌더링 시 배경색

private:
	//Geometry
	vector<Vertex> _vertices;
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;

	vector<uint32> _indices;
	ComPtr<ID3D11Buffer> _indexBuffer = nullptr;

	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;
	//[CPU<->RAM] [GPU<->VRAM]

	//VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr; // 정점 쉐이더
	ComPtr<ID3DBlob> _vsBlob = nullptr; // 정점 쉐이더 바이너리

	//RS
	ComPtr<ID3D11RasterizerState> _resterizerstate = nullptr;

	//PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr; // 픽셀 쉐이더
	ComPtr<ID3DBlob> _psBlob = nullptr; // 픽셀 쉐이더 바이너리

	//SRV
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView;
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView2;

	ComPtr<ID3D11SamplerState> _samplerState;
	ComPtr<ID3D11BlendState> _blendState;
private:
	TransformData _transformData;
	ComPtr<ID3D11Buffer> _constantBuffer;


	Vec3 _localPosition = { 0,0,0 };
	Vec3 _localRotation = { 0,0,0 };
	Vec3 _localScale = { 1,1,1 };

};
