struct VS_INPUT {
	float4 position : POSITION;
	float4 color : COLOR; // 정점의 색상, COLOR 세맨틱을 사용
};

struct VS_OUTPUT {
	float4 position : SV_POSITION;
	float4 color : COLOR; // 정점의 색상, COLOR 세맨틱을 사용
};

cbuffer TransformData : register(b0) {
	row_major matrix matWorld;
	row_major matrix matView;
	row_major matrix matProjection;
}

VS_OUTPUT VS(VS_INPUT input) {
	VS_OUTPUT output;

	//WVP
	float4 position = mul(input.position, matWorld);
	position = mul(position, matView);
	position = mul(position, matProjection);

	output.position = position;
	output.color = input.color;
	return output;
}

Texture2D tex : register(t0);
SamplerState sam : register(s0);

float4 PS(VS_INPUT input) : SV_Target{
	return input.color;
}