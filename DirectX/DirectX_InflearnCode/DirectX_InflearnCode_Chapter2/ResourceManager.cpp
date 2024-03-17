#include "pch.h" // 프로젝트의 사전 컴파일 헤더 파일
#include "ResourceManager.h" // ResourceManager 클래스 정의 포함
#include "Texture.h" // Texture 클래스 정의 포함
#include "Mesh.h" // Mesh 클래스 정의 포함
#include "Shader.h" // Shader 클래스 정의 포함
#include "Material.h" // Material 클래스 정의 포함
#include "Animation.h" // Animation 클래스 정의 포함

// ResourceManager 클래스 생성자
ResourceManager::ResourceManager(ComPtr<ID3D11Device> device) :
	_device(device) // DirectX 11 디바이스를 멤버 변수에 할당
{
}

// 자원 관리자 초기화 함수
void ResourceManager::Init()
{
	CreateDefaultTexture(); // 기본 텍스처 생성
	CreateDefaultMesh(); // 기본 메시 생성
	CreateDefaultShader(); // 기본 쉐이더 생성
	CreateDefaultMaterial(); // 기본 재료 생성
	CreateDefaultAnimation(); // 기본 애니메이션 생성
}

// 기본 텍스처 생성 함수
void ResourceManager::CreateDefaultTexture()
{
	// Unity 로고 텍스처 생성
	{
		auto texture = make_shared<Texture>(_device); // Texture 객체 생성
		texture->SetName(L"UnityLogo"); // 텍스처 이름 설정
		texture->Create(L"UnityLogo.png"); // 텍스처 파일 로드
		Add(texture->GetName(), texture); // ResourceManager에 텍스처 추가
	}

	// Snake 이미지 텍스처 생성
	{
		auto texture = make_shared<Texture>(_device); // Texture 객체 생성
		texture->SetName(L"Snake"); // 텍스처 이름 설정
		texture->Create(L"Snake.bmp"); // 텍스처 파일 로드
		Add(texture->GetName(), texture); // ResourceManager에 텍스처 추가
	}
}

// 기본 메시 생성 함수
void ResourceManager::CreateDefaultMesh()
{
	// Rectangle 메시 생성
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>(_device); // Mesh 객체 생성
		mesh->SetName(L"Rectangle"); // 메시 이름 설정
		mesh->CreateDefaultRectangle(); // 기본 사각형 메시 생성
		Add(mesh->GetName(), mesh); // ResourceManager에 메시 추가
	}
}

// 기본 쉐이더 생성 함수
void ResourceManager::CreateDefaultShader()
{
	auto vertexShader = make_shared<VertexShader>(_device); // VertexShader 객체 생성
	vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0"); // 정점 쉐이더 로드

	auto inputLayout = make_shared<InputLayout>(_device); // InputLayout 객체 생성
	inputLayout->Create(VertexTextureData::descs, vertexShader->GetBlob()); // 입력 레이아웃 생성

	auto pixelShader = make_shared<PixelShader>(_device); // PixelShader 객체 생성
	pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0"); // 픽셀 쉐이더 로드

	// Shader 객체 생성 및 설정
	shared_ptr<Shader> shader = make_shared<Shader>();
	shader->SetName(L"Default"); // 쉐이더 이름 설정
	shader->_vertexShader = vertexShader; // 정점 쉐이더 설정
	shader->_inputLayout = inputLayout; // 입력 레이아웃 설정
	shader->_pixelShader = pixelShader; // 픽셀 쉐이더 설정

	Add(shader->GetName(), shader); // ResourceManager에 쉐이더 추가
}

// 기본 재료 생성 함수
void ResourceManager::CreateDefaultMaterial()
{
	shared_ptr<Material> material = make_shared<Material>(); // Material 객체 생성
	material->SetName(L"Default"); // 재료 이름 설정
	material->SetShader(Get<Shader>(L"Default")); // Default 쉐이더 설정
	material->SetTexture(Get<Texture>(L"UnityLogo")); // UnityLogo 텍스처 설정
	Add(material->GetName(), material); // ResourceManager에 재료 추가
}

// 기본 애니메이션 생성 함수
void ResourceManager::CreateDefaultAnimation()
{
	shared_ptr<Animation> animation = make_shared<Animation>(); // Animation 객체 생성

	animation->SetName(L"SnakeAnim"); // 애니메이션 이름 설정
	animation->SetTexture(Get<Texture>(L"Snake")); // Snake 텍스처 설정
	animation->SetLoop(true); // 반복 설정

	// 애니메이션 키프레임 추가
	animation->AddKeyframe(Keyframe{ Vec2{0.f, 0.f}, Vec2{100.f, 100.f}, 0.1f });
	animation->AddKeyframe(Keyframe{ Vec2{100.f, 0.f}, Vec2{100.f, 100.f}, 0.1f });
	animation->AddKeyframe(Keyframe{ Vec2{200.f, 0.f}, Vec2{100.f, 100.f}, 0.1f });
	animation->AddKeyframe(Keyframe{ Vec2{300.f, 0.f}, Vec2{100.f, 100.f}, 0.1f });

	Add(animation->GetName(), animation); // ResourceManager에 애니메이션 추가

	// 애니메이션 저장 및 로드 예제 (XML, JSON 등을 사용할 수 있음)
	animation->Save(L"TestAnim.xml"); // 애니메이션 상태 저장

	shared_ptr<Animation> anim2 = make_shared<Animation>(); // 새 Animation 객체 생성
	anim2->Load(L"TestAnim.xml"); // 저장된 애니메이션 상태 로드
}
