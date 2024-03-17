#include "pch.h"
#include "InputLayout.h"

InputLayout::InputLayout(ComPtr<ID3D11Device> device):
	_device(device)
{
}

InputLayout::~InputLayout()
{
}

void InputLayout::Create(const vector<D3D11_INPUT_ELEMENT_DESC>& descs, ComPtr<ID3DBlob> blob)
{
	//// 입력 요소 설명 배열
	//D3D11_INPUT_ELEMENT_DESC layout[] =
	//{
	//	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, // "POSITION" 세맨틱: 3D 위치 데이터
	//	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}, // "COLOR" 세맨틱: 색상 데이터
	//};

	const int32 count = static_cast<int32>(descs.size());
	_device->CreateInputLayout(descs.data(), count,
		blob->GetBufferPointer(), blob->GetBufferSize(), _inputLayout.GetAddressOf()); // 입력 레이아웃 생성
}
