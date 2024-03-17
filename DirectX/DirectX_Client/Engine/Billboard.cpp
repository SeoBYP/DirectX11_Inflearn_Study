#include "pch.h"
#include "Billboard.h"
#include "Material.h"
#include "Camera.h"

Billboard::Billboard() : Super(ComponentType::Billboard)
{
	// 최대 빌보드 수에 따라 버텍스와 인덱스 버퍼 크기를 결정
	int32 vertexCount = MAX_BILLBOARD_COUNT * 4; // 각 빌보드당 4개의 버텍스
	int32 indexCount = MAX_BILLBOARD_COUNT * 6; // 각 빌보드당 6개의 인덱스 (2개의 삼각형)

	_vertices.resize(vertexCount);
	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_vertices, 0, true); // 동적 업데이트를 위해 cpuWrite 옵션을 true로 설정

	_indices.resize(indexCount);

	// 각 빌보드를 위한 인덱스 설정
	for (int32 i = 0; i < MAX_BILLBOARD_COUNT; i++)
	{
		_indices[i * 6 + 0] = i * 4 + 0;
		_indices[i * 6 + 1] = i * 4 + 1;
		_indices[i * 6 + 2] = i * 4 + 2;
		_indices[i * 6 + 3] = i * 4 + 2;
		_indices[i * 6 + 4] = i * 4 + 1;
		_indices[i * 6 + 5] = i * 4 + 3;
	}

	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_indices); // 인덱스 버퍼 생성
}

Billboard::~Billboard()
{
}

void Billboard::Update()
{
	// 빌보드의 수가 변경되면 버텍스 버퍼를 업데이트
	if (_drawCount != _prevCount)
	{
		_prevCount = _drawCount;

		D3D11_MAPPED_SUBRESOURCE subResource;
		DC->Map(_vertexBuffer->GetComPtr().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		{
			memcpy(subResource.pData, _vertices.data(), sizeof(VertexBillboard) * _vertices.size());
		}
		DC->Unmap(_vertexBuffer->GetComPtr().Get(), 0);
	}

	auto shader = _material->GetShader();

	// Transform
	auto world = GetTransform()->GetWorldMatrix();
	shader->PushTransformData(TransformDesc{ world });

	// GlobalData
	shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

	// Light
	_material->Update();

	// IA
	_vertexBuffer->PushData();
	_indexBuffer->PushData();

	shader->DrawIndexed(0, _pass, _drawCount * 6); // 빌보드 그리기
}

void Billboard::Add(Vec3 position, Vec2 scale)
{
	// 새로운 빌보드를 추가할 때 각 버텍스에 위치, UV 좌표, 크기 정보를 설정
	_vertices[_drawCount * 4 + 0].position = position;
	_vertices[_drawCount * 4 + 1].position = position;
	_vertices[_drawCount * 4 + 2].position = position;
	_vertices[_drawCount * 4 + 3].position = position;

	_vertices[_drawCount * 4 + 0].uv = Vec2(0, 1);
	_vertices[_drawCount * 4 + 1].uv = Vec2(0, 0);
	_vertices[_drawCount * 4 + 2].uv = Vec2(1, 1);
	_vertices[_drawCount * 4 + 3].uv = Vec2(1, 0);

	_vertices[_drawCount * 4 + 0].scale = scale;
	_vertices[_drawCount * 4 + 1].scale = scale;
	_vertices[_drawCount * 4 + 2].scale = scale;
	_vertices[_drawCount * 4 + 3].scale = scale;

	_drawCount++; // 빌보드 수 증가
}
