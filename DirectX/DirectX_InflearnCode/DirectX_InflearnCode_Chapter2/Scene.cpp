#include "pch.h"
#include "Scene.h"
#include "GameObject.h"

// 장면의 모든 게임 오브젝트를 초기화합니다.
void Scene::Awake()
{
	for (const shared_ptr<GameObject>& gameObejct : _gameObjects) {
		gameObejct->Awake();
	}
}

// 장면이 시작될 때 각 게임 오브젝트의 Start 메소드를 호출합니다.
void Scene::Start()
{
	for (const shared_ptr<GameObject>& gameObejct : _gameObjects) {
		gameObejct->Start();
	}
}

// 매 프레임마다 각 게임 오브젝트의 Update 메소드를 호출합니다.
void Scene::Update()
{
	for (const shared_ptr<GameObject>& gameObejct : _gameObjects) {
		gameObejct->Update();
	}
}

// 모든 Update 메소드 호출 후 각 게임 오브젝트의 LateUpdate 메소드를 호출합니다.
void Scene::LateUpdate()
{
	for (const shared_ptr<GameObject>& gameObejct : _gameObjects) {
		gameObejct->LateUpdate();
	}
}

// 고정된 시간 간격으로 각 게임 오브젝트의 FixedUpdate 메소드를 호출합니다.
void Scene::FixedUpdate()
{
	for (const shared_ptr<GameObject>& gameObejct : _gameObjects) {
		gameObejct->FixedUpdate();
	}
}

// 장면에 새로운 게임 오브젝트를 추가합니다.
void Scene::AddGameObject(shared_ptr<GameObject> gameObject)
{
	_gameObjects.push_back(gameObject);
}

// 장면에서 특정 게임 오브젝트를 제거합니다.
void Scene::RemoveGameObject(shared_ptr<GameObject> gameobject)
{
	auto findIt = std::find(_gameObjects.begin(), _gameObjects.end(), gameobject);
	if (findIt != _gameObjects.end()) {
		_gameObjects.erase(findIt);
	}
}