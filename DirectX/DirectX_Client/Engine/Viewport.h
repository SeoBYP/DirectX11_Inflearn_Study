#pragma once

// 뷰포트를 정의하는 클래스
class Viewport
{
public:
	// 기본 생성자: 비어있는 뷰포트를 생성
	Viewport();

	// 파라미터를 받는 생성자: 뷰포트의 크기와 위치, 깊이 범위를 설정
	Viewport(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 0);

	// 소멸자
	~Viewport();

	// 뷰포트를 Direct3D에 설정
	void RSSetViewport();

	// 뷰포트의 속성을 설정
	void Set(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 0);

	// 뷰포트의 너비를 가져옴
	float GetWidth() { return _vp.Width; }

	// 뷰포트의 높이를 가져옴
	float GetHeight() { return _vp.Height; }

	// 3D 좌표를 2D 화면 좌표로 프로젝션
	Vec3 Project(const Vec3& pos, const Matrix& W, const Matrix& V, const Matrix& P);

	// 2D 화면 좌표를 3D 좌표로 역프로젝션
	Vec3 Unproject(const Vec3& pos, const Matrix& W, const Matrix& V, const Matrix& P);

private:
	// Direct3D 뷰포트 구조체
	D3D11_VIEWPORT _vp;
};
