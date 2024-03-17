#pragma once

class GameObject;

// Scene 클래스는 게임의 장면을 관리하는 클래스입니다. 여러 게임 오브젝트들을 포함하고 있습니다.
class Scene
{
public:
	// 게임 오브젝트들의 초기화를 위한 메소드
	void Awake();
	// 게임 시작 시 호출되는 메소드
	void Start();
	// 매 프레임마다 호출되는 업데이트 메소드
	void Update();
	// 모든 업데이트 로직이 실행된 후 호출되는 메소드
	void LateUpdate();
	// 고정된 업데이트 주기로 호출되는 메소드, 물리 계산 등에 사용
	void FixedUpdate();

public:
	// 새 게임 오브젝트를 장면에 추가하는 메소드
	void AddGameObject(shared_ptr<GameObject> gameObject);
	// 장면에서 게임 오브젝트를 제거하는 메소드
	void RemoveGameObject(shared_ptr<GameObject> gameobject);

	// 현재 장면에 있는 모든 게임 오브젝트들을 반환하는 메소드
	const vector<shared_ptr<GameObject>>& GetGameObjects() { return _gameObjects; }

private:
	// 현재 장면에 있는 게임 오브젝트들을 저장하는 벡터
	vector<shared_ptr<GameObject>> _gameObjects;
};
