#include "pch.h"
#include "18. NormalMappingDemo.h"
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

void NormalMappingDemo::Init()
{
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"14. NormalMapping.fx");

	//Material
	{
		shared_ptr<Material> material = make_shared<Material>();
		{
			material->SetShader(_shader);
		} 
		{
			auto texture = RESOURCES->Load<Texture>(L"Leather", L"..\\Resources\\Texture\\Leather.jpg");
			material->SetDiffuseMap(texture);
		}
		{
			auto texture = RESOURCES->Load<Texture>(L"LeatherNormal", L"..\\Resources\\Texture\\Leather_Normal.jpg");
			material->SetNormalMap(texture);
		}

		MaterialDesc& desc = material->GetMaterialDesc();
		desc.ambient = Vec4(1.0f);
		desc.diffuse = Vec4(1.0f);
		desc.specular = Vec4(1.0f);
		desc.emissive = Vec4(1.0f);

		RESOURCES->Add(L"Leather", material);
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
		auto material = RESOURCES->Get<Material>(L"Leather");
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
		auto material = RESOURCES->Get<Material>(L"Leather");
		MaterialDesc& desc = material->GetMaterialDesc();

		_obj2->GetMeshRenderer()->SetMaterial(material);
	}

	// Object
	//RENDER->Init(_shader);
}

void NormalMappingDemo::Update()
{
	_camera->Update();
	//RENDER->Update();
	{
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.f);
		lightDesc.diffuse = Vec4(0.f);
		lightDesc.specular = Vec4(0.f);
		lightDesc.emissive = Vec4(1.f,0.0f,0.0f,1.0f);
		lightDesc.direction = Vec3(1.f,0.0f,1.0f);
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

void NormalMappingDemo::Render()
{

}
