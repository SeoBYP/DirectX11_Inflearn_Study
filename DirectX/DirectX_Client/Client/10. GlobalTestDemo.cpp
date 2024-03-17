#include "pch.h"
#include "10. GlobalTestDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "Transform.h"
#include "Texture.h"
#include "ResourceBase.h"
#include "MeshRenderer.h"
#include "GameObject.h"
//#include "RenderManager.h"

void GlobalTestDemo::Init()
{
	_shader = make_shared<Shader>(L"08. GlobalTest.fx");
	// Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	_camera->GetTransform()->SetWorldPosition(Vec3(0.f, 0.f, -2.f));

	//
	_obj = make_shared < GameObject>();
	_obj->GetOrAddTransform();
	_obj->AddComponent(make_shared<MeshRenderer>()); 
	{
		_obj->GetMeshRenderer()->SetShader(_shader);
	}
	{
		RESOURCES->Init();
		auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
		_obj->GetMeshRenderer()->SetMesh(mesh);
	}
	{
	 	auto texture = RESOURCES->Load<Texture>(L"UnityLogo", L"..\\Resources\\Texture\\UnityLogo.png");
		_obj->GetMeshRenderer()->SetTexture(texture);
	}
	// Object
	//RENDER->Init(_shader);
}

void GlobalTestDemo::Update()
{
	_camera->Update();
	//RENDER->Update();
	_obj->Update();
}

void GlobalTestDemo::Render()
{

}
