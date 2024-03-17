#include "pch.h"
#include "CameraMove.h"
#include "GameObject.h"
#include "Camera.h"

void CameraMove::Update()
{
	auto pos = GetTransform()->GetWorldPosition();
	pos.x += 0.01f;
	GetTransform()->SetWorldPosition(pos);
}
