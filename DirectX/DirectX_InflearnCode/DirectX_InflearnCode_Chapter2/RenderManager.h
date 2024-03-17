#pragma once
#include "RenderHelper.h"

// RenderManager 클래스: 게임 내 모든 렌더링 작업을 관리합니다.
class RenderManager
{
public:
	// 생성자: DirectX 11 디바이스와 디바이스 컨텍스트를 초기화합니다.
	RenderManager(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);

	// 초기화 함수: 필요한 리소스와 상태를 생성 및 초기화합니다.
	void Init();

	// 프레임별 업데이트 함수: 렌더링 작업을 시작하고, 카메라 및 객체 데이터를 업데이트한 후 렌더링을 수행합니다.
	void Update(shared_ptr<Graphics> graphics);

private:
	// 카메라 데이터를 상수 버퍼에 복사합니다.
	void PushCameraData();

	// 변환 데이터(위치, 회전, 크기)를 상수 버퍼에 복사합니다.
	void PushTransfromData();

	// 애니메이션 데이터를 상수 버퍼에 복사합니다.
	void PushAnimationData();

	// 렌더링할 게임 오브젝트를 수집합니다.
	void GatherRenderableObjects();

	// 수집된 게임 오브젝트를 렌더링합니다.
	void RenderObjects();

private:
	ComPtr<ID3D11Device> _device; // DirectX 디바이스
	ComPtr<ID3D11DeviceContext> _deviceContext; // 디바이스 컨텍스트
	shared_ptr<Pipeline> _pipeline; // 렌더링 파이프라인

	// 카메라와 관련된 데이터 및 상수 버퍼
	CameraData _cameraData;
	shared_ptr<ConstantBuffer<CameraData>> _cameraBuffer;

	// 객체의 변환과 관련된 데이터 및 상수 버퍼
	TransformData _transformData;
	shared_ptr<ConstantBuffer<TransformData>> _transformBuffer;

	// 애니메이션과 관련된 데이터 및 상수 버퍼
	AnimationData _animationData;
	shared_ptr<ConstantBuffer<AnimationData>> _animationBuffer;

	// 래스터라이저, 샘플러, 블렌드 상태
	shared_ptr<ResterizerState> _rasterizerState;
	shared_ptr<SamplerState> _samplerState;
	shared_ptr<BlendState> _blendState;

	// 렌더링할 게임 오브젝트 목록
	vector<shared_ptr<GameObject>> _renderObjects;
};
