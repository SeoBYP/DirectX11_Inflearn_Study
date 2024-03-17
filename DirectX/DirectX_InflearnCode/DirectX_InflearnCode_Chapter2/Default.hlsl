// 정점 입력 구조체: 메시의 각 정점에 대한 정보를 정의합니다.
struct VS_INPUT
{
	float4 position : POSITION; // 정점의 위치
	float2 uv : TEXCOORD; // 텍스처 좌표
};

// 정점 쉐이더 출력 구조체: 정점 쉐이더 처리 후의 데이터를 픽셀 쉐이더로 전달하기 위한 구조체입니다.
struct VS_OUTPUT
{
	float4 position : SV_POSITION; // 스크린 좌표계에서의 정점 위치
	float2 uv : TEXCOORD; // 텍스처 좌표
};

// 카메라 데이터 상수 버퍼: 뷰 매트릭스와 투영 매트릭스를 포함합니다.
cbuffer CameraData : register(b0)
{
	row_major matrix matView; // 뷰 매트릭스
	row_major matrix matProjection; // 투영 매트릭스
};

// 변환 데이터 상수 버퍼: 월드 매트릭스를 포함합니다.
cbuffer TransformData : register(b1)
{
	row_major matrix matWorld; // 월드 매트릭스
};

// 애니메이션 데이터 상수 버퍼: 애니메이션을 위한 데이터를 포함합니다.
cbuffer AnimationData : register(b2)
{
	float2 spriteOffset; // 스프라이트 오프셋
	float2 spriteSize; // 스프라이트 크기
	float2 textureSize; // 텍스처 크기
	float useAnimation; // 애니메이션 사용 여부 (1.0f는 사용, 0.0f는 미사용)
};

// 정점 쉐이더: 입력된 정점 데이터를 기반으로 스크린 좌표계의 위치와 텍스처 좌표를 계산합니다.
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	// 월드, 뷰, 투영 매트릭스를 곱하여 스크린 좌표계의 위치를 계산합니다.
	float4 position = mul(input.position, matWorld); // 월드 변환
	position = mul(position, matView); // 뷰 변환
	position = mul(position, matProjection); // 투영 변환

	output.position = position;
	output.uv = input.uv;

	// 애니메이션 사용 시, 텍스처 좌표를 조정합니다.
	if (useAnimation == 1.0f)
	{
		output.uv *= spriteSize / textureSize; // 스프라이트 크기에 따라 텍스처 좌표 조정
		output.uv += spriteOffset / textureSize; // 스프라이트 오프셋에 따라 텍스처 좌표 조정
	}

	return output;
}

// 텍스처와 샘플러 상태
Texture2D texture0 : register(t0); // 텍스처
SamplerState sampler0 : register(s0); // 샘플러

// 픽셀 쉐이더: 정점 쉐이더로부터 받은 텍스처 좌표를 사용하여 최종 색상을 계산합니다.
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color = texture0.Sample(sampler0, input.uv); // 텍스처 샘플링

	return color; // 계산된 색상 반환
}
