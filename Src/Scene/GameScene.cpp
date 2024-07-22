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
	//�J����
	Camera* camera = SceneManager::GetInstance().GetCamera();


	//�J�������[�h�F�t���[�J����
	//camera->ChangeMode(Camera::MODE::FREE);


	grid_ = new Grid();
	grid_->Init();
	
	//�v���C���[�V�b�v
	player_ = new Player();
	player_->Init();

	//�X�e�[�W
	stage_ = new Stage(*player_);
	stage_->Init();
	
	//�X�e�[�W(2D�p)
	stage2_ = new Stage2D(*player_);
	stage2_->Init();

	//�X�J�C�h�[��
	skyDome_ = new SpaceDome(player_->GetTransform().pos);
	skyDome_->Init();
	
	skyDomeMap_ = new SpaceDome(camera->GetSkyDomePos());
	skyDomeMap_->Init();
	

	//�ŏ��̓J�����͂QD���[�h����n�܂�
	cameraMode_ = -1;

	//�ŏ��͍�������
	isMiddle_ = false;

	//�J����(�e�X�g)
	camera->SetFollow(&player_->GetTransform());
	camera->ChangeMode(Camera::MODE::FOLLOW_2D);

}

void GameScene::Update(void)
{
	//grid_->Update();

	//�J����
	Camera* camera = SceneManager::GetInstance().GetCamera();

	//�J�������[�h�F2D��3D�̐؂�ւ�
	camera->SetFollow(&player_->GetTransform());

	auto& ins = InputManager::GetInstance();

	//�I�[�g�Z�[�u
	OrtSave();

	//���[�h�؂�ւ�
	if (ins.IsTrgDown(KEY_INPUT_TAB)) {

		cameraMode_ *= -1;

	}		
	if (cameraMode_ == -1)
	{
		//�J�������[�h�F2D
		camera->ChangeMode(Camera::MODE::FOLLOW_2D);
		skyDome_->Update(VECTOR{-player_->GetTransform().pos.x,-player_->GetTransform().pos.y,-player_->GetTransform().pos.z});
		stage2_->Update();


		// �X�e�[�W�̓����蔻����v���C���[�ɐݒ�(2D��)
		player_->ClearCollider();
		player_->AddCollider(stage2_->GetTransform().collider);

	}
	else
	{
		//�J�������[�h�F�Ǐ]�J����
		camera->ChangeMode(Camera::MODE::FOLLOW_3D);
		//�J�������[�h�F�Ǐ]�J����
		skyDome_->Update(VECTOR{});
		stage_->Update();


		// �X�e�[�W�̓����蔻����v���C���[�ɐݒ�
		player_->ClearCollider();
		player_->AddCollider(stage_->GetTransform().collider);

	}

	Collision(cameraMode_);


	player_->Update(camera->GetCameraMode());

	if (ins.IsTrgDown(KEY_INPUT_R))
	{
		ReSpawn();
	}



	// �V�[���J��
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
		//�J�������[�h�F2D
		skyDomeMap_->Draw();
		stage2_->Draw();
	}
	else
	{
		//�J�������[�h�F�Ǐ]�J����
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

	//�RD�̏ꍇ�̓����蔻��
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

	//���X�|�[��
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


	////�O���b�h��
	//// XY��{��(�O���b�h)
	//int num;
	//int ham = 50;
	//Vector2 sPos;
	//Vector2 ePos;
	//for (int i = -Application::SCREEN_SIZE_X / 2; i < Application::SCREEN_SIZE_X/2; i++)
	//{

	//	num = (i);

	//	// X��(��)
	//	sPos = { 0, num * ham };
	//	ePos = { Application::SCREEN_SIZE_X, num * ham };
	//	DrawLine(sPos.x,sPos.y, ePos.x,ePos.y, 0xff0000);

	//	// Y��(��)
	//	sPos = { num * ham, 0};
	//	ePos = { num * ham, Application::SCREEN_SIZE_Y };
	//	DrawLine(sPos.x,sPos.y, ePos.x,ePos.y, 0x0000ff);


	//}

	//�f�o�b�O�p������

	//�v���C���[���
	DrawFormatString(0, 70, 0x9900ff, "�v���C���[���W�F(%.1f,%.1f,%.1f)", player_->GetTransform().pos.x, player_->GetTransform().pos.y, player_->GetTransform().pos.z);

	//���݂̃J�������[�h
	DrawFormatString(Application::SCREEN_SIZE_X - 400.0f, 10, 0xffffff, "�J�������[�h�F(%dD)", (int)camera->GetCameraMode() - 3);

	camera->DrawDebug();
}

