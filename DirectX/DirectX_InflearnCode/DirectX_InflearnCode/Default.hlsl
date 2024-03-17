struct VS_INPUT {
	float4 position : POSITION; // 정점의 위치, POSITION 세맨틱을 사용
	//float4 color : COLOR; // 정점의 색상, COLOR 세맨틱을 사용
	float2 uv : TEXCOORD;
};

struct VS_OUTPUT {
	float4 position : SV_POSITION; // 정점 쉐이더 출력 위치, SV_POSITION 시스템 세맨틱 사용
	//float4 color : COLOR; // 출력 색상, COLOR 세맨틱을 사용
	float2 uv : TEXCOORD;
};

cbuffer TransformData : register(b0) {
	row_major matrix matWorld;
	row_major matrix matView;
	row_major matrix matProjection;
}

//VS => Vertex Shader
//IA - VS - RS - PS - OM
VS_OUTPUT VS(VS_INPUT input) {
	VS_OUTPUT output; // 출력 구조체 초기화

	// WVP
	float4 position = mul(input.position, matWorld); // W
	position = mul(position, matView); // V
	position = mul(position, matProjection); // P

	output.position = position;
	//output.position = input.position + offset;  // 로고의 위치 조정
	//output.color = input.color; // 입력 색상을 출력 색상으로 전달
	output.uv= input.uv; // 입력 색상을 출력 색상으로 전달

	return output; // 변환된 출력 반환
}

// 픽셀 쉐이더에 사용될 텍스처와 샘플러를 선언합니다.
Texture2D tex : register(t0); // 텍스처
Texture2D tex2 : register(t0); // 텍스처
SamplerState sam : register(s0); // 샘플러

// 픽셀 쉐이더: 정점 쉐이더에서 전달받은 UV 좌표를 사용하여 텍스처에서 색상을 샘플링합니다.
float4 PS(VS_INPUT input) : SV_Target
{
	float4 color = tex.Sample(sam, input.uv); // UV 좌표를 사용하여 텍스처 샘플링
	return color; // 샘플링된 색상을 픽셀의 색상으로 사용
	//return tex.Sample(sam, input.uv);
}