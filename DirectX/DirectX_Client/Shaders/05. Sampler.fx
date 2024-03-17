// 변환 행렬을 정의합니다. 이 행렬들은 3D 오브젝트를 월드 공간, 뷰 공간, 프로젝션 공간으로 변환하는 데 사용됩니다.
matrix World;
matrix View;
matrix Projection;

// 텍스처 자원을 정의합니다.
Texture2D Texture0;

// 텍스처 주소 지정 모드를 결정하기 위한 변수입니다.
uint Address;

// 버텍스 쉐이더의 입력 구조체입니다. 각 버텍스에 대한 정보를 담고 있습니다.
struct VertexInput
{
	float4 position : POSITION; // 버텍스의 위치
	float2 uv : TEXCOORD;       // 텍스처 좌표
};

// 버텍스 쉐이더의 출력 구조체입니다. 처리된 버텍스 정보를 담고 있습니다.
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

// 샘플러 상태를 정의합니다. 이 상태는 텍스처 샘플링 시의 주소 지정 모드를 결정합니다.
SamplerState Sampler0;

// Wrap 모드: 텍스처 좌표가 [0,1] 범위를 벗어났을 때, 좌표를 '반복'합니다.
SamplerState SamplerAddressWrap
{
	AddressU = Wrap;
	AddressV = Wrap;
};

// Mirror 모드: 텍스처 좌표가 [0,1] 범위를 벗어났을 때, 좌표를 '반사'하여 반복합니다.
SamplerState SamplerAddressMirror
{
	AddressU = Mirror;
	AddressV = Mirror;
};

// Clamp 모드: 텍스처 좌표가 [0,1] 범위를 벗어났을 때, 좌표를 경계값에 '고정'합니다.
SamplerState SamplerAddressClamp
{
	AddressU = Clamp;
	AddressV = Clamp;
};

// Border 모드: 텍스처 좌표가 [0,1] 범위를 벗어났을 때, 지정된 '경계 색상'을 사용합니다.
SamplerState SamplerAddressBorder
{
	AddressU = Border;
	AddressV = Border;
	BorderColor = float4(1, 0, 0, 1); // 경계 색상을 빨간색으로 설정
};

// 픽셀 쉐이더 함수입니다.
float4 PS(VertexOutput input) : SV_TARGET
{
	// Address 변수의 값에 따라 다른 샘플러를 사용하여 텍스처를 샘플링합니다.
	if (Address == 0)
		return Texture0.Sample(SamplerAddressWrap, input.uv);
	if (Address == 1)
		return Texture0.Sample(SamplerAddressMirror, input.uv);
	if (Address == 2)
		return Texture0.Sample(SamplerAddressClamp, input.uv);
	if (Address == 3)
		return Texture0.Sample(SamplerAddressBorder, input.uv);

	// 기본 샘플러를 사용한 샘플링
	return Texture0.Sample(Sampler0, input.uv);
}

// 렌더링 기법을 정의합니다. 여기서는 두 개의 패스를 사용합니다.
technique11 T0
{
	pass P0
	{
		// 첫 번째 패스: 버텍스 쉐이더와 픽셀 쉐이더를 설정합니다.
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
	pass P1
	{
		// 두 번째 패스: 동일한 쉐이더를 재사용합니다.
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};
