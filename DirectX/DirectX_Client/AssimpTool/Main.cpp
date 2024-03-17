#include "pch.h"
#include "Main.h"
#include "Engine/Game.h"
#include "AsimpTool.h"
#include "StaticMeshDemo.h"
#include "ImGUIDemo.h"
#include "AnimationDemo.h"
#include "TweenDemo.h"
#include "SkyDemo.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	GameDesc desc;
	desc.appName = L"GameClient";
	desc.hInstance = hInstance;
	desc.vsync = false;
	desc.hWnd = NULL;
	desc.width = 800;
	desc.height = 600;
	desc.clearColor = Color(0.f, 0.f, 0.f, 0.f);
	desc.app = make_shared<AnimationDemo>();
	//desc.app = make_shared<AsimpTool>();

	GAME->Run(desc);

	return 0;
}