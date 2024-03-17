#pragma once
#include "Component.h"

class Mesh;
class Material;
#include "Material.h"
#include "Shader.h"

// MeshRenderer 클래스는 게임 오브젝트의 메시를 렌더링하는 컴포넌트입니다.
class MeshRenderer : public Component
{
	using Super = Component; // 부모 클래스인 Component에 대한 별칭을 설정합니다.
public:
	// 생성자: DirectX 디바이스와 디바이스 컨텍스트를 인자로 받습니다.
	MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	virtual ~MeshRenderer(); // 소멸자

	// 머티리얼 설정 메소드
	void SetMaterial(shared_ptr<Material> material) { _material = material; }
	// 셰이더 설정 메소드
	void SetShader(shared_ptr<Shader> shader) { _material->SetShader(shader); }
	// 메시 설정 메소드
	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	// 텍스처 설정 메소드
	void SetTexture(shared_ptr<Texture> texture) { _material->SetTexture(texture); }

	// 머티리얼 가져오기 메소드
	shared_ptr<Material> GetMaterial() { return _material; }
	// 버텍스 셰이더 가져오기 메소드
	shared_ptr<VertexShader> GetVertexShader() { return GetMaterial()->GetShader()->GetVertexShader(); }
	// 인풋 레이아웃 가져오기 메소드
	shared_ptr<InputLayout> GetInputLayout() { return GetMaterial()->GetShader()->GetInputLayout(); }
	// 픽셀 셰이더 가져오기 메소드
	shared_ptr<PixelShader> GetPixelShader() { return GetMaterial()->GetShader()->GetPixelShader(); }

	// 메시 가져오기 메소드
	shared_ptr<Mesh> GetMesh() { return _mesh; }
	// 텍스처 가져오기 메소드
	shared_ptr<Texture> GetTexture() { return _material->GetTexture(); }

private:
	ComPtr<ID3D11Device> _device; // DirectX 디바이스 인스턴스

	friend class RenderManager; // RenderManager 클래스에서 MeshRenderer의 private 멤버에 접근할 수 있도록 허용

	// 메시와 머티리얼 정보를 저장하는 멤버 변수
	shared_ptr<Mesh> _mesh; // 메시 정보
	shared_ptr<Material> _material; // 머티리얼 정보
};
