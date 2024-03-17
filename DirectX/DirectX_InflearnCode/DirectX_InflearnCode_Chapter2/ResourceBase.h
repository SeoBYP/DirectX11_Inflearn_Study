#pragma once

// 자원 유형을 나타내는 열거형
enum class ResourceType : uint8
{
	None = -1,
	Mesh,
	Shader,
	Texture,
	Material,
	Animation,
	End,
};

// 사용 가능한 자원 유형의 수
enum {
	RESOURCE_TYPE_COUNT = static_cast<uint8>(ResourceType::End)
};

// 모든 자원의 기본 클래스
class ResourceBase
{
public:
	ResourceBase(ResourceType type); // 생성자
	virtual ~ResourceBase(); // 가상 소멸자

	ResourceType GetType() { return _type; } // 자원 유형 반환

	void SetName(const wstring& name) { _name = name; } // 자원 이름 설정
	const wstring& GetName() { return _name; } // 자원 이름 반환
	uint32 GetId() { return _id; } // 자원 ID 반환

protected:
	virtual void Load(const wstring& path) {} // 자원 로딩 (가상 함수)
	virtual void Save(const wstring& path) {} // 자원 저장 (가상 함수)

protected:
	ResourceType _type = ResourceType::None; // 자원 유형
	wstring _name; // 자원 이름
	wstring _path; // 자원 경로
	uint32 _id = 0; // 자원 ID
};