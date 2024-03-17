#pragma once

#include "Component.h" // 기본 컴포넌트 클래스 포함

class MonoBehaviour;
class Transform;
class Camera;
class MeshRenderer;
class Animator;

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	// 생성자: DirectX 디바이스와 컨텍스트를 받아 초기화
	GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	// 소멸자
	~GameObject();

	void Awake();
	void Start();

	// 업데이트 메서드: 게임 오브젝트의 로직 업데이트를 처리
	void Update();
	void LateUpdate();
	void FixedUpdate();

	shared_ptr<Component> GetFixedComponent(ComponentType type);
	shared_ptr<Transform> GetTransform();
	shared_ptr<Camera> GetCamera();
	shared_ptr<MeshRenderer> GetMeshRenderer();
	shared_ptr<Animator> GetAnimator();

	shared_ptr<Transform> GetOrAddTransform();

	void AddComponent(shared_ptr<Component> component);

private:
	ComPtr<ID3D11Device> _device; // DirectX 디바이스

protected:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<shared_ptr<MonoBehaviour>> _scripts;
};