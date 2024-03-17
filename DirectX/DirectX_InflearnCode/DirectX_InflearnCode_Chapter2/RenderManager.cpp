#include "pch.h" // 프로젝트의 사전 컴파일 헤더 파일
#include "RenderManager.h" // RenderManager 클래스 정의 포함
#include "Pipeline.h" // 파이프라인 관련 클래스 정의 포함
#include "MeshRenderer.h" // 메시 렌더링 관련 클래스 정의 포함
#include "Camera.h" // 카메라 관련 클래스 정의 포함
#include "SceneManager.h" // 씬 관리자 관련 클래스 정의 포함
#include "Scene.h" // 씬 관련 클래스 정의 포함
#include "Game.h" // 게임 관련 클래스 정의 포함
#include "Mesh.h" // 메시 관련 클래스 정의 포함
#include "Animator.h" // 애니메이터 관련 클래스 정의 포함
#include "Animation.h" // 애니메이션 관련 클래스 정의 포함

// 생성자: DirectX 11 디바이스와 디바이스 컨텍스트를 받아 멤버 변수를 초기화합니다.
RenderManager::RenderManager(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) :
	_device(device),
	_deviceContext(deviceContext)
{
}

// Init 함수: 렌더링에 필요한 리소스 및 상태를 생성하고 초기화하는 작업을 수행합니다.
void RenderManager::Init()
{
	_pipeline = make_shared<Pipeline>(_deviceContext); // 파이프라인 객체 생성

	// 카메라, 변환, 애니메이션 데이터를 위한 상수 버퍼 생성
	_cameraBuffer = make_shared<ConstantBuffer<CameraData>>(_device, _deviceContext);
	_transformBuffer = make_shared<ConstantBuffer<TransformData>>(_device, _deviceContext);
	_animationBuffer = make_shared<ConstantBuffer<AnimationData>>(_device, _deviceContext);
	_cameraBuffer->Create();
	_transformBuffer->Create();
	_animationBuffer->Create();

	// 래스터라이저, 샘플러, 블렌드 상태 생성 및 초기화
	_rasterizerState = make_shared<ResterizerState>(_device);
	_blendState = make_shared<BlendState>(_device);
	_samplerState = make_shared<SamplerState>(_device);
	_rasterizerState->Create();
	_blendState->Create();
	_samplerState->Create();
}

// Update 함수: 매 프레임마다 호출되어 렌더링을 수행합니다.
void RenderManager::Update(shared_ptr<Graphics> graphics)
{
	graphics->RenderBegin(); // 렌더링 시작

	PushCameraData(); // 카메라 데이터를 상수 버퍼에 복사
	GatherRenderableObjects(); // 렌더링할 객체 수집
	RenderObjects(); // 수집된 객체 렌더링

	graphics->RenderEnd(); // 렌더링 종료
}

// PushCameraData 함수: 카메라 관련 데이터를 상수 버퍼에 복사합니다.
void RenderManager::PushCameraData()
{
	// 카메라 뷰와 투영 행렬을 상수 버퍼에 복사
	_cameraData.matView = Camera::S_MatView;
	_cameraData.matProjection = Camera::S_MatProjection;
	_cameraBuffer->CopyData(_cameraData);
}

// PushTransformData 함수: 객체의 변환 데이터를 상수 버퍼에 복사합니다.
void RenderManager::PushTransfromData()
{
	_transformBuffer->CopyData(_transformData); // 변환 데이터 복사
}

// PushAnimationData 함수: 애니메이션 데이터를 상수 버퍼에 복사합니다.
void RenderManager::PushAnimationData()
{
	_animationBuffer->CopyData(_animationData); // 애니메이션 데이터 복사
}

// GatherRenderableObjects 함수: 현재 씬에서 렌더링할 객체들을 수집합니다.
void RenderManager::GatherRenderableObjects()
{
	_renderObjects.clear(); // 이전에 수집된 객체들을 초기화

	// 현재 활성 씬의 게임 오브젝트들 중에서 렌더링 가능한 객체들을 선택
	auto& gameObjects = SCENE->GetActiveScene()->GetGameObjects();
	for (const shared_ptr<GameObject>& gameObject : gameObjects) {
		if (auto meshRenderer = gameObject->GetMeshRenderer()) {
			_renderObjects.push_back(gameObject);
		}
	}
}

// RenderObjects 함수: 수집된 게임 오브젝트들을 렌더링합니다.
void RenderManager::RenderObjects()
{
	for (const shared_ptr<GameObject>& gameObject : _renderObjects) {
		auto meshRenderer = gameObject->GetMeshRenderer();
		if (!meshRenderer) continue; // 메시 렌더러가 없으면 무시

		auto transform = gameObject->GetTransform();
		if (!transform) continue; // 변환 컴포넌트가 없으면 무시

		// 변환 데이터 설정 및 복사
		_transformData.matWorld = transform->GetWorldMatrix();
		PushTransfromData();

		// 애니메이션 데이터 설정 및 복사 (해당되는 경우)
		shared_ptr<Animator> animator = gameObject->GetAnimator();
		if (animator)
		{
			const Keyframe& keyframe = animator->GetCurrentKeyframe();
			_animationData.spriteOffset = keyframe.offset;
			_animationData.spriteSize = keyframe.size;
			_animationData.textureSize = animator->GetCurrentAnimation()->GetTextureSize();
			_animationData.useAnimation = 1.f;
			PushAnimationData();

			_pipeline->SetConstantBuffer(2, SS_VertexShader, _animationBuffer);
			_pipeline->SetTexture(0, SS_PixelShader, animator->GetCurrentAnimation()->GetTexture());
		}
		else
		{
			_animationData.spriteOffset = Vec2(0.f, 0.f);
			_animationData.spriteSize = Vec2(0.f, 0.f);
			_animationData.textureSize = Vec2(0.f, 0.f);
			_animationData.useAnimation = 0.f;
			PushAnimationData();

			_pipeline->SetConstantBuffer(2, SS_VertexShader, _animationBuffer);
			_pipeline->SetTexture(0, SS_PixelShader, meshRenderer->GetTexture());
		}

		PipelineInfo info;
		info.inputLayout = meshRenderer->GetInputLayout();
		info.vertexShader = meshRenderer->GetVertexShader();
		info.pixelShader = meshRenderer->GetPixelShader();
		info.resterizerstate = _rasterizerState;
		info.blendState = _blendState;
		_pipeline->UpdatePipeline(info);

		_pipeline->SetVertexBuffer(meshRenderer->GetMesh()->GetVertexBuffer());
		_pipeline->SetIndexBuffer(meshRenderer->GetMesh()->GetIndexBuffer());

		_pipeline->SetConstantBuffer(0, SS_VertexShader, _cameraBuffer);
		_pipeline->SetConstantBuffer(1, SS_VertexShader, _transformBuffer);

		//_pipeline->SetTexture(0, SS_PixelShader, meshRenderer->GetTexture());
		_pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);

		_pipeline->DrawIndexed(meshRenderer->GetMesh()->GetIndexBuffer()->GetCount(), 0, 0);
	}
}
