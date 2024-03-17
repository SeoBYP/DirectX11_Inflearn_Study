#pragma once

#include <memory> // shared_ptr과 weak_ptr을 위한 헤더 포함

class GameObject; // GameObject 클래스의 전방 선언
class Transform; // Transform 클래스의 전방 선언

// 컴포넌트 유형을 나타내는 열거형
enum class ComponentType : uint8_t {
	Transform,
	MeshRenderer,
	Camera,
	Animator,
	Script, // 사용자 정의 스크립트를 위한 컴포넌트 유형
	End, // 유효한 컴포넌트 유형의 끝을 표시, 총 개수 계산에 사용
};

// 고정된 컴포넌트 수를 정의 (Script 제외한 기본 컴포넌트)
enum {
	FIXED_COMPONENT_COUNT = static_cast<uint8_t>(ComponentType::End) - 1
};

// Component 클래스 정의
class Component
{
public:
	Component(ComponentType type); // 생성자
	virtual ~Component(); // 가상 소멸자

	// 생명주기 메서드, 상속받는 컴포넌트에서 오버라이드 가능
	virtual void Awake() {}; // 초기화 직전 호출
	virtual void Start() {}; // 첫 번째 Update 호출 전 호출

	virtual void Update() {}; // 매 프레임마다 호출
	virtual void LateUpdate() {}; // 모든 Update 메서드 호출 후 호출
	virtual void FixedUpdate() {}; // 고정된 시간 간격으로 호출

public:
	ComponentType GetType() { return _type; } // 컴포넌트 유형 반환

	std::shared_ptr<GameObject> GetGameObject(); // 이 컴포넌트가 부착된 GameObject 반환
	std::shared_ptr<Transform> GetTransform(); // 이 컴포넌트의 Transform 반환

private:
	friend class GameObject; // GameObject 클래스가 private 멤버에 접근할 수 있도록 함
	void SetGameObject(std::shared_ptr<GameObject> gameObject) { _gameObject = gameObject; } // GameObject 설정

protected:
	ComponentType _type; // 컴포넌트 유형
	std::weak_ptr<GameObject> _gameObject; // 이 컴포넌트를 소유한 GameObject에 대한 약한 참조
};
