#include "pch.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Game.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Light.h"

MeshRenderer::MeshRenderer() : Super(ComponentType::MeshRenderer)
{

}

MeshRenderer::~MeshRenderer()
{

}

//void MeshRenderer::Update()
//{
//	if (_mesh == nullptr || _texture == nullptr || _shader == nullptr)
//		return;
//
//	_shader->GetSRV("Texture0")->SetResource(_texture->GetComPtr().Get());
//
//	auto world = GetTransform()->GetWorldMatrix();
//	RENDER->PushTransformData(TransformDesc{ world });
//
//	//auto world = GetTransform()->GetWorldMatrix();
//	//_shader->GetMatrix("World")->SetMatrix((float*)&world);
//	//
//	//_shader->GetMatrix("View")->SetMatrix((float*)&Camera::S_MatView);
//	//_shader->GetMatrix("Projection")->SetMatrix((float*)&Camera::S_MatProjection);
//	//_shader->GetSRV("Texture0")->SetResource(_texture->GetComPtr().Get());
//	//
//	// TEMP
//	//Vec3 lightDir = {0.f, 0.f, 1.f};
//	//_shader->GetVector("LightDir")->SetFloatVector((float*)&lightDir);
//
//	uint32 stride = _mesh->GetVertexBuffer()->GetStride();
//	uint32 offset = _mesh->GetVertexBuffer()->GetOffset();
//
//	DC->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
//	DC->IASetIndexBuffer(_mesh->GetIndexBuffer()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//	_shader->DrawIndexed(0, 0, _mesh->GetIndexBuffer()->GetCount(), 0, 0);
//}

///// <summary>
///// Diffusemap
///// </summary>
//void MeshRenderer::Update()
//{
//	if (_mesh == nullptr || _texture == nullptr || _shader == nullptr)
//		return;
//
//	_shader->GetSRV("DiffuseMap")->SetResource(_texture->GetComPtr().Get());
//
//	auto world = GetTransform()->GetWorldMatrix();
//	RENDER->PushTransformData(TransformDesc{ world });
//
//	uint32 stride = _mesh->GetVertexBuffer()->GetStride();
//	uint32 offset = _mesh->GetVertexBuffer()->GetOffset();
//
//	DC->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
//	DC->IASetIndexBuffer(_mesh->GetIndexBuffer()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//	_shader->DrawIndexed(0, 0, _mesh->GetIndexBuffer()->GetCount(), 0, 0);
//}


/// <summary>
/// Diffusemap
/// </summary>
void MeshRenderer::Update()
{
	//if (_mesh == nullptr || _material == nullptr)
	//	return;
	//auto shader = _material->GetShader();
	//if (shader == nullptr) return;

	//_material->Update();

	//auto world = GetTransform()->GetWorldMatrix();
	//RENDER->PushTransformData(TransformDesc{ world });

	//uint32 stride = _mesh->GetVertexBuffer()->GetStride();
	//uint32 offset = _mesh->GetVertexBuffer()->GetOffset();

	//DC->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
	//DC->IASetIndexBuffer(_mesh->GetIndexBuffer()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	//shader->DrawIndexed(0, 0, _mesh->GetIndexBuffer()->GetCount(), 0, 0);
}

// 메시를 인스턴싱하여 렌더링하는 함수
void MeshRenderer::RenderInstancing(shared_ptr<class InstancingBuffer>& buffer)
{
	// 메시나 재질이 설정되지 않았으면 함수를 종료합니다.
	if (_mesh == nullptr || _material == nullptr) return;

	// 재질로부터 셰이더 객체를 가져옵니다.
	auto shader = _material->GetShader();
	if (shader == nullptr) return; // 셰이더가 없다면 함수 종료

	// 카메라의 뷰 매트릭스와 프로젝션 매트릭스를 셰이더에 전역 데이터로 전달합니다.
	shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

	// 현재 씬의 광원 객체를 가져와 셰이더에 광원 데이터를 전달합니다.
	auto lightObj = SCENE->GetCurrentScene()->GetLight();
	if (lightObj == nullptr) return; // 광원 객체가 없다면 함수 종료
	shader->PushLightData(lightObj->GetLight()->GetLightDesc());

	// 재질을 업데이트합니다. 재질에 설정된 모든 유니폼 변수들이 GPU로 전송됩니다.
	_material->Update();

	// 메시의 정점 버퍼와 인덱스 버퍼 데이터를 GPU로 전송합니다.
	_mesh->GetVertexBuffer()->PushData();
	_mesh->GetIndexBuffer()->PushData();

	// 인스턴싱 버퍼의 데이터를 GPU로 전송합니다. 인스턴싱 데이터에는 각 인스턴스의 변환 정보 등이 포함됩니다.
	buffer->PushData();

	// 인스턴스화된 드로우 콜을 수행하여 메시를 렌더링합니다. 이때, 인스턴싱 버퍼에 저장된 각 인스턴스 정보를 사용해
	// 동일한 메시를 여러 번 그리되, 각각 다른 변환을 적용하여 그립니다.
	shader->DrawIndexedInstanced(0, _pass, _mesh->GetIndexBuffer()->GetCount(), buffer->GetCount());
}


InstanceID MeshRenderer::GetInstanceID()
{
	return make_pair((uint64)_mesh.get(), (uint64)_material.get());
}
