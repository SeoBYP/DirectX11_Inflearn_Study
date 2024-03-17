#include "pch.h"
#include "SphereCollider.h" // SphereCollider 헤더 파일 포함
#include "AABBBoxCollider.h" // 축 정렬 경계 상자(AABB) 충돌 감지기 헤더 파일 포함
#include "OBBBoxCollider.h" // 객체 정렬 경계 상자(OBB) 충돌 감지기 헤더 파일 포함

// SphereCollider 클래스의 생성자
SphereCollider::SphereCollider()
	: BaseCollider(ColliderType::Sphere) // 기본 충돌 감지기에 구체 충돌 감지기 타입 지정
{
}

// SphereCollider 클래스의 소멸자
SphereCollider::~SphereCollider()
{
}

// 매 프레임마다 충돌 감지기 상태를 업데이트하는 함수
void SphereCollider::Update()
{
	// 게임 오브젝트의 월드 위치를 경계 구체의 중심으로 설정
	_boundingSphere.Center = GetGameObject()->GetTransform()->GetWorldPosition();

	// 게임 오브젝트의 월드 스케일을 기반으로 경계 구체의 반지름 계산
	Vec3 scale = GetGameObject()->GetTransform()->GetWorldScale();
	_boundingSphere.Radius = _radius * max(max(scale.x, scale.y), scale.z);
}

// 레이와 충돌 감지기의 교차 여부 판단 함수
bool SphereCollider::Intersects(Ray& ray, OUT float& distance)
{
	// DirectXMath 라이브러리의 교차 함수 사용하여 교차 판단
	return _boundingSphere.Intersects(ray.position, ray.direction, OUT distance);
}

// 다른 충돌 감지기와의 교차 여부 판단 함수
bool SphereCollider::Intersects(shared_ptr<BaseCollider>& other)
{
	// 다른 충돌 감지기의 타입을 가져옴
	ColliderType type = other->GetColliderType();

	// 타입에 따라 적절한 교차 판단 로직 수행
	switch (type)
	{
	case ColliderType::Sphere:
		// 다른 충돌 감지기가 구체인 경우
		return _boundingSphere.Intersects(dynamic_pointer_cast<SphereCollider>(other)->GetBoundingSphere());
	case ColliderType::AABB:
		// 다른 충돌 감지기가 AABB인 경우
		return _boundingSphere.Intersects(dynamic_pointer_cast<AABBBoxCollider>(other)->GetBoundingBox());
	case ColliderType::OBB:
		// 다른 충돌 감지기가 OBB인 경우
		return _boundingSphere.Intersects(dynamic_pointer_cast<OBBBoxCollider>(other)->GetBoundingBox());
	}

	// 위의 경우에 해당하지 않는 경우, 교차하지 않는 것으로 처리
	return false;
}
