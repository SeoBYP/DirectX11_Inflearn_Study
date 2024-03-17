#include "pch.h"
#include "ModelAnimator.h"
#include "Material.h"
#include "ModelMesh.h"
#include "Model.h"
#include "ModelAnimation.h"
#include "Camera.h"
#include "Light.h"
ModelAnimator::ModelAnimator(shared_ptr<Shader> shader)
	: Super(ComponentType::Animator), _shader(shader)
{
	//text
	_tweenDesc.next.animIndex = rand() % 3;
	_tweenDesc.tweenSumTime += rand() % 100;
}

ModelAnimator::~ModelAnimator()
{

}
void ModelAnimator::SetModel(shared_ptr<Model> model)
{
	_model = model;

	const auto& materials = _model->GetMaterials();
	for (auto& material : materials)
	{
		material->SetShader(_shader);
	}
}
/*
//void ModelAnimator::Update()
//{
//	if (_model == nullptr)
//		return;
//	if (_texture == nullptr)
//		CreateTexture();
//
//	// Anim Update
//	ImGui::InputInt("AnimIndex", &_keyframeDesc.animIndex);
//	_keyframeDesc.animIndex %= _model->GetAnimationCount();
//	ImGui::InputInt("CurrFrame", (int*)&_keyframeDesc.currFrame);
//	_keyframeDesc.currFrame %= _model->GetAnimationByIndex(_keyframeDesc.animIndex)->frameCount;
//
//	// 애니메이션 현재 프레임 정보
//	RENDER->PushKeyframeData(_keyframeDesc);
//
//	// SRV를 통해 정보 전달
//	_shader->GetSRV("TransformMap")->SetResource(_srv.Get());
//
//	// Bones
//	BoneDesc boneDesc;
//
//	const uint32 boneCount = _model->GetBoneCount();
//	for (uint32 i = 0; i < boneCount; i++)
//	{
//		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
//		boneDesc.transforms[i] = bone->transform;
//	}
//	RENDER->PushBoneData(boneDesc);
//
//	// Transform
//	auto world = GetTransform()->GetWorldMatrix();
//	RENDER->PushTransformData(TransformDesc{ world });
//
//	const auto& meshes = _model->GetMeshes();
//	for (auto& mesh : meshes)
//	{
//		if (mesh->material)
//			mesh->material->Update();
//
//		// BoneIndex
//		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);
//
//		uint32 stride = mesh->vertexBuffer->GetStride();
//		uint32 offset = mesh->vertexBuffer->GetOffset();
//
//		DC->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
//		DC->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//		_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
//	}
//}
//
//// 모델 애니메이터의 업데이트 함수
//void ModelAnimator::Update()
//{
//	// 모델이 설정되지 않았다면 업데이트를 진행하지 않음
//	if (_model == nullptr)
//		return;
//	// 텍스처가 없으면 생성
//	if (_texture == nullptr)
//		CreateTexture();
//
//	// 애니메이션 재생을 위한 시간 누적
//	_keyframeDesc.sumTime += DT;
//
//	// 현재 재생 중인 애니메이션 가져오기
//	shared_ptr<ModelAnimation> current = _model->GetAnimationByIndex(_keyframeDesc.animIndex);
//	if (current)
//	{
//		// 프레임 당 시간 계산 (프레임레이트와 재생 속도 고려)
//		float timePerFrame = 1 / (current->frameRate * _keyframeDesc.speed);
//		// 누적 시간이 프레임 당 시간을 초과하면 다음 프레임으로
//		if (_keyframeDesc.sumTime >= timePerFrame)
//		{
//			_keyframeDesc.sumTime = 0.f;
//			_keyframeDesc.currFrame = (_keyframeDesc.currFrame + 1) % current->frameCount;
//			_keyframeDesc.nextFrame = (_keyframeDesc.currFrame + 1) % current->frameCount;
//		}
//
//		// 다음 프레임으로 넘어가는 비율 계산
//		_keyframeDesc.ratio = (_keyframeDesc.sumTime / timePerFrame);
//	}
//
//	// ImGui를 통해 애니메이션 인덱스와 속도 조절
//	ImGui::InputInt("AnimIndex", &_keyframeDesc.animIndex);
//	_keyframeDesc.animIndex %= _model->GetAnimationCount();
//	ImGui::InputFloat("Speed", &_keyframeDesc.speed, 0.5f, 4.f);
//
//	// 현재 프레임 정보를 렌더러에 전달
//	RENDER->PushKeyframeData(_keyframeDesc);
//
//	// 셰이더 리소스 뷰(SRV)를 통해 변형 맵 정보 전달
//	_shader->GetSRV("TransformMap")->SetResource(_srv.Get());
//
//	// 본 데이터 준비
//	BoneDesc boneDesc;
//
//	// 모델의 본 갯수 가져오기
//	const uint32 boneCount = _model->GetBoneCount();
//	for (uint32 i = 0; i < boneCount; i++)
//	{
//		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
//		// 각 본의 변환 정보를 본 설명자에 저장
//		boneDesc.transforms[i] = bone->transform;
//	}
//	// 렌더러에 본 데이터 전달
//	RENDER->PushBoneData(boneDesc);
//
//	// 월드 변환 행렬 전달
//	auto world = GetTransform()->GetWorldMatrix();
//	RENDER->PushTransformData(TransformDesc{ world });
//
//	// 모델의 메시들을 순회하며 렌더링 준비
//	const auto& meshes = _model->GetMeshes();
//	for (auto& mesh : meshes)
//	{
//		// 메시의 재질이 있으면 업데이트
//		if (mesh->material)
//			mesh->material->Update();
//
//		// 셰이더에 본 인덱스 설정
//		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);
//
//		// 메시의 정점 버퍼 설정
//		uint32 stride = mesh->vertexBuffer->GetStride();
//		uint32 offset = mesh->vertexBuffer->GetOffset();
//
//		// 드로우 콜
//		DC->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
//		DC->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//		// 셰이더를 사용하여 인덱스 버퍼를 기반으로 메시를 렌더링
//		_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
//	}
//}
//
//
//void ModelAnimator::Update()
//{
//	if (_model == nullptr)
//		return;
//	if (_texture == nullptr)
//		CreateTexture();
//
//	TweenDesc& desc = _tweenDesc;
//
//	desc.curr.sumTime += DT;
//	// 현재 애니메이션
//	{
//		shared_ptr<ModelAnimation> currentAnim = _model->GetAnimationByIndex(desc.curr.animIndex);
//		if (currentAnim)
//		{
//			float timePerFrame = 1 / (currentAnim->frameRate * desc.curr.speed);
//			if (desc.curr.sumTime >= timePerFrame)
//			{
//				desc.curr.sumTime = 0;
//				desc.curr.currFrame = (desc.curr.currFrame + 1) % currentAnim->frameCount;
//				desc.curr.nextFrame = (desc.curr.currFrame + 1) % currentAnim->frameCount;
//			}
//
//			desc.curr.ratio = (desc.curr.sumTime / timePerFrame);
//		}
//	}
//
//	// 다음 애니메이션이 예약 되어 있다면
//	if (desc.next.animIndex >= 0)
//	{
//		desc.tweenSumTime += DT;
//		desc.tweenRatio = desc.tweenSumTime / desc.tweenDuration;
//
//		if (desc.tweenRatio >= 1.f)
//		{
//			// 애니메이션 교체 성공
//			desc.curr = desc.next;
//			desc.ClearNextAnim();
//		}
//		else
//		{
//			// 교체중
//			shared_ptr<ModelAnimation> nextAnim = _model->GetAnimationByIndex(desc.next.animIndex);
//			desc.next.sumTime += DT;
//
//			float timePerFrame = 1.f / (nextAnim->frameRate * desc.next.speed);
//
//			if (desc.next.ratio >= 1.f)
//			{
//				desc.next.sumTime = 0;
//
//				desc.next.currFrame = (desc.next.currFrame + 1) % nextAnim->frameCount;
//				desc.next.nextFrame = (desc.next.currFrame + 1) % nextAnim->frameCount;
//			}
//
//			desc.next.ratio = desc.next.sumTime / timePerFrame;
//		}
//	}
//
//	// Anim Update
//	ImGui::InputInt("AnimIndex", &desc.curr.animIndex);
//	_keyframeDesc.animIndex %= _model->GetAnimationCount();
//
//	static int32 nextAnimIndex = 0;
//	if (ImGui::InputInt("NextAnimIndex", &nextAnimIndex))
//	{
//		nextAnimIndex %= _model->GetAnimationCount();
//		desc.ClearNextAnim(); // 기존꺼 밀어주기
//		desc.next.animIndex = nextAnimIndex;
//	}
//
//	if (_model->GetAnimationCount() > 0)
//		desc.curr.animIndex %= _model->GetAnimationCount();
//
//	ImGui::InputFloat("Speed", &desc.curr.speed, 0.5f, 4.f);
//
//	RENDER->PushTweenData(desc);
//
//	// SRV를 통해 정보 전달
//	_shader->GetSRV("TransformMap")->SetResource(_srv.Get());
//
//	// Bones
//	BoneDesc boneDesc;
//
//	const uint32 boneCount = _model->GetBoneCount();
//	for (uint32 i = 0; i < boneCount; i++)
//	{
//		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
//		boneDesc.transforms[i] = bone->transform;
//	}
//	RENDER->PushBoneData(boneDesc);
//
//	// Transform
//	auto world = GetTransform()->GetWorldMatrix();
//	RENDER->PushTransformData(TransformDesc{ world });
//
//	const auto& meshes = _model->GetMeshes();
//	for (auto& mesh : meshes)
//	{
//		if (mesh->material)
//			mesh->material->Update();
//
//		// BoneIndex
//		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);
//
//		uint32 stride = mesh->vertexBuffer->GetStride();
//		uint32 offset = mesh->vertexBuffer->GetOffset();
//
//		DC->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
//		DC->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//		_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
//	}
//}
//
//void ModelAnimator::SetModel(shared_ptr<Model> model)
//{
//	_model = model;
//
//	const auto& materials = _model->GetMaterials();
//	for (auto& material : materials)
//	{
//		material->SetShader(_shader);
//	}
//}
*/
// 현재와 다음 애니메이션 사이의 트윈 데이터를 업데이트하는 함수
void ModelAnimator::UpdateTweenData()
{
	TweenDesc& desc = _tweenDesc; // 트윈 설명자에 대한 참조

	// 현재 애니메이션 시간 업데이트
	desc.curr.sumTime += DT; // DT는 델타 시간

	// 현재 애니메이션 처리
	{
		// 현재 애니메이션 객체 가져오기
		shared_ptr<ModelAnimation> currentAnim = _model->GetAnimationByIndex(desc.curr.animIndex);
		if (currentAnim) // 유효한 애니메이션인 경우
		{
			// 프레임 당 시간 계산
			float timePerFrame = 1 / (currentAnim->frameRate * desc.curr.speed);
			// 현재 시간이 프레임 당 시간을 초과한 경우 프레임 업데이트
			if (desc.curr.sumTime >= timePerFrame)
			{
				desc.curr.sumTime = 0; // 시간 초기화
				// 현재 프레임과 다음 프레임 업데이트, 프레임 수로 나눈 나머지를 사용하여 반복
				desc.curr.currFrame = (desc.curr.currFrame + 1) % currentAnim->frameCount;
				desc.curr.nextFrame = (desc.curr.currFrame + 1) % currentAnim->frameCount;
			}

			// 보간 비율 계산
			desc.curr.ratio = (desc.curr.sumTime / timePerFrame);
		}
	}

	// 다음 애니메이션으로 전환 예정인 경우 처리
	if (desc.next.animIndex >= 0)
	{
		desc.tweenSumTime += DT; // 트윈 시간 업데이트
		// 트윈 진행 비율 계산
		desc.tweenRatio = desc.tweenSumTime / desc.tweenDuration;

		// 트윈 비율이 1 이상인 경우, 즉 전환 완료인 경우
		if (desc.tweenRatio >= 1.f)
		{
			// 현재 애니메이션을 다음 애니메이션으로 교체
			desc.curr = desc.next;
			// 다음 애니메이션 정보 초기화
			desc.ClearNextAnim();
		}
		else // 아직 교체 중인 경우
		{
			// 다음 애니메이션 객체 가져오기
			shared_ptr<ModelAnimation> nextAnim = _model->GetAnimationByIndex(desc.next.animIndex);
			desc.next.sumTime += DT; // 다음 애니메이션 시간 업데이트

			// 프레임 당 시간 계산
			float timePerFrame = 1.f / (nextAnim->frameRate * desc.next.speed);

			// 다음 애니메이션의 보간 비율 업데이트
			if (desc.next.ratio >= 1.f)
			{
				desc.next.sumTime = 0; // 시간 초기화

				// 다음 애니메이션의 현재 프레임과 다음 프레임 업데이트
				desc.next.currFrame = (desc.next.currFrame + 1) % nextAnim->frameCount;
				desc.next.nextFrame = (desc.next.currFrame + 1) % nextAnim->frameCount;
			}

			desc.next.ratio = desc.next.sumTime / timePerFrame;
		}
	}
}


// 모델의 메시들을 인스턴싱을 사용하여 렌더링하는 함수
void ModelAnimator::RenderInstancing(shared_ptr<class InstancingBuffer>& buffer)
{
	// 모델이 설정되지 않은 경우 함수를 종료합니다.
	if (_model == nullptr)
		return;
	// 텍스처가 설정되지 않은 경우 텍스처를 생성합니다.
	if (_texture == nullptr)
		CreateTexture();

	// 카메라의 뷰 매트릭스와 프로젝션 매트릭스를 셰이더의 전역 데이터로 설정합니다.
	_shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

	// 현재 씬의 광원 정보를 가져와서 셰이더에 광원 데이터를 설정합니다.
	auto lightObj = SCENE->GetCurrentScene()->GetLight();
	if (lightObj == nullptr) return; // 광원 객체가 없으면 함수를 종료합니다.
	_shader->PushLightData(lightObj->GetLight()->GetLightDesc());

	// 변환 매트릭스 정보를 셰이더 리소스 뷰(SRV)를 통해 셰이더에 전달합니다.
	_shader->GetSRV("TransformMap")->SetResource(_srv.Get());

	// 본 데이터를 준비합니다.
	BoneDesc boneDesc;

	// 모델의 본 개수를 가져와서 각 본의 변환 매트릭스를 셰이더에 설정합니다.
	const uint32 boneCount = _model->GetBoneCount();
	for (uint32 i = 0; i < boneCount; i++)
	{
		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
		boneDesc.transforms[i] = bone->transform;
	}
	_shader->PushBoneData(boneDesc);

	// 모델의 모든 메시에 대해 반복 처리합니다.
	const auto& meshes = _model->GetMeshes();
	for (auto& mesh : meshes)
	{
		if (mesh->material)
			mesh->material->Update(); // 메시의 재질이 있으면 업데이트합니다.

		// 현재 메시의 본 인덱스를 셰이더에 설정합니다.
		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		// 메시의 정점 버퍼와 인덱스 버퍼 데이터를 GPU에 전송합니다.
		mesh->vertexBuffer->PushData();
		mesh->indexBuffer->PushData();

		// 인스턴싱 버퍼의 데이터를 GPU에 전송합니다.
		buffer->PushData();

		// 인스턴싱을 사용하여 메시를 렌더링합니다.
		_shader->DrawIndexedInstanced(0, _pass, mesh->indexBuffer->GetCount(), buffer->GetCount());
	}
}


InstanceID ModelAnimator::GetInstanceID()
{
	return make_pair((uint64)_model.get(), (uint64)_shader.get());
}

void ModelAnimator::CreateTexture()
{
	// 애니메이션이 없는 경우 함수 종료
	if (_model->GetAnimationCount() == 0)
		return;

	// 애니메이션 트랜스폼 벡터 사이즈 조정
	_animTransforms.resize(_model->GetAnimationCount());
	for (uint32 i = 0; i < _model->GetAnimationCount(); i++)
		CreateAnimationTransform(i); // 각 애니메이션에 대한 트랜스폼 정보 생성

	// 텍스처 생성
	{
		D3D11_TEXTURE2D_DESC desc; // 텍스처 설명자 초기화
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = MAX_MODEL_TRANSFORMS * 4; // 너비 설정, 변환 수 * 4 (행렬의 열 수)
		desc.Height = MAX_MODEL_KEYFRAMES; // 높이 설정, 최대 키 프레임 수
		desc.ArraySize = _model->GetAnimationCount(); // 배열 크기 설정, 애니메이션 수
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 포맷 설정, 32비트 부동 소수점
		desc.Usage = D3D11_USAGE_IMMUTABLE; // 사용 용도 설정, 변경 불가
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // 바인딩 옵션 설정, 셰이더 리소스로 사용
		desc.MipLevels = 1; // 미프맵 레벨 설정
		desc.SampleDesc.Count = 1; // 멀티샘플링 옵션 설정

		const uint32 dataSize = MAX_MODEL_TRANSFORMS * sizeof(Matrix); // 한 프레임의 데이터 크기
		const uint32 pageSize = dataSize * MAX_MODEL_KEYFRAMES; // 한 애니메이션의 데이터 크기
		void* mallocPtr = ::malloc(pageSize * _model->GetAnimationCount()); // 필요한 전체 메모리 할당

		// 할당된 메모리에 애니메이션 데이터 복사
		for (uint32 c = 0; c < _model->GetAnimationCount(); c++)
		{
			uint32 startOffset = c * pageSize;
			BYTE* pageStartPtr = reinterpret_cast<BYTE*>(mallocPtr) + startOffset;

			for (uint32 f = 0; f < MAX_MODEL_KEYFRAMES; f++)
			{
				void* ptr = pageStartPtr + dataSize * f;
				::memcpy(ptr, _animTransforms[c].transforms[f].data(), dataSize);
			}
		}

		// 텍스처 서브리소스 데이터 준비
		vector<D3D11_SUBRESOURCE_DATA> subResources(_model->GetAnimationCount());
		for (uint32 c = 0; c < _model->GetAnimationCount(); c++)
		{
			void* ptr = (BYTE*)mallocPtr + c * pageSize;
			subResources[c].pSysMem = ptr;
			subResources[c].SysMemPitch = dataSize;
			subResources[c].SysMemSlicePitch = pageSize;
		}

		// 텍스처 리소스 생성
		HRESULT hr = DEVICE->CreateTexture2D(&desc, subResources.data(), _texture.GetAddressOf());
		CHECK(hr); // 오류 검사

		::free(mallocPtr); // 메모리 해제
	}

	// 셰이더 리소스 뷰(SRV) 생성
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc; // SRV 설명자 초기화
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 포맷 설정
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY; // 뷰 차원 설정, 텍스처 2D 배열
		desc.Texture2DArray.MipLevels = 1; // 미프맵 레벨 설정
		desc.Texture2DArray.ArraySize = _model->GetAnimationCount(); // 배열 크기 설정

		HRESULT hr = DEVICE->CreateShaderResourceView(_texture.Get(), &desc, _srv.GetAddressOf());
		CHECK(hr); // 오류 검사
	}
}

// 애니메이션 인덱스에 따라 변환 행렬을 생성하는 함수
void ModelAnimator::CreateAnimationTransform(uint32 index)
{
	// 임시 변환 행렬 벡터를 초기화합니다. 모든 행렬은 단위 행렬로 시작합니다.
	vector<Matrix> tempAnimBoneTransforms(MAX_MODEL_TRANSFORMS, Matrix::Identity);

	// 인덱스에 해당하는 애니메이션을 모델에서 가져옵니다.
	shared_ptr<ModelAnimation> animation = _model->GetAnimationByIndex(index);

	// 애니메이션의 모든 프레임에 대해 반복합니다.
	for (uint32 f = 0; f < animation->frameCount; f++)
	{
		// 모델의 모든 본에 대해 반복합니다.
		for (uint32 b = 0; b < _model->GetBoneCount(); b++)
		{
			shared_ptr<ModelBone> bone = _model->GetBoneByIndex(b);

			Matrix matAnimation; // 본에 적용될 애니메이션 행렬

			// 현재 본에 해당하는 키프레임을 가져옵니다.
			shared_ptr<ModelKeyframe> frame = animation->GetKeyframe(bone->name);
			if (frame != nullptr) // 키프레임이 존재하는 경우
			{
				ModelKeyframeData& data = frame->transforms[f];

				// 스케일, 회전, 이동에 대한 행렬을 생성하고 결합합니다.
				Matrix S = Matrix::CreateScale(data.scale.x, data.scale.y, data.scale.z);
				Matrix R = Matrix::CreateFromQuaternion(data.rotation);
				Matrix T = Matrix::CreateTranslation(data.translation.x, data.translation.y, data.translation.z);

				matAnimation = S * R * T; // 최종 애니메이션 행렬
			}
			else // 키프레임이 없는 경우
			{
				matAnimation = Matrix::Identity; // 단위 행렬 사용
			}

			// 본의 전역 변환 행렬을 루트까지의 변환 행렬로 계산
			Matrix toRootMatrix = bone->transform;
			Matrix invGlobal = toRootMatrix.Invert();

			// 부모 본의 인덱스를 가져옵니다.
			int32 parentIndex = bone->parentIndex;

			Matrix matParent = Matrix::Identity; // 부모 행렬 초기화
			if (parentIndex >= 0) // 유효한 부모 인덱스인 경우
				matParent = tempAnimBoneTransforms[parentIndex]; // 부모 행렬을 가져옵니다.

			tempAnimBoneTransforms[b] = matAnimation * matParent; // 현재 본의 변환 행렬을 계산합니다.

			// 최종 변환 행렬을 전역에서 로컬 좌표계로 변환하여 저장합니다.
			_animTransforms[index].transforms[f][b] = invGlobal * tempAnimBoneTransforms[b];
		}
	}
}
