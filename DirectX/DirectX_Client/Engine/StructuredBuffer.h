#pragma once

// 클래스 정의
class StructuredBuffer
{
public:
	// 생성자: 입력 데이터와 스트라이드(단위 데이터의 크기), 개수를 받습니다.
	// 출력 버퍼의 스트라이드와 개수는 선택적입니다.
	StructuredBuffer(void* inputData, uint32 inputStride, uint32 inputCount, uint32 outputStride = 0, uint32 outputCount = 0);
	// 소멸자
	~StructuredBuffer();

public:
	// 버퍼 생성 관련 주요 함수들
	void CreateBuffer();

private:
	// 입력 버퍼 생성
	void CreateInput();
	// 입력 버퍼에 대한 SRV(Shader Resource View) 생성
	void CreateSRV();
	// 출력 버퍼 생성
	void CreateOutput();
	// 출력 버퍼에 대한 UAV(Unordered Access View) 생성
	void CreateUAV();
	// 결과 데이터를 CPU에서 읽을 수 있도록 하는 결과 버퍼 생성
	void CreateResult();

public:
	// 입력 버퍼의 바이트 크기 계산
	uint32 GetInputByteWidth() { return _inputStride * _inputCount; }
	// 출력 버퍼의 바이트 크기 계산
	uint32 GetOutputByteWidth() { return _outputStride * _outputCount; }

	// 입력 데이터를 버퍼에 복사하는 함수
	void CopyToInput(void* data);
	// 출력 데이터를 CPU로 복사하는 함수
	void CopyFromOutput(void* data);

public:
	// SRV와 UAV에 접근할 수 있는 getter 함수들
	ComPtr<ID3D11ShaderResourceView> GetSRV() { return _srv; }
	ComPtr<ID3D11UnorderedAccessView> GetUAV() { return _uav; }

private:
	// Direct3D 버퍼와 뷰를 관리하기 위한 멤버 변수들
	ComPtr<ID3D11Buffer> _input; // 입력 버퍼
	ComPtr<ID3D11ShaderResourceView> _srv; // 입력 SRV
	ComPtr<ID3D11Buffer> _output; // 출력 버퍼
	ComPtr<ID3D11UnorderedAccessView> _uav; // 출력 UAV
	ComPtr<ID3D11Buffer> _result; // 결과 버퍼

private:
	// 생성자에서 받은 입력 데이터 및 버퍼 설정 관련 멤버 변수들
	void* _inputData; // 입력 데이터 포인터
	uint32 _inputStride = 0; // 단위 입력 데이터의 크기
	uint32 _inputCount = 0; // 입력 데이터 개수
	uint32 _outputStride = 0; // 단위 출력 데이터의 크기
	uint32 _outputCount = 0; // 출력 데이터 개수
};
