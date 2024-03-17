#include "00. Global.fx" // 전역 변수와 함수를 포함하는 셰이더 파일
#include "00. Light.fx" // 조명 처리를 위한 셰이더 파일

// 정점 셰이더 출력 구조체 정의
struct VS_OUT
{
	float4 position : SV_POSITION; // 클립 공간의 정점 위치
	float2 uv : TEXCOORD; // 텍스처 좌표
};

// 정점 셰이더 함수 정의
VS_OUT VS(VertexTextureNormalTangent input)
{
	VS_OUT output;

	// 모델 좌표계에서 정점 위치를 뷰 좌표계로 변환
	float4 viewPos = mul(float4(input.position.xyz, 1), V);
	// 뷰 좌표계에서 클립 공간으로 변환
	float4 clipSpacePos = mul(viewPos, P);

	output.position = clipSpacePos;
	// 깊이 값 조정으로 깊이 클리핑 문제를 완화
	output.position.z = output.position.w * 0.999999f;

	output.uv = input.uv; // 텍스처 좌표는 그대로 출력 구조체로 복사

	return output;
}

// 픽셀 셰이더 함수 정의
float4 PS(VS_OUT input) : SV_TARGET
{
	// 텍스처 샘플링을 통해 픽셀 색상 결정
	float4 color = DiffuseMap.Sample(LinearSampler, input.uv);
	return color; // 최종 픽셀 색상 반환
}

// 렌더링 기술 정의
technique11 T0
{
	pass P0 // 첫 번째 패스
	{
		SetRasterizerState(FrontCounterClockwiseTrue); // 래스터라이저 상태 설정
		SetVertexShader(CompileShader(vs_5_0, VS())); // 정점 셰이더 설정
		SetPixelShader(CompileShader(ps_5_0, PS())); // 픽셀 셰이더 설정
	}
};
