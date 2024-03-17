#include "pch.h"
#include "AsimpTool.h"
#include "Converter.h"
void AsimpTool::Init()
{

	//{
	//	shared_ptr<Converter> converter = make_shared<Converter>();

	//	converter->ReadAssetFile(L"Tank/Tank.fbx");
	//	converter->ExportMaterialData(L"Tank/Tank");
	//	converter->ExportModelData(L"Tank/Tank");
	//}
	{
		shared_ptr<Converter> converter = make_shared<Converter>();

		converter->ReadAssetFile(L"Kachujin/Mesh.fbx");
		converter->ExportMaterialData(L"Kachujin/Kachujin");
		converter->ExportModelData(L"Kachujin/Kachujin");
	}
	{
		shared_ptr<Converter> converter = make_shared<Converter>();

		converter->ReadAssetFile(L"Kachujin/Idle.fbx");
		converter->ExportAnimationData(L"Kachujin/Idle");
	}
	{
		shared_ptr<Converter> converter = make_shared<Converter>();

		converter->ReadAssetFile(L"Kachujin/Run.fbx");
		converter->ExportAnimationData(L"Kachujin/Run");
	}
	{
		shared_ptr<Converter> converter = make_shared<Converter>();

		converter->ReadAssetFile(L"Kachujin/Slash.fbx");
		converter->ExportAnimationData(L"Kachujin/Slash");
	}
}

void AsimpTool::Update()
{

}

void AsimpTool::Render()
{

}
