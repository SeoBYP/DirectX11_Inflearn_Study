#pragma once
#include "Component.h"

class Mesh;
class Shader;
class Material;

class MeshRenderer : public Component
{
	using Super = Component;
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	virtual void Update() override;

	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetMaterial(shared_ptr<Material> material) { _material = material; }

	void SetTexture(shared_ptr<Texture> texture) { }
	void SetShader(shared_ptr<Shader> shader) { }
	void SetPass(uint8 pass) { _pass = pass; }

	void RenderInstancing(shared_ptr<class InstancingBuffer>& buffer);
	InstanceID GetInstanceID();

	//void SetTexture(shared_ptr<Texture> texture) { _texture = texture; }
	//void SetShader(shared_ptr<Shader> shader) { _shader = shader; }

private:
	shared_ptr<Mesh> _mesh;
	shared_ptr<Material> _material;
	uint8 _pass = 0;
	//shared_ptr<Texture> _texture;
	//shared_ptr<Shader> _shader;
};

