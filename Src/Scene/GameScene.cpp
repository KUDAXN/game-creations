#include <string>
#include <DxLib.h>
#include"../Application.h"
#include"../Utility/AsoUtility.h"
#include"../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include"../Object/Grid.h"
#include"../Object/Stage.h"
#include"../Object/Stage2D.h"
#include "../Object/SpaceDome.h"
#include "../Object/Character/Player.h"
#include "GameScene.h"

GameScene::GameScene(void)
{

}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	//カメラ
	Camera* camera = SceneManager::GetInstance().GetCamera();


	//カメラモード：フリーカメラ
	//camera->ChangeMode(Camera::MODE::FREE);


	grid_ = new Grid();
	grid_->Init();
	
	//プレイヤーシップ
	player_ = new Player();
	player_->Init();

	//ステージ
	stage_ = new Stage(*player_);
	stage_->Init();
	
	//ステージ(2D用)
	stage2_ = new Stage2D(*player_);
	stage2_->Init();

	//スカイドーム
	skyDome_ = new SpaceDome(player_->GetTransform().pos);
	skyDome_->Init();
	
	skyDomeMap_ = new SpaceDome(camera->GetSkyDomePos());
	skyDomeMap_->Init();
	

	//最初はカメラは２Dモードから始まる
	cameraMode_ = -1;

	//最初は左側から
	isMiddle_ = false;

	//カメラ(テスト)
	camera->SetFollow(&player_->GetTransform());
	camera->ChangeMode(Camera::MODE::FOLLOW_2D);

}

void GameScene::Update(void)
{
	//grid_->Update();

	//カメラ
	Camera* camera = SceneManager::GetInstance().GetCamera();

	//カメラモード：2Dと3Dの切り替え
	camera->SetFollow(&player_->GetTransform());

	auto& ins = InputManager::GetInstance();

	//オートセーブ
	OrtSave();

	//モード切り替え
	if (ins.IsTrgDown(KEY_INPUT_TAB)) {

		cameraMode_ *= -1;

	}		
	if (cameraMode_ == -1)
	{
		//カメラモード：2D
		camera->ChangeMode(Camera::MODE::FOLLOW_2D);
		skyDome_->Update(VECTOR{-player_->GetTransform().pos.x,-player_->GetTransform().pos.y,-player_->GetTransform().pos.z});
		stage2_->Update();


		// ステージの当たり判定をプレイヤーに設定(2D時)
		player_->ClearCollider();
		player_->AddCollider(stage2_->GetTransform().collider);

	}
	else
	{
		//カメラモード：追従カメラ
		camera->ChangeMode(Camera::MODE::FOLLOW_3D);
		//カメラモード：追従カメラ
		skyDome_->Update(VECTOR{});
		stage_->Update();


		// ステージの当たり判定をプレイヤーに設定
		player_->ClearCollider();
		player_->AddCollider(stage_->GetTransform().collider);

	}

	Collision(cameraMode_);


	player_->Update(camera->GetCameraMode());

	if (ins.IsTrgDown(KEY_INPUT_R))
	{
		ReSpawn();
	}



	// シーン遷移
	if (DeathCheck())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	}
	
	if (isClear())
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::CLEAR);
	}


}

void GameScene::Draw(void)
{
	if (cameraMode_ == -1)
	{
		//カメラモード：2D
		skyDomeMap_->Draw();
		stage2_->Draw();
	}
	else
	{
		//カメラモード：追従カメラ
		skyDome_->Draw();
		stage_->Draw();
	}

	player_->Draw();

	//DrawDebug();

}

void GameScene::Release(void)
{
}

void GameScene::Collision(int cameraMode)
{
	//MV1CollCheck_Sphere

	//３Dの場合の当たり判定
	if (cameraMode == 1)
	{

	}

}


bool GameScene::DeathCheck(void)
{

	if (player_->GetTransform().pos.y <= -1000.0f)
	{
		return true;
	}

	return false;
}

bool GameScene::isClear(void)
{
	if (player_->GetTransform().pos.x >= 600.0f) {
		return true;
	}

	return false;
}

void GameScene::ReSpawn(void)
{

	//リスポーン
	if (isMiddle_)
	{
		player_->SetPos({ 100.0f, 0.0f, 0.0f });
	}
	else
	{
		player_->SetPos({ -600.0f, 0.0f, -10.0f });
	}

}

void GameScene::OrtSave(void)
{

	if (player_->GetTransform().pos.x > 0.0f)
	{
		isMiddle_ = true;
	}

}

void GameScene::DrawDebug(void)
{
	Camera* camera = SceneManager::GetInstance().GetCamera();

	VECTOR to, Rd;
	to = stage_->GetCol();
	Rd = stage_->GetCold();

	to = VScale(to, 50);
	Rd = VScale(Rd, 30);

	VECTOR go = { 550.0f,400,0.0f };

	auto c = VAdd(go, to);
	auto d = VAdd(go, Rd);

	auto a = VSub(c, player_->GetTransform().pos);
	auto b = VSub(d, player_->GetTransform().pos);

	//DrawBox(a.x, a.y, b.x, b.y, 0xff0000, false);

	DrawFormatString(Application::SCREEN_SIZE_X - 200.0f, 10, 0x000000, "Lt:(%.1f,%.1f)", a.x, a.y);
	DrawFormatString(Application::SCREEN_SIZE_X - 200.0f, 30, 0x000000, "Lt:(%.1f,%.1f)", c.x, c.y);


	////グリッド線
	//// XY基本軸(グリッド)
	//int num;
	//int ham = 50;
	//Vector2 sPos;
	//Vector2 ePos;
	//for (int i = -Application::SCREEN_SIZE_X / 2; i < Application::SCREEN_SIZE_X/2; i++)
	//{

	//	num = (i);

	//	// X軸(赤)
	//	sPos = { 0, num * ham };
	//	ePos = { Application::SCREEN_SIZE_X, num * ham };
	//	DrawLine(sPos.x,sPos.y, ePos.x,ePos.y, 0xff0000);

	//	// Y軸(青)
	//	sPos = { num * ham, 0};
	//	ePos = { num * ham, Application::SCREEN_SIZE_Y };
	//	DrawLine(sPos.x,sPos.y, ePos.x,ePos.y, 0x0000ff);


	//}

	//デバッグ用文字列

	//プレイヤー情報
	DrawFormatString(0, 70, 0x9900ff, "プレイヤー座標：(%.1f,%.1f,%.1f)", player_->GetTransform().pos.x, player_->GetTransform().pos.y, player_->GetTransform().pos.z);

	//現在のカメラモード
	DrawFormatString(Application::SCREEN_SIZE_X - 400.0f, 10, 0xffffff, "カメラモード：(%dD)", (int)camera->GetCameraMode() - 3);

	camera->DrawDebug();
}

