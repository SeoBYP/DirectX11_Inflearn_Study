#pragma once

class Scene;

// SceneManager 클래스는 게임의 여러 장면(Scene)을 관리하는 역할을 합니다.
class SceneManager
{
public:
	// 생성자에서는 그래픽 관련 설정을 초기화합니다.
	SceneManager(shared_ptr<Graphics> graphics);

	// 초기화 함수, 현재 활성화된 장면을 초기화합니다.
	void Init();
	// 업데이트 함수, 현재 활성화된 장면을 업데이트합니다.
	void Update();
	// 새로운 장면을 로드하는 함수, 장면 이름을 기반으로 장면을 로드합니다.
	void LoadScene(wstring sceneName);

public:
	// 현재 활성화된 장면을 가져오는 함수
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }

private:
	// 테스트 장면을 로드하는 내부 함수, 여기서 장면 설정을 진행합니다.
	shared_ptr<Scene> LoadTestScene();

private:
	shared_ptr<Graphics> _graphics; // 그래픽스 관련 설정을 저장하는 변수

private:
	shared_ptr<Scene> _activeScene; // 현재 활성화된 장면
};