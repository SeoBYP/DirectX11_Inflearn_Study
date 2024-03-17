#include "pch.h"
#include "CameraScript.h"
#include "Transform.h"

void CameraScript::Start()
{
}

void CameraScript::Update()
{
	float dt = TIME->GetDeltaTime();

	// Position
	Vec3 pos = GetTransform()->GetWorldPosition();

	if (INPUT->GetButton(KEY_TYPE::W)) {
		pos += GetTransform()->GetLook()* _speed *  dt;
	}
	if (INPUT->GetButton(KEY_TYPE::S)) {
		pos -= GetTransform()->GetLook()* _speed* dt;
	}
	if (INPUT->GetButton(KEY_TYPE::A)) {
		pos -= GetTransform()->GetRight()* _speed* dt;
	}
	if (INPUT->GetButton(KEY_TYPE::D)) {
		pos += GetTransform()->GetRight()* _speed* dt;
	}

	GetTransform()->SetWorldPosition(pos);

	//Rotation
	if (INPUT->GetButton(KEY_TYPE::Q)) {
		Vec3 rot =  GetTransform()->GetLocalRotation();
		rot.x += dt * 0.5f;
		GetTransform()->SetLocalRotation(rot);
	}
	if (INPUT->GetButton(KEY_TYPE::E)) {
		Vec3 rot = GetTransform()->GetLocalRotation();
		rot.x -= dt * 0.5f;
		GetTransform()->SetLocalRotation(rot);
	}
	if (INPUT->GetButton(KEY_TYPE::Z)) {
		Vec3 rot = GetTransform()->GetLocalRotation();
		rot.y += dt * 0.5f;
		GetTransform()->SetLocalRotation(rot);
	}
	if (INPUT->GetButton(KEY_TYPE::C)) {
		Vec3 rot = GetTransform()->GetLocalRotation();
		rot.y -= dt * 0.5f;
		GetTransform()->SetLocalRotation(rot);
	}
}
