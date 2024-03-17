#include "pch.h" // 프리컴파일 헤더
#include "07. HeightMapDemo.h" // 이 클래스의 헤더 파일
#include "GeometryHelper.h" // 지오메트리 생성을 돕는 함수들이 있는 헤더 파일
#include "Camera.h" // 카메라 구현 헤더 파일
#include "CameraScript.h" // 카메라 스크립트(조작 등) 헤더 파일
#include "Transform.h" // 변환(위치, 회전, 크기 조정) 헤더 파일
#include "Texture.h" // 텍스처 관련 헤더 파일
#include "ResourceBase.h" // 리소스 관리 기본 클래스 헤더 파일

void HeightMapDemo::Init()
{
	// 쉐이더와 텍스처 로드
	_shader = make_shared<Shader>(L"06. Terrain.fx");
	_heightMap = RESOURCES->Load<Texture>(L"Height", L"..\\Resources\\Texture\\Terrain\\height.png");
	_texture = RESOURCES->Load<Texture>(L"Grass", L"..\\Resources\\Texture\\Terrain\\grass.jpg");

	// 높이 맵의 크기 정보를 가져옴
	const int32 width = _heightMap->GetSize().x;
	const int32 height = _heightMap->GetSize().y;

	// 픽셀 버퍼에서 높이 데이터를 가져옴
	const DirectX::ScratchImage& info = _heightMap->GetInfo();
	uint8* pixelBuffer = info.GetPixels();

	// 지형 메시 생성
	_geometry = make_shared<Geometry<VertexTextureData>>();
	GeometryHelper::CreateGrid(_geometry, width, height);

	// CPU에서 지형의 높이를 설정
	{
		vector<VertexTextureData>& v = const_cast<vector<VertexTextureData>&>(_geometry->GetVertices());

		for (int32 z = 0; z < height; z++)
		{
			for (int32 x = 0; x < width; x++)
			{
				int32 idx = width * z + x;
				uint8 height = pixelBuffer[idx] / 255.f * 25.f; // 픽셀 값에 따라 높이 결정
				v[idx].position.y = height; // 높이 값 설정
			}
		}
	}

	// 버텍스 버퍼와 인덱스 버퍼 생성 및 데이터 설정
	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_geometry->GetVertices());
	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_geometry->GetIndices());

	// 카메라 설정
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform();
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	// 카메라 초기 위치 및 회전 설정
	_camera->GetTransform()->SetWorldPosition(Vec3(0.f, 5.f, 0.f));
	_camera->GetTransform()->SetWorldRotation(Vec3(25.f, 0.f, 0.f));
}

void HeightMapDemo::Update()
{
	_camera->Update(); // 카메라 업데이트
}

void HeightMapDemo::Render()
{
	// 쉐이더에 변환 행렬과 텍스처 리소스 바인딩
	_shader->GetMatrix("World")->SetMatrix((float*)&_world);
	_shader->GetMatrix("View")->SetMatrix((float*)&Camera::S_MatView);
	_shader->GetMatrix("Projection")->SetMatrix((float*)&Camera::S_MatProjection);
	_shader->GetSRV("Texture0")->SetResource(_texture->GetComPtr().Get());

	// 버텍스 버퍼와 인덱스 버퍼를 입력 어셈블러에 바인딩
	uint32 stride = _vertexBuffer->GetStride();
	uint32 offset = _vertexBuffer->GetOffset();
	DC->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
	DC->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	// 지형 메시를 렌더링
	_shader->DrawIndexed(0, 0, _indexBuffer->GetCount(), 0, 0);
}
