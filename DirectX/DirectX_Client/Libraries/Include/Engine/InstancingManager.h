#pragma once
#include "InstancingBuffer.h"

#define MAX_MESH_INSTANCE 500

class GameObject;

class InstancingManager
{
	DECLARE_SINGLE(InstancingManager);
public:

	void Render(vector<shared_ptr<GameObject>>& gameObjects);
	void ClearData();

private:
	void RenderMeshRenderer(vector<shared_ptr<GameObject>>& gameObjects);
	void RenderModelRenderer(vector<shared_ptr<GameObject>>& gameObjects);
	void RenderAnimRenderer(vector<shared_ptr<GameObject>>& gameObjects);

private:
	void AddData(InstanceID instanceId, InstancingData& data);

private:
	map<InstanceID, shared_ptr<InstancingBuffer>> _buffers;

};

