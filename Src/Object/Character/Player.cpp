#include <string>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Common/Collider.h"
#include"../Common/Capsule.h"
#include"../Stage.h"
#include "Player.h"

Player::Player(void)
{
	stage_ = nullptr;
	animationController_ = nullptr;

	isJump_ = false;
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	auto& resouce = ResourceManager::GetInstance();

	//モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::HUMAN));

	//float scale = 10.0f;
	float scale = 0.2f;
	transform_.scl = { scale,scale,scale };
	//transform_.pos = { 0.0f,0.0f,0.0f };
	//transform_.pos = { 580.0f,300.0f,0.0f };
	transform_.pos = { -600.0f,0.0f,0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion();
	transform_.quaRotLocal = transform_.GetDir({0.0f,AsoUtility::Deg2RadF(180.0f),0.0f});
	transform_.Update();

	//衝突チェック
	gravHitPosUp_ = AsoUtility::VECTOR_ZERO;
	gravHitPosDown_ = AsoUtility::VECTOR_ZERO;

	//アニメーションの初期化
	InitAnimation();

	//状態
	//state_ = ANIM_TYPE::IDLE;

	capsule_ = std::make_unique<Capsule>(transform_);
	capsule_->SetLocalPosTop({0.0f,30.0f,0.0f});
	capsule_->SetLocalPosDown({0.0f,7.0f,0.0f});
	capsule_->SetRadius(7.0f);
}

void Player::Update(const Camera::MODE& cameraMode)
{

	//カメラモードによって移動方法を変える
	switch (cameraMode)
	{
	case Camera::MODE::FOLLOW_2D:

		Update2D();

		// 現在座標を起点に移動後座標を決める
		movedPos_ = VAdd(transform_.pos, movePow_);

		// 移動
		transform_.pos = movedPos_;

		//Gravity2D();

		break;

	case Camera::MODE::FOLLOW_3D:

		Update3D();

		break;

	default:
		break;
	}

	Collision();

	//重力による移動
	Gravity();

	//ジャンプ
	ProcessJump();


	auto& ins = InputManager::GetInstance();

	//アニメーションの再生
	animationController_->Update();

	transform_.Update();

}

void Player::Update2D(void)
{

	//Process2D();
	ProcessMove2D();

	//カメラの角度に応じた回転
	Rotate2D();

	//回転させる
	transform_.quaRot = Quaternion();
	transform_.quaRot = transform_.quaRot.Mult(Rotate2D());
}

void Player::Update3D(void)
{
	Process3D();

	// 移動方向に応じた回転
	Rotate();

	//回転させる
	transform_.quaRot = Quaternion();
	transform_.quaRot = transform_.quaRot.Mult(playerRotY_);
}

void Player::Draw(void)
{
	MV1DrawModel(transform_.modelId);

	//デバッグ用

	//DrawFormatString(0, 110, 0x00ff99, "アニメーション経過時間：(%.1f)", stepAnim_);
	//capsule_->Draw();

}

void Player::Release(void)
{
}

void Player::SetPos(VECTOR pos)
{
	transform_.pos = pos;
}

const Transform& Player::GetTransform(void) const
{
	// TODO: return ステートメントをここに挿入します
	return transform_;
}

void Player::AddCollider(std::weak_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void Player::ClearCollider(void)
{
	colliders_.clear();
}

const Capsule& Player::GetCapsule(void) const
{
	return *capsule_;
}

void Player::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "Anim_Idle.mv1",20.0f);
	animationController_->Add((int)ANIM_TYPE::WALK, path + "Walk.mv1",30.0f);
	//animationController_->Add((int)ANIM_TYPE::JUMP, path + "Jumping.mv1",20.0f);
	animationController_->Add((int)ANIM_TYPE::JUMP, path + "Jumping.mv1",40.0f);

	animationController_->Play((int)ANIM_TYPE::IDLE);
}

#pragma region 自動3D

void Player::ProcessTurn(void)
{
	auto& ins = InputManager::GetInstance();

	//右旋回
	if (ins.IsNew(KEY_INPUT_RIGHT)) {
		Turn(SPEED_ROT_DEG_Y,AsoUtility::AXIS_Y);
	}
	
	//左旋回
	if (ins.IsNew(KEY_INPUT_LEFT)) {
		Turn(-SPEED_ROT_DEG_Y,AsoUtility::AXIS_Y);
	}
	
	if (ins.IsNew(KEY_INPUT_UP)) {
		Turn(-SPEED_ROT_DEG_X,AsoUtility::AXIS_X);
	}
	
	if (ins.IsNew(KEY_INPUT_DOWN)) {
		Turn(SPEED_ROT_DEG_X,AsoUtility::AXIS_X);
	}

}

void Player::Turn(float deg, VECTOR axis)
{
	//①デグリーをラジアンに変換
	float rad = AsoUtility::Deg2RadF(deg);

	//ラジアンをクォータニオンに変換
	//	(回転を作る＝回転量と軸を使用)
	//	(矢印キーを押したことによる今回の回転量を作る)
	Quaternion rotPow;
	rotPow = Quaternion::AngleAxis(rad, axis);

	//③自機の回転に回転を加える(合成する)
	transform_.quaRot = Quaternion::Euler({ 0.0f,1.0f,0.0f });
	transform_.quaRot = transform_.quaRot.Mult(rotPow);

}

void Player::Process(void)
{
	auto& ins = InputManager::GetInstance();
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	//回転操作(移動の前に)
	ProcessTurn();

	//前方方向を取得
	VECTOR forward = transform_.GetForward();

	//移動量(方向×スピード)
	VECTOR movePow = VScale(forward, SPEED_MOVE);

	//移動処理(座標＋移動量)
	transform_.pos = VAdd(transform_.pos, movePow);


	transform_.Update();
}

#pragma endregion

#pragma region 3D操作

void Player::Process3D(void)
{

	auto& ins = InputManager::GetInstance();

	//カメラ
	Camera* camera = SceneManager::GetInstance().GetCamera();

	//X軸回転を除いた、重力方向に垂直なカメラ(XZ平面)を取得
	Quaternion cameraRot = camera->GetQuaRotOutX();

	//移動量をゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;

	VECTOR dir = AsoUtility::VECTOR_ZERO;
	double rotRad = 0.0;


	//移動方向を取得
	//カメラ方向の左右移動
	if (ins.IsNew(KEY_INPUT_A)) {
		dir = cameraRot.GetLeft();
		rotRad = AsoUtility::Deg2RadD(-90.0);
	}
	if (ins.IsNew(KEY_INPUT_D)) {
		dir = cameraRot.GetRight();
		rotRad = AsoUtility::Deg2RadD(90.0);
	}
	
	//カメラ方向の前後移動
	if (ins.IsNew(KEY_INPUT_W)) {
		dir = cameraRot.GetForward();
		rotRad = AsoUtility::Deg2RadD(0.0);
	}
	if (ins.IsNew(KEY_INPUT_S)) {
		dir = cameraRot.GetBack();
		rotRad = AsoUtility::Deg2RadD(180.0);
		//dir = transform_.GetBack();
	}

	//Turn3D(dir);

	if (!AsoUtility::EqualsVZero(dir) && (isJump_ || IsEndLanding()))
	{

		moveDir_ = dir;
		movePow_ = VScale(dir, SPEED_MOVE);

		//移動量(方向×スピード)
		//VECTOR movePow = VScale(dir, SPEED_MOVE);

		//移動処理(座標＋移動量)
		//transform_.pos = VAdd(transform_.pos, movePow);

		SetGoalRotate(rotRad);


		if (!isJump_ && IsEndLanding())
		{
			//歩くアニメーションの再生
			animationController_->Play((int)ANIM_TYPE::WALK);
		}
	}
	else
	{
		if (!isJump_ && IsEndLanding())
		{

			//待機アニメーションの再生
			animationController_->Play((int)ANIM_TYPE::IDLE);
		}
	}

	//transform_.Update();
}

void Player::SetGoalRotate(double rotRad)
{
	//カメラ
	Camera* camera = SceneManager::GetInstance().GetCamera();

	//現在のカメラ角度(rad)
	VECTOR cameraRot = camera->GetAngles();

	//目標とする角度を求める
	Quaternion axis =
		Quaternion::AngleAxis(
			(double)cameraRot.y + rotRad, AsoUtility::AXIS_Y);

	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}
	goalQuaRot_ = axis;
}

void Player::Rotate(void)
{
	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();
	// 回転の球面補間
	//playerRotY_ = Quaternion::Slerp(playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
	playerRotY_ = Quaternion::Slerp(playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

Quaternion Player::Rotate2D(void)
{
	//カメラ
	Camera* camera = SceneManager::GetInstance().GetCamera();

	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();

	// 回転の球面補間
	return  Quaternion::Slerp(camera->GetRot(), goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);

}

void Player::Turn3D( VECTOR axis)
{
	////①デグリーをラジアンに変換
	//float rad = AsoUtility::Deg2RadF(deg);

	////ラジアンをクォータニオンに変換
	////	(回転を作る＝回転量と軸を使用)
	////	(矢印キーを押したことによる今回の回転量を作る)
	//Quaternion rotPow;
	//rotPow = Quaternion::AngleAxis(rad, axis);

	//-----------------------------------------------------------------------------

	Quaternion rotPow;
	//rotPow = rotPow.Mult(
	//	Quaternion::AngleAxis(
	//		AsoUtility::Deg2RadF(axis.z), AsoUtility::AXIS_Z));
	rotPow = rotPow.Mult(
		Quaternion::AngleAxis(
			AsoUtility::Deg2RadF(axis.y), AsoUtility::AXIS_Y));
	//rotPow = rotPow.Mult(
	//	Quaternion::AngleAxis(
	//		AsoUtility::Deg2RadF(axis.x), AsoUtility::AXIS_X));

	//------------------------------------------------------------------------------

	//		// XYZの回転行列
	//	// XZ平面移動にする場合は、XZの回転を考慮しないようにする
	//MATRIX mat = MGetIdent();
	////mat = MMult(mat, MGetRotX(angles_.x));
	//mat = MMult(mat, MGetRotY(cameraAngles.y));
	////mat = MMult(mat, MGetRotZ(angles_.z));

	//// 回転行列を使用して、ベクトルを回転させる
	//VECTOR moveDir = VTransform(axis, mat);

	//// 方向×スピードで移動量を作って、座標に足して移動
	//pos_ = VAdd(pos_, VScale(moveDir, movePow));

	//// 方向を角度に変換する(XZ平面 Y軸)
	//float angle = atan2f(dir.x, dir.z);
	//// カメラの角度を基準とし、方向分の角度を加える
	////rot_.y = cameraAngles.y + angle;

	//// カメラの角度を基準とし、方向分の角度へゆっくり近づく
	//LazyRotation(cameraAngles.y + angle);

	//---------------------------------------------------------------------------------

	//③自機の回転に回転を加える(合成する)
	transform_.quaRot = transform_.quaRot.Mult(rotPow);
}

#pragma endregion

#pragma region 2Dモードの操作

//移動方向
void Player::ProcessMove2D(void)
{

	//カメラ
	Camera* camera = SceneManager::GetInstance().GetCamera();

	auto& ins = InputManager::GetInstance();

	//移動量をゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;
	VECTOR dir = AsoUtility::VECTOR_ZERO;
	double rotRad = 0.0;

	//画面右に移動
	if (ins.IsNew(KEY_INPUT_D)) {


		dir = VECTOR{ 1.0f,0.0f,0.0f };

		//dir = camera->GetRot().GetForward();
		//double rad = camera->GetAngles().y + AsoUtility::Deg2RadD(90.0);

		//auto cameraRot = Quaternion::ToEuler( camera->GetRot());
		auto cameraRot = camera->GetRot().GetForward();

		VECTOR pRot = Quaternion::ToEuler(playerRotY_);

		auto rot = AsoUtility::Deg2RadD(90.0);


		rotRad = rot;

	}
	//左に移動
	if (ins.IsNew(KEY_INPUT_A)) {

		dir = VECTOR{ -1.0f,0.0f,0.0f };

		//dir = camera->GetRot().GetBack();
		rotRad = AsoUtility::Deg2RadD(-90.0);

	}

	if (!AsoUtility::EqualsVZero(dir))
	{

		moveDir_ = dir;
		movePow_ = VScale(dir, SPEED_MOVE);

		//移動量(方向×スピード)
		//VECTOR movePow = VScale(dir, SPEED_MOVE);

		//移動処理(座標＋移動量)
		//transform_.pos = VAdd(transform_.pos, movePow);

		SetGoalRotate(rotRad);

		//歩くアニメーションの再生
		animationController_->Play((int)ANIM_TYPE::WALK);
	}
	else
	{
		//待機アニメーションの再生
		animationController_->Play((int)ANIM_TYPE::IDLE);
	}


}

void Player::Process2D(void)
{
	////移動方向
	//VECTOR moveDir = ProcessMove2D();

	////移動量(方向×スピード)
	//VECTOR movePow = VScale(moveDir, SPEED_MOVE);


	////移動方向
	//moveDir_ = ProcessMove2D();

	//movePow_ = VScale(moveDir_, SPEED_MOVE);

	////移動処理(座標＋移動量)
	//transform_.pos = VAdd(transform_.pos, movePow);


	//transform_.Update();
}

void Player::Gravity2D(void)
{
	//下方向への重力
	jumpPow_.y -= GRAVITY * SceneManager::GetInstance().GetDeltaTime();

	//float speed;
	//transform_.pos = VAdd(transform_.pos, VScale(transform_.rot, speed));

	//重力方向
	VECTOR dirGravity = AsoUtility::DIR_D;

	//重力
	VECTOR grv = VScale(AsoUtility::DIR_D, GRAVITY);

	// メンバ変数 jumpPow_ に重力計算を行う(加速度)
	jumpPow_ = VAdd(jumpPow_, grv);

	//内積
	float dot = VDot(dirGravity, jumpPow_);

	if (dot >= 0.0f)
	{
		//重力方向と反対方向(マイナス)でなければ、ジャンプ力をなくす
		jumpPow_ = grv;
	}

	Collision2D();

}

void Player::Collision2D(void)
{
	//足元の当たり判定
		//設置判定(足元の衝突判定)

	//足元座標(中心)
	VECTOR footPos = transform_.pos;	//中心
	//Vector2 footPos = pos_.ToVector2();	//中心
	//footPos.y += SIZE_Y / 2;			//足元中央

	//足元座標(右)
	//Vector2 footPosR = GetColPos(COL_LR::R, COL_TD::D);
	//Vector2 footPosR = pos_.ToVector2();
	//footPosR.y += 16;

	//足元座標(左)
	//Vector2 footPosL = GetColPos(COL_LR::L, COL_TD::D);
	//Vector2 footPosL = pos_.ToVector2();
	//footPosL.y -= 16;

	//if (sceneGame_->IsCollisionStage(footPos)
	//	|| sceneGame_->IsCollisionStage(footPosR)
	//	|| sceneGame_->IsCollisionStage(footPosL))
	//if (sceneGame_->IsCollisionStage(footPos))
	//{
	//	//当たり判定箇所のどこかが壁にめり込んだ時
	//	//①足元の中心座標からマップのどこにいるかを探す
	//	Vector2 mapPos = sceneGame_->World2MapPos(footPos);
	//	//②めり込んだブロックの上
	//	transform_.pos.y = mapPos.y * Stage::CHIP_SIZE_Y - 1 - hitBox_.y - hitPos_.y;

	//	//地面にいる場合は
	//	isJump_ = false;
	//	SetJumpPower(0.0f);

	//	// 接地したらジャンプカウンタを元に戻す
	//	cntJumpInput_ = 0;

	//}

	
}

#pragma endregion

//重力処理
void Player::Gravity(void)
{
	//float speed;
	//transform_.pos = VAdd(transform_.pos, VScale(transform_.rot, speed));

	//重力方向
	VECTOR dirGravity = AsoUtility::DIR_D;

	//重力
	VECTOR grv = VScale(dirGravity, GRAVITY);

	// メンバ変数 jumpPow_ に重力計算を行う(加速度)
	jumpPow_ = VAdd(jumpPow_, grv);

	//内積
	float dot = VDot(dirGravity, jumpPow_);

	if (dot >= 0.0f)
	{
		//重力方向と反対方向(マイナス)でなければ、ジャンプ力をなくす
		jumpPow_ = grv;
	}

}

void Player::ProcessJump(void)
{
	bool isHit = CheckHitKey(KEY_INPUT_BACKSLASH);
	//bool isHit = CheckHitKey(KEY_INPUT_RSHIFT);

	// ジャンプ
	if (isHit && (isJump_ || IsEndLanding()))
	{
		if (!isJump_)
		{
			// 制御無しジャンプ
			//animationController_->Play((int)ANIM_TYPE::JUMP);
			// この後、いくつかのジャンプパターンを試します1

			// ループしないジャンプ
			//animationController_->Play((int)ANIM_TYPE::JUMP,false);

			//切り取りアニメーション
			//animationController_->Play(
			//	(int)ANIM_TYPE::JUMP, false, 13.0f, 24.0f);

			//無理やりアニメーション
			animationController_->Play(
				(int)ANIM_TYPE::JUMP, true, 13.0f, 25.0f);

			animationController_->SetEndLoop(23.0f, 25.0f, 20.0f);

		}

		isJump_ = true;

		// ジャンプの入力受付時間をヘラス 
		stepJump_ += SceneManager::GetInstance().GetDeltaTime();

		if (stepJump_ < TIME_JUMP_IN)
		{
			jumpPow_ = VScale(AsoUtility::DIR_U, POW_JUMP);
		}
	}

	// ボタンを離したらジャンプ力に加算しない
	if (!isHit)
	{
		stepJump_ = TIME_JUMP_IN;
	}

}

void Player::Collision(void)
{
	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);

	//衝突(カプセル)
	CollisionCapsule();

	// 衝突(重力)
	CollisionGravity();

	// 移動
	transform_.pos = movedPos_;
}

void Player::CollisionGravity(void)
{
	// ジャンプ量を加算
	movedPos_ = VAdd(movedPos_, jumpPow_);

	// 重力方向
	VECTOR dirGravity = AsoUtility::DIR_D;

	// 重力方向の反対
	VECTOR dirUpGravity = AsoUtility::DIR_U;

	// 重力の強さ
	float gravityPow = GRAVITY;

	//プレイヤーのひざ元から足裏下までの線分(レイ)を作る
	float checkPow = 10.0f;
	//始点
	gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * 2.0f));
	//終点
	gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
	for (const auto c : colliders_)
	{
		// 地面との衝突
		auto hit = MV1CollCheck_Line(c.lock()->modelId_, -1, gravHitPosUp_, gravHitPosDown_);
		//if (hit.HitFlag > 0)
		if (hit.HitFlag > 0 && VDot(dirGravity, jumpPow_) > 0.9f)
		{
			// 地面と衝突している
			// 押し戻し処理とジャンプ力の打ち消しを実装しましょう

			//衝突地点から、少し上に移動
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));

			//ジャンプリセット
			jumpPow_ = AsoUtility::VECTOR_ZERO;
			stepJump_ = 0.0f;

			if (isJump_)
			{
				// 着地モーション
				//animationController_->Play(
				//	(int)ANIM_TYPE::JUMP, false, 29.0f, 45.0f, false, true);
				// 着地モーション
				animationController_->Play(
					(int)ANIM_TYPE::JUMP, false, 30.0f, 40.0f, false, true);
			}

			isJump_ = false;

			//VECTOR vec = VSub(gravHitPosDown_, hit.HitPosition);
			//movedPos_ = VAdd(movedPos_,VScale(vec,-1.0f));
			//jumpPow_ = VAdd(jumpPow_, VScale(dirUpGravity,gravityPow));
		}
	}
}

void Player::CollisionCapsule(void)
{
	// カプセルを移動させる
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = Capsule(*capsule_, trans);

	// カプセルとの衝突判定
	for (const auto c : colliders_)
	{
		auto hits = MV1CollCheck_Capsule(
			c.lock()->modelId_, -1,
			cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());

		// 衝突した複数のポリゴンと衝突回避するまで、
		// プレイヤーの位置を移動させる
		for (int i = 0; i < hits.HitNum; i++)
		{
			auto hit = hits.Dim[i];

			// 地面と異なり、衝突回避位置が不明なため、何度か移動させる
			// この時、移動させる方向は、移動前座標に向いた方向であったり、
			// 衝突したポリゴンの法線方向だったりする
			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{

				// 再度、モデル全体と衝突検出するには、効率が悪過ぎるので、
				// 最初の衝突判定で検出した衝突ポリゴン1枚と衝突判定を取る
				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(),
					hit.Position[0], hit.Position[1], hit.Position[2]);
				if (pHit)
				{

					// 法線の方向にちょっとだけ移動させる
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));

					// カプセルも一緒に移動させる
					trans.pos = movedPos_;
					trans.Update();
					continue;
				}

				break;
			}
		}

		// 検出した地面ポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(hits);
	}
}

bool Player::IsEndLanding(void)
{
	bool ret = true;

	// アニメーションがジャンプではない
	if (animationController_->GetPlayType() != (int)ANIM_TYPE::JUMP)
	{
		return ret;
	}
	// アニメーションが終了しているか
	if (animationController_->IsEnd())
	{
		return ret;
	}

	return false;
}

