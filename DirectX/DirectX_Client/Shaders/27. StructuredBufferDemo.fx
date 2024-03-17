// 입력 데이터 구조체 정의: 4x4 행렬
struct InputDesc
{
	matrix input;
};

// 출력 데이터 구조체 정의: 연산 결과를 담을 4x4 행렬
struct OutputDesc
{
	matrix result;
};

// 입력 버퍼: 구조화된 버퍼로 정의된 InputDesc 구조체의 인스턴스들을 저장
StructuredBuffer<InputDesc> Input;

// 출력 버퍼: 쓰기 가능한 구조화된 버퍼로 OutputDesc 구조체의 인스턴스들을 저장
RWStructuredBuffer<OutputDesc> Output;

// 컴퓨트 셰이더의 엔트리 포인트
// numthreads 어트리뷰트는 한 번에 실행될 쓰레드 그룹의 크기를 지정 (이 경우, x축으로 500개 쓰레드)
[numthreads(500, 1, 1)]
void CS(uint id : SV_GroupIndex) // 각 쓰레드의 고유 ID
{
	// 입력 버퍼에서 id에 해당하는 행렬을 가져와 2배로 확장
	matrix result = Input[id].input * 2;

	// 계산된 결과를 출력 버퍼의 해당 위치에 저장
	Output[id].result = result;
}

// 셰이더 기술 세트 정의
technique11 T0
{
	pass P0
	{
		// 이 테크닉에서는 버텍스 셰이더와 픽셀 셰이더는 사용하지 않으므로 NULL로 설정
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		// 컴파일된 컴퓨트 셰이더를 사용
		SetComputeShader(CompileShader(cs_5_0, CS()));
	}
};
