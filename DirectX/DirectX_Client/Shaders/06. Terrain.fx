// 3D 오브젝트의 위치, 방향, 크기 등을 결정하는 변환 행렬들을 정의합니다.
matrix World;
matrix View;
matrix Projection;

// 쉐이더에서 사용할 텍스처를 정의합니다.
Texture2D Texture0;

// 버텍스 쉐이더로 넘어오는 입력 데이터 구조체입니다. 각 버텍스의 위치와 텍스처 좌표를 포함합니다.
struct VertexInput
{
	float4 position : POSITION; // 3D 공간에서의 버텍스 위치
	float2 uv : TEXCOORD;       // 텍스처 좌표
};

// 버텍스 쉐이더의 출력 데이터 구조체입니다. 처리된 버텍스 위치와 텍스처 좌표를 포함합니다.
struct VertexOutput
{
	float4 position : SV_POSITION; // 스크린 공간에서의 버텍스 위치
	float2 uv : TEXCOORD;          // 텍스처 좌표
};

// 버텍스 쉐이더 함수입니다. 3D 오브젝트의 각 버텍스 위치를 처리합니다.
VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	// 입력된 버텍스 위치를 월드, 뷰, 프로젝션 행렬을 사용하여 변환합니다.
	output.position = mul(input.position, World);
	output.position = mul(output.position, View);
	output.position = mul(output.position, Projection);

	// 입력된 텍스처 좌표를 출력 데이터에 그대로 전달합니다.
	output.uv = input.uv;

	return output;
}

// 텍스처 샘플링을 위한 샘플러 상태를 정의합니다. 여기서는 UV 좌표가 범위를 벗어날 경우 텍스처를 반복합니다.
SamplerState Sampler0
{
	AddressU = Wrap; // U 좌표가 1을 초과할 경우 텍스처를 반복
	AddressV = Wrap; // V 좌표가 1을 초과할 경우 텍스처를 반복
};

// 픽셀 쉐이더 함수입니다. 최종적으로 화면에 렌더링될 픽셀의 색상을 결정합니다.
float4 PS(VertexOutput input) : SV_TARGET
{
	// 정의된 샘플러를 사용하여 텍스처에서 색상 값을 샘플링하고 반환합니다.
	return Texture0.Sample(Sampler0, input.uv);
}

// 래스터라이저 상태를 정의합니다. 이 예제에서는 와이어프레임 모드를 설정합니다.
RasterizerState FillModeWireFrame
{
	FillMode = Wireframe; // 렌더링 모드를 와이어프레임으로 설정
};

// 렌더링 기법을 정의합니다. 이 기법은 두 개의 패스를 사용합니다.
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
		// 두 번째 패스에서는 래스터라이저 상태를 와이어프레임 모드로 설정하고, 
		// 동일한 버텍스 쉐이더와 픽셀 쉐이더를 재사용합니다.
		SetRasterizerState(FillModeWireFrame);

		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};
