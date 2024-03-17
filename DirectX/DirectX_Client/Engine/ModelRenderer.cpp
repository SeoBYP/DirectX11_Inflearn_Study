#include "pch.h"
#include "ModelRenderer.h"
#include "Material.h"
#include "ModelMesh.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
ModelRenderer::ModelRenderer(shared_ptr<Shader> shader)
	: Super(ComponentType::ModelRenderer), _shader(shader)
{

}

ModelRenderer::~ModelRenderer()
{

}
//
//void ModelRenderer::Update()
//{
//	if (_model == nullptr)
//		return;
//
//	auto world = GetTransform()->GetWorldMatrix();
//	RENDER->PushTransformData(TransformDesc{ world });
//
//	const auto& meshes = _model->GetMeshes();
//	for (auto& mesh : meshes)
//	{
//		if (mesh->material)
//			mesh->material->Update();
//
//		uint32 stride = mesh->vertexBuffer->GetStride();
//		uint32 offset = mesh->vertexBuffer->GetOffset();
//
//		DC->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
//		DC->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//		_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
//	}
//}

//void ModelRenderer::Update()
//{
//	if (_model == nullptr)
//		return;
//
//	// Bones
//	BoneDesc boneDesc;
//
//	const uint32 boneCount = _model->GetBoneCount();
//	for (uint32 i = 0; i < boneCount; i++)
//	{
//		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
//		boneDesc.transforms[i] = bone->transform;
//	}
//	RENDER->PushBoneData(boneDesc);
//
//	// Transform
//	auto world = GetTransform()->GetWorldMatrix();
//	RENDER->PushTransformData(TransformDesc{ world });
//
//	const auto& meshes = _model->GetMeshes();
//	for (auto& mesh : meshes)
//	{
//		if (mesh->material)
//			mesh->material->Update();
//
//		// BoneIndex
//		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);
//
//		uint32 stride = mesh->vertexBuffer->GetStride();
//		uint32 offset = mesh->vertexBuffer->GetOffset();
//
//		DC->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
//		DC->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//		_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
//	}
//}


void ModelRenderer::SetModel(shared_ptr<Model> model)
{
	_model = model;

	const auto& materials = _model->GetMaterials();
	for (auto& material : materials)
	{
		material->SetShader(_shader);
	}
}

// 인스턴싱 버퍼를 사용하여 모델 렌더링
void ModelRenderer::RenderInstancing(shared_ptr<class InstancingBuffer>& buffer)
{
	// 모델이 설정되지 않았으면 함수 종료
	if (_model == nullptr)
		return;

	// 전역 데이터(카메라의 뷰 매트릭스와 프로젝션 매트릭스)를 셰이더에 전달
	_shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

	// 현재 장면의 광원 데이터 가져오기 및 셰이더에 전달
	auto lightObj = SCENE->GetCurrentScene()->GetLight();
	if (lightObj == nullptr) return; // 광원 객체가 없으면 함수 종료
	_shader->PushLightData(lightObj->GetLight()->GetLightDesc());

	// 본 데이터 준비
	BoneDesc boneDesc;

	// 모델의 본 개수를 가져오고 각 본의 변환 매트릭스를 셰이더에 전달
	const uint32 boneCount = _model->GetBoneCount();
	for (uint32 i = 0; i < boneCount; i++)
	{
		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
		boneDesc.transforms[i] = bone->transform; // 각 본의 변환 매트릭스를 본 데이터에 저장
	}
	_shader->PushBoneData(boneDesc); // 준비된 본 데이터를 셰이더에 전달

	// 모델의 모든 메시에 대해 반복하여 렌더링
	const auto& meshes = _model->GetMeshes();
	for (auto& mesh : meshes)
	{
		if (mesh->material)
			mesh->material->Update(); // 메시의 재질이 있다면 업데이트

		// 현재 메시의 본 인덱스를 셰이더에 설정
		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		// 입력 어셈블러 설정: 정점 버퍼와 인덱스 버퍼를 GPU에 전달
		mesh->vertexBuffer->PushData();
		mesh->indexBuffer->PushData();

		// 인스턴싱 버퍼의 데이터를 GPU에 전달
		buffer->PushData();

		// 인스턴싱을 사용하여 메시 렌더링
		_shader->DrawIndexedInstanced(0, _pass, mesh->indexBuffer->GetCount(), buffer->GetCount(), 0);
	}
}


InstanceID ModelRenderer::GetInstanceID()
{
	return make_pair((uint64)_model.get(), (uint64)_shader.get());
}
