#include "../../Manager/SceneManager.h"
#include "../../Scene/GameScene.h"
#include "CharBase.h"

CharBase::CharBase(int baseModelId)
{
	baseModelId_ = baseModelId;
}

CharBase::~CharBase(void)
{
}

void CharBase::Init(void)
{
	SetParam();
	Update();
}

void CharBase::SetParam(void)
{
	transform_.SetModel(baseModelId_);

	//float scale = 10.0f;
	float scale = 0.2f;
	transform_.scl = { scale,scale,scale };
	transform_.pos = { 0.0f,3.0f,0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion();

	// 移動スピード
	speed_ = 1.5f;
	
	// 初期は生存状態
	isAlive_ = true;

	// アニメーションをアタッチする
	animAttachNo_ = MV1AttachAnim(transform_.modelId, 10);
	
	// アタッチしているアニメーションの総再生時間を取得する
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);
	
	// 再生中のアニメーション時間
	stepAnim_ = 0.0f;

	// アニメーション速度
	speedAnim_ = 30.0f;

	// HPの設定
	hp_ = hpMax_ = 2;
	
	// 衝突判定用の球体半径
	collisionRadius_ = 35.0f;
	
	// 衝突判定用の球体中心の調整座標
	collisionLocalPos_ = { 0.0f, 50.0f, 0.0f };


	transform_.Update();
}

void CharBase::Update(void)
{
	if (!isAlive_)
	{
		//生存していなければ描画しない
		return;
	}
		//// 弾を移動させる

		////移動量の計算(方向×スピード)
		//VECTOR movePow;
		//movePow = VScale(dir_, speed_);

		////移動処理(座標+移動量)
		//pos_ = VAdd(pos_, movePow);

#pragma region アニメーション処理

		// アニメーション再生
		
		// 経過時間の取得
		float deltaTime = SceneManager::GetInstance().GetDeltaTime();
		
		// アニメーション時間の進行
		stepAnim_ += (speedAnim_ * deltaTime);
		if (stepAnim_ > animTotalTime_)
		{
			// ループ再生
			stepAnim_ = 0.0f;
		}
		
		// 再生するアニメーション時間の設定
		MV1SetAttachAnimTime(baseModelId_, animAttachNo_, stepAnim_);

#pragma endregion

		//// ３Ｄモデルの大きさを設定
		//MV1SetScale(baseModelId_, transform_.scl);

		//// ３Ｄモデルの位置
		//MV1SetPosition(baseModelId_, transform_.pos);

		//// ３Ｄモデルの向き
		//MV1SetRotationXYZ(baseModelId_, transform_.rot);

		transform_.Update();

}

void CharBase::Draw(void)
{
	if (!isAlive_)
	{
		return;
	}

	//デバッグ用:衝突判定の球体描画
	//DrawSphere3D(VAdd(pos_,collisionLocalPos_), collisionRadius_, 10, 0xff0000, 0xff0000, false);

	// ロードされた３Ｄモデルを画面に描画
	MV1DrawModel(transform_.modelId);
}

void CharBase::Release(void)
{
	// ロードされた３Ｄモデルをメモリから解放
	MV1DeleteModel(transform_.modelId);
}

VECTOR CharBase::GetPos(void)
{
	return transform_.pos;
}

void CharBase::SetPos(VECTOR pos)
{
	transform_.pos = pos;
}

bool CharBase::IsAlive(void)
{
	return isAlive_;
}

void CharBase::SetAlive(bool alive)
{
	isAlive_ = alive;
}

void CharBase::Damage(int damage)
{
	hp_ -= damage;
	if (hp_ <= 0)
	{
		SetAlive(false);
	}
}

VECTOR CharBase::GetCollisionPos(void)
{
	return VAdd(transform_.pos, collisionLocalPos_);
}

float CharBase::GetCollisionRadius(void)
{
	return collisionRadius_;
}
