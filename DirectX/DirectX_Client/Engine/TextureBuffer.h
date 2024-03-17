#pragma once

class TextureBuffer
{
public:
	// 생성자: 원본 텍스처를 인자로 받습니다.
	TextureBuffer(ComPtr<ID3D11Texture2D> src);
	// 소멸자: 자원을 해제합니다.
	~TextureBuffer();

public:
	// 버퍼를 생성하는 주 함수입니다.
	void CreateBuffer();

private:
	// 입력 텍스처를 설정합니다.
	void CreateInput(ComPtr<ID3D11Texture2D> src);
	// 입력 텍스처에 대한 셰이더 리소스 뷰(SRV)를 생성합니다.
	void CreateSRV();
	// 출력 텍스처를 생성합니다.
	void CreateOutput();
	// 출력 텍스처에 대한 비순차적 액세스 뷰(UAV)를 생성합니다.
	void CreateUAV();
	// 처리 결과를 저장할 텍스처를 생성합니다.
	void CreateResult();

public:
	// 텍스처의 너비, 높이, 배열 크기를 반환하는 접근자 함수들입니다.
	uint32 GetWidth() { return _width; }
	uint32 GetHeight() { return _height; }
	uint32 GetArraySize() { return _arraySize; }

	// 출력 텍스처와 그에 대한 SRV를 반환하는 접근자 함수입니다.
	ComPtr<ID3D11Texture2D> GetOutput() { return (ID3D11Texture2D*)_output.Get(); }
	ComPtr<ID3D11ShaderResourceView> GetOutputSRV() { return _outputSRV; }

public:
	// 입력 및 출력 텍스처에 대한 SRV와 UAV를 반환하는 접근자 함수입니다.
	ComPtr<ID3D11ShaderResourceView> GetSRV() { return _srv; }
	ComPtr<ID3D11UnorderedAccessView> GetUAV() { return _uav; }

private:
	// 입력 및 출력 텍스처와 관련 자원을 저장하는 멤버 변수입니다.
	ComPtr<ID3D11Texture2D> _input;
	ComPtr<ID3D11ShaderResourceView> _srv; // 입력 텍스처의 SRV
	ComPtr<ID3D11Texture2D> _output;
	ComPtr<ID3D11UnorderedAccessView> _uav; // 출력 텍스처의 UAV

private:
	// 텍스처의 너비, 높이, 배열 크기 및 포맷을 저장하는 멤버 변수입니다.
	uint32 _width = 0;
	uint32 _height = 0;
	uint32 _arraySize = 0;
	DXGI_FORMAT _format;
	ComPtr<ID3D11ShaderResourceView> _outputSRV; // 출력 텍스처의 SRV
};