
#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/Transform.h"
#include "Camera.h"

Camera::Camera(void)
{
	angles_ = VECTOR();
	cameraUp_ = VECTOR();

	mode_ = MODE::NONE;
	pos_ = { 0.0f, 0.0f, 0.0f };
	targetPos_ = { 0.0f, 0.0f, 0.0f };
	rot_ = Quaternion::Identity();

}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	// カメラの初期設定
	SetDefault();
}

void Camera::Update(void)
{
}

//3D時のカメラ操作
void Camera::Update3D(void)
{
}

void Camera::SetBeforeDraw(void)
{

	//SetupCamera_Ortho(1000.0f);
	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FOLLOW:
		SetupCamera_Perspective(AsoUtility::Deg2RadF(60.0f));
		SetBeforeDrawFollow();
		break;
	case Camera::MODE::FOLLOW_SPRING:
		//SetupCamera_Perspective(AsoUtility::Deg2RadF(60.0f));
		SetBeforeDrawFollow_Spring();
		break;
	case Camera::MODE::FOLLOW_2D:
		SetupCamera_Ortho(1000.0f);

		//デバッグ用操作
		SetBeforeDrawFollow_2D();
		break;
	case Camera::MODE::FOLLOW_3D:
		SetupCamera_Perspective(AsoUtility::Deg2RadF(60.0f));
		SetBeforeDrawFollow_3D();
		break;
	}

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

}

void Camera::SetBeforeDrawFixedPoint(void)
{
	// 何もしない
}

void Camera::SetBeforeDrawFree(void)
{
	auto& ins = InputManager::GetInstance();

	//回転
	VECTOR axisDeg = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_UP)) {axisDeg.x += -1.0f;}
	if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x += 1.0f; }
	if (ins.IsNew(KEY_INPUT_LEFT)) {axisDeg.y += -1.0f;}
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y += 1.0f; }


	//移動
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }

	//カメラ座標を中心として、注視点を回転させる
	if (!AsoUtility::EqualsVZero(axisDeg))
	{
		//
		Quaternion rotPow;
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.z),AsoUtility::AXIS_Z));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.y),AsoUtility::AXIS_Y));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.x),AsoUtility::AXIS_X));

		//カメラの回転に合成
		rot_ = rot_.Mult(rotPow);

		//注視点の相対座標を回転させる
		VECTOR localRotPos = rot_.PosAxis(RELATIVE_C2T_POS);

		//カメラ位置に回転した注視点の相対座標を足す
		//（注視点のワールド座標の更新）
		targetPos_ = VAdd(pos_, localRotPos);

		//カメラの上方向更新
		cameraUp_ = rot_.GetUp();

	}

	//移動処理
	if (!AsoUtility::EqualsVZero(moveDir))
	{
		//カメラの向いている方向に、
		//注視点も、カメラ位置も移動

		//移動方向を回転
		VECTOR moveDirRot = VNorm(rot_.PosAxis(moveDir));
		

		//移動量(方向×スピード)
		VECTOR movePow = VScale(moveDirRot, SPEED);

		//移動処理(座標＋移動量)
		pos_ = VAdd(pos_, movePow);

		targetPos_ = VAdd(targetPos_, movePow);


	}

}

void Camera::SetBeforeDrawFollow(void)
{

	// 追従対象の位置
	VECTOR followPos = followTransform_->pos;

	// 追従対象の向き
	Quaternion followRot = followTransform_->quaRot;
	
	// 追従対象からカメラまでの相対座標
	VECTOR relativeCPos = followRot.PosAxis(RELATIVE_F2C_POS_FOLLOW);
	
	// カメラ位置の更新
	pos_ = VAdd(followPos, relativeCPos);
	
	// カメラ位置から注視点までの相対座標
	VECTOR relativeTPos = followRot.PosAxis(RELATIVE_C2T_POS);
	
	// 注視点の更新
	targetPos_ = VAdd(pos_, relativeTPos);
	
	// カメラの上方向
	cameraUp_ = followRot.PosAxis(rot_.GetUp());
}

void Camera::SetBeforeDrawFollow_Spring(void)
{

	// 追従対象の位置
	VECTOR followPos = followTransform_->pos;

	// 追従対象の向き
	Quaternion followRot = followTransform_->quaRot;

	//バネ定義
	float POW_SPRING = 24.0f;
	float dampening = 2.0f * sqrt(POW_SPRING);

	//デルタタイム
	float delta = SceneManager::GetInstance().GetDeltaTime();
	//float delta = 1.0f / 60.0f;

	// 追従対象からカメラまでの相対座標
	VECTOR relativeCPos = followRot.PosAxis(RELATIVE_F2C_POS_SPRING);

	//理想のワールド座標(ゴールとなる今までのカメラ位置)
	VECTOR idealPos = VAdd(followTransform_->pos, relativeCPos);
	
	// 実際と理想の差
	VECTOR diff = VSub(pos_, idealPos);
	
	// 力 = -バネの強さ × バネの伸び - 抵抗 × カメラの速度
	VECTOR force = VScale(diff, -POW_SPRING);
	force = VSub(force, VScale(Velocity_, dampening));
	

	//移動量の更新
	Velocity_ = VAdd(Velocity_, VScale(force, delta));

	// カメラ位置の更新
	//pos_ = VAdd(followPos, relativeCPos);
	pos_ = VAdd(pos_, VScale(Velocity_,delta));

	// カメラ位置から注視点までの相対座標
	VECTOR relativeTPos = followRot.PosAxis(RELATIVE_C2T_POS);

	// 注視点の更新
	targetPos_ = VAdd(pos_, relativeTPos);

	// カメラの上方向
	cameraUp_ = followRot.PosAxis(rot_.GetUp());

}

#pragma region カメラの2D操作

void Camera::SetBeforeDrawFollow_2D(void)
{
	ProcessRotY();

	auto& ins = InputManager::GetInstance();

	//右
	//if (ins.IsTrgDown(KEY_INPUT_RIGHT)) {
	//	Turn(-90.0f, AsoUtility::AXIS_Y);
	//	//Turn2D(-1.0f, AsoUtility::AXIS_Y, Turn2DGole(-90.0f, AsoUtility::AXIS_Y));

	//	//angle_ += AsoUtility::Deg2RadF(-90.0f);
	//}

	//左旋回
	//if (ins.IsTrgDown(KEY_INPUT_LEFT)) {
	//	Turn(90.0f, AsoUtility::AXIS_Y);
	//	//Turn2D(1.0f, AsoUtility::AXIS_Y, Turn2DGole(90.0f, AsoUtility::AXIS_Y));

	//	//angle_ += AsoUtility::Deg2RadF(90.0f);
	//}

	// 追従相手の位置
	//VECTOR followPos = VAdd(followTransform_->pos, LOCALE_T_POS);
	VECTOR followPos = VAdd(VECTOR{}, LOCALE_T_POS);

	//回転行列の場合
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotY(angle_));

	//相対座標を回転
	VECTOR localPos = rot_.PosAxis(RELATIVE_F2C_POS_2DMODE);
	VECTOR localRotPos = VTransform(localPos, mat);

	//相対座標を基点となる座標に加え、ワールド座標にする
	pos_ = VAdd(followPos, localRotPos);

	// カメラ位置から注視点までの相対座標
	VECTOR relativeTPos = rot_.PosAxis(RELATIVE_C2T_POS_2D);

	// 注視点の更新
	targetPos_ = VAdd(pos_, relativeTPos);

	// カメラの上方向
	//cameraUp_ = rot_.PosAxis(rot_.GetUp());
	cameraUp_ = AsoUtility::DIR_U;

}


#pragma endregion

#pragma region カメラの3D操作

void Camera::SetBeforeDrawFollow_3D(void)
{

	////3D回回転
	//ProcessTurn(SPEED_ROT_DEG_Y);

	//// 追従対象の位置
	//VECTOR followPos = followTransform_->pos;

	//// 追従対象からカメラまでの相対座標
	//VECTOR relativeCPos = rot_.PosAxis(RELATIVE_F2C_POS_FOLLOW);

	//// カメラ位置の更新
	//pos_ = VAdd(followPos, relativeCPos);

	//// カメラ位置から注視点までの相対座標
	//VECTOR relativeTPos = rot_.PosAxis(RELATIVE_C2T_POS);

	//// 注視点の更新
	//targetPos_ = VAdd(pos_, relativeTPos);

	//// カメラの上方向
	//cameraUp_ = rot_.PosAxis(rot_.GetUp());


	// カメラ操作
	//ProcessRot();

	// 追従対象との相対位置を同期
	SyncFollow();

}

#pragma endregion

#pragma region 保留

void Camera::SyncFollow2D(void)
{	// 同期先の位置
	VECTOR pos = VAdd(followTransform_->pos, LOCALE_T_POS);

	// 正面から設定されたY軸分、回転させる
	rot_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis(LOCAL_F2C_POS);
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = AsoUtility::DIR_U;
}

void Camera::ProcessRotY(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_RIGHT)) { angles_.y += SPEED; }
	if (ins.IsTrgDown(KEY_INPUT_LEFT)) { angles_.y -= SPEED; }

}

#pragma endregion

#pragma region 未使用


//回転処理
void Camera::MoveTurn2D(float deg, VECTOR axis, Quaternion gole)
{

	for (float i = deg*-1; i < 90.0f; i++)
	{

	//①デグリーをラジアンに変換
	float rad = AsoUtility::Deg2RadF(deg);

	//ラジアンをクォータニオンに変換
	//	(回転を作る＝回転量と軸を使用)
	//	(矢印キーを押したことによる今回の回転量を作る)
	Quaternion rotPow;
	rotPow = Quaternion::AngleAxis(rad, axis);

	//③自機の回転に回転を加える(合成する)
	rot_ = rot_.Mult(rotPow);


	}

}

#pragma endregion 

void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angles_;
}

Quaternion Camera::GetQuaRotOutX(void) const
{
	return rotOutX_;
}

Quaternion Camera::GetRot(void) const
{
	return rot_;
}

Camera::MODE Camera::GetCameraMode(void) const
{
	return mode_;
}

VECTOR Camera::GetSkyDomePos(void)
{
	VECTOR sPos;

	sPos = VAdd(pos_, {-100.0f,500.0f});

	return pos_;
}

void Camera::ChangeMode(MODE mode)
{

	// カメラの初期設定
	SetDefault();

	// カメラモードの変更
	mode_ = mode;

	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FOLLOW:
		break;
	case Camera::MODE::FOLLOW_SPRING:
		break;
	case Camera::MODE::FOLLOW_2D:
		break;
	case Camera::MODE::FOLLOW_3D:
		break;
	}

}

void Camera::SetDefault(void)
{

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = VAdd(pos_, RELATIVE_C2T_POS);

	// カメラの上方向
	cameraUp_ = AsoUtility::DIR_U;

	
	angles_.x = AsoUtility::Deg2RadF(30.0f);
	angles_.y = 0.0f;
	angles_.z = 0.0f;

	angle_ = 0.0f;

	rot_ = Quaternion::Identity();

}

void Camera::SyncFollow(void)
{

	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	// 正面から設定されたY軸分、回転させる
	rotOutX_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis(LOCAL_F2C_POS);
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = AsoUtility::DIR_U;
}

void Camera::ProcessRot(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_RIGHT)) { angles_.y += SPEED / 60.0f; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { angles_.y -= SPEED / 60.0f; }
	if (LIMIT_X_UP_RAD > angles_.x) {
		if (ins.IsNew(KEY_INPUT_UP)) { angles_.x += SPEED / 60.0f; }
	}
	if (-LIMIT_X_DW_RAD < angles_.x) {
		if (ins.IsNew(KEY_INPUT_DOWN)) { angles_.x -= SPEED / 60.0f; }
	}
}

//Turn関数を使って回転操作
void Camera::ProcessTurn(float deg)
{
	auto& ins = InputManager::GetInstance();

	//右旋回
	if (ins.IsNew(KEY_INPUT_RIGHT)) {
		Turn(deg, AsoUtility::AXIS_Y);
	}

	//左旋回
	if (ins.IsNew(KEY_INPUT_LEFT)) {
		Turn(-deg, AsoUtility::AXIS_Y);
	}
}

//回転処理
void Camera::Turn(float deg, VECTOR axis)
{
	//①デグリーをラジアンに変換
	float rad = AsoUtility::Deg2RadF(deg);

	//ラジアンをクォータニオンに変換
	//	(回転を作る＝回転量と軸を使用)
	//	(矢印キーを押したことによる今回の回転量を作る)
	Quaternion rotPow;
	rotPow = Quaternion::AngleAxis(rad, axis);

	//③自機の回転に回転を加える(合成する)
	rot_ = rot_.Mult(rotPow);
}

//debug用UI
void Camera::DrawDebug(void)
{
	VECTOR followPos = VAdd(followTransform_->pos, LOCALE_T_POS);

	DrawFormatString(0, 10, 0xff0000, "カメラ座標　：(%.1f, %.1f, %.1f)", pos_.x, pos_.y, pos_.z);

	DrawFormatString(0, 30, 0x00ff00, "追従対象座標：(%.1f, %.1f, %.1f)", followPos.x, followPos.y, followPos.z);

	DrawFormatString(0, 50, 0x0000ff, "注視点座標：(%.1f, %.1f, %.1f)", targetPos_.x, targetPos_.y, targetPos_.z);

	DrawFormatString(Application::SCREEN_SIZE_X - 400, 30, 0xffffff, "カメラRot：(%.1f,%.1f,%.1f)", rot_.x, rot_.y, rot_.z);
	DrawFormatString(Application::SCREEN_SIZE_X - 400, 50, 0xffffff, "カメラAngle：(%.1f,%.1f,%.1f)", angles_.x, angles_.y, angles_.z);

	//DrawFormatString(0, 30, 0x00ff00, "カメラ角度　：(%.1f, %.1f, %.1f)", AsoUtility::Rad2DegD(rot_.x), AsoUtility::Rad2DegD(rot_.y), AsoUtility::Rad2DegD(rot_.z));

	
}

void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}
