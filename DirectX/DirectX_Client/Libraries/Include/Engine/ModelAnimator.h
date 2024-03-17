#pragma once
#include "Component.h"

class Model;

// 애니메이션 변환 정보를 담는 구조체
struct AnimTransform {
	// 최대 모델 변환 수를 고려한 배열 타입 정의
	using TransformArrayType = array<Matrix, MAX_MODEL_TRANSFORMS>;

	// 최대 키 프레임 수에 따른 변환 정보 배열
	array<TransformArrayType, MAX_MODEL_KEYFRAMES> transforms;
};

// 모델 애니메이터 컴포넌트 클래스
class ModelAnimator : public Component
{
	using Super = Component; // 기반 클래스를 Super라는 별칭으로 정의

public:
	// 생성자와 소멸자
	ModelAnimator(shared_ptr<Shader> shader);
	~ModelAnimator();

	// 모델 설정
	void SetModel(shared_ptr<Model> model);
	// 렌더링 패스 설정
	void SetPass(uint8 pass) { _pass = pass; }
	// 셰이더 객체 반환
	shared_ptr<Shader> GetShader() { return _shader; }

	// 트윈(중간 애니메이션) 데이터 업데이트
	void UpdateTweenData();

	// 인스턴싱 렌더링 수행
	void RenderInstancing(shared_ptr<class InstancingBuffer>& buffer);
	// 인스턴스 ID 가져오기
	InstanceID GetInstanceID();
	// 트윈 설명자 반환
	TweenDesc& GetTweenDesc() { return _tweenDesc; }

private:
	// 텍스처 생성
	void CreateTexture();
	// 애니메이션 변환 정보 생성
	void CreateAnimationTransform(uint32 index);

private:
	// 애니메이션 변환 정보 벡터
	vector<AnimTransform> _animTransforms;
	// 텍스처와 해당 텍스처의 셰이더 리소스 뷰
	ComPtr<ID3D11Texture2D> _texture;
	ComPtr<ID3D11ShaderResourceView> _srv;

private:
	// 트윈(중간 애니메이션) 데이터 설명자
	TweenDesc _tweenDesc;

private:
	// 사용할 셰이더 객체
	shared_ptr<Shader>	_shader;
	// 사용할 렌더링 패스 번호
	uint8				_pass = 0;
	// 애니메이션을 적용할 모델
	shared_ptr<Model>	_model;
};
