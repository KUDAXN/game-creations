
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "ResultScene.h"

ResultScene::ResultScene(void)
{
	Logo_ = -1;
}

ResultScene::~ResultScene(void)
{
}

void ResultScene::Init(void)
{
	//ロゴ
	Logo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAMEOVER).handleId_;
}

void ResultScene::Update(void)
{

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	
	if (ins.IsTrgDown(KEY_INPUT_R))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}
	

}

void ResultScene::Draw(void)
{
	//DrawString(100, 100, "RESULT", GetColor(255, 255, 255));

	DrawLogo();
}

void ResultScene::Release(void)
{
}

void ResultScene::DrawLogo(void)
{
	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;


	DrawRotaGraph(cx, cy, 1.0, 0.0, Logo_, true);

	// Pushメッセージ
	std::string msg = "[Space] To Title     [R] Reset";
	SetFontSize(28);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), cy + 200, 0x87cefa, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);
}
