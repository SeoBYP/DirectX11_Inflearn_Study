#include "pch.h"
#include "Animator.h"
#include "Game.h"
#include "TimeManager.h"

// 생성자: 컴포넌트 타입을 Animator로 설정
Animator::Animator() : Super(ComponentType::Animator)
{

}

// 소멸자: 추가적인 정리 필요시 구현
Animator::~Animator()
{

}

// Init 함수: 초기화 로직 구현
void Animator::Init()
{

}

// Update 함수: 매 프레임마다 애니메이션 상태를 업데이트
void Animator::Update()
{
	// 현재 애니메이션 객체 가져오기
	shared_ptr<Animation> animation = GetCurrentAnimation();
	if (!animation) return; // 애니메이션 객체가 없으면 업데이트 중단

	// 현재 키 프레임 정보 가져오기
	const Keyframe& keyframe = GetCurrentKeyframe();

	// 경과 시간 업데이트
	float deltaTime = TIME->GetDeltaTime();
	_sumTime += deltaTime;

	// 다음 키 프레임으로 전환 조건 체크
	if (_sumTime >= keyframe.time)
	{
		_currentKeyframeIndex++; // 다음 키 프레임으로 인덱스 이동
		int32 totalCount = animation->GetKeyframeCount(); // 전체 키 프레임 수

		// 마지막 키 프레임을 넘어설 경우 처리
		if (_currentKeyframeIndex >= totalCount)
		{
			if (animation->IsLoop())
				_currentKeyframeIndex = 0; // 반복 설정이 true면 처음으로
			else
				_currentKeyframeIndex = totalCount - 1; // 반복이 아니면 마지막 키 프레임 유지
		}

		_sumTime = 0.f; // 키 프레임 시간 초기화
	}
}

// 현재 애니메이션 객체 반환
std::shared_ptr<Animation> Animator::GetCurrentAnimation()
{
	return _currentAnimation;
}

// 현재 키 프레임 반환
const Keyframe& Animator::GetCurrentKeyframe()
{
	return _currentAnimation->GetKeyframe(_currentKeyframeIndex);
}