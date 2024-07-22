#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/SpaceDome.h"
#include "../Object/Sphere.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene(void)
{
	imgTutorial_ = -1;
}

TutorialScene::~TutorialScene(void)
{
}

void TutorialScene::Init(void)
{

	// カメラモード：定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// タイトルロゴ
	imgTutorial_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TL_TAB).handleId_;

	//点滅用
	cnt_ = 0;


}

void TutorialScene::Update(void)
{

	// 時間進行
	//cnt_ += SceneManager::GetInstance().GetDeltaTime();
	cnt_++;

	
	//cnt_ = AsoUtility::Deg2RadF(10.0f);

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

}

void TutorialScene::Draw(void)
{

	//背景
	for (int y = 0; y < Application::SCREEN_SIZE_Y; y++)
	{
		for (int x = 0; x < Application::SCREEN_SIZE_X; x++)
		{
			//DrawPixel(x, y, GetColor(255, 255, 255));	//白背景
			//DrawPixel(x, y, GetColor(0,0,0));			//黒背景
		}
	}

	// ロゴ描画
	DrawText();
}

void TutorialScene::Release(void)
{
}

void TutorialScene::DrawText(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;
	int alpha = cy-200;

	SetFontSize(30);

	//タブ
	DrawString(cx-500, alpha, "[R] : リスタート", 0xffffff);
	DrawString(cx-500, alpha+30, "[TAB] : 3D/2Dの切り替え", 0xffffff);
	DrawString(cx-500, alpha+60, "[WASD] : 移動", 0xffffff);
	DrawString(cx-500, alpha+90, "[バックスラッシュ] : ジャンプ", 0xffffff);
	DrawString(cx-500, alpha+120, "[← →] : (2D)視点変更", 0xffffff);

	DrawString(cx-500, alpha+180, "2D画面 : 全体のマップを確認できます", 0xffffff);
	DrawString(cx-500, alpha+230, "3D画面 : ジャンプと移動でゴールを目指します", 0xffffff);


	// Pushメッセージ
	std::string msg = "操作説明";
	SetFontSize(28);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), cy + 200, 0x87cefa, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);
}


