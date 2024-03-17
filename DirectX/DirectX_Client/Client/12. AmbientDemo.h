#pragma once
#include "IExecute.h"
#include "Geometry.h"
#include "GameObject.h"

class AmbientDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	shared_ptr<Shader> _shader;

	//Object
	shared_ptr<GameObject> _obj;
	shared_ptr<GameObject> _obj2;
	shared_ptr<GameObject> _camera;
};

