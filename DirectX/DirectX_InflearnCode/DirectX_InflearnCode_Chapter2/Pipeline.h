#pragma once

struct PipelineInfo {
	shared_ptr<InputLayout> inputLayout;
	shared_ptr<VertexShader> vertexShader;
	shared_ptr<PixelShader> pixelShader; // «»ºø Ω¶¿Ã¥ı
	shared_ptr<ResterizerState> resterizerstate;
	shared_ptr<BlendState> blendState;
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

class Pipeline
{
public:
	Pipeline(ComPtr<ID3D11DeviceContext> deviceContext);
	~Pipeline();

	void UpdatePipeline(PipelineInfo info);
	void SetVertexBuffer(shared_ptr<VertexBuffer> buffer);
	void SetIndexBuffer(shared_ptr<IndexBuffer> buffer);

	template<typename T>
	void SetConstantBuffer(uint32 slot, uint32 scope,shared_ptr<ConstantBuffer<T>> buffer) 
	{
		if(scope & SS_VertexShader)
			_deviceContext->VSSetConstantBuffers(slot, 1, buffer->GetComptr().GetAddressOf());

		if (scope & SS_PixelShader)
			_deviceContext->PSSetConstantBuffers(slot, 1, buffer->GetComptr().GetAddressOf());
	}

	void SetTexture(uint32 slot, uint32 scope, shared_ptr<Texture> texture);
	void SetSamplerState(uint32 slot, uint32 scope, shared_ptr<SamplerState> samplerState);

	void Draw(int32 vertexCount, int32 startVertexLocation);
	void DrawIndexed(int32 indexCount, int32 startindexLocation,int32 baseVertexLocation);

private:
	ComPtr<ID3D11DeviceContext> _deviceContext;
};

