#pragma once
#include "Component.h" // 기본 컴포넌트 클래스를 상속받기 위함
#include "Animation.h" // Animation 클래스 사용을 위한 포함

// Animator 클래스: 개별 게임 오브젝트의 애니메이션을 관리
class Animator : public Component
{
	using Super = Component; // 상위 클래스 별칭

public:
	Animator(); // 생성자
	virtual ~Animator(); // 소멸자

	void Init(); // 컴포넌트 초기화 함수
	void Update(); // 프레임마다 애니메이션 상태 업데이트 함수

	shared_ptr<Animation> GetCurrentAnimation(); // 현재 애니메이션 객체 반환
	const Keyframe& GetCurrentKeyframe(); // 현재 키 프레임 반환

	void SetAnimation(shared_ptr<Animation> animation); // 애니메이션 설정 함수

private:
	float _sumTime = 0.f; // 현재 키 프레임 진행 시간

	int32 _currentKeyframeIndex = 0; // 현재 키 프레임 인덱스
	shared_ptr<Animation> _currentAnimation; // 현재 애니메이션 객체
};
