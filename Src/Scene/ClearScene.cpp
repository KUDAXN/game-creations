
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "ClearScene.h"

ClearScene::ClearScene(void)
{
	Logo_ = -1;
}

ClearScene::~ClearScene(void)
{
}

void ClearScene::Init(void)
{

	//ƒƒS
	Logo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::GAMECLEAR).handleId_;
}

void ClearScene::Update(void)
{

	// ƒV[ƒ“‘JˆÚ
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

}

void ClearScene::Draw(void)
{
	//”’”wŒi
	for (int y = 0; y < Application::SCREEN_SIZE_Y; y++)
	{
		for (int x = 0; x < Application::SCREEN_SIZE_X; x++)
		{
			//DrawPixel(x, y, GetColor(255, 255, 255));
			//DrawPixel(x, y, GetColor(0, 0, 0));
		}
	}
	DrawLogo();

}

void ClearScene::Release(void)
{
}

void ClearScene::DrawLogo(void)
{
	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;


	DrawRotaGraph(cx, cy,1.0, 0.0, Logo_, true);

	// PushƒƒbƒZ[ƒW
	std::string msg = "Push Space To Title";
	SetFontSize(28);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), cy + 200, 0x87cefa, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);
}
