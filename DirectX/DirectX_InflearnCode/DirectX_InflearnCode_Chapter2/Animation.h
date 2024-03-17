#pragma once
#include "ResourceBase.h" // 기본 리소스 클래스를 상속받기 위함

// 키 프레임 구조체: 애니메이션의 각 프레임 정보를 저장
struct Keyframe
{
	Vec2 offset = Vec2{ 0.f, 0.f }; // 텍스처 내에서의 오프셋
	Vec2 size = Vec2{ 0.f, 0.f }; // 프레임의 크기
	float time = 0.f; // 이 프레임이 표시되는 시간
};

class Texture; // Texture 클래스의 전방 선언

// Animation 클래스: 애니메이션 리소스 관리
class Animation : public ResourceBase // ResourceBase를 상속받음
{
	using Super = ResourceBase; // 상위 클래스 타입 별칭

public:
	Animation(); // 생성자
	virtual ~Animation(); // 소멸자

	// 리소스 로딩 및 저장
	virtual void Load(const wstring& path) override;
	virtual void Save(const wstring& path) override;

	// 애니메이션 설정
	void SetLoop(bool loop); // 애니메이션 반복 설정
	bool IsLoop(); // 애니메이션이 반복되는지 확인

	// 텍스처 관련 함수
	void SetTexture(shared_ptr<Texture> texture); // 애니메이션에 사용될 텍스처 설정
	shared_ptr<Texture> GetTexture(); // 현재 설정된 텍스처 반환
	Vec2 GetTextureSize(); // 텍스처 크기 반환

	// 키 프레임 관련 함수
	const Keyframe& GetKeyframe(int32 index); // 특정 인덱스의 키 프레임 반환
	int32 GetKeyframeCount(); // 키 프레임의 총 개수 반환
	void AddKeyframe(const Keyframe& keyframe); // 새로운 키 프레임 추가

private:
	bool _loop = false; // 애니메이션 반복 여부
	shared_ptr<Texture> _texture; // 애니메이션에 사용될 텍스처
	vector<Keyframe> _keyframes; // 키 프레임 목록
};
