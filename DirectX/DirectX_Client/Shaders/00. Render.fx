#ifndef _RENDER_FX_
#define _RENDER_FX_
#include "00. Global.fx" // 전역 설정과 변수를 포함합니다.
#include "00. Light.fx" // 조명 처리를 위한 함수와 변수를 포함합니다.

// 최대 변환 행렬, 키프레임, 모델 인스턴스의 수를 정의합니다.
#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 500
#define MAX_MODEL_INSTANCE 500

//*******************메시 렌더링을 위한 구조체와 셰이더******************
struct VertexMesh
{
	float4 position : POSITION; // 정점 위치
	float2 uv : TEXCOORD; // 텍스처 좌표
	float3 normal : NORMAL; // 정점의 법선
	float3 tangent : TANGENT; // 정점의 탄젠트
	// 인스턴싱에 사용될 월드 변환 행렬
	matrix world : INST;
};

// 메시 버텍스 셰이더
MeshOutput VS_Mesh(VertexMesh input)
{
	MeshOutput output;

	output.position = mul(input.position, input.world); // 월드 변환 적용
	output.worldPosition = output.position;
	output.position = mul(output.position, VP); // 뷰-프로젝션 변환 적용
	output.uv = input.uv;
	output.normal = input.normal;

	return output;
}

//*******************모델 렌더링을 위한 구조체와 셰이더******************
struct VertexModel
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4 blendIndices : BLEND_INDICES; // 블렌딩에 사용될 본 인덱스
	float4 blendWeights : BLEND_WEIGHTS; // 본 가중치
	// 인스턴싱에 사용될 인스턴스 ID와 월드 변환 행렬
	uint instanceID : SV_INSTANCEID;
	matrix world : INST;
};

// 본 변환 행렬을 저장할 상수 버퍼
cbuffer BoneBuffer
{
	matrix BoneTransforms[MAX_MODEL_TRANSFORMS];
};

uint BoneIndex; // 현재 본 인덱스

// 모델 버텍스 셰이더
MeshOutput VS_Model(VertexModel input)
{
	MeshOutput output;

	// 본 변환을 적용합니다.
	output.position = mul(input.position, BoneTransforms[BoneIndex]);
	output.position = mul(output.position, input.world); // 월드 변환 적용
	output.worldPosition = output.position;
	output.position = mul(output.position, VP); // 뷰-프로젝션 변환 적용
	output.uv = input.uv;
	output.normal = input.normal;

	return output;
}

//*******************애니메이션 렌더링을 위한 구조체와 셰이더******************
struct KeyframeDesc
{
	int animIndex; // 애니메이션 인덱스
	uint currFrame; // 현재 프레임
	uint nextFrame; // 다음 프레임
	float ratio; // 현재 프레임과 다음 프레임 사이의 보간 비율
	float sumTime; // 애니메이션 진행 시간 합계
	float speed; // 애니메이션 재생 속도
	float2 padding; // 패딩
};

struct TweenFrameDesc
{
	float tweenDuration; // 트윈(중간 상태) 지속 시간
	float tweenRatio; // 트윈 비율
	float tweenSumTime; // 트윈 진행 시간 합계
	float padding; // 패딩
	KeyframeDesc curr; // 현재 키프레임 정보
	KeyframeDesc next; // 다음 키프레임 정보
};

// 트윈 프레임 정보를 저장할 상수 버퍼
cbuffer TweenBuffer
{
	TweenFrameDesc TweenFrames[MAX_MODEL_INSTANCE];
};

Texture2DArray TransformMap; // 변환 행렬을 저장한 텍스처 배열

// 애니메이션 행렬을 계산하는 함수
matrix GetAnimationMatrix(VertexModel input)
{
	// 각 정점에 대한 본 변환 행렬을 계산하는 로직을 구현
	float indices[4] = { input.blendIndices.x, input.blendIndices.y, input.blendIndices.z, input.blendIndices.w };
	float weights[4] = { input.blendWeights.x, input.blendWeights.y, input.blendWeights.z, input.blendWeights.w };

	int animIndex[2];
	int currFrame[2];
	int nextFrame[2];
	float ratio[2];
	// 인덱스와 가중치를 사용하여 본 변환 행렬을 계산하고,
	animIndex[0] = TweenFrames[input.instanceID].curr.animIndex;
	currFrame[0] = TweenFrames[input.instanceID].curr.currFrame;
	nextFrame[0] = TweenFrames[input.instanceID].curr.nextFrame;
	ratio[0] = TweenFrames[input.instanceID].curr.ratio;

	animIndex[1] = TweenFrames[input.instanceID].next.animIndex;
	currFrame[1] = TweenFrames[input.instanceID].next.currFrame;
	nextFrame[1] = TweenFrames[input.instanceID].next.nextFrame;
	ratio[1] = TweenFrames[input.instanceID].next.ratio;

	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
	matrix curr = 0;
	matrix next = 0;
	matrix transform = 0;
	// 텍스처 배열에서 해당하는 애니메이션 프레임의 변환 정보를 로드하여 보간
	for (int i = 0; i < 4; i++)
	{
		c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0));
		c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0));
		c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0));
		c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0));
		curr = matrix(c0, c1, c2, c3);

		n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
		n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
		n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
		n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));
		next = matrix(n0, n1, n2, n3);

		matrix result = lerp(curr, next, ratio[0]);

		if (animIndex[1] >= 0)
		{
			c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[1], animIndex[1], 0));
			c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[1], animIndex[1], 0));
			c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[1], animIndex[1], 0));
			c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[1], animIndex[1], 0));
			curr = matrix(c0, c1, c2, c3);

			n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0));
			n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0));
			n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0));
			n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0));
			next = matrix(n0, n1, n2, n3);

			matrix nextResult = lerp(curr, next, ratio[1]);
			result = lerp(result, nextResult, TweenFrames[input.instanceID].tweenRatio);
		}

		transform += mul(weights[i], result);
	}

	return transform;
}

// 애니메이션 버텍스 셰이더
MeshOutput VS_Anim(VertexModel input)
{
	MeshOutput output;

	// 애니메이션 행렬을 계산
	matrix m = GetAnimationMatrix(input);

	// 계산된 애니메이션 행렬을 적용하여 정점 위치를 변환
	output.position = mul(input.position, m);
	output.position = mul(output.position, input.world); // 월드 변환 적용
	output.worldPosition = output.position;
	output.position = mul(output.position, VP); // 뷰-프로젝션 변환 적용
	output.uv = input.uv;
	output.normal = mul(input.normal, (float3x3)input.world); // 월드 변환을 적용한 법선 벡터
	output.tangent = mul(input.tangent, (float3x3)input.world); // 월드 변환을 적용한 탄젠트 벡터

	return output;
}
#endif

