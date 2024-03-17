#include "pch.h"
#include "ResterizerState.h"

ResterizerState::ResterizerState(ComPtr<ID3D11Device> device) :
	_device(device)
{
}

ResterizerState::~ResterizerState()
{
}

void ResterizerState::Create()
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = false;

	HRESULT hr = _device->CreateRasterizerState(&desc, _resterizerstate.GetAddressOf());

	CHECK(hr);
}
