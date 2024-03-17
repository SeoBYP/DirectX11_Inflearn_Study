#include "pch.h"
#include "MonoBehaviour.h"

// MonoBehaviour의 생성자: ComponentType::Script 유형으로 부모 클래스 초기화
MonoBehaviour::MonoBehaviour() : Super(ComponentType::Script) {
}

MonoBehaviour::~MonoBehaviour() {
}

void MonoBehaviour::Awake() {
	// 초기화 코드 여기에 작성
}

void MonoBehaviour::Update() {
	// 프레임마다 실행할 코드 여기에 작성
}