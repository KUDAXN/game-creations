#pragma once
#include "SceneBase.h"

class Grid;
class Stage;
class Stage2D;
class SpaceDome;
class Player;

class GameScene : public SceneBase
{

public:

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//グリッド線
	Grid* grid_;

	//ステージ
	Stage* stage_;
	Stage2D* stage2_;
	
	//スカイドーム
	SpaceDome* skyDome_;
	SpaceDome* skyDomeMap_;

	//プレイヤーシップ
	Player* player_;

	//カメラモードの切り替えフラグ
	int cameraMode_;

	//中央までこれたか
	bool isMiddle_;

	//衝突判定
	void Collision(int mode);

	bool DeathCheck(void);

	//ゲームクリア
	bool isClear(void);

	void ReSpawn(void);

	void OrtSave(void);

	//デバッグ用関数
	void DrawDebug(void);

};

