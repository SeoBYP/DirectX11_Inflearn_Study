#pragma once

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	ComPtr<ID3D11Buffer> GetComPtr() { return _vertexBuffer; }
	uint32 GetStride() { return _stride; } // 정점 하나의 크기
	uint32 GetOffset() { return _offset; } // 버퍼 내에서의 오프셋
	uint32 GetCount() { return _count; } // 버퍼에 저장된 정점의 수
	uint32 GetSlot() { return _slot; } // 이 버퍼가 바인드될 슬롯 번호

	// 정점 데이터를 이용해 버퍼를 생성하는 함수
	template<typename T>
	void Create(const vector<T>& vertices, uint32 slot = 0, bool cpuWrite = false, bool gpuWrite = false)
	{
		_stride = sizeof(T);
		_count = static_cast<uint32>(vertices.size());

		_slot = slot;
		_cpuWrite = cpuWrite;
		_gpuWrite = gpuWrite;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 이 버퍼가 정점 버퍼로 사용됨을 명시
		desc.ByteWidth = (uint32)(_stride * _count); // 버퍼의 전체 크기

		// 버퍼의 용도 설정
		if (cpuWrite == false && gpuWrite == false)
		{
			desc.Usage = D3D11_USAGE_IMMUTABLE; // 변경 불가, GPU만 읽기
		}
		else if (cpuWrite == true && gpuWrite == false)
		{
			desc.Usage = D3D11_USAGE_DYNAMIC; // CPU에서 쓰기 가능, GPU에서 읽기
			desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		}
		else if (cpuWrite == false && gpuWrite == true)
		{
			desc.Usage = D3D11_USAGE_DEFAULT; // 기본 설정, GPU에서 읽고 쓰기
		}
		else
		{
			desc.Usage = D3D11_USAGE_STAGING; // CPU와 GPU 모두에서 읽고 쓰기 가능
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		}

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices.data(); // 정점 데이터의 포인터

		// 버퍼 생성
		HRESULT hr = DEVICE->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		CHECK(hr); // 성공 여부 확인
	}

	// 이 함수를 호출하면 버퍼가 입력 어셈블러 스테이지에 바인드됩니다.
	void PushData()
	{
		DC->IASetVertexBuffers(_slot, 1, _vertexBuffer.GetAddressOf(), &_stride, &_offset);
	}

private:
	ComPtr<ID3D11Buffer> _vertexBuffer; // DirectX 정점 버퍼

	uint32 _stride = 0; // 정점 하나의 바이트 크기
	uint32 _offset = 0; // 버퍼 내에서의 오프셋, 일반적으로 0
	uint32 _count = 0; // 버퍼에 저장된 정점의 수

	uint32 _slot = 0; // 이 버퍼가 바인드될 슬롯 번호
	bool _cpuWrite = false; // CPU에서 버퍼에 쓰기 가능 여부
	bool _gpuWrite = false; // GPU에서 버퍼에 쓰기 가능 여부
};
