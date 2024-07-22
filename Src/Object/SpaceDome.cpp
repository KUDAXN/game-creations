#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "SpaceDome.h"

SpaceDome::SpaceDome(const VECTOR& follow) : follow_(follow) //初期化指定子
{
	
}

SpaceDome::~SpaceDome(void)
{
}

void SpaceDome::Init(void)
{

	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::SKYDOME));
	float scale = 10.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, -600.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f);
	transform_.quaRotLocal = Quaternion();
		transform_.Update();
}

void SpaceDome::Update(VECTOR pos)
{
	transform_.pos = VAdd(follow_,pos);

	transform_.Update();
}

void SpaceDome::Draw(void)
{
	MV1DrawModel(transform_.modelId);

	//デバッグ用
	//DrawFormatString(0, 500, 0xffffff, "SkayDome:(%.2f,%.2f,%.2f)", transform_.pos.x, transform_.pos.y, transform_.pos.z);
}

void SpaceDome::Release(void)
{
}
