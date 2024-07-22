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

	// �J�������[�h�F��_�J����
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// �^�C�g�����S
	imgTutorial_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TL_TAB).handleId_;

	//�_�ŗp
	cnt_ = 0;


}

void TutorialScene::Update(void)
{

	// ���Ԑi�s
	//cnt_ += SceneManager::GetInstance().GetDeltaTime();
	cnt_++;

	
	//cnt_ = AsoUtility::Deg2RadF(10.0f);

	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

}

void TutorialScene::Draw(void)
{

	//�w�i
	for (int y = 0; y < Application::SCREEN_SIZE_Y; y++)
	{
		for (int x = 0; x < Application::SCREEN_SIZE_X; x++)
		{
			//DrawPixel(x, y, GetColor(255, 255, 255));	//���w�i
			//DrawPixel(x, y, GetColor(0,0,0));			//���w�i
		}
	}

	// ���S�`��
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

	//�^�u
	DrawString(cx-500, alpha, "[R] : ���X�^�[�g", 0xffffff);
	DrawString(cx-500, alpha+30, "[TAB] : 3D/2D�̐؂�ւ�", 0xffffff);
	DrawString(cx-500, alpha+60, "[WASD] : �ړ�", 0xffffff);
	DrawString(cx-500, alpha+90, "[�o�b�N�X���b�V��] : �W�����v", 0xffffff);
	DrawString(cx-500, alpha+120, "[�� ��] : (2D)���_�ύX", 0xffffff);

	DrawString(cx-500, alpha+180, "2D��� : �S�̂̃}�b�v���m�F�ł��܂�", 0xffffff);
	DrawString(cx-500, alpha+230, "3D��� : �W�����v�ƈړ��ŃS�[����ڎw���܂�", 0xffffff);


	// Push���b�Z�[�W
	std::string msg = "�������";
	SetFontSize(28);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), cy + 200, 0x87cefa, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);
}


