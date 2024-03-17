#pragma once 
#include "Component.h" 

// 충돌 감지기의 타입을 나타내는 열거형
enum class ColliderType
{
	Sphere, // 구체
	AABB,   // 축 정렬 경계 상자(Axis-Aligned Bounding Box)
	OBB,    // 객체 정렬 경계 상자(Object-Oriented Bounding Box)
};

// 모든 충돌 감지기의 기본 클래스
class BaseCollider : public Component // Component 클래스를 상속받음
{
public:
	// 생성자. 충돌 감지기의 타입을 인자로 받음
	BaseCollider(ColliderType colliderType);
	virtual ~BaseCollider(); // 가상 소멸자

	// 레이와의 교차 여부를 판단하는 순수 가상 함수. 하위 클래스에서 구현 필요
	virtual bool Intersects(Ray& ray, OUT float& distance) = 0;
	// 다른 충돌 감지기와의 교차 여부를 판단하는 순수 가상 함수. 하위 클래스에서 구현 필요
	virtual bool Intersects(shared_ptr<BaseCollider>& other) = 0;

	// 충돌 감지기의 타입을 반환하는 함수
	ColliderType GetColliderType() { return _colliderType; }

protected:
	ColliderType _colliderType; // 충돌 감지기의 타입을 저장하는 멤버 변수
};
