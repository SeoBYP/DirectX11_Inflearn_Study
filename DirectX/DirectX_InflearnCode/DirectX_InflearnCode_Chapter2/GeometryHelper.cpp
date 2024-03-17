#include "pch.h"
#include "GeometryHelper.h"

void GeometryHelper::CreateRectangle(shared_ptr<Geometry<VertexTextureData>> geometry)
{
	vector<VertexTextureData> vertieces;
	vertieces.resize(4);

	// 첫 번째 정점: 위치와 색상 설정
	vertieces[0].position = Vec3(-0.5f, -0.5f, 0.f); // 화면 중앙 좌측 하단
	vertieces[0].uv = Vec2(0.f, 1.f);

	// 두 번째 정점: 위치와 색상 설정
	vertieces[1].position = Vec3(-0.5f, 0.5f, 0.f); // 화면 중앙 상단
	vertieces[1].uv = Vec2(0.f, 0.f);

	// 세 번째 정점: 위치와 색상 설정
	vertieces[2].position = Vec3(0.5f, -0.5f, 0.f); // 화면 중앙 우측 하단
	vertieces[2].uv = Vec2(1.f, 1.f);

	// 네 번째 정점: 위치와 색상 설정
	vertieces[3].position = Vec3(0.5f, 0.5f, 0.f); // 화면 중앙 우측 하단
	vertieces[3].uv = Vec2(1.f, 0.f);
	geometry->SetVertices(vertieces);

	vector<uint32> indices = { 0,1,2,2,1,3 };
	geometry->SetIndices(indices);
}

void GeometryHelper::CreateRectangle(shared_ptr<Geometry<VertexColorData>> geometry, Color color)
{
	vector<VertexColorData> vertieces;
	vertieces.resize(4);

	// 첫 번째 정점: 위치와 색상 설정
	vertieces[0].position = Vec3(-0.5f, -0.5f, 0.f); // 화면 중앙 좌측 하단
	vertieces[0].color = color;

	// 두 번째 정점: 위치와 색상 설정
	vertieces[1].position = Vec3(-0.5f, 0.5f, 0.f); // 화면 중앙 상단
	vertieces[1].color = color;

	// 세 번째 정점: 위치와 색상 설정
	vertieces[2].position = Vec3(0.5f, -0.5f, 0.f); // 화면 중앙 우측 하단
	vertieces[2].color = color;

	// 세 번째 정점: 위치와 색상 설정
	vertieces[3].position = Vec3(0.5f, 0.5f, 0.f); // 화면 중앙 우측 하단
	vertieces[3].color = color;
	geometry->SetVertices(vertieces);

	vector<uint32> indices = { 0,1,2,2,1,3 };
	geometry->SetIndices(indices);
}
