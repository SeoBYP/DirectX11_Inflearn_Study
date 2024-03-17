#pragma once
#include "BaseCollider.h" // 기본 충돌 감지기 클래스를 상속받기 위해 포함

// AABB 충돌 감지기 클래스 정의
class AABBBoxCollider : public BaseCollider
{
public:
	// 기본 생성자
	AABBBoxCollider(); // BaseCollider의 생성자에 ColliderType을 AABB로 설정

	// 소멸자
	virtual ~AABBBoxCollider();
	// 매 프레임마다 호출되어 객체 상태를 업데이트하는 메소드
	virtual void Update() override;
	// 레이와의 충돌 검출
	virtual bool Intersects(Ray& ray, OUT float& distance) override;

	// 다른 충돌 감지기와의 충돌 검출
	virtual bool Intersects(shared_ptr<BaseCollider>& other) override;
	// 경계 상자 가져오기
	BoundingBox& GetBoundingBox() { return _boundingBox; } // 현재 충돌 감지기의 AABB 반환

private:
	BoundingBox _boundingBox; // 내부적으로 사용하는 AABB
};
