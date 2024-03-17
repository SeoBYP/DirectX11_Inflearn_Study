#pragma once
#include "Graphics.h"
#include "Geometry.h"
#include "VertexBuffer.h"
class GeometryHelper
{
public:
	// 사각형, 큐브, 구, 그리드 생성 메서드 선언.
	// 각 형태에 대해 다양한 정점 데이터 유형을 사용할 수 있음을 보여줌.
	static void CreateQuad(shared_ptr<Geometry<VertexColorData>> geometry, Color color);

	static void CreateQuad(shared_ptr<Geometry<VertexTextureData>> geometry);
	static void CreateCube(shared_ptr<Geometry<VertexTextureData>> geometry);
	static void CreateSphere(shared_ptr<Geometry<VertexTextureData>> geometry);
	static void CreateGrid(shared_ptr<Geometry<VertexTextureData>> geometry, int32 sizeX, int32 sizeZ);


	static void CreateQuad(shared_ptr<Geometry<VertexTextureNormalData>> geometry);
	static void CreateCube(shared_ptr<Geometry<VertexTextureNormalData>> geometry);
	static void CreateSphere(shared_ptr<Geometry<VertexTextureNormalData>> geometry);
	static void CreateGrid(shared_ptr<Geometry<VertexTextureNormalData>> geometry, int32 sizeX, int32 sizeZ);	

	static void CreateQuad(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);
	static void CreateCube(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);
	static void CreateSphere(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry);
	static void CreateGrid(shared_ptr<Geometry<VertexTextureNormalTangentData>> geometry, int32 sizeX, int32 sizeZ);
};

