// 변환 행렬을 정의합니다. 이 행렬들은 3D 오브젝트를 월드 공간에서 뷰 공간으로, 그리고 프로젝션 공간으로 변환하는 데 사용됩니다.
matrix World;
matrix View;
matrix Projection;

// 텍스처 자원을 정의합니다.
Texture2D Texture0;

// 버텍스 쉐이더의 입력 구조체입니다. 이 구조체는 메쉬의 각 버텍스에 대한 정보를 담고 있습니다.
struct VertexInput
{
	float4 position : POSITION; // 버텍스의 위치
	float2 uv : TEXCOORD;       // 텍스처 좌표
};

// 버텍스 쉐이더의 출력 구조체입니다. 이 구조체는 버텍스 쉐이더에서 처리된 후의 버텍스 정보를 담고 있습니다.
struct VertexOutput
{
	float4 position : SV_POSITION; // 스크린 공간에서의 버텍스 위치
	float2 uv : TEXCOORD;          // 텍스처 좌표
};

// 버텍스 쉐이더 함수입니다.
VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	// 입력된 버텍스 위치를 월드, 뷰, 프로젝션 행렬을 사용하여 변환합니다.
	output.position = mul(input.position, World);
	output.position = mul(output.position, View);
	output.position = mul(output.position, Projection);

	// 텍스처 좌표를 출력 구조체로 복사합니다.
	output.uv = input.uv;

	return output;
}

// 샘플러 상태를 정의합니다. 이것은 텍스처 샘플링 방법을 정의합니다.
SamplerState Sampler0;

// 픽셀 쉐이더 함수입니다.
float4 PS(VertexOutput input) : SV_TARGET
{
	// 입력된 텍스처 좌표를 사용하여 텍스처에서 색상 값을 샘플링하고 반환합니다.
	return Texture0.Sample(Sampler0, input.uv);
}

// 래스터라이저 상태를 정의합니다. 이 상태는 와이어프레임 렌더링 모드를 설정합니다.
RasterizerState FillModeWireFrame
{
	FillMode = WireFrame;
};

// 렌더링 기법을 정의합니다. 이 기법은 두 개의 패스를 가집니다.
technique11 T0
{
	pass P0
	{
		// 첫 번째 패스에서는 버텍스 쉐이더와 픽셀 쉐이더를 설정합니다.
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

	pass P1
	{
		// 두 번째 패스에서는 와이어프레임 모드를 설정하고, 버텍스 쉐이더와 픽셀 쉐이더를 재설정합니다.
		SetRasterizerState(FillModeWireFrame);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};
