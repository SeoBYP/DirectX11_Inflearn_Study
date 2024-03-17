#pragma once
#include "Component.h"

// 빌보드를 구성하는 각 정점을 정의하는 구조체
struct VertexBillboard
{
	Vec3 position; // 정점의 위치
	Vec2 uv; // 텍스처 좌표
	Vec2 scale; // 빌보드의 크기
};

// 한 번에 렌더링할 수 있는 최대 빌보드 수를 정의
#define MAX_BILLBOARD_COUNT 500

// Component 클래스를 상속받아 빌보드 기능을 구현한 클래스
class Billboard : public Component
{
	using Super = Component; // 부모 클래스의 별칭을 Super로 정의

public:
	Billboard(); // 생성자
	~Billboard(); // 소멸자

	void Update(); // 매 프레임마다 호출되는 업데이트 함수
	void Add(Vec3 position, Vec2 scale); // 새로운 빌보드를 추가하는 함수

	// 빌보드에 사용될 재질을 설정하는 함수
	void SetMaterial(shared_ptr<Material> material) { _material = material; }
	// 렌더링에 사용될 패스를 설정하는 함수
	void SetPass(uint8 pass) { _pass = pass; }

private:
	vector<VertexBillboard> _vertices; // 빌보드의 정점들을 저장하는 벡터
	vector<uint32> _indices; // 인덱스 버퍼에 사용될 인덱스들을 저장하는 벡터
	shared_ptr<VertexBuffer> _vertexBuffer; // 정점 버퍼
	shared_ptr<IndexBuffer> _indexBuffer; // 인덱스 버퍼

	int32 _drawCount = 0; // 현재 렌더링할 빌보드의 수
	int32 _prevCount = 0; // 이전 프레임에서 렌더링한 빌보드의 수

	shared_ptr<Material> _material; // 빌보드에 사용될 재질
	uint8 _pass = 0; // 렌더링 패스
};
