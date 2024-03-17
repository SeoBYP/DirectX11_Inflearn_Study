#pragma once

#include "Component.h" // 컴포넌트 클래스 포함

class Transform : public Component
{
	using Super = Component;
public:
	// 생성자와 소멸자
	Transform();
	~Transform();

	// 컴포넌트 기본 함수들
	virtual void Awake() override;
	virtual void Update() override;

	// 트랜스폼 업데이트 함수
	void UpdateTransform();

	// 로컬 변환 관련 함수들
	Vec3 GetLocalScale() { return _localScale; }
	void SetLocalScale(const Vec3& localScale) { _localScale = localScale; UpdateTransform(); }
	Vec3 GetLocalRotation() { return _localRotation; }
	void SetLocalRotation(const Vec3& localRotation) { _localRotation = localRotation; UpdateTransform(); }
	Vec3 GetLocalPosition() { return _localPosition; }
	void SetLocalPosition(const Vec3& localPosition) { _localPosition = localPosition; UpdateTransform(); }

	// 월드 변환 관련 함수들
	Vec3 GetWorldScale() { return _scale; }
	void SetWorldScale(const Vec3& worldScale);
	Vec3 GetWorldRotation() { return _rotation; }
	void SetWorldRotation(const Vec3& worldRotation);
	Vec3 GetWorldPosition() { return _position; }
	void SetWorldPosition(const Vec3& worldPosition);

	Vec3 GetRight() { return _matWorld.Right(); }
	Vec3 Getup() { return _matWorld.Up(); }
	Vec3 GetLook() { return _matWorld.Backward(); }

	// 월드 행렬 반환
	Matrix GetWorldMatrix() { return _matWorld; }

	// 부모-자식 관계 관련 함수들
	bool HasParent() { return _parent != nullptr; }
	shared_ptr<Transform> GetParent() { return _parent; }
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	vector<shared_ptr<Transform>> GetChildren() { return _children; }
	void AddChildren(shared_ptr<Transform> child) { _children.push_back(child); }

private:
	// 로컬 변환 데이터
	Vec3 _localScale = { 1,1,1 };
	Vec3 _localRotation = { 0,0,0 };
	Vec3 _localPosition = { 0,0,0 };

	// 로컬 및 월드 행렬
	Matrix _matLocal = Matrix::Identity;
	Matrix _matWorld = Matrix::Identity;

	// 캐시된 월드 변환 데이터
	Vec3 _scale;
	Vec3 _rotation;
	Vec3 _position;

	//// 방향 벡터
	//Vec3 _right;
	//Vec3 _up;
	//Vec3 _look;

	// 부모와 자식 트랜스폼
	shared_ptr<Transform> _parent;
	vector<shared_ptr<Transform>> _children;
};
