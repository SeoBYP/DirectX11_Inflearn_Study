#include "pch.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "GameObject.h"
#include "Game.h"
#include "Pipeline.h"
#include "Mesh.h"

// 생성자 구현: MeshRenderer 컴포넌트를 초기화합니다.
MeshRenderer::MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) 
	: Super(ComponentType::MeshRenderer), _device(device)
{

}

// 소멸자 구현
MeshRenderer::~MeshRenderer()
{

}