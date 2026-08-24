#pragma once
#include "Component.h"

// 한 번에 렌더링할 수 있는 최대 빌보드 수를 정의합니다.
#define MAX_BILLBOARD_COUNT 500

// 눈 빌보드를 구성하는 각 정점의 구조체입니다.
struct VertexSnow
{
	Vec3 position; // 정점의 위치
	Vec2 uv; // 텍스처 좌표
	Vec2 scale; // 빌보드의 크기
	Vec2 random; // 랜덤 값, 각 눈송이의 고유한 변화를 주기 위해 사용
};

// 눈 빌보드 효과를 구현하는 클래스입니다.
class SnowBillboard : public Component
{
	using Super = Component; // 부모 클래스에 대한 별칭을 정의합니다.

public:
	// 생성자는 눈이 내리는 영역의 크기(extent)와 그려질 빌보드의 수(drawCount)를 매개변수로 받습니다.
	SnowBillboard(Vec3 extent, int32 drawCount = 100);
	~SnowBillboard(); // 소멸자

	void Update(); // 매 프레임마다 호출되는 업데이트 함수입니다.

	// 눈 빌보드에 사용될 재질을 설정하는 함수입니다.
	void SetMaterial(shared_ptr<Material> material) { _material = material; }
	// 렌더링에 사용될 패스를 설정하는 함수입니다.
	void SetPass(uint8 pass) { _pass = pass; }
	// 특정 위치에 빌보드를 추가하는 함수입니다.
	void Add(Vec3 position, Vec2 scale);

private:
	vector<VertexSnow> _vertices; // 빌보드의 정점들을 저장하는 벡터입니다.
	vector<uint32> _indices; // 인덱스 버퍼에 사용될 인덱스들을 저장하는 벡터입니다.
	shared_ptr<VertexBuffer> _vertexBuffer; // 정점 버퍼입니다.
	shared_ptr<IndexBuffer> _indexBuffer; // 인덱스 버퍼입니다.

	int32 _drawCount = 0; // 실제로 그려질 빌보드의 수입니다.

	shared_ptr<Material> _material; // 눈 빌보드에 사용될 재질입니다.
	uint8 _pass = 0; // 렌더링 패스입니다.

	SnowBillboardDesc _desc; // 눈 빌보드의 설명(파라미터)을 저장하는 구조체입니다. (구조체 정의 누락)
	float _elpasedTime = 0.f; // 눈이 내리는 시뮬레이션에서 경과된 시간을 추적합니다.
};
