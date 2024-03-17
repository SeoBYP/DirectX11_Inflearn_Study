#include "pch.h" // 사전 컴파일된 헤더 포함
#include "SceneManager.h" // SceneManager 클래스 헤더 포함
#include "Scene.h" // Scene 클래스 헤더 포함
#include "GameObject.h" // GameObject 클래스 헤더 포함
#include "Camera.h" // Camera 클래스 헤더 포함
#include "MeshRenderer.h" // MeshRenderer 클래스 헤더 포함
#include "ResourceManager.h" // ResourceManager 클래스 헤더 포함
#include "Game.h" // Game 클래스 헤더 포함
#include "Mesh.h" // Mesh 클래스 헤더 포함
#include "Animator.h" // Animator 클래스 헤더 포함

// SceneManager 클래스의 생성자입니다. Graphics 객체를 인자로 받아 멤버 변수를 초기화합니다.
SceneManager::SceneManager(shared_ptr<Graphics> graphics) : _graphics(graphics)
{
}

// 초기화 함수입니다. 활성화된 장면이 없으면 함수를 종료합니다.
// 활성화된 장면이 있으면, 해당 장면의 Awake와 Start 함수를 호출합니다.
void SceneManager::Init()
{
	if (_activeScene == nullptr) {
		return;
	}

	_activeScene->Awake();
	_activeScene->Start();
}

// 매 프레임마다 호출되는 업데이트 함수입니다.
// 활성화된 장면이 없으면 함수를 종료합니다.
// 활성화된 장면이 있으면, 해당 장면의 FixedUpdate, Update, LateUpdate 함수를 차례로 호출합니다.
void SceneManager::Update()
{
	if (_activeScene == nullptr) {
		return;
	}

	_activeScene->FixedUpdate();
	_activeScene->Update();
	_activeScene->LateUpdate();
}

// 새로운 장면을 로드하는 함수입니다. 장면 이름을 인자로 받습니다.
// 여기서는 장면 이름을 사용하지 않고, 단순히 LoadTestScene 함수를 호출하여 테스트 장면을 로드합니다.
void SceneManager::LoadScene(wstring sceneName)
{
	// 장면 리소스 관련 로직은 여기에 구현될 수 있습니다.

	_activeScene = LoadTestScene(); // 테스트 장면을 로드합니다.
	Init(); // 로드된 장면을 초기화합니다.
}

//// 테스트 장면을 로드하는 내부 함수입니다.
//// 실제 게임에서는 다양한 장면을 로드하기 위해 이 함수를 수정하거나 다른 함수를 추가할 수 있습니다.
//shared_ptr<Scene> SceneManager::LoadTestScene()
//{
//	shared_ptr<Scene> scene = make_shared<Scene>(); // 새로운 Scene 객체를 생성합니다.
//
//	// 카메라 객체를 생성하고 장면에 추가하는 코드 블록입니다.
//	{
//		shared_ptr<GameObject> camera = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
//		{
//			camera->GetOrAddTransform(); // 카메라의 Transform 컴포넌트를 추가합니다.
//			camera->AddComponent(make_shared<Camera>()); // 카메라 컴포넌트를 추가합니다.
//			scene->AddGameObject(camera); // 생성된 카메라 객체를 장면에 추가합니다.
//		}
//	}
//
//	// 몬스터 객체를 생성하고 장면에 추가하는 코드 블록입니다.
//		// 아래에는 몬스터 객체를 두 번 생성하는 예시가 있습니다. 실제 게임에서는 다양한 종류의 객체를 생성할 수 있습니다.
//	{
//		shared_ptr<GameObject> monster = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
//		monster->GetOrAddTransform()->SetWorldPosition(Vec3{ 2.0f,2.0f,0 }); // 몬스터의 위치를 설정합니다.
//		{
//			monster->GetOrAddTransform(); // 몬스터의 Transform 컴포넌트를 추가합니다.
//			auto meshRenderer = make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext());
//			monster->AddComponent(meshRenderer); // MeshRenderer 컴포넌트를 추가합니다.
//
//			// 재질과 메시를 설정합니다. 이 부분은 향후 수정될 수 있습니다.
//			auto material = RESOURCES->Get<Material>(L"Default"); // 기본 재질을 가져옵니다.
//			meshRenderer->SetMaterial(material); // 재질을 설정합니다.
//
//			auto mesh = RESOURCES->Get<Mesh>(L"Rectangle"); // 사각형 메시를 가져옵니다.
//			meshRenderer->SetMesh(mesh); // 메시를 설정합니다.
//		}
//		{
//			auto animator = make_shared<Animator>(); // Animator 컴포넌트를 생성합니다.
//			monster->AddComponent(animator); // Animator 컴포넌트를 추가합니다.
//			auto anim = RESOURCES->Get<Animation>(L"SnakeAnim"); // 애니메이션 리소스를 가져옵니다.
//			animator->SetAnimation(anim); // 애니메이션을 설정합니다.
//		}
//		scene->AddGameObject(monster); // 생성된 몬스터 객체를 장면에 추가합니다.
//	}
//
//	// 몬스터 객체를 생성하고 장면에 추가하는 코드 블록입니다.
//		// 아래에는 몬스터 객체를 두 번 생성하는 예시가 있습니다. 실제 게임에서는 다양한 종류의 객체를 생성할 수 있습니다.
//	{
//		shared_ptr<GameObject> monster = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
//		monster->GetOrAddTransform()->SetWorldPosition(Vec3{ 2.0f,2.0f,0 }); // 몬스터의 위치를 설정합니다.
//		{
//			monster->GetOrAddTransform(); // 몬스터의 Transform 컴포넌트를 추가합니다.
//			auto meshRenderer = make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext());
//			monster->AddComponent(meshRenderer); // MeshRenderer 컴포넌트를 추가합니다.
//
//			// 재질과 메시를 설정합니다. 이 부분은 향후 수정될 수 있습니다.
//			auto material = RESOURCES->Get<Material>(L"Default"); // 기본 재질을 가져옵니다.
//			meshRenderer->SetMaterial(material); // 재질을 설정합니다.
//
//			auto mesh = RESOURCES->Get<Mesh>(L"Rectangle"); // 사각형 메시를 가져옵니다.
//			meshRenderer->SetMesh(mesh); // 메시를 설정합니다.
//		}
//		{
//			auto animator = make_shared<Animator>(); // Animator 컴포넌트를 생성합니다.
//			monster->AddComponent(animator); // Animator 컴포넌트를 추가합니다.
//			auto anim = RESOURCES->Get<Animation>(L"SnakeAnim"); // 애니메이션 리소스를 가져옵니다.
//			animator->SetAnimation(anim); // 애니메이션을 설정합니다.
//		}
//		scene->AddGameObject(monster); // 생성된 몬스터 객체를 장면에 추가합니다.
//	}
//
//	return scene; // 설정이 완료된 장면 객체를 반환합니다.
//}
std::shared_ptr<Scene> SceneManager::LoadTestScene()
{
	shared_ptr<Scene> scene = make_shared<Scene>();


	// Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
		{
			camera->GetOrAddTransform();
			camera->AddComponent(make_shared<Camera>());
			scene->AddGameObject(camera);
		}
	}

	// Monster
	{
		shared_ptr<GameObject> monster = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
		monster->GetOrAddTransform()->SetWorldPosition(Vec3{ 1.f, 1.f, 0.f });
		{
			monster->GetOrAddTransform();
			auto meshRenderer = make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext());
			monster->AddComponent(meshRenderer);
			auto material = RESOURCES->Get<Material>(L"Default");
			meshRenderer->SetMaterial(material);
			auto mesh = RESOURCES->Get<Mesh>(L"Rectangle");
			meshRenderer->SetMesh(mesh);
		}
		{
			auto animator = make_shared<Animator>();
			monster->AddComponent(animator);
			auto anim = RESOURCES->Get<Animation>(L"SnakeAnim");
			animator->SetAnimation(anim);
		}
		scene->AddGameObject(monster);
	}

	// Monster
	{
		shared_ptr<GameObject> monster = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
		{
			monster->GetOrAddTransform();
			auto meshRenderer = make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext());
			monster->AddComponent(meshRenderer);
			auto material = RESOURCES->Get<Material>(L"Default");
			meshRenderer->SetMaterial(material);
			auto mesh = RESOURCES->Get<Mesh>(L"Rectangle");
			meshRenderer->SetMesh(mesh);
		}
		{
			auto animator = make_shared<Animator>();
			monster->AddComponent(animator);
			auto anim = RESOURCES->Get<Animation>(L"SnakeAnim");
			animator->SetAnimation(anim);
		}
		scene->AddGameObject(monster);
	}

	return scene;
}
