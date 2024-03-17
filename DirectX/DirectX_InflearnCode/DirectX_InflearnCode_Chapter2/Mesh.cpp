#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(ComPtr<ID3D11Device> device) : Super(ResourceType::Mesh),
	_device(device)
{

}

Mesh::~Mesh()
{
}

void Mesh::CreateDefaultRectangle()
{
	_geometry = make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateRectangle(_geometry); // 사각형 모양의 기하 구조 생성

	_vertexBuffer = make_shared<VertexBuffer>(_device);
	_vertexBuffer->Create(_geometry->GetVertices()); // 정점 버퍼 생성

	_indexBuffer = make_shared<IndexBuffer>(_device);
	_indexBuffer->Create(_geometry->GetIndices()); // 인덱스 버퍼 생성
}
