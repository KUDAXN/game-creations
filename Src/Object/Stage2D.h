#pragma once
#include<vector>
#include "Common/Transform.h"
#include "StageObject/Block.h"

class Player;

class Stage2D
{
	//線の長さ
	static constexpr float LEN = 500.0f;

public:

	//ブロック位置情報
	struct pBlock
	{
		VECTOR cPos = {};
		VECTOR LtPos = {};
		VECTOR RbPos = {};
	};

	//struct name
	//{
	//	pBlock 	Block_;
	//	pBlock	Plane_;
	//	pBlock	Wall_;
	//};

	//ブロック名
	enum class BLOCK_NAME
	{
		BLOCK,
		PLANE,
		WALL,
	};

	//ステージ名
	enum class STAGE_NAME
	{
		MAIN_STAGE,
	};

	// コンストラクタ
	Stage2D(Player& player);

	// デストラクタ
	~Stage2D(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	VECTOR GetColc(void);
	VECTOR GetCol(void);
	VECTOR GetCold(void);

	Transform GetTransform(void) const;

private:

	//リセット地点のエフェクト
	int effectResetResId_;
	int effectResetPlayId_;

	//追従対象
	//const Transform& follow_;

	//プレイヤー
	Player& player_;

	//ブロック
	std::unique_ptr < Block > block_;

	// モデル制御の基本情報
	Transform transform_;

	//オブジェクトの当たり判定用座標
	float objBlock_[2][2];
	float objPlane_[2][2];
	float objWall_[2][2];
	pBlock 	Block_;
	pBlock	Plane_;
	pBlock	Wall_;

	//デバッグ
	void DrawDebug(void);
};


