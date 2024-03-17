RWByteAddressBuffer Output; // 데이터 쓰기가 가능한 UAV 선언

struct ComputeInput
{
	uint3 groupID : SV_GroupID; // 디스패치 호출 시 사용된 그룹 ID
	uint3 groupThreadID : SV_GroupThreadID; // 현재 스레드 그룹 내 스레드 ID
	uint3 dispatchThreadID : SV_DispatchThreadID; // 전체 디스패치에서의 스레드 ID
	uint groupIndex : SV_GroupIndex; // 스레드 그룹 내의 스레드 인덱스
};

[numthreads(10, 8, 3)]
void CS(ComputeInput input)
{
	uint index = input.groupIndex;
	uint outAddress = index * 10 * 4; // 버퍼 내 쓰기 시작 위치 계산

	// 입력 정보를 버퍼에 저장
	Output.Store3(outAddress + 0, input.groupID);
	Output.Store3(outAddress + 12, input.groupThreadID);
	Output.Store3(outAddress + 24, input.dispatchThreadID);
	Output.Store(outAddress + 36, input.groupIndex);
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS()));
	}
};
