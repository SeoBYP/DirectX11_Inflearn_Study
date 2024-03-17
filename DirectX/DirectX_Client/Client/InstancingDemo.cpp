#include "pch.h"
#include "InstancingDemo.h"
#include "CameraScript.h"
#include "Camera.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "VertexBuffer.h"

void InstancingDemo::Init()
{
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"19. InstancingDemo.fx");

	// Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform()->SetWorldPosition(Vec3{ 0.f, 0.f, -5.f });
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	// Material
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

		//Instancing
		_material = material;
	}

	for (int32 i = 0; i < 10000; i++) {
		auto obj = make_shared<GameObject>();
		obj->GetOrAddTransform()->SetWorldPosition(Vec3(rand() % 10, 0, rand() % 10));
		obj->AddComponent(make_shared<MeshRenderer>());
		{
			obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"UnityLogo"));
		}
		{
			auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
			obj->GetMeshRenderer()->SetMesh(mesh);

			_mesh = mesh;
		}
		_objs.push_back(obj);
	}


	//RENDER->Init(_shader);

	// INSTANCING
	_instanceBuffer = make_shared<VertexBuffer>();

	for (auto& obj : _objs)
	{
		Matrix world = obj->GetTransform()->GetWorldMatrix();
		_worlds.push_back(world);
	}

	_instanceBuffer->Create(_worlds, 1);
}

void InstancingDemo::Update()
{
	_camera->Update();
	//RENDER->Update();

	{
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.4f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(0.f);
		lightDesc.direction = Vec3(1.f, 0.f, 1.f);
		//RENDER->PushLightData(lightDesc);
	}

	//for (auto obj : _objs)
	//{
	//	obj->Update();
	//}

	_material->Update();

	//auto world = GetTransform()->GetWorldMatrix();
	//RENDER->PushTransformData(TransformDesc{ world });

	_mesh->GetVertexBuffer()->PushData();
	_instanceBuffer->PushData();
	_mesh->GetIndexBuffer()->PushData();

	//_shader->DrawIndexed(0, 0, _mesh->GetIndexBuffer()->GetCount(), 0, 0);
	_shader->DrawIndexedInstanced(0, 0, _mesh->GetIndexBuffer()->GetCount(), _objs.size());
}

void InstancingDemo::Render()
{
}
