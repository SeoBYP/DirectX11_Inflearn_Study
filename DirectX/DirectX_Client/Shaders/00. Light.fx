#ifndef _LIGHT_FX_
#define _LIGHT_FX_
#include "00. Global.fx" // 전역 쉐이더 변수와 함수를 포함하는 파일을 포함

//////////
//Struct//
//////////

// 광원에 대한 설명을 포함하는 구조체
struct LightDesc {
	float4 ambient; // 주변 광
	float4 diffuse; // 확산 광
	float4 specular; // 반사 광
	float4 emissive; // 자체 발광
	float3 direction; // 광원의 방향
	float padding; // 패딩 (16바이트 경계를 맞추기 위함)
};

// 재질에 대한 설명을 포함하는 구조체
struct MaterialDesc {
	float4 ambient; // 재질의 주변 광 속성
	float4 diffuse; // 재질의 확산 광 속성
	float4 specular; // 재질의 반사 광 속성
	float4 emissive; // 재질의 자체 발광 속성
};

///////////////
//ConstBuffer//
///////////////

// 광원에 대한 상수 버퍼
cbuffer LightBuffer {
	LightDesc GlobalLight;
};

// 재질에 대한 상수 버퍼
cbuffer MaterialBuffer {
	MaterialDesc Material;
};

///////
//SRV//
///////

// 텍스처 샘플링을 위한 쉐이더 리소스 뷰(SRV)
Texture2D DiffuseMap; // 확산 맵
Texture2D SpecularMap; // 반사 맵
Texture2D NormalMap; // 정규 맵

////////////
//Function//
////////////

// 광원 계산 함수
float4 ComputeLight(float3 normal, float2 uv, float3 worldPosition)
{
	// 각 광원 요소에 대한 색상을 초기화
	float4 ambientColor = 0;
	float4 diffuseColor = 0;
	float4 specularColor = 0;
	float4 emissiveColor = 0;

	// Ambient 처리
	{
		float4 color = GlobalLight.ambient * Material.ambient;
		ambientColor = DiffuseMap.Sample(LinearSampler, uv) * color;
	}

	// Diffuse 처리
	{
		float4 color = DiffuseMap.Sample(LinearSampler, uv);
		float value = dot(-GlobalLight.direction, normalize(normal));
		diffuseColor = color * value * GlobalLight.diffuse * Material.diffuse;
	}

	// Specular 처리
	{
		// 반사 벡터 계산
		float3 R = GlobalLight.direction - (2 * normal * dot(GlobalLight.direction, normal));
		R = normalize(R);

		// 카메라 위치에서 세계 좌표까지의 방향 벡터
		float3 cameraPosition = CameraPosition();
		float3 E = normalize(cameraPosition - worldPosition);

		float value = saturate(dot(R, E)); // 0~1 사이의 값으로 제한
		float specular = pow(value, 10);

		specularColor = GlobalLight.specular * Material.specular * specular;
	}

	// Emissive 처리
	{
		float3 cameraPosition = CameraPosition();
		float3 E = normalize(cameraPosition - worldPosition);

		float value = saturate(dot(E, normal));
		float emissive = 1.0f - value;

		emissive = smoothstep(0.0f, 1.0f, emissive);
		emissive = pow(emissive, 2);

		emissiveColor = GlobalLight.emissive * Material.emissive * emissive;
	}

	return ambientColor + diffuseColor + specularColor + emissiveColor;
}

// 정규 매핑 계산 함수
void ComputeNormalMapping(inout float3 normal, float3 tangent, float2 uv)
{
	// 텍스처에서 정규 벡터를 샘플링
	float4 map = NormalMap.Sample(LinearSampler, uv);
	if (any(map.rgb) == false) return;

	float3 N = normalize(normal); // Z 방향
	float3 T = normalize(tangent); // X 방향
	float3 B = normalize(cross(N, T)); // Y 방향, 외적을 통해 계산
	float3x3 TBN = float3x3(T, B, N); // 탄젠트 공간의 기저를 형성

	// 탄젠트 공간 정규 벡터를 [-1,1] 범위로 재조정
	float3 tangentSpaceNormal = (map.rgb * 2.0f - 1.0f);
	float3 worldNormal = mul(tangentSpaceNormal, TBN);

	normal = worldNormal;
}

#endif
