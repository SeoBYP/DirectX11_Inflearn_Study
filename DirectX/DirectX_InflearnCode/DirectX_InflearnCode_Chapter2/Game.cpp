#include "pch.h"
#include "Game.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "RenderManager.h"

unique_ptr<Game> G_Game = make_unique<Game>();

Game::Game() // 생성자 구현
{

}

Game::~Game() // 소멸자 구현
{
}

void Game::Init(HWND hwnd) // 초기화 함수 구현
{
	_hwnd = hwnd;

	_graphics = make_shared<Graphics>(hwnd);

	_input = make_shared<InputManager>();
	_input->Init(hwnd);
	_time = make_shared<TimeManager>();
	_time->Init();
	_scene = make_shared<SceneManager>(_graphics);
	_scene->Init();
	_resource = make_shared<ResourceManager>(_graphics->GetDevice());
	_resource->Init();
	_render = make_shared<RenderManager>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	_render->Init();

	SCENE->LoadScene(L"Test");
}

void Game::Update() // 게임 로직 업데이트
{

	TIME->Update();
	INPUT->Update();
	SCENE->Update();
}

void Game::Render() // 렌더링 함수 구현
{
	RENDER->Update(_graphics);
}

//DXGI
//D3D11_USAGE_IMMUTABLE