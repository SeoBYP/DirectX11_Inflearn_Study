#pragma once
class ResterizerState
{
public:
	ResterizerState(ComPtr<ID3D11Device> device);
	~ResterizerState();

	ComPtr<ID3D11RasterizerState> GetComptr() { return _resterizerstate; }

	void Create();

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11RasterizerState> _resterizerstate;
};

