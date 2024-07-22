#pragma once
#include <memory>
#include "../Common/Transform.h"
#include "../../Manager/Camera.h"
#include "CharBase.h"

class Quaternion;
class AnimationController;
class Collider;
class Capsule;
class Stage;

class Player
{
public:

	static constexpr float TIME_ROT = 1.0f;

	// 移動スピード
	static constexpr float SPEED_MOVE = 1.5f;

	// 自機の回転量
	static constexpr float SPEED_ROT_DEG_Y = 1.0f;
	static constexpr float SPEED_ROT_DEG_X = 1.0f;

	//重力の大きさ
	static constexpr float GRAVITY = 9.81f;

	//質量
	static constexpr float MASS = 0.5f;

	// ジャンプ力
	//static constexpr float POW_JUMP = 20.0f;
	static constexpr float POW_JUMP = 11.0f;

	// ジャンプ受付時間
	//static constexpr float TIME_JUMP_IN = 0.3f;
	static constexpr float TIME_JUMP_IN = 0.4f;


	//アニメーション
	enum class ANIM_TYPE
	{
		IDLE,	//待機
		WALK,	//歩行
		JUMP,	//ジャンプ
	};

	// コンストラクタ
	Player(void);

	// デストラクタ
	~Player(void);
	
	void Init(void);
	void Update(const Camera::MODE& cameraMode);
	void Update2D(void);
	void Update3D(void);
	void Draw(void);
	void Release(void);

	void SetPos(VECTOR pos);

	const Transform& GetTransform(void) const;

	// 衝突判定に用いられるコライダ制御
	void AddCollider(std::weak_ptr<Collider> collider);
	void ClearCollider(void);

	//衝突用カプセルの取得
	const Capsule& GetCapsule(void) const;

private:

	// モデル制御の基本情報
	Transform transform_;

	//ステージ
	std::unique_ptr<Stage> stage_;

	//アニメーション
	std::unique_ptr<AnimationController> animationController_;

	//状態管理
	//ANIM_TYPE state_;

	//ジャンプ量
	VECTOR jumpPow_;

	// 移動方向
	VECTOR moveDir_;

	// 移動量
	VECTOR movePow_;

	// 移動後の座標
	VECTOR movedPos_;

	//重力
	float gravityPow_;

	// 回転
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	// ジャンプ判定
	bool isJump_;

	// ジャンプの入力受付時間
	float stepJump_;

	//衝突チェック
	VECTOR gravHitPosUp_;
	VECTOR gravHitPosDown_;

	// 衝突判定に用いられるコライダ
	std::vector<std::weak_ptr<Collider>> colliders_;

	//当たり判定
	std::unique_ptr<Capsule> capsule_;


	//アニメーションの初期化
	void InitAnimation(void);

	//自動3D
	void ProcessTurn(void);
	void Turn(float deg, VECTOR axis);
	void Process(void);
	
	//3D操作
	void Process3D(void);
	void Turn3D( VECTOR axis);
	//void Process3D(void);

	//2D操作
	void ProcessMove2D(void);
	void Process2D(void);
	void Gravity2D(void);
	void Collision2D(void);

	// 回転
	void SetGoalRotate(double rotRad);

	void Rotate(void);
	Quaternion Rotate2D(void);

	void LazyRotation(float goalRot);

	//重力
	void Gravity(void);

	//ジャンプ処理
	void ProcessJump(void);

	//衝突判定
	void Collision(void);
	void CollisionGravity(void);

	//カプセルと衝突物の当たり判定
	void CollisionCapsule(void);
 

	// 着地モーション終了
	bool IsEndLanding(void);

};

