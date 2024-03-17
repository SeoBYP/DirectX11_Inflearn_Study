// 입력 데이터를 저장하는 2D 텍스처 배열
Texture2DArray<float4> Input;
// 출력 데이터를 저장할 2D 텍스처 배열로, 읽기와 쓰기가 가능합니다.
RWTexture2DArray<float4> Output;

// 컴퓨트 셰이더의 각 스레드가 실행할 함수를 정의합니다.
// numthreads 어트리뷰트는 각 디스패치에서 스레드 그룹의 크기를 지정합니다.
[numthreads(32, 32, 1)]
void CS(uint3 id : SV_DispatchThreadID) // 스레드 식별자
{
	// 입력 텍스처에서 현재 스레드에 해당하는 픽셀의 색상을 로드합니다.
	float4 color = Input.Load(int4(id, 0));

	// 아래의 세 가지 방법 중 하나를 선택하여 출력 텍스처에 색상 값을 쓸 수 있습니다.

	// 1. 입력 텍스처의 색상을 그대로 출력 텍스처에 저장합니다.
	//Output[id] = color;

	// 2. 입력 텍스처의 색상을 반전시켜 출력 텍스처에 저장합니다.
	Output[id] = 1.0f - color;

	// 3. 입력 텍스처의 색상의 평균 값을 계산하여, 출력 텍스처에 그레이스케일 색상으로 저장합니다.
	//Output[id] = float4((color.r + color.g + color.b) / 3.0f, (color.r + color.g + color.b) / 3.0f, (color.r + color.g + color.b) / 3.0f, color.a);
}

// 컴퓨트 셰이더를 사용하는 테크닉을 정의합니다.
technique11 T0
{
	pass P0
	{
		// 컴퓨트 셰이더를 제외하고는 버텍스 셰이더나 픽셀 셰이더를 사용하지 않으므로 NULL로 설정합니다.
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		// 컴퓨트 셰이더를 컴파일하여 설정합니다.
		SetComputeShader(CompileShader(cs_5_0, CS()));
	}
};
