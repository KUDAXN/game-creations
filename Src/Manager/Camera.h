#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"

class Transform;

class Camera
{

public:

	// カメラスピード：NEAR
	static constexpr float SPEED = 1.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_FAR = 15000.0f;

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// カメラ位置から注視点までの相対座標
	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -100.0f, 500.0f };

	//追従対象の位置から追従位置までの相対座標
	static constexpr VECTOR LOCALE_T_POS = { 0.0f,25.0f,0.0f };

	// カメラ位置から注視点までの相対座標(2D)
	//static constexpr VECTOR RELATIVE_C2T_POS_2D_R = { -500.0f, -100.0f, 0.0f };		//右から見る
	//static constexpr VECTOR RELATIVE_C2T_POS_2D_R = { -500.0f, 25.0f, 0.0f };		//右から見る
	//static constexpr VECTOR RELATIVE_C2T_POS_2D_F = { 0.0f, -100.0f, -500.0f };		//前から見る
	//static constexpr VECTOR RELATIVE_C2T_POS_2D_L = { 500.0f, -100.0f, 0.0f };		//左から見る
	//static constexpr VECTOR RELATIVE_C2T_POS_2D_B = { 0.0f, -100.0f, 500.0f };		//後から見る
	static constexpr VECTOR RELATIVE_C2T_POS_2D = { 0.0f, 0.0f, 500.0f };		//-z座標から見る


	// 追従対象からカメラ位置までの相対座標(完全追従)
	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 25.0f, -80.0f };

	// 追従対象からカメラ位置までの相対座標(バネ付き)
	static constexpr VECTOR RELATIVE_F2C_POS_SPRING = { 0.0f, 40.0f, 150.0f };
	
	// 追従対象からカメラ位置までの相対座標(2Dモード)
	//static constexpr VECTOR RELATIVE_F2C_POS_2DMODE_R = { 600.0f, 25.0f, 0.0f };		//右から見る
	//static constexpr VECTOR RELATIVE_F2C_POS_2DMODE_F = { 0.0f, 25.0f, 100.0f };		//前から見る
	//static constexpr VECTOR RELATIVE_F2C_POS_2DMODE_L = { -100.0f, 25.0f, 0.0f };		//左から見る
	//static constexpr VECTOR RELATIVE_F2C_POS_2DMODE_B = { 0.0f, 25.0f, -100.0f };		//後から見る
	static constexpr VECTOR RELATIVE_F2C_POS_2DMODE = { 0.0f, 0.0f, -600.0f };		//-z座標から見る

	//追従位置から注視点までの相対座標(3D)
	static constexpr VECTOR LOCAL_F2T_POS = {0.0f,0.0f,125.0f};

	//追従位置からカメラ位置までの相対座標(3D)
	static constexpr VECTOR LOCAL_F2C_POS = {0.0f,25.0f,-250.0f};

	//カメラのY軸回転量
	static constexpr float SPEED_ROT_DEG_Y = 1.0f;

	// カメラのX回転上限度角
	static constexpr float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = 15.0f * (DX_PI_F / 180.0f);

	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,	// 定点カメラ
		FREE,			//フリーモード
		FOLLOW,			//追従モード
		FOLLOW_SPRING,	//追従モード(バネ付き)
		FOLLOW_2D,		//２Ｄモード
		FOLLOW_3D,		//追従モード(3D)
	};

	Camera(void);
	~Camera(void);

	void Init(void);

	void Update(void);
	void Update3D(void);


	void SetBeforeDraw(void);

	void SetBeforeDrawFixedPoint(void);		//定点カメラ
	void SetBeforeDrawFree(void);			//フリーカメラ
	void SetBeforeDrawFollow(void);			//追従カメラ
	void SetBeforeDrawFollow_Spring(void);	//追従カメラ(ばね付き)

	//追従カメラ：2D
	void SetBeforeDrawFollow_2D(void);

	//追従カメラ：3D
	void SetBeforeDrawFollow_3D(void);
	void ProcessTurn3D(void);

	void Draw(void);
	void Release(void);

	//カメラ位置を取得する
	VECTOR GetPos(void) const;
	//カメラの操作角度
	VECTOR GetAngles(void) const;

	//X回転を抜いたカメラ角度
	Quaternion GetQuaRotOutX(void) const;
	//カメラ回転を取得する
	Quaternion GetRot(void) const;

	//現在のカメラモードを取得する
	MODE GetCameraMode(void) const;

	//スカイドームの設定で使う座標
	VECTOR GetSkyDomePos(void);

	// カメラモードの変更
	void ChangeMode(MODE mode);

	//追従対象の設定
	void SetFollow(const Transform* follow);

	//(デバッグ用)
	void DrawDebug(void);

private:

	//追従対象
	const Transform* followTransform_;

	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_;

	// カメラ角度(rad)
	VECTOR angles_;

	// X軸回転が無い角度
	Quaternion rotOutX_;

	// カメラの角度
	Quaternion rot_;

	// カメラの注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;


	//速度
	VECTOR Velocity_;

	//2D視点移動
	float angle_;

	// カメラを初期位置に戻す
	void SetDefault(void);

	// 追従対象との位置同期を取る
	void SyncFollow(void);

	// カメラ操作
	void ProcessRot(void);

	// 追従対象との位置同期を取る(2D)
	void SyncFollow2D(void);

	// カメラ操作(XZ平面)
	void ProcessRotY(void);


	//回転操作
	void ProcessTurn(float deg);

	//回転量(度)と回転軸で回転処理
	void Turn(float deg, VECTOR axis);

	//動く回転
	void MoveTurn2D(float deg, VECTOR axis, Quaternion gole);

	//回転目標
	Quaternion Turn2DGole(float deg, VECTOR axis);

};

