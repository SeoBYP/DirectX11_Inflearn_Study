#include "pch.h"
#include "SnowBillboard.h"
#include "Material.h"
#include "Camera.h"
#include "MathUtils.h"

SnowBillboard::SnowBillboard(Vec3 extent, int32 drawCount /*= 100*/)
	: Super(ComponentType::SnowBillBoard)
{
	// 빌보드를 표시할 범위와 빌보드 개수 초기화
	_desc.extent = extent;
	_desc.drawDistance = _desc.extent.z * 2.0f;
	_drawCount = drawCount;

	const int32 vertexCount = _drawCount * 4;
	_vertices.resize(vertexCount);

	for (int32 i = 0; i < _drawCount * 4; i += 4)
	{
		// 빌보드의 크기를 랜덤하게 설정
		Vec2 scale = MathUtils::RandomVec2(0.1f, 0.5f);

		// 빌보드의 위치를 extent 범위 내에서 랜덤하게 설정
		Vec3 position;
		position.x = MathUtils::Random(-_desc.extent.x, _desc.extent.x);
		position.y = MathUtils::Random(-_desc.extent.y, _desc.extent.y);
		position.z = MathUtils::Random(-_desc.extent.z, _desc.extent.z);

		// 추가적인 랜덤 값을 설정하여 각 빌보드마다 다른 효과를 적용할 수 있게 함
		Vec2 random = MathUtils::RandomVec2(0.0f, 1.0f);

		// 빌보드의 버텍스에 위치, UV 좌표, 크기, 랜덤 값을 설정
		_vertices[i + 0].position = position;
		_vertices[i + 1].position = position;
		_vertices[i + 2].position = position;
		_vertices[i + 3].position = position;

		_vertices[i + 0].uv = Vec2(0, 1);
		_vertices[i + 1].uv = Vec2(0, 0);
		_vertices[i + 2].uv = Vec2(1, 1);
		_vertices[i + 3].uv = Vec2(1, 0);

		_vertices[i + 0].scale = scale;
		_vertices[i + 1].scale = scale;
		_vertices[i + 2].scale = scale;
		_vertices[i + 3].scale = scale;

		_vertices[i + 0].random = random;
		_vertices[i + 1].random = random;
		_vertices[i + 2].random = random;
		_vertices[i + 3].random = random;
	}

	// 버텍스 버퍼 생성
	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_vertices, 0);

	// 인덱스 버퍼 생성을 위한 인덱스 설정
	const int32 indexCount = _drawCount * 6;
	_indices.resize(indexCount);

	for (int32 i = 0; i < _drawCount; i++)
	{
		_indices[i * 6 + 0] = i * 4 + 0;
		_indices[i * 6 + 1] = i * 4 + 1;
		_indices[i * 6 + 2] = i * 4 + 2;
		_indices[i * 6 + 3] = i * 4 + 2;
		_indices[i * 6 + 4] = i * 4 + 1;
		_indices[i * 6 + 5] = i * 4 + 3;
	}

	// 인덱스 버퍼 생성
	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_indices);
}

SnowBillboard::~SnowBillboard()
{
	// 소멸자에서 특별한 처리가 필요 없음
}

void SnowBillboard::Update()
{
	// 메인 카메라의 위치를 기준으로 눈이 내리는 효과를 구현
	_desc.origin = CUR_SCENE->GetMainCamera()->GetTransform()->GetWorldPosition();
	_desc.time = _elpasedTime;
	_elpasedTime += DT; // 경과 시간 업데이트

	auto shader = _material->GetShader();

	// 변환 데이터, 글로벌 데이터, 눈 데이터를 셰이더에 전달
	auto world = GetTransform()->GetWorldMatrix();
	shader->PushTransformData(TransformDesc{ world });
	shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);
	shader->PushSnowData(_desc);

	// 빛 데이터 업데이트
	_material->Update();

	// 입력 어셈블러에 데이터 전달
	_vertexBuffer->PushData();
	_indexBuffer->PushData();

	// 셰이더를 사용하여 빌보드 렌더링
	shader->DrawIndexed(0, _pass, _drawCount * 6);
}

void SnowBillboard::Add(Vec3 position, Vec2 scale)
{
	// 특정 위치에 새로운 빌보드 추가하는 로직은 본 예제에서 구현되지 않았음
}
