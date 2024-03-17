#include "pch.h"
#include "Viewport.h"

// 기본 생성자: 기본 크기로 뷰포트 설정
Viewport::Viewport()
{
	Set(800, 600); // 기본 크기 800x600으로 설정
}

// 매개변수를 받는 생성자: 사용자 지정 크기와 위치, 깊이 범위로 뷰포트 설정
Viewport::Viewport(float width, float height, float x, float y, float minDepth, float maxDepth)
{
	Set(width, height, x, y, minDepth, maxDepth);
}

// 소멸자: 특별한 정리 작업 없음
Viewport::~Viewport()
{
}

// Direct3D에 뷰포트 설정을 적용하는 함수
void Viewport::RSSetViewport()
{
	DC->RSSetViewports(1, &_vp); // 설정된 뷰포트를 렌더링 파이프라인에 적용
}

// 뷰포트 속성 설정 함수
void Viewport::Set(float width, float height, float x, float y, float minDepth, float maxDepth)
{
	_vp.TopLeftX = x;
	_vp.TopLeftY = y;
	_vp.Width = width;
	_vp.Height = height;
	_vp.MinDepth = minDepth;
	_vp.MaxDepth = maxDepth;
}

// 3D 좌표를 2D 스크린 좌표로 변환하는 함수
Vec3 Viewport::Project(const Vec3& pos, const Matrix& W, const Matrix& V, const Matrix& P)
{
	Matrix wvp = W * V * P; // 월드, 뷰, 프로젝션 행렬을 결합

	Vec3 p = Vec3::Transform(pos, wvp); // 좌표 변환

	// 스크린 좌표로 변환
	p.x = (p.x + 1.0f) * (_vp.Width / 2) + _vp.TopLeftX;
	p.y = (-p.y + 1.0f) * (_vp.Height / 2) + _vp.TopLeftY;
	p.z = p.z * (_vp.MaxDepth - _vp.MinDepth) + _vp.MinDepth;

	return p;
}

// 2D 스크린 좌표를 3D 좌표로 역변환하는 함수
Vec3 Viewport::Unproject(const Vec3& pos, const Matrix& W, const Matrix& V, const Matrix& P)
{
	Vec3 p = pos;

	// 스크린 좌표를 정규화된 좌표로 변환
	p.x = 2.f * (p.x - _vp.TopLeftX) / _vp.Width - 1.f;
	p.y = -2.f * (p.y - _vp.TopLeftY) / _vp.Height + 1.f;
	p.z = ((p.z - _vp.MinDepth) / (_vp.MaxDepth - _vp.MinDepth));

	Matrix wvp = W * V * P;
	Matrix wvpInv = wvp.Invert(); // 변환 행렬의 역행렬 계산

	p = Vec3::Transform(p, wvpInv); // 역변환
	return p;
}
