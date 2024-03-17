#pragma once // 현재 파일이 프로그램 내 다른 소스 파일에 의해 여러 번 include 되는 것을 방지
#include <string>
#include "GameObject.h"
#include "Pipeline.h"

class InputManager;
class TimeManager;
class SceneManager;
class ResourceManager;
class RenderManager;

class Game
{
public:
	Game(); // 생성자
	~Game(); // 소멸자

public:
	void Init(HWND hwnd); // 게임 초기화 함수, 윈도우 핸들을 받음
	void Update(); // 게임 로직 업데이트 함수
	void Render(); // 렌더링 함수

	shared_ptr<SceneManager> GetSceneManager() { return _scene; }
	shared_ptr<TimeManager> GetTimeManager() { return _time; }
	shared_ptr<InputManager> GetInputManager() { return _input; }
	shared_ptr<ResourceManager> GetResourceManager() { return _resource; }
	shared_ptr<RenderManager> GetRenderManager() { return _render; }

private:

private:
	HWND _hwnd;

	//shared_ptr<Graphics> _graphics;
	shared_ptr<Graphics> _graphics;

	shared_ptr<GameObject> _monster;
	shared_ptr<GameObject> _camera;

private:
	shared_ptr<SceneManager> _scene;
	shared_ptr<InputManager> _input;
	shared_ptr<ResourceManager> _resource;
	shared_ptr<TimeManager> _time;
	shared_ptr<RenderManager> _render;
};

extern unique_ptr<Game> G_Game;
