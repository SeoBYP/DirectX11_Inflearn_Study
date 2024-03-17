#include "pch.h"
#include "Button.h"
#include "MeshRenderer.h"
#include "Material.h"

// Button 클래스의 생성자
Button::Button() : Super(ComponentType::Button)
{
	// 기본 컴포넌트 타입을 Button으로 설정합니다.
}

// Button 클래스의 소멸자
Button::~Button()
{
	// 리소스 해제 또는 정리가 필요한 경우 여기에 작성합니다.
}

// Button이 클릭되었는지 판단하는 함수
bool Button::Picked(POINT screenPos)
{
	// 클릭된 화면 좌표가 버튼 영역 안에 있는지 확인합니다.
	return ::PtInRect(&_rect, screenPos);
}

// Button을 생성하고 초기화하는 함수
void Button::Create(Vec2 screenPos, Vec2 size, shared_ptr<class Material> material)
{
	auto go = _gameObject.lock(); // 소유한 GameObject에 대한 약한 참조를 얻습니다.

	float height = GRAPHICS->GetViewport().GetHeight(); // 뷰포트 높이
	float width = GRAPHICS->GetViewport().GetWidth(); // 뷰포트 너비

	// 화면 좌표를 월드 좌표로 변환합니다.
	float x = screenPos.x - width / 2;
	float y = height / 2 - screenPos.y;
	Vec3 position = Vec3(x, y, 0);

	// GameObject의 위치와 크기를 설정합니다.
	go->GetOrAddTransform()->SetWorldPosition(position);
	go->GetOrAddTransform()->SetWorldScale(Vec3(size.x, size.y, 1));

	// GameObject의 레이어를 UI로 설정합니다.
	go->SetLayerIndex(Layer_UI);

	// MeshRenderer 컴포넌트를 추가하거나 가져옵니다.
	if (go->GetMeshRenderer() == nullptr)
		go->AddComponent(make_shared<MeshRenderer>());

	// MeshRenderer에 Material을 설정합니다.
	go->GetMeshRenderer()->SetMaterial(material);

	// 기본적으로 "Quad" 메쉬를 사용하여 버튼을 표현합니다.
	auto mesh = RESOURCES->Get<Mesh>(L"Quad");
	go->GetMeshRenderer()->SetMesh(mesh);
	go->GetMeshRenderer()->SetPass(0);

	// 버튼의 픽킹 영역을 설정합니다.
	_rect.left = screenPos.x - size.x / 2;
	_rect.right = screenPos.x + size.x / 2;
	_rect.top = screenPos.y - size.y / 2;
	_rect.bottom = screenPos.y + size.y / 2;
}

// 클릭 이벤트에 콜백 함수를 추가하는 함수
void Button::AddOnClickedEvent(std::function<void(void)> func)
{
	_onClicked = func; // 등록된 클릭 이벤트 콜백 함수를 설정합니다.
}

// 클릭 이벤트 콜백 함수를 호출하는 함수
void Button::InvokeOnClicked()
{
	if (_onClicked) // 클릭 이벤트 콜백 함수가 설정되어 있다면
		_onClicked(); // 해당 함수를 호출합니다.
}
