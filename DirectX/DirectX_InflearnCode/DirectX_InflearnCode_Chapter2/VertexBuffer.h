#pragma once
class VertexBuffer
{
public:
	VertexBuffer(ComPtr<ID3D11Device> device);
	~VertexBuffer();

	ComPtr<ID3D11Buffer> GetComptr() { return _vertexBuffer; }
	uint32 GetStride() { return _stride; }
	uint32 GetOffset() { return _offset; }
	uint32 GetCount() { return _count; }


	template<typename T> 
	void Create(const vector<T>& vertices ) {
		_stride = sizeof(T);
		_count = static_cast<uint32>(vertices.size());

		D3D11_BUFFER_DESC desc; // 버퍼 설명 구조체
		ZeroMemory(&desc, sizeof(desc)); // 메모리 초기화
		desc.Usage = D3D11_USAGE_IMMUTABLE; // 버퍼는 GPU에서만 사용되며 CPU에서는 수정할 수 없음
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 이 버퍼를 정점 버퍼로 사용하겠다는 의미
		desc.ByteWidth = (uint32)sizeof(T) * vertices.size(); // 버퍼의 전체 크기

		D3D11_SUBRESOURCE_DATA data; // 초기 데이터를 지정하기 위한 구조체
		ZeroMemory(&data, sizeof(data)); // 메모리 초기화
		data.pSysMem = vertices.data(); // 정점 데이터의 포인터

		_device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf()); // 버퍼 생성
	}

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;

	uint32 _stride = 0;
	uint32 _offset = 0;
	uint32 _count = 0;
};

