#include "pch.h"
#include "17. MaterialDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "Transform.h"
#include "Texture.h"
#include "ResourceBase.h"
#include "MeshRenderer.h"
#include "GameObject.h"
//#include "RenderManager.h"
#include "Material.h"

void MaterialDemo::Init()
{
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"13. Lighting.fx");

	//Material
	{
		shared_ptr<Material> material = make_shared<Material>();
		{
			material->SetShader(_shader);
		} 
		{
			auto texture = RESOURCES->Load<Texture>(L"UnityLogo", L"..\\Resources\\Texture\\UnityLogo.png");
			material->SetDiffuseMap(texture);
		}

		MaterialDesc& desc = material->GetMaterialDesc();
		desc.ambient = Vec4(1.0f);
		desc.diffuse = Vec4(1.0f);

		RESOURCES->Add(L"UnityLogo", material);
		RESOURCES->Add(L"UnityLogo", material);
	}

	// Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform()->SetWorldPosition(Vec3{ 0.f, 0.f, -10.f });
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	// Object
	_obj = make_shared<GameObject>();
	_obj->GetOrAddTransform();
	_obj->AddComponent(make_shared<MeshRenderer>());
	{
		auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
		_obj->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		auto material = RESOURCES->Get<Material>(L"UnityLogo");
		_obj->GetMeshRenderer()->SetMaterial(material);
	}

	// Object2
	_obj2 = make_shared<GameObject>();
	_obj2->GetOrAddTransform()->SetWorldPosition(Vec3{ 0.5f, 0.f, 2.f });
	_obj2->AddComponent(make_shared<MeshRenderer>());
	{
		auto mesh = RESOURCES->Get<Mesh>(L"Cube");
		_obj2->GetMeshRenderer()->SetMesh(mesh);
	}
	{
		auto material = RESOURCES->Get<Material>(L"UnityLogo")->Clone();
		MaterialDesc& desc = material->GetMaterialDesc();
		desc.ambient = Vec4(0.5f);
		desc.diffuse = Vec4(0.5f);

		_obj2->GetMeshRenderer()->SetMaterial(material);
	}

	// Object
	//RENDER->Init(_shader);
}

void MaterialDemo::Update()
{
	_camera->Update();
	//RENDER->Update();
	{
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.5f);
		lightDesc.diffuse = Vec4(1.0f);
		lightDesc.specular = Vec4(1.0f);
		lightDesc.direction = Vec3(0.f,-1.0f,0.f);
		//RENDER->PushLightData(lightDesc);

	}
	{
		MaterialDesc desc;
		desc.ambient = Vec4(0.2f);
		desc.diffuse = Vec4(1.0f);
		desc.specular = Vec4(1.0f);
		desc.emissive = Color(0.3f,0.f,0.f,0.5f);

		//RENDER->PushMaterialData(desc);
		_obj->Update();
	}
	{
		MaterialDesc desc;
		desc.ambient = Vec4(0.1f);
		desc.diffuse = Vec4(1.0f);
		//desc.specular = Vec4(1.0f);

		//RENDER->PushMaterialData(desc);
		_obj2->Update();
	}
}

void MaterialDemo::Render()
{

}
