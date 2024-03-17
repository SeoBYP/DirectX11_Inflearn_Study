#include "pch.h"
#include "RenderDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "GameObject.h"
#include "CameraScript.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"
#include "Mesh.h"
#include "Transform.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

void RenderDemo::Init()
{
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"23. RenderDemo.fx");

	// Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform()->SetWorldPosition(Vec3{ 0.f, 0.f, -5.f });
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	//Animation
	{
		shared_ptr<class Model> m1 = make_shared<Model>();
		m1->ReadModel(L"Kachujin/Kachujin");
		m1->ReadMaterial(L"Kachujin/Kachujin");
		m1->ReadAnimation(L"Kachujin/Idle");
		m1->ReadAnimation(L"Kachujin/Run");
		m1->ReadAnimation(L"Kachujin/Slash");

		for (int32 i = 0; i < 500; i++)
		{
			auto obj = make_shared<GameObject>();
			obj->GetOrAddTransform()->SetWorldPosition(Vec3(rand() % 100, 0, rand() % 100));
			obj->GetOrAddTransform()->SetWorldScale(Vec3(0.01f));
			obj->AddComponent(make_shared<ModelAnimator>(_shader));
			{
				obj->GetModelAnimator()->SetModel(m1);
				obj->GetModelAnimator()->SetPass(2);
			}
			_objs.push_back(obj);
		}

	}

	//Model
	{	
		shared_ptr<class Model> m1 = make_shared<Model>();
		m1->ReadModel(L"Tower/Tower");
		m1->ReadMaterial(L"Tower/Tower");

		for (int32 i = 0; i < 500; i++)
		{
			auto obj = make_shared<GameObject>();
			obj->GetOrAddTransform()->SetWorldPosition(Vec3(rand() % 100, 0, rand() % 100));
			obj->GetOrAddTransform()->SetWorldScale(Vec3(0.01f));
			obj->AddComponent(make_shared<ModelRenderer>(_shader));
			{
				obj->GetModelRenderer()->SetModel(m1);
				obj->GetModelRenderer()->SetPass(1);
			}
			_objs.push_back(obj);
		}
	}

	// Mesh
	// Material
	{
		{
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(_shader);
			auto texture = RESOURCES->Load<Texture>(L"UnityLogo", L"..\\Resources\\Texture\\UnityLogo.png");
			material->SetDiffuseMap(texture);
			MaterialDesc& desc = material->GetMaterialDesc();
			desc.ambient = Vec4(1.f);
			desc.diffuse = Vec4(1.f);
			desc.specular = Vec4(1.f);
			RESOURCES->Add(L"UnityLogo", material);
		}

		for (int32 i = 0; i < 500; i++) {
			auto obj = make_shared<GameObject>();
			obj->GetOrAddTransform()->SetWorldPosition(Vec3(rand() % 100, 0, rand() % 100));
			obj->AddComponent(make_shared<MeshRenderer>());
			{
				obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"UnityLogo"));
			}
			{
				auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
				obj->GetMeshRenderer()->SetMesh(mesh);
			}
			_objs.push_back(obj);
		}
	}


	//RENDER->Init(_shader);
}

void RenderDemo::Update()
{
	_camera->Update();
	//RENDER->Update();

	{
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.4f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(0.1f);
		lightDesc.direction = Vec3(1.f, 0.f, 1.f);
		//RENDER->PushLightData(lightDesc);
	}

	// INSTANCING
	INSTANCING->Render(_objs);
}

void RenderDemo::Render()
{

}