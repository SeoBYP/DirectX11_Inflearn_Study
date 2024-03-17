#include "pch.h"
#include "Texture.h"

Texture::Texture(ComPtr<ID3D11Device> device) : Super(ResourceType::Texture),
	_device(device)
{
}

Texture::~Texture()
{
}

void Texture::Create(const wstring& path)
{
	// TexMetadata와 ScratchImage는 DirectX 텍스처 로딩에 사용되는 구조체입니다.
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;

	// LoadFromWICFile 함수는 WIC(Windows Imaging Component)를 사용하여
	// "UnityLogo.png" 이미지 파일을 로드합니다. 로드된 이미지 데이터는 img에 저장되고,
	// 이미지의 메타데이터는 md에 저장됩니다.
	HRESULT hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, img);
	// 결과를 확인하여 로드 과정에 문제가 없는지 검사합니다.
	CHECK(hr);

	// CreateShaderResourceView 함수는 로드된 이미지 데이터를 사용하여
	// 셰이더 리소스 뷰(Shader Resource View, SRV)를 생성합니다. 이 SRV는
	// DirectX 11의 셰이더에서 텍스처로 사용될 수 있습니다.
	hr = ::CreateShaderResourceView(_device.Get(), img.GetImages(), img.GetImageCount(), md,
		_shaderResourceView.GetAddressOf());
	// 결과를 확인하여 SRV 생성 과정에 문제가 없는지 검사합니다.
	CHECK(hr);
	_size.x = md.width;
	_size.y = md.height;
}
