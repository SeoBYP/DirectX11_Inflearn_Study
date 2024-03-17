#include "pch.h"
#include "StructuredBuffer.h"
// 생성자: 입력 데이터, 데이터의 구조 크기, 갯수 등을 초기화하고 버퍼 생성 함수를 호출합니다.
StructuredBuffer::StructuredBuffer(void* inputData, uint32 inputStride, uint32 inputCount, uint32 outputStride, uint32 outputCount)
	: _inputData(inputData), _inputStride(inputStride), _inputCount(inputCount), _outputStride(outputStride), _outputCount(outputCount)
{
	// 출력 스트라이드나 카운트가 지정되지 않았다면 입력 스트라이드와 카운트를 사용합니다.
	if (outputStride == 0 || outputCount == 0)
	{
		_outputStride = inputStride;
		_outputCount = inputCount;
	}

	// 버퍼 생성 함수 호출
	CreateBuffer();
}

// 소멸자
StructuredBuffer::~StructuredBuffer()
{
	// Direct3D 자원 해제는 ComPtr에 의해 자동으로 관리되므로 별도로 할 작업이 없습니다.
}

// 버퍼 생성 전체 과정을 담당하는 함수
void StructuredBuffer::CreateBuffer()
{
	CreateInput(); // 입력 데이터에 대한 버퍼 생성
	CreateSRV();   // 입력 버퍼에 대한 SRV 생성
	CreateOutput(); // 출력 데이터를 위한 버퍼 생성
	CreateUAV();    // 출력 버퍼에 대한 UAV 생성
	CreateResult(); // 결과 데이터를 위한 버퍼 생성 (주로 CPU에서 읽기 위함)
}

// 입력 버퍼 생성
void StructuredBuffer::CreateInput()
{
	// 버퍼 설명 구조체 초기화
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	// 입력 버퍼의 바이트 크기, 바인딩 옵션, 구조체 바이트 크기 설정
	desc.ByteWidth = GetInputByteWidth(); // 입력 데이터 전체 크기 계산
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = _inputStride; // 개별 요소의 크기
	desc.Usage = D3D11_USAGE_DYNAMIC; // CPU가 쓰기 가능
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU 쓰기 액세스 허용

	// 입력 데이터가 있는 경우 해당 데이터로 초기화
	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = _inputData;

	// 입력 버퍼 생성
	if (_inputData != nullptr)
		CHECK(DEVICE->CreateBuffer(&desc, &subResource, _input.GetAddressOf()));
	else
		CHECK(DEVICE->CreateBuffer(&desc, nullptr, _input.GetAddressOf()));
}

// 입력 버퍼에 대한 SRV 생성
void StructuredBuffer::CreateSRV()
{
	// 입력 버퍼의 설명을 가져와서 SRV를 생성합니다.
	D3D11_BUFFER_DESC desc;
	_input->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_UNKNOWN; // 구조화된 버퍼이므로 포맷은 알 수 없음
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX; // 확장 버퍼 뷰 사용
	srvDesc.BufferEx.NumElements = _inputCount; // 요소의 갯수

	// SRV 생성
	CHECK(DEVICE->CreateShaderResourceView(_input.Get(), &srvDesc, _srv.GetAddressOf()));
}

// 출력 버퍼 생성
void StructuredBuffer::CreateOutput()
{
	// 출력 버퍼의 설명 구조체를 설정합니다.
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.ByteWidth = GetOutputByteWidth(); // 출력 데이터의 전체 크기
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS; // UAV에 바인딩될 예정
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = _outputStride; // 출력 구조체의 바이트 크기

	// 출력 버퍼 생성
	CHECK(DEVICE->CreateBuffer(&desc, nullptr, _output.GetAddressOf()));
}

// 출력 버퍼에 대한 UAV 생성
void StructuredBuffer::CreateUAV()
{
	// 출력 버퍼의 설명을 가져옵니다.
	D3D11_BUFFER_DESC desc;
	_output->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.Format = DXGI_FORMAT_UNKNOWN; // 포맷은 알 수 없음
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER; // 버퍼 뷰
	uavDesc.Buffer.NumElements = _outputCount; // 출력 요소의 갯수

	// UAV 생성
	CHECK(DEVICE->CreateUnorderedAccessView(_output.Get(), &uavDesc, _uav.GetAddressOf()));
}

// 결과 버퍼 생성
void StructuredBuffer::CreateResult()
{
	// 출력 버퍼를 기반으로 결과 버퍼의 설명 구조체를 설정합니다.
	D3D11_BUFFER_DESC desc;
	_output->GetDesc(&desc);

	desc.Usage = D3D11_USAGE_STAGING; // CPU에서 읽을 수 있도록 설정
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // CPU 읽기 액세스
	desc.BindFlags = 0; // 바인딩 없음
	desc.MiscFlags = 0; // 추가 플래그 없음

	// 결과 버퍼 생성
	CHECK(DEVICE->CreateBuffer(&desc, NULL, _result.GetAddressOf()));
}

// 입력 버퍼로 데이터를 복사하는 함수
void StructuredBuffer::CopyToInput(void* data)
{
	D3D11_MAPPED_SUBRESOURCE subResource;
	DC->Map(_input.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource); // 입력 버퍼 매핑
	{
		memcpy(subResource.pData, data, GetInputByteWidth()); // 데이터 복사
	}
	DC->Unmap(_input.Get(), 0); // 매핑 해제
}

// 출력 버퍼에서 데이터를 복사하여 가져오는 함수
void StructuredBuffer::CopyFromOutput(void* data)
{
	DC->CopyResource(_result.Get(), _output.Get()); // 출력 버퍼에서 결과 버퍼로 복사

	D3D11_MAPPED_SUBRESOURCE subResource;
	DC->Map(_result.Get(), 0, D3D11_MAP_READ, 0, &subResource); // 결과 버퍼 매핑
	{
		memcpy(data, subResource.pData, GetOutputByteWidth()); // 데이터 복사
	}
	DC->Unmap(_result.Get(), 0); // 매핑 해제
}