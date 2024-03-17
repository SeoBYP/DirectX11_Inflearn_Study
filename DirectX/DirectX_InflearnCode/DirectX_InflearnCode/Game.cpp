#include "pch.h" // 사전 컴파일된 헤더 파일
#include "Game.h" // Game 클래스 정의 포함

Game::Game() // 생성자 구현
{
}

Game::~Game() // 소멸자 구현
{
}

void Game::Init(HWND hwnd) // 초기화 함수 구현
{
	_hwnd = hwnd; // 윈도우 핸들 저장
	_width = GWinSizeX; // 전역 변수에서 윈도우 너비 설정
	_height = GWinSizeY; // 전역 변수에서 윈도우 높이 설정

	CreateDeviceAndSwapChain(); // 디바이스와 스왑 체인 생성
	CreateRenderTargetView(); // 렌더 타겟 뷰 생성
	SetViewPort(); // 뷰포트 설정

	CreateGeometry(); // 기하 구조 생성
	CreateVS(); // 정점 쉐이더 생성
	CreateInputLayout(); // 입력 레이아웃 생성
	CreatePS(); // 픽셀 쉐이더 생성

	CreateSRV();
	CreateConstantBuffer();
	//CreateRasterizerState();
	//CreateSamplerState();
	CreateBlenderState();
}

void Game::Update() // 게임 로직 업데이트
{
	_localPosition.x += 0.001f;

	Matrix matScale = Matrix::CreateScale(_localScale / 3);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	Matrix matWorld = matScale * matRotation * matTranslation;//SRT
	_transformData.matWorld = matWorld;

	//_transformData.offset.x += 0.003f;
	//_transformData.offset.y += 0.003f;

	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));

	_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	::memcpy(subResource.pData, &_transformData, sizeof(_transformData));
	_deviceContext->Unmap(_constantBuffer.Get(), 0);

}

void Game::Render() // 렌더링 함수 구현
{
	RenderBegin(); // 렌더링 시작 전 설정

	// 렌더링 할 내용 TODO: 실제 렌더링 로직 추가
	//IA - VS - RS - PS - OM
	{
		uint32 stride = sizeof(Vertex); // 정점 데이터의 크기
		uint32 offset = 0; // 버퍼 시작 위치
		// 입력 어셈블러(IA) 스테이지 설정
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		_deviceContext->IASetInputLayout(_inputLayout.Get());
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 정점 쉐이더(VS) 스테이지 설정
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
		_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
		
		//RS
		//_deviceContext->RSSetState(_resterizerstate.Get());


		// 픽셀 쉐이더(PS) 스테이지 설정
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
		_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
		_deviceContext->PSSetShaderResources(1, 1, _shaderResourceView2.GetAddressOf());
		//_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());

		// 출력 병합(OM) 스테이지 설정
		_deviceContext->OMSetBlendState(_blendState.Get(),nullptr,0xFFFFFFFF);


		//_deviceContext->Draw(_vertices.size(), 0); // 정점들을 그림
		_deviceContext->DrawIndexed(_indices.size(), 0, 0);
	}


	RenderEnd(); // 렌더링 후 처리
}

void Game::RenderBegin() // 렌더링 시작 전 설정
{
	_deviceContext->OMSetRenderTargets(1, _renderTargeView.GetAddressOf(), nullptr); // 렌더 타겟 뷰 설정
	_deviceContext->ClearRenderTargetView(_renderTargeView.Get(), _clearColor); // 렌더 타겟 뷰를 지정된 색으로 클리어
	_deviceContext->RSSetViewports(1, &_viewPort); // 뷰포트 설정
}

void Game::RenderEnd() // 렌더링 후 처리
{
	_swapChain->Present(1, 0); // 스왑 체인을 통해 렌더링된 내용을 화면에 표시
}

void Game::CreateDeviceAndSwapChain() {
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height;
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

void Game::CreateRenderTargetView()
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr; // 백 버퍼를 가리키는 포인터

	// 스왑 체인에서 백 버퍼 가져오기
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	// 백 버퍼를 사용해 렌더 타겟 뷰 생성
	hr = _device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargeView.GetAddressOf());
	CHECK(hr);
}

void Game::SetViewPort()
{
	_viewPort.TopLeftX = 0.0f; // 뷰포트의 왼쪽 상단 X 좌표
	_viewPort.TopLeftY = 0.0f; // 뷰포트의 왼쪽 상단 Y 좌표
	_viewPort.Width = static_cast<float>(_width); // 뷰포트의 너비
	_viewPort.Height = static_cast<float>(_height); // 뷰포트의 높이
	_viewPort.MinDepth = 0.0f; // 뷰포트의 최소 깊이
	_viewPort.MaxDepth = 1.0f; // 뷰포트의 최대 깊이
}

void Game::CreateGeometry()
{
	/*정점 데이터 설정 Vertex Data(삼각형)
	{
		_vertices.resize(3); // 삼각형을 구성할 세 개의 정점

		// 첫 번째 정점: 위치와 색상 설정
		_vertices[0].position = Vec3(-0.5f, -0.5f, 0.f); // 화면 중앙 좌측 하단
		_vertices[0].color = Color(1.f, 0.f, 0.f, 1.f); // 빨간색

		// 두 번째 정점: 위치와 색상 설정
		_vertices[1].position = Vec3(0.f, 0.5f, 0.f); // 화면 중앙 상단
		_vertices[1].color = Color(0.f, 1.f, 0.f, 1.f); // 녹색

		// 세 번째 정점: 위치와 색상 설정
		_vertices[2].position = Vec3(0.5f, -0.5f, 0.f); // 화면 중앙 우측 하단
		_vertices[2].color = Color(0.f, 0.f, 1.f, 1.f); // 파란색

		// 세 번째 정점: 위치와 색상 설정
		_vertices[2].position = Vec3(0.5f, -0.5f, 0.f); // 화면 중앙 우측 하단
		_vertices[2].color = Color(0.f, 0.f, 1.f, 1.f); // 파란색
	}
	*/ 

	/*정점 데이터 설정 Vertex Data(사각형)
	{
		// 정점 데이터 수정: 사각형을 그리기 위해 4개의 정점 정의
		_vertices.resize(6); // 사각형을 그리기 위해 2개의 삼각형, 즉 6개의 정점이 필요합니다.

		// 첫 번째 삼각형
		_vertices[0].position = Vec3(-0.5f, -0.5f, 0.f); // 왼쪽 아래
		_vertices[0].color = Color(1.f, 0.f, 0.f, 1.f); // 빨간색

		_vertices[1].position = Vec3(-0.5f, 0.5f, 0.f); // 왼쪽 위
		_vertices[1].color = Color(0.f, 1.f, 0.f, 1.f); // 녹색

		_vertices[2].position = Vec3(0.5f, -0.5f, 0.f); // 오른쪽 아래
		_vertices[2].color = Color(0.f, 0.f, 1.f, 1.f); // 파란색

		// 두 번째 삼각형
		_vertices[3].position = Vec3(-0.5f, 0.5f, 0.f); // 왼쪽 위
		_vertices[3].color = Color(0.f, 1.f, 0.f, 1.f); // 녹색

		_vertices[4].position = Vec3(0.5f, 0.5f, 0.f); // 오른쪽 위
		_vertices[4].color = Color(1.f, 0.f, 0.f, 1.f); // 빨간색

		_vertices[5].position = Vec3(0.5f, -0.5f, 0.f); // 오른쪽 아래
		_vertices[5].color = Color(0.f, 0.f, 1.f, 1.f); // 파란색
	}
	*/

	{
		_vertices.resize(4); // 삼각형을 구성할 세 개의 정점

		//13
		//02
		// 첫 번째 정점: 위치와 색상 설정
		_vertices[0].position = Vec3(-0.5f, -0.5f, 0.f); // 화면 중앙 좌측 하단
		_vertices[0].uv = Vec2(0.f, 1.f);

		// 두 번째 정점: 위치와 색상 설정
		_vertices[1].position = Vec3(-0.5f, 0.5f, 0.f); // 화면 중앙 상단
		_vertices[1].uv = Vec2(0.f, 0.f);

		// 세 번째 정점: 위치와 색상 설정
		_vertices[2].position = Vec3(0.5f, -0.5f, 0.f); // 화면 중앙 우측 하단
		_vertices[2].uv = Vec2(1.f, 1.f);

		// 네 번째 정점: 위치와 색상 설정
		_vertices[3].position = Vec3(0.5f, 0.5f, 0.f); // 화면 중앙 우측 하단
		_vertices[3].uv = Vec2(1.f, 0.f);
	}


	// 정점 버퍼 생성 Vectex Buffer
	{
		D3D11_BUFFER_DESC desc; // 버퍼 설명 구조체
		ZeroMemory(&desc, sizeof(desc)); // 메모리 초기화
		desc.Usage = D3D11_USAGE_IMMUTABLE; // 버퍼는 GPU에서만 사용되며 CPU에서는 수정할 수 없음
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 이 버퍼를 정점 버퍼로 사용하겠다는 의미
		desc.ByteWidth = (uint32)sizeof(Vertex) * _vertices.size(); // 버퍼의 전체 크기

		D3D11_SUBRESOURCE_DATA data; // 초기 데이터를 지정하기 위한 구조체
		ZeroMemory(&data, sizeof(data)); // 메모리 초기화
		data.pSysMem = _vertices.data(); // 정점 데이터의 포인터

		_device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf()); // 버퍼 생성
	}

	// 인덱스 버퍼 생성 
	{
		_indices = { 0,1,2,2,1,3 };
	}

	// IndexBuffer
	{
		D3D11_BUFFER_DESC desc; // 버퍼 설명 구조체
		ZeroMemory(&desc, sizeof(desc)); // 메모리 초기화
		desc.Usage = D3D11_USAGE_IMMUTABLE; // 버퍼는 GPU에서만 사용되며 CPU에서는 수정할 수 없음
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER; // 이 버퍼를 인덱스 버퍼로 사용하겠다는 의미
		desc.ByteWidth = (uint32)sizeof(uint32) * _indices.size(); // 버퍼의 전체 크기

		D3D11_SUBRESOURCE_DATA data; // 초기 데이터를 지정하기 위한 구조체
		ZeroMemory(&data, sizeof(data)); // 메모리 초기화
		data.pSysMem = _indices.data(); // 인덱스 데이터의 포인터
		// 인덱스 버퍼 생성
		HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf()); 
		
		CHECK(hr);
	}
}

void Game::CreateInputLayout()
{
	// 입력 요소 설명 배열
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, // "POSITION" 세맨틱: 3D 위치 데이터
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}, // "COLOR" 세맨틱: 색상 데이터
	};
	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC); // 요소 개수 계산
	_device->CreateInputLayout(layout, count,
		_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf()); // 입력 레이아웃 생성
}

void Game::CreateVS()
{
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob); // "VS" 함수를 포함하는 쉐이더 파일 로드

	HRESULT hr = _device->CreateVertexShader(_vsBlob->GetBufferPointer(),
		_vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf()); // 정점 쉐이더 생성

	CHECK(hr); // 성공 여부 검증
}

void Game::CreatePS()
{
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob); // "PS" 함수를 포함하는 쉐이더 파일 로드

	HRESULT hr = _device->CreatePixelShader(_psBlob->GetBufferPointer(),
		_psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf()); // 픽셀 쉐이더 생성

	CHECK(hr); // 성공 여부 검증
}

void Game::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = false;

	HRESULT hr = _device->CreateRasterizerState(&desc, _resterizerstate.GetAddressOf());

	CHECK(hr);
}

void Game::CreateSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.BorderColor[0] = 1;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 1;

	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MaxAnisotropy = 16;
	desc.MaxLOD = FLT_MAX;
	desc.MinLOD = FLT_MIN;
	desc.MipLODBias = 0.0f;

	HRESULT hr = _device->CreateSamplerState(&desc,_samplerState.GetAddressOf());
	CHECK(hr);
}

void Game::CreateBlenderState()
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = _device->CreateBlendState(&desc, _blendState.GetAddressOf());
	CHECK(hr);
}

void Game::CreateSRV()
{
	// TexMetadata와 ScratchImage는 DirectX 텍스처 로딩에 사용되는 구조체입니다.
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;

	// LoadFromWICFile 함수는 WIC(Windows Imaging Component)를 사용하여
	// "UnityLogo.png" 이미지 파일을 로드합니다. 로드된 이미지 데이터는 img에 저장되고,
	// 이미지의 메타데이터는 md에 저장됩니다.
	HRESULT hr = ::LoadFromWICFile(L"UnityLogo.png", WIC_FLAGS_NONE, &md, img);
	// 결과를 확인하여 로드 과정에 문제가 없는지 검사합니다.
	CHECK(hr);

	// CreateShaderResourceView 함수는 로드된 이미지 데이터를 사용하여
	// 셰이더 리소스 뷰(Shader Resource View, SRV)를 생성합니다. 이 SRV는
	// DirectX 11의 셰이더에서 텍스처로 사용될 수 있습니다.
	hr = ::CreateShaderResourceView(_device.Get(), img.GetImages(), img.GetImageCount(), md,
		_shaderResourceView.GetAddressOf());
	// 결과를 확인하여 SRV 생성 과정에 문제가 없는지 검사합니다.
	CHECK(hr);

	hr = ::LoadFromWICFile(L"Skeleton.png", WIC_FLAGS_NONE, &md, img);
	// 결과를 확인하여 로드 과정에 문제가 없는지 검사합니다.
	CHECK(hr);
	hr = ::CreateShaderResourceView(_device.Get(), img.GetImages(), img.GetImageCount(), md,
		_shaderResourceView2.GetAddressOf());
	// 결과를 확인하여 SRV 생성 과정에 문제가 없는지 검사합니다.
	CHECK(hr);
}

void Game::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC; // CPU_Write + GPU_Read
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(TransformData); // 구조체 크기만큼 할당
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU 접근 권한 설정

	HRESULT hr = _device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
	CHECK(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& name,
	const string& version, ComPtr<ID3DBlob>& blob)
{
	// 쉐이더 컴파일 옵션
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(), // 쉐이더 파일 경로
		nullptr, // 매크로 정의 없음
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // 표준 include 처리
		name.c_str(), // 쉐이더 엔트리 포인트 함수 이름
		version.c_str(), // 쉐이더 모델 버전
		compileFlag, // 컴파일 플래그
		0, // 플래그 없음
		blob.GetAddressOf(), // 컴파일된 쉐이더를 받을 blob
		nullptr); // 에러 메시지를 받을 blob 없음

	CHECK(hr); // 컴파일 성공 여부 검사
}


//DXGI
//D3D11_USAGE_IMMUTABLE