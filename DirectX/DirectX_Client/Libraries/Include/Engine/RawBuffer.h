#pragma once



class RawBuffer
{
public:
	// 생성자: 초기 입력 데이터, 입력 및 출력 데이터의 바이트 크기를 인자로 받음
	RawBuffer(void* inputData, uint32 inputByte, uint32 outputByte);
	// 소멸자: 생성된 D3D11 자원 해제
	~RawBuffer();

public:
	// 버퍼 생성 함수: 입력, 출력 및 결과 버퍼 생성
	void CreateBuffer();
	// 입력 데이터를 GPU로 복사
	void CopyToInput(void* data);
	// GPU 처리 결과를 CPU로 복사
	void CopyFromOutput(void* data);

public:
	// 쉐이더 리소스 뷰(SRV) 접근자
	ComPtr<ID3D11ShaderResourceView> GetSRV() { return _srv; }
	// 비순차적 액세스 뷰(UAV) 접근자
	ComPtr<ID3D11UnorderedAccessView> GetUAV() { return _uav; }

private:
	// 입력 버퍼 생성
	void CreateInput();
	// 쉐이더 리소스 뷰 생성
	void CreateSRV();
	// 출력 버퍼 생성
	void CreateOutput();
	// 비순차적 액세스 뷰 생성
	void CreateUAV();
	// 결과 버퍼 생성
	void CreateResult();

private:
	ComPtr<ID3D11Buffer> _input; // 입력 버퍼
	ComPtr<ID3D11ShaderResourceView> _srv; // 입력 데이터의 SRV
	ComPtr<ID3D11Buffer> _output; // 출력 버퍼
	ComPtr<ID3D11UnorderedAccessView> _uav; // 출력 데이터의 UAV
	ComPtr<ID3D11Buffer> _result; // 결과 데이터 버퍼

private:
	void* _inputData; // 초기 입력 데이터
	uint32 _inputByte = 0; // 입력 데이터 크기
	uint32 _outputByte = 0; // 출력 데이터 크기
};

