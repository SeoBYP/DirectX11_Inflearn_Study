#pragma once

#include "ResourceBase.h"

// 필요한 클래스 선언
class Mesh;
class Shader;
class Texture;
class Material;
class Animation;

// 자원 관리자 클래스
class ResourceManager
{
public:
	ResourceManager(ComPtr<ID3D11Device> device); // 생성자

	void Init(); // 초기화 메서드

	// 자원 로딩
	template<typename T>
	shared_ptr<T> Load(const wstring& key, const wstring& path);

	// 자원 추가
	template<typename T>
	bool Add(const wstring& key, shared_ptr<T> object);

	// 자원 검색
	template<typename T>
	shared_ptr<T> Get(const wstring& key);

	// 자원 유형 반환
	template<typename T>
	ResourceType GetResourceType();

private:
	// 기본 자원 생성 메서드
	void CreateDefaultTexture();
	void CreateDefaultMesh();
	void CreateDefaultShader();
	void CreateDefaultMaterial();
	void CreateDefaultAnimation();

private:
	ComPtr<ID3D11Device> _device; // DirectX 11 디바이스

	// 자원 저장을 위한 컨테이너
	using KeyObject = map<wstring, shared_ptr<ResourceBase>>;
	array<KeyObject, RESOURCE_TYPE_COUNT> _resources;
};

// 자원 로딩 메서드 구현
template<typename T>
inline shared_ptr<T> ResourceManager::Load(const wstring& key, const wstring& path)
{
	// 자원 유형을 결정
	ResourceType resourceType = GetResourceType<T>();
	KeyObject& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	// 이미 로드된 자원 검색
	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end()) {
		return static_pointer_cast<T>(findIt->second);
	}

	// 새 자원 로드 및 저장
	shared_ptr<T> object = make_shared<T>(_device);
	object->Load(path);
	keyObjMap[key] = object;

	return object;
}

// 자원 추가 메서드 구현
template<typename T>
inline bool ResourceManager::Add(const wstring& key, shared_ptr<T> object)
{
	ResourceType resourceType = GetResourceType<T>();
	KeyObject& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end()) {
		return false; // 이미 존재하는 키
	}

	keyObjMap[key] = object; // 자원 추가

	return true;
}

// 자원 검색 메서드 구현
template<typename T>
inline shared_ptr<T> ResourceManager::Get(const wstring& key)
{
	ResourceType resourceType = GetResourceType<T>();
	KeyObject& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end()) {
		return static_pointer_cast<T>(findIt->second);
	}
	return nullptr; // 자원을 찾지 못함
}

// 자원 유형 반환 메서드 구현
template<typename T>
inline ResourceType ResourceManager::GetResourceType()
{
	// T 유형에 따라 적절한 ResourceType 반환
	if (std::is_same_v<T, Mesh>)
		return ResourceType::Mesh;
	if (std::is_same_v<T, Shader>)
		return ResourceType::Shader;
	if (std::is_same_v<T, Texture>)
		return ResourceType::Texture;
	if (std::is_same_v<T, Material>)
		return ResourceType::Material;
	if (std::is_same_v<T, Animation>)
		return ResourceType::Animation;

	assert(false); // 지원되지 않는 유형
	return ResourceType::None;
}
