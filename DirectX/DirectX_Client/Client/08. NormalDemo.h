#pragma once
#include "IExecute.h"
#include "Geometry.h"
#include "GameObject.h"

class NormalDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	shared_ptr<Shader> _shader;

	//Object
	shared_ptr<Geometry<VertexTextureNormalData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;

	Vec3 _translation = Vec3(0.f, 0.f, 0.f);

	Matrix _world = Matrix::Identity;

	//Camera
	shared_ptr<GameObject> _camera;

	shared_ptr<Texture> _heightMap;
	shared_ptr<Texture> _texture;

	Vec3 _lightDir = Vec3{ 0.f,0.f,1.f };
};

