#include "pch.h" // 프리컴파일 헤더 파일
#include "08. NormalDemo.h" // 이 클래스의 헤더 파일
#include "GeometryHelper.h" // 지오메트리 생성 도우미 함수가 있는 헤더 파일
#include "Camera.h" // 카메라 클래스 헤더 파일
#include "GameObject.h" // 게임 오브젝트 관리를 위한 클래스 헤더 파일
#include "CameraScript.h" // 카메라 스크립트(카메라 제어) 헤더 파일

void NormalDemo::Init()
{
	// 쉐이더 로드
	_shader = make_shared<Shader>(L"07. Normal.fx");

	// 지오메트리 생성: 여기서는 구(Sphere)를 생성함
	_geometry = make_shared<Geometry<VertexTextureNormalData>>();
	GeometryHelper::CreateSphere(_geometry); // 구를 생성하는 도우미 함수 호출

	// 버텍스 버퍼 생성 및 지오메트리의 버텍스 데이터로 초기화
	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_geometry->GetVertices());

	// 인덱스 버퍼 생성 및 지오메트리의 인덱스 데이터로 초기화
	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_geometry->GetIndices());

	// 카메라 설정
	_camera = make_shared<GameObject>(); // 카메라를 위한 게임 오브젝트 생성
	_camera->GetOrAddTransform(); // 변환 컴포넌트 추가 혹은 가져오기
	_camera->AddComponent(make_shared<Camera>()); // 카메라 컴포넌트 추가
	_camera->AddComponent(make_shared<CameraScript>()); // 카메라 스크립트(제어) 추가

	// 텍스처 로드
	_texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
}

void NormalDemo::Update()
{
	_camera->Update(); // 카메라 업데이트(예: 위치, 회전 등의 변화 반영)
}

void NormalDemo::Render()
{
	// 쉐이더에 행렬 및 텍스처 리소스 바인딩
	_shader->GetMatrix("World")->SetMatrix((float*)&_world);
	_shader->GetMatrix("View")->SetMatrix((float*)&Camera::S_MatView);
	_shader->GetMatrix("Projection")->SetMatrix((float*)&Camera::S_MatProjection);
	_shader->GetSRV("Texture0")->SetResource(_texture->GetComPtr().Get());
	_shader->GetVector("LightDir")->SetFloatVector((float*)&_lightDir); // 조명 방향 설정

	// 버텍스 및 인덱스 버퍼를 입력 어셈블러 스테이지에 바인딩
	uint32 stride = _vertexBuffer->GetStride();
	uint32 offset = _vertexBuffer->GetOffset();
	DC->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
	DC->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	// 쉐이더를 사용하여 지오메트리 렌더링
	_shader->DrawIndexed(0, 0, _indexBuffer->GetCount(), 0, 0);
}
