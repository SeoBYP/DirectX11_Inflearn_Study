#include "00. Global.fx" // 전역 설정과 공유 변수 포함
#include "00. Light.fx" // 조명 처리를 위한 설정과 함수 포함

#define MAX_MODEL_TRANSFORMS 250 // 최대 본(뼈) 변환 수 정의
#define MAX_MODEL_KEYFRAMES 500 // 최대 키프레임 수 정의

// 키프레임 정보를 담는 구조체 정의
struct KeyframeDesc
{
	int animIndex; // 애니메이션 인덱스
	uint currFrame; // 현재 프레임 번호
	uint nextFrame; // 다음 프레임 번호
	float ratio; // 현재와 다음 프레임 사이의 보간 비율
	float sumTime; // 총 경과 시간
	float speed; // 애니메이션 재생 속도
	float2 padding; // 패딩
};

// 키프레임 정보를 저장하는 상수 버퍼
cbuffer KeyframeBuffer {
	KeyframeDesc Keyframes;
};

// 본 변환 정보를 저장하는 상수 버퍼
cbuffer BoneBuffer
{
	matrix BoneTransforms[MAX_MODEL_TRANSFORMS];
};

uint BoneIndex; // 현재 처리 중인 본의 인덱스
Texture2DArray TransformMap; // 본 변환을 저장한 텍스처 배열

// 정점 데이터에 애니메이션 행렬을 적용하여 변환을 계산하는 함수
matrix GetAnimationMatrix(VertexTextureNormalTangentBlend input)
{
	// 입력된 정점의 블렌드 인덱스와 가중치
	float indices[4] = { input.blendIndices.x, input.blendIndices.y, input.blendIndices.z, input.blendIndices.w };
	float weights[4] = { input.blendWeights.x, input.blendWeights.y, input.blendWeights.z, input.blendWeights.w };

	// 현재 애니메이션 상태에 대한 정보
	int animIndex = Keyframes.animIndex;
	int currFrame = Keyframes.currFrame;
	int nextFrame = Keyframes.nextFrame;
	float ratio = Keyframes.ratio;

	float4 c0, c1, c2, c3; // 현재 프레임의 변환 컴포넌트
	float4 n0, n1, n2, n3; // 다음 프레임의 변환 컴포넌트

	matrix curr = 0; // 현재 프레임의 변환 행렬
	matrix next = 0; // 다음 프레임의 변환 행렬
	matrix transform = 0; // 최종 계산된 변환 행렬

	// 각 본에 대해 변환 행렬을 계산
	for (int i = 0; i < 4; i++)
	{
		// 현재 프레임의 변환 컴포넌트 로드
		c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currFrame, animIndex, 0));
		c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currFrame, animIndex, 0));
		c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currFrame, animIndex, 0));
		c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currFrame, animIndex, 0));
		curr = matrix(c0, c1, c2, c3); // 현재 프레임의 변환 행렬 생성

		// 다음 프레임의 변환 컴포넌트 로드
		n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame, animIndex, 0));
		n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame, animIndex, 0));
		n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame, animIndex, 0));
		n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame, animIndex, 0));
		next = matrix(n0, n1, n2, n3); // 다음 프레임의 변환 행렬 생성

		matrix result = lerp(curr, next, ratio); // 현재와 다음 프레임 사이 보간

		transform += mul(weights[i], result); // 가중치 적용하여 변환 행렬 누적
	}

	return transform; // 최종 변환 행렬 반환
}

// 버텍스 셰이더: 정점 데이터를 애니메이션 변환 행렬로 변환
MeshOutput VS(VertexTextureNormalTangentBlend input)
{
	MeshOutput output;

	matrix m = GetAnimationMatrix(input); // 애니메이션 변환 행렬 계산

	output.position = mul(input.position, m); // 정점 위치 변환
	output.position = mul(output.position, W); // 월드 변환 적용
	output.worldPosition = output.position.xyz; // 월드 좌표 설정
	output.position = mul(output.position, VP); // 뷰-프로젝션 변환 적용
	output.uv = input.uv; // UV 좌표는 변경 없음
	output.normal = mul(input.normal, (float3x3)W); // 법선 벡터 변환
	output.tangent = mul(input.tangent, (float3x3)W); // 탄젠트 벡터 변환

	return output; // 변환된 정점 데이터 반환
}

// 기본 픽셀 셰이더: 텍스처 샘플링 결과 반환
float4 PS(MeshOutput input) : SV_TARGET
{
	float4 color = DiffuseMap.Sample(LinearSampler, input.uv); // DiffuseMap에서 색상 샘플링
	return color;
}

// 레드 컬러 픽셀 셰이더: 단일 색상(빨간색) 반환
float4 PS_RED(MeshOutput input) : SV_TARGET
{
	return float4(1,0,0,1); // 빨간색 반환
}

// 렌더링 기술 정의
technique11 T0
{
	PASS_VP(P0, VS, PS) // 기본 렌더링 패스
		PASS_RS_VP(P1, FillModeWireFrame, VS, PS_RED) // 와이어프레임 모드 렌더링 패스
};
