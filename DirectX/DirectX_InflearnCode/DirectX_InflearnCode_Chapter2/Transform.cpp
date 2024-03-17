#include "pch.h"
#include "Transform.h"

Transform::Transform() : Super(ComponentType::Transform)
{
	// 생성자에서는 특별한 초기화 로직이 없습니다.
}

Transform::~Transform()
{
	// 소멸자에서도 특별한 처리가 없습니다.
}

void Transform::Awake()
{
	// 초기화 함수에서도 현재는 특별한 처리가 없습니다.
}

void Transform::Update()
{
	// 업데이트 함수에서는 현재 특별한 처리가 없습니다.
}
Vec3 ToEulerAngles(Quaternion q) {
	Vec3 angles;

	// roll (x-axis rotation)
	double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	angles.y = 2 * std::atan2(sinp, cosp) - 3.17159f / 2;

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}

void Transform::UpdateTransform()
{
	// 로컬 변환 데이터를 바탕으로 로컬 변환 행렬을 계산합니다.
	_localPosition.x += 0.001f; // 예제로, 매 업데이트마다 x 위치를 조금씩 이동시킵니다.

	// 스케일, 회전, 이동 행렬을 계산하고, 이를 조합하여 로컬 행렬을 생성합니다.
	Matrix matScale = Matrix::CreateScale(_localScale / 3);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x) *
		Matrix::CreateRotationY(_localRotation.y) *
		Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	_matLocal = matScale * matRotation * matTranslation;

	// 부모 트랜스폼이 있을 경우, 부모의 월드 행렬과 조합하여 최종 월드 행렬을 계산합니다.
	if (HasParent())
	{
		_matWorld = _matLocal * _parent->GetWorldMatrix();
	}
	else
	{
		_matWorld = _matLocal;
	}

	// 월드 행렬을 분해하여 스케일, 회전(쿼터니언), 위치 데이터를 추출하고, 이를 바탕으로 방향 벡터를 계산합니다.
	Quaternion quat;
	_matWorld.Decompose(_scale, quat, _position);
	_rotation = ToEulerAngles(quat); // 쿼터니언을 오일러 각도로 변환합니다.

	//_right = Vec3::TransformNormal(Vec3::Right, _matWorld); // 오른쪽 방향 벡터
	//_up = Vec3::TransformNormal(Vec3::Up, _matWorld); // 위쪽 방향 벡터
	//_look = Vec3::TransformNormal(Vec3::Backward, _matWorld); // 앞쪽 방향 벡터

	// 자식 트랜스폼들에 대해서도 업데이트를 호출하여 계층적 변환을 적용합니다.
	for (const shared_ptr<Transform>& child : _children) {
		child->UpdateTransform();
	}
}
// 월드 좌표계에서의 스케일을 설정하는 메서드입니다.
void Transform::SetWorldScale(const Vec3& worldScale)
{
	if (HasParent())
	{
		Vec3 parentScale = _parent->GetWorldScale();
		// 부모 스케일을 고려하여 로컬 스케일 계산
		Vec3 scale = worldScale;
		scale.x /= parentScale.x;
		scale.y /= parentScale.y;
		scale.z /= parentScale.z;
		SetLocalScale(scale);
	}
	else
	{
		SetLocalScale(worldScale);
	}
}
// 월드 좌표계에서의 회전을 설정하는 메서드입니다.
void Transform::SetWorldRotation(const Vec3& worldRotation)
{
	if (HasParent())
	{
		// 부모의 월드 행렬을 역행렬로 변환하여, 월드 좌표계에서 부모 로컬 좌표계로의 변환 행렬을 얻습니다.
		Matrix worldToParentLocalMatrix = _parent->GetWorldMatrix().Invert();

		// 월드 회전 값을 부모의 로컬 좌표계로 변환합니다.
		Vec3 rotation;
		rotation.Transform(worldRotation, worldToParentLocalMatrix);

		// 계산된 회전 값을 현재 트랜스폼의 로컬 위치로 설정합니다.
		SetLocalPosition(rotation);
	}
	else
	{
		SetLocalRotation(worldRotation);
	}

}
// 월드 좌표계에서의 위치를 설정하는 메서드입니다.
void Transform::SetWorldPosition(const Vec3& worldPosition)
{
	if (HasParent())
	{
		// 부모의 월드 행렬을 역행렬로 변환하여, 월드 좌표계에서 부모 로컬 좌표계로의 변환 행렬을 얻습니다.
		Matrix worldToParentLocalMatrix = _parent->GetWorldMatrix().Invert();

		// 월드 위치 값을 부모의 로컬 좌표계로 변환합니다.
		Vec3 position;
		position.Transform(worldPosition, worldToParentLocalMatrix);

		// 계산된 위치 값을 현재 트랜스폼의 로컬 위치로 설정합니다.
	}
	else
	{
		SetLocalPosition(worldPosition);
	}
}
