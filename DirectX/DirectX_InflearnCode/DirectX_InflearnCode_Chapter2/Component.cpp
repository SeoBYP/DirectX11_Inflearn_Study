#include "pch.h"
#include "Component.h"
#include "GameObject.h"

// 생성자 구현
Component::Component(ComponentType type) : _type(type)
{
}

// 소멸자 구현
Component::~Component()
{
}

// GetGameObject 구현: 이 컴포넌트가 부착된 GameObject의 shared_ptr 반환
std::shared_ptr<GameObject> Component::GetGameObject()
{
	return _gameObject.lock(); // 약한 참조를 강한 참조로 변환하여 반환
}

// GetTransform 구현: 이 컴포넌트가 부착된 GameObject의 Transform 컴포넌트 반환
std::shared_ptr<Transform> Component::GetTransform()
{
	return _gameObject.lock()->GetTransform(); // GameObject의 Transform을 반환
}