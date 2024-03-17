#include "pch.h"
#include "13. DiffuseDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "Transform.h"
#include "Texture.h"
#include "ResourceBase.h"
#include "MeshRenderer.h"
#include "GameObject.h"
//#include "RenderManager.h"

void DiffuseDemo::Init()
{
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"10. Lighting_Diffuse.fx");
	// Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform()->SetWorldPosition(Vec3{ 0.f, 0.f, -10.f });
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());


	//
	_obj = make_shared<GameObject>();
	_obj->GetOrAddTransform();
	_obj->AddComponent(make_shared<MeshRenderer>());
	{
		_obj->GetMeshRenderer()->SetShader(_shader);
	}
	{
		auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
		_obj->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		auto texture = RESOURCES->Load<Texture>(L"UnityLogo", L"..\\Resources\\Texture\\UnityLogo.png");
		_obj->GetMeshRenderer()->SetTexture(texture);
	}

	//Object2

	_obj2 = make_shared<GameObject>();
	_obj2->GetOrAddTransform()->SetWorldPosition(Vec3{ 0.5f, 0.f, 2.f });
	_obj2->AddComponent(make_shared<MeshRenderer>());
	{
		_obj2->GetMeshRenderer()->SetShader(_shader);
	}
	{
		auto mesh = RESOURCES->Get<Mesh>(L"Cube");
		_obj2->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		auto texture = RESOURCES->Load<Texture>(L"UnityLogo", L"..\\Resources\\Texture\\UnityLogo.png");
		_obj2->GetMeshRenderer()->SetTexture(texture);
	}

	// Object
	//RENDER->Init(_shader);
}

void DiffuseDemo::Update()
{
	_camera->Update();
	//RENDER->Update();

	// 
	Vec4 lightDiffuse{ 0.5f };
	_shader->GetVector("LightDiffuse")->SetFloatVector((float*)&lightDiffuse);

	Vec3 lightDir{ 1.f, 0.f, 0.f };
	lightDir.Normalize();
	_shader->GetVector("LightDir")->SetFloatVector((float*)&lightDir);


	{
		Vec4 material(1.f);
		_shader->GetVector("MaterialDiffuse")->SetFloatVector((float*)&material);
		_obj->Update();
	}

	{
		Vec4 material(1.f);
		_shader->GetVector("MaterialDiffuse")->SetFloatVector((float*)&material);
		_obj2->Update();
	}
}

void DiffuseDemo::Render()
{

}
