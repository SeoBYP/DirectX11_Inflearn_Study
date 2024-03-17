#include "pch.h" // 프리컴파일 헤더
#include "TextureBuffer.h" // TextureBuffer 클래스 정의 포함

// 생성자: 소스 텍스처를 받아서 입력 텍스처를 생성하고, 관련 버퍼를 모두 생성합니다.
TextureBuffer::TextureBuffer(ComPtr<ID3D11Texture2D> src)
{
	CreateInput(src); // 입력 텍스처 생성
	CreateBuffer(); // 나머지 버퍼(SRV, 출력, UAV, 결과) 생성
}

// 소멸자
TextureBuffer::~TextureBuffer()
{
	// Direct3D 자원 해제는 ComPtr에 의해 자동으로 관리됩니다.
}

// 전체 버퍼 생성 작업을 수행합니다.
void TextureBuffer::CreateBuffer()
{
	CreateSRV(); // 입력 텍스처에 대한 SRV 생성
	CreateOutput(); // 출력 텍스처 생성
	CreateUAV(); // 출력 텍스처에 대한 UAV 생성
	CreateResult(); // 결과 데이터를 저장할 텍스처 생성 (주로 읽기 전용)
}

// 입력 텍스처 생성
void TextureBuffer::CreateInput(ComPtr<ID3D11Texture2D> src)
{
	D3D11_TEXTURE2D_DESC srcDesc;
	src->GetDesc(&srcDesc); // 소스 텍스처의 설명 가져오기

	// 입력 텍스처의 기본 정보를 설정합니다.
	_width = srcDesc.Width;
	_height = srcDesc.Height;
	_arraySize = srcDesc.ArraySize;
	_format = srcDesc.Format;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc)); // 메모리 초기화
	desc.Width = _width;
	desc.Height = _height;
	desc.ArraySize = _arraySize;
	desc.Format = _format;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // SRV로 사용됨을 명시
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	// 입력 텍스처 생성
	CHECK(DEVICE->CreateTexture2D(&desc, NULL, _input.GetAddressOf()));

	// 소스 텍스처의 데이터를 입력 텍스처로 복사합니다.
	DC->CopyResource(_input.Get(), src.Get());
}

// 입력 텍스처에 대한 SRV 생성
void TextureBuffer::CreateSRV()
{
	D3D11_TEXTURE2D_DESC desc;
	_input->GetDesc(&desc); // 입력 텍스처의 설명 가져오기

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC)); // 메모리 초기화
	srvDesc.Format = desc.Format; // 포맷 설정
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY; // 텍스처 배열로 사용
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = _arraySize;

	// SRV 생성
	CHECK(DEVICE->CreateShaderResourceView(_input.Get(), &srvDesc, _srv.GetAddressOf()));
}

// 출력 텍스처 생성
void TextureBuffer::CreateOutput()
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC)); // 메모리 초기화
	desc.Width = _width;
	desc.Height = _height;
	desc.ArraySize = _arraySize;
	desc.Format = _format;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE; // UAV 및 SRV로 사용됨
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	// 출력 텍스처 생성
	CHECK(DEVICE->CreateTexture2D(&desc, nullptr, _output.GetAddressOf()));
}

// 출력 텍스처에 대한 UAV 생성
void TextureBuffer::CreateUAV()
{
	D3D11_TEXTURE2D_DESC desc;
	_output->GetDesc(&desc); // 출력 텍스처의 설명 가져오기

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC)); // 메모리 초기화
	uavDesc.Format = DXGI_FORMAT_UNKNOWN; // 포맷 자동 결정
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY; // 텍스처 배열로 사용
	uavDesc.Texture2DArray.ArraySize = _arraySize;

	// UAV 생성
	CHECK(DEVICE->CreateUnorderedAccessView(_output.Get(), &uavDesc, _uav.GetAddressOf()));
}

// 결과 데이터를 저장할 텍스처 생성 (주로 읽기 전용 SRV를 위해 사용됩니다)
void TextureBuffer::CreateResult()
{
	D3D11_TEXTURE2D_DESC desc;
	_output->GetDesc(&desc); // 출력 텍스처의 설명 가져오기

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC)); // 메모리 초기화
	srvDesc.Format = desc.Format; // 포맷 설정
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY; // 텍스처 배열로 사용
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = _arraySize;

	// 결과용 SRV 생성
	CHECK(DEVICE->CreateShaderResourceView(_output.Get(), &srvDesc, _outputSRV.GetAddressOf()));
}
