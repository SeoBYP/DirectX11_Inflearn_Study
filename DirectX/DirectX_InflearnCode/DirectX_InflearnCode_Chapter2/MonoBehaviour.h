#pragma once
#include "Component.h" // 기본 컴포넌트 클래스 포함

// MonoBehaviour 클래스 정의: 사용자 정의 스크립트를 위한 기반 클래스
class MonoBehaviour : public Component {
	using Super = Component; // 부모 클래스에 대한 별칭 설정

public:
	MonoBehaviour(); // 생성자
	~MonoBehaviour(); // 소멸자

	// 생명주기 메서드 오버라이드
	virtual void Awake() override; // 초기화 직전에 호출됨
	virtual void Update() override; // 매 프레임마다 호출됨
};
