#pragma once
#include "Component.h"

// Component 클래스를 상속받는 Button 클래스 선언
class Button : public Component
{
	using Super = Component; // 부모 클래스 Component에 대한 별칭으로 Super를 사용

public:
	Button(); // Button 클래스의 생성자
	virtual ~Button(); // Button 클래스의 가상 소멸자

	// 화면상의 위치에 대한 픽킹(선택) 검사를 수행하는 함수
	bool Picked(POINT screenPos);

	// 버튼을 생성하는 함수. 화면 위치, 크기, 사용할 재질을 매개변수로 받음
	void Create(Vec2 screenPos, Vec2 size, shared_ptr<class Material> material);

	// 클릭 이벤트에 대한 콜백 함수를 추가하는 함수
	void AddOnClickedEvent(std::function<void(void)> func);

	// 클릭 이벤트 발생 시 등록된 콜백 함수를 호출하는 함수
	void InvokeOnClicked();

private:
	std::function<void(void)> _onClicked; // 클릭 이벤트에 대한 콜백 함수를 저장하는 멤버 변수
	RECT _rect; // 버튼의 화면상 위치와 크기를 나타내는 RECT 구조체

};
