#include "pch.h"
#include "ShaderBase.h"

ShaderBase::ShaderBase(ComPtr<ID3D11Device> device) : 
	_device(device)
{
}

ShaderBase::~ShaderBase()
{
}

void ShaderBase::LoadShaderFromFile(const wstring& path, const string& name, const string& version)
{
	_path = path;
	_name = name;

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
		_blob.GetAddressOf(), // 컴파일된 쉐이더를 받을 blob
		nullptr); // 에러 메시지를 받을 blob 없음

	CHECK(hr); // 컴파일 성공 여부 검사
}


VertexShader::VertexShader(ComPtr<ID3D11Device> device) : Super(device)
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::Create(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version); // "VS" 함수를 포함하는 쉐이더 파일 로드

	HRESULT hr = _device->CreateVertexShader(_blob->GetBufferPointer(),
		_blob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf()); // 정점 쉐이더 생성

	CHECK(hr); // 성공 여부 검증
}


PixelShader::PixelShader(ComPtr<ID3D11Device> device) : Super(device)
{
}

PixelShader::~PixelShader()
{
}

void PixelShader::Create(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version); // "VS" 함수를 포함하는 쉐이더 파일 로드

	HRESULT hr = _device->CreatePixelShader(_blob->GetBufferPointer(),
		_blob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf()); // 정점 쉐이더 생성

	CHECK(hr); // 성공 여부 검증
}
