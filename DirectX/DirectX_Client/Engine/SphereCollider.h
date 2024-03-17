#pragma once
#include "BaseCollider.h" // 기본 충돌 감지기 클래스를 포함
#include "Component.h" // 게임 오브젝트의 구성요소를 나타내는 클래스 포함

// SphereCollider 클래스는 BaseCollider로부터 상속받아 구현됨
class SphereCollider : public BaseCollider
{
public:
	SphereCollider(); // 기본 생성자
	virtual ~SphereCollider(); // 소멸자

	// 매 프레임마다 호출되어 충돌 감지기 상태를 업데이트
	virtual void Update() override;

	// 특정 레이와 이 충돌 감지기가 교차하는지 판단
	virtual bool Intersects(Ray& ray, OUT float& distance) override;

	// 다른 충돌 감지기와의 교차 여부를 판단
	virtual bool Intersects(shared_ptr<BaseCollider>& other) override;

	// 이 구체 충돌 감지기의 경계 구체를 반환
	BoundingSphere& GetBoundingSphere() { return _boundingSphere; }

	// 구체의 반지름을 설정
	void SetRadius(float radius) { _radius = radius; }
private:
	float _radius = 1.0; // 구체의 기본 반지름
	BoundingSphere _boundingSphere; // 구체 형태의 충돌 감지를 위한 경계 구체
};
