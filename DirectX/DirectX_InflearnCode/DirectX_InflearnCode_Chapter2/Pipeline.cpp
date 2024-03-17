#include "pch.h"
#include "Pipeline.h"

Pipeline::Pipeline(ComPtr<ID3D11DeviceContext> deviceContext) :
	_deviceContext(deviceContext)
{
}

Pipeline::~Pipeline()
{
}

void Pipeline::UpdatePipeline(PipelineInfo info)
{
	//IA
	_deviceContext->IASetInputLayout(info.inputLayout->GetComPtr().Get());
	_deviceContext->IASetPrimitiveTopology(info.topology);

	// 정점 쉐이더(VS) 스테이지 설정
	if(info.vertexShader)
		_deviceContext->VSSetShader(info.vertexShader->GetComptr().Get(), nullptr, 0);

	if(info.resterizerstate)
		_deviceContext->RSSetState(info.resterizerstate->GetComptr().Get());
	// 픽셀 쉐이더(PS) 스테이지 설정
	if (info.pixelShader)
		_deviceContext->PSSetShader(info.pixelShader->GetComptr().Get(), nullptr, 0);

	// 출력 병합(OM) 스테이지 설정
	if (info.blendState)
		_deviceContext->OMSetBlendState(info.blendState->GetComptr().Get(), info.blendState->GetBlendFactor(),
			info.blendState->GetSampleMask());

}

void Pipeline::SetVertexBuffer(shared_ptr<VertexBuffer> buffer)
{
	uint32 stride = buffer->GetStride();
	uint32 offset = buffer->GetOffset();
	_deviceContext->IASetVertexBuffers(0, 1, buffer->GetComptr().GetAddressOf(), &stride, &offset);
}

void Pipeline::SetIndexBuffer(shared_ptr<IndexBuffer> buffer)
{
	_deviceContext->IASetIndexBuffer(buffer->GetComptr().Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Pipeline::SetTexture(uint32 slot, uint32 scope, shared_ptr<Texture> texture)
{
	if (scope & SS_VertexShader)
		_deviceContext->VSSetShaderResources(slot, 1, texture->GetComPtr().GetAddressOf());

	if (scope & SS_PixelShader)
		_deviceContext->PSSetShaderResources(slot, 1, texture->GetComPtr().GetAddressOf());
}

void Pipeline::SetSamplerState(uint32 slot, uint32 scope, shared_ptr<SamplerState> samplerState)
{
	if (scope & SS_VertexShader)
		_deviceContext->VSSetSamplers(slot, 1, samplerState->GetComPtr().GetAddressOf());

	if (scope & SS_PixelShader)
		_deviceContext->PSSetSamplers(slot, 1, samplerState->GetComPtr().GetAddressOf());
}

void Pipeline::Draw(int32 vertexCount, int32 startVertexLocation)
{
	_deviceContext->Draw(vertexCount, startVertexLocation);
}

void Pipeline::DrawIndexed(int32 indexCount, int32 startindexLocation, int32 baseVertexLocation)
{
	_deviceContext->DrawIndexed(indexCount , startindexLocation, baseVertexLocation);
}
