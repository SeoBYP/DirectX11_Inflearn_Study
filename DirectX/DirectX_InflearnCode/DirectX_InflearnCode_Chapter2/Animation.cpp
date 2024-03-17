#include "pch.h"
#include "Animation.h"
#include "Texture.h"

Animation::Animation() : Super(ResourceType::Animation)
{
	// 기본 리소스 타입을 Animation으로 설정
}

Animation::~Animation()
{
	// 소멸자 구현
}

// XML 파일로부터 애니메이션 데이터 로딩
void Animation::Load(const wstring& path)
{
	tinyxml2::XMLDocument doc;
	string pathStr(path.begin(), path.end());
	XMLError error = doc.LoadFile(pathStr.c_str());
	assert(error == XMLError::XML_SUCCESS); // 파일 로딩 성공 확인

	XmlElement* root = doc.FirstChildElement();
	string nameStr = root->Attribute("Name");
	_name = wstring(nameStr.begin(), nameStr.end()); // 애니메이션 이름 설정

	_loop = root->BoolAttribute("Loop"); // 반복 여부 설정
	_path = path; // 파일 경로 저장

	// 키 프레임 데이터 로딩
	XmlElement* node = root->FirstChildElement();
	for (; node != nullptr; node = node->NextSiblingElement())
	{
		Keyframe keyframe;
		keyframe.offset.x = node->FloatAttribute("OffsetX");
		keyframe.offset.y = node->FloatAttribute("OffsetY");
		keyframe.size.x = node->FloatAttribute("SizeX");
		keyframe.size.y = node->FloatAttribute("SizeY");
		keyframe.time = node->FloatAttribute("Time");
		AddKeyframe(keyframe); // 키 프레임 추가
	}
}

// 애니메이션 데이터를 XML 파일로 저장
void Animation::Save(const wstring& path)
{
	tinyxml2::XMLDocument doc;
	XMLElement* root = doc.NewElement("Animation");
	doc.LinkEndChild(root);

	string nameStr(GetName().begin(), GetName().end());
	root->SetAttribute("Name", nameStr.c_str());
	root->SetAttribute("Loop", _loop);
	root->SetAttribute("TexturePath", "TODO"); // 텍스처 경로 TODO: 실제 텍스처 경로 저장 구현 필요

	// 키 프레임 데이터 저장
	for (const auto& keyframe : _keyframes)
	{
		XMLElement* node = doc.NewElement("Keyframe");
		root->LinkEndChild(node);
		node->SetAttribute("OffsetX", keyframe.offset.x);
		node->SetAttribute("OffsetY", keyframe.offset.y);
		node->SetAttribute("SizeX", keyframe.size.x);
		node->SetAttribute("SizeY", keyframe.size.y);
		node->SetAttribute("Time", keyframe.time);
	}

	string pathStr(path.begin(), path.end());
	auto result = doc.SaveFile(pathStr.c_str());
	assert(result == XMLError::XML_SUCCESS); // 파일 저장 성공 확인
}

// 텍스처 크기 반환
Vec2 Animation::GetTextureSize()
{
	return _texture->GetSize();
}

// 지정된 인덱스의 키 프레임 반환
const Keyframe& Animation::GetKeyframe(int32 index)
{
	return _keyframes[index];
}

// 키 프레임 개수 반환
int32 Animation::GetKeyframeCount()
{
	return static_cast<int32>(_keyframes.size());
}

// 새로운 키 프레임 추가
void Animation::AddKeyframe(const Keyframe& keyframe)
{
	_keyframes.push_back(keyframe);
}