#pragma once
#include "ResourceBase.h"
class Shader :
    public ResourceBase
{
	using Super = ResourceBase;
public:
	Shader();
	virtual ~Shader();

	shared_ptr<InputLayout> GetInputLayout() { return _inputLayout; }
	shared_ptr<VertexShader> GetVertexShader() { return _vertexShader; }
	shared_ptr<PixelShader> GetPixelShader() { return _pixelShader; }

private:
	friend class ResourceManager;

	// ½¦ÀÌ´õ ¹× ·»´õ¸µ »óÅÂ(Material)
	shared_ptr<InputLayout> _inputLayout;
	shared_ptr<VertexShader> _vertexShader; // Á¤Á¡ ½¦ÀÌ´õ
	shared_ptr<PixelShader> _pixelShader; // ÇÈ¼¿ ½¦ÀÌ´õ
};

