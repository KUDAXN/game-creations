#include<fstream>
#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "Character/Player.h"
#include "Stage2D.h"


Stage2D::Stage2D(Player& player) : player_(player)	//初期化指定子
{
	block_ = nullptr;
}

Stage2D::~Stage2D(void)
{
}

void Stage2D::Init(void)
{

	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::STAGE2));
	//float scale = 30.0f;
	float scale = 0.3f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 200.0f, 0.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(0.0f),
		0.0f);
	transform_.quaRotLocal = Quaternion();


	// 当たり判定(コライダ)作成
	transform_.MakeCollider(Collider::TYPE::STAGE);

	transform_.Update();

	////ステージを取得する
	//auto activeStage = transform_;

	//// ステージの当たり判定をプレイヤーに設定
	//player_.ClearCollider();
	//player_.AddCollider(activeStage.collider);

	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 2; x++)
		{
			objBlock_[y][x] = 0;
			objPlane_[y][x] = 0;
			objWall_[y][x] = 0;
		}
	}

	//LoadUsedCsvCol();

}

void Stage2D::Update(void)
{
	//transform_.pos = follow_.pos;

	transform_.Update();
}

void Stage2D::Draw(void)
{
	int i = MV1DrawModel(transform_.modelId);

	//DrawDebug();
	//DrawFormatString(0, 90, 0xff0099, "ステージ座標：(%.1f,%.1f,%.1f)", transform_.pos.x, transform_.pos.y, transform_.pos.z);

}

void Stage2D::DrawDebug(void)
{
	//デバッグ用
	///-----------------------------------------------------------------------------------------------------
	DrawString(0, 280, "Block", 0x000000);
	DrawFormatString(0, 300, 0xff000000, "Lt：(%.1f , %.1f)", Block_.LtPos.x, Block_.LtPos.y);
	DrawFormatString(0, 320, 0xff000000, "Rb：(%.1f , %.1f)", Block_.RbPos.x, Block_.RbPos.y);

	DrawString(0, 350, "Plane", 0x000000);
	DrawFormatString(0, 370, 0xff000000, "Lt：(%.1f , %.1f)", Plane_.LtPos.x, Plane_.LtPos.y);
	DrawFormatString(0, 390, 0xff000000, "Rb：(%.1f , %.1f)", Plane_.RbPos.x, Plane_.RbPos.y);

	DrawString(0, 420, "Wall", 0x000000);
	DrawFormatString(0, 440, 0xff000000, "Lt：(%.1f , %.1f)", Wall_.LtPos.x, Wall_.LtPos.y);
	DrawFormatString(0, 460, 0xff000000, "Rb：(%.1f , %.1f)", Wall_.RbPos.x, Wall_.RbPos.y);

}

void Stage2D::Release(void)
{
}

VECTOR Stage2D::GetColc(void)
{
	return block_->GetPos();
}

VECTOR Stage2D::GetCol(void)
{
	return Plane_.LtPos;
}

VECTOR Stage2D::GetCold(void)
{
	return Plane_.RbPos;
}

Transform Stage2D::GetTransform(void) const
{
	return transform_;
}
