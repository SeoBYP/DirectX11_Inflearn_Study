#include "pch.h"
#include "RawBuffer.h"

RawBuffer::RawBuffer(void* inputData, uint32 inputByte, uint32 outputByte)
	: _inputData(inputData), _inputByte(inputByte), _outputByte(outputByte)
{
	CreateBuffer();
}

RawBuffer::~RawBuffer()
{

}

// 버퍼 생성 관련 여러 함수를 순서대로 호출
void RawBuffer::CreateBuffer()
{
	CreateInput(); // 입력 버퍼 생성
	CreateSRV(); // 쉐이더 리소스 뷰 생성
	CreateOutput(); // 출력 버퍼 생성
	CreateUAV(); // 비순차적 액세스 뷰 생성
	CreateResult(); // 결과 버퍼 생성
}

// 입력 버퍼에 데이터를 복사
void RawBuffer::CopyToInput(void* data)
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	// 입력 버퍼를 맵핑하여 CPU에서 데이터를 쓸 수 있도록 준비
	DC->Map(_input.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		// 맵핑된 메모리에 데이터 복사
		memcpy(subResource.pData, data, _inputByte);
	}
	// 맵핑 해제
	DC->Unmap(_input.Get(), 0);
}

// 출력 버퍼에서 데이터를 복사
void RawBuffer::CopyFromOutput(void* data)
{
	// 출력 버퍼의 데이터를 결과 버퍼로 복사
	DC->CopyResource(_result.Get(), _output.Get());

	D3D11_MAPPED_SUBRESOURCE subResource;
	// 결과 버퍼를 맵핑하여 CPU에서 데이터를 읽을 수 있도록 준비
	DC->Map(_result.Get(), 0, D3D11_MAP_READ, 0, &subResource);
	{
		// 맵핑된 메모리로부터 데이터 복사
		memcpy(data, subResource.pData, _outputByte);
	}
	// 맵핑 해제
	DC->Unmap(_result.Get(), 0);
}

// 입력 버퍼 생성 로직
void RawBuffer::CreateInput()
{
	if (_inputByte == 0)
		return; // 입력 바이트가 0이면 함수 종료

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = _inputByte; // 버퍼 크기 설정
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // SRV로 사용될 것임을 명시
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS; // RAW 버퍼 허용
	desc.Usage = D3D11_USAGE_DYNAMIC; // CPU에서 쓰기 가능하며 GPU에서 읽기 가능
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU에서 쓰기 가능

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = _inputData; // 초기 데이터 설정

	// 버퍼 생성
	if (_inputData != nullptr)
		CHECK(DEVICE->CreateBuffer(&desc, &subResource, _input.GetAddressOf()));
	else
		CHECK(DEVICE->CreateBuffer(&desc, nullptr, _input.GetAddressOf()));
}

// 쉐이더 리소스 뷰(SRV) 생성 로직
void RawBuffer::CreateSRV()
{
	if (_inputByte == 0)
		return; // 입력 바이트가 0이면 함수 종료

	D3D11_BUFFER_DESC desc;
	_input->GetDesc(&desc); // 입력 버퍼의 설명 가져오기

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R32_TYPELESS; // 포맷은 쉐이더에서 해석될 것임을 명시
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX; // 확장된 버퍼 뷰
	srvDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW; // RAW 플래그 설정
	srvDesc.BufferEx.NumElements = desc.ByteWidth / 4; // 원소 개수 설정

	// SRV 생성
	CHECK(DEVICE->CreateShaderResourceView(_input.Get(), &srvDesc, _srv.GetAddressOf()));
}

// 출력 버퍼 생성 로직
void RawBuffer::CreateOutput()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = _outputByte; // 버퍼 크기 설정
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS; // UAV로 사용될 것임을 명시
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS; // RAW 버퍼 허용

	// 버퍼 생성
	CHECK(DEVICE->CreateBuffer(&desc, NULL, _output.GetAddressOf()));
}

// 비순차적 액세스 뷰(UAV) 생성 로직
void RawBuffer::CreateUAV()
{
	D3D11_BUFFER_DESC desc;
	_output->GetDesc(&desc); // 출력 버퍼의 설명 가져오기

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS; // 포맷은 쉐이더에서 해석될 것임을 명시
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER; // 버퍼 뷰
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW; // RAW 플래그 설정
	uavDesc.Buffer.NumElements = desc.ByteWidth / 4; // 원소 개수 설정

	// UAV 생성
	CHECK(DEVICE->CreateUnorderedAccessView(_output.Get(), &uavDesc, _uav.GetAddressOf()));
}

// 결과 버퍼 생성 로직
void RawBuffer::CreateResult()
{
	D3D11_BUFFER_DESC desc;
	_output->GetDesc(&desc); // 출력 버퍼의 설명을 다시 가져오기

	// 결과 버퍼를 CPU에서 읽을 수 있도록 설정 변경
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.BindFlags = 0; // 더 이상 UAV로 바인드되지 않음
	desc.MiscFlags = 0;

	// 결과 버퍼 생성
	CHECK(DEVICE->CreateBuffer(&desc, nullptr, _result.GetAddressOf()));
}