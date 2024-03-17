#pragma once
#include "Component.h"

// 카메라 투영 타입을 정의하는 열거형
enum class ProjectionType
{
	Perspertive, // 원근 투영
	Orthographic, // 직교 투영
};

// Component 클래스를 상속받는 Camera 클래스 정의
class Camera : public Component
{
	using Super = Component; // 부모 클래스를 Super로 별칭 정의

public:
	Camera(); // 생성자
	virtual ~Camera(); // 가상 소멸자

	virtual void Update() override; // 컴포넌트의 Update 메소드 오버라이드

	// 투영 타입 설정 및 반환
	void SetProjectionType(ProjectionType type) { _type = type; }
	ProjectionType GetProjectionType() { return _type; }

	void UpdateMatrix(); // 뷰 및 투영 매트릭스 업데이트

	// 카메라 설정 관련 메소드들
	void SetNear(float value) { _near = value; }
	void SetFar(float value) { _far = value; }
	void SetFov(float value) { _fov = value; } // Field of View
	void SetWidth(float value) { _width = value; }
	void SetHeight(float value) { _height = value; }

	// 매트릭스 반환 메소드들
	Matrix& GetViewMatrix() { return _matView; }
	Matrix& GetProjectionMatrix() { return _matProjection; }

	// 너비와 높이 반환 메소드들
	float GetWidth() { return _width; }
	float GetHeight() { return _height; }

private:
	ProjectionType _type = ProjectionType::Perspertive; // 투영 타입
	Matrix _matView = Matrix::Identity; // 뷰 매트릭스
	Matrix _matProjection = Matrix::Identity; // 투영 매트릭스

	// 카메라 관련 설정 변수들
	float _near = 1.0f;
	float _far = 1000.f;
	float _fov = XM_PI / 4.0f; // 기본적으로 45도 설정
	float _width = 0.f;
	float _height = 0.f;

public:
	static Matrix S_MatView; // 정적 뷰 매트릭스
	static Matrix S_MatProjection; // 정적 투영 매트릭스

public:
	void SortGameObject(); // 게임 오브젝트 정렬
	void Render_Forward(); // 포워드 렌더링 실행

	// 렌더링 시 셀링 마스크 관련 메소드
	void SetCullingMaskLayerOnOff(uint8 layer, bool on) {
		if (on)
			_cullingMask |= (1 << layer);
		else
			_cullingMask &= ~(1 << layer);
	}

	void SetCullingMaskAll() { SetCullingMask(UINT32_MAX); } // 모든 레이어를 켬
	void SetCullingMask(uint32 mask) { _cullingMask = mask; } // 셀링 마스크 설정
	bool IsCulled(uint8 layer) { return (_cullingMask & (1 << layer)) != 0; }; // 특정 레이어가 컬링됐는지 확인

private:
	uint32 _cullingMask = 0; // 렌더링할 레이어를 결정하는 비트 마스크
	vector<shared_ptr<GameObject>> _vecForward; // 포워드 렌더링할 게임 오브젝트 목록
};
