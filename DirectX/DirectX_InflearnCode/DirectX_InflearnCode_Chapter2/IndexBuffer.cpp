#include "pch.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(ComPtr<ID3D11Device> device) :
	_device(device)
{
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::Create(const vector<uint32>& indices)
{
	_stride = sizeof(uint32);
	_count = static_cast<uint32>(indices.size());

	D3D11_BUFFER_DESC desc; // 버퍼 설명 구조체
	ZeroMemory(&desc, sizeof(desc)); // 메모리 초기화
	desc.Usage = D3D11_USAGE_IMMUTABLE; // 버퍼는 GPU에서만 사용되며 CPU에서는 수정할 수 없음
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER; // 이 버퍼를 인덱스 버퍼로 사용하겠다는 의미
	desc.ByteWidth = (uint32)(_stride * _count); // 버퍼의 전체 크기

	D3D11_SUBRESOURCE_DATA data; // 초기 데이터를 지정하기 위한 구조체
	ZeroMemory(&data, sizeof(data)); // 메모리 초기화
	data.pSysMem = indices.data(); // 인덱스 데이터의 포인터
	// 인덱스 버퍼 생성
	HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());

	CHECK(hr);
}
