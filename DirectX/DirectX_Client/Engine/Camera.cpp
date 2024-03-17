#include "pch.h"
#include "Camera.h"
#include "Scene.h"

// 정적 멤버 변수 초기화
Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;

// 카메라 생성자
Camera::Camera() : Super(ComponentType::Camera)
{
	// 게임 화면의 너비와 높이로 카메라의 초기 크기 설정
	_width = static_cast<float>(GAME->GetGameDesc().width);
	_height = static_cast<float>(GAME->GetGameDesc().height);
}

// 카메라 소멸자
Camera::~Camera()
{
}

// 매 프레임마다 호출되는 업데이트 함수
void Camera::Update()
{
	UpdateMatrix(); // 매트릭스 업데이트
	// 전역 데이터로 뷰 매트릭스와 투영 매트릭스를 렌더러에 푸시
	//RENDER->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);
}

// 카메라의 뷰 및 투영 매트릭스를 업데이트하는 함수
void Camera::UpdateMatrix()
{
	Vec3 eyePosition = GetTransform()->GetWorldPosition(); // 카메라의 위치
	Vec3 focusPosition = eyePosition + GetTransform()->GetLook(); // 카메라가 바라보는 방향
	Vec3 upDirection = GetTransform()->Getup(); // 카메라의 상단 방향

	// 뷰 매트릭스 계산
	_matView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);

	// 투영 매트릭스 계산
	if (_type == ProjectionType::Perspertive)
	{
		// 원근 투영을 사용하는 경우
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, _width / _height, _near, _far);
	}
	else
	{
		// 직교 투영을 사용하는 경우
		_matProjection = ::XMMatrixOrthographicLH(_width, _height, _near, _far);
	}
}

// 게임 오브젝트를 정렬하는 함수
void Camera::SortGameObject()
{
	shared_ptr<Scene> scene = CUR_SCENE; // 현재 씬
	unordered_set<shared_ptr<GameObject>>& gameObjects = scene->GetObjects(); // 씬에 있는 게임 오브젝트들

	_vecForward.clear(); // 정렬된 오브젝트 목록 초기화

	for (auto& gameobject : gameObjects) {
		if (IsCulled(gameobject->GetLayerIndex())) // 레이어에 따라 컬링 여부 결정
			continue;

		// 렌더링 가능한 컴포넌트가 없는 오브젝트는 무시
		if (gameobject->GetMeshRenderer() == nullptr
			&& gameobject->GetModelRenderer() == nullptr
			&& gameobject->GetModelAnimator() == nullptr)
			continue;

		_vecForward.push_back(gameobject); // 렌더링 대상 목록에 추가
	}
}

// 포워드 렌더링을 실행하는 함수
void Camera::Render_Forward()
{
	S_MatView = _matView; // 정적 뷰 매트릭스 업데이트
	S_MatProjection = _matProjection; // 정적 투영 매트릭스 업데이트

	// 인스턴싱 매니저를 통해 렌더링 실행
	GET_SINGLE(InstancingManager)->Render(_vecForward);
}
