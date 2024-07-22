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
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	imgTitleLogo_ = -1;
	imgTitleLogo2_ = -1;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{

	// �J�������[�h�F��_�J����
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// �^�C�g�����S
	imgTitleLogo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE).handleId_;
	// �^�C�g�����S2
	imgTitleLogo2_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE2).handleId_;

	//sky_ = new SpaceDome(spaceDomeTran_.pos);
	//sky_->Init();

	//����
	Sphere_ = new Sphere();
	Sphere_->Init({ -100,-220,0.0f });
	Sphere_->SetPos({-100,-220,0.0f });
	
	pRot_ = AsoUtility::VECTOR_ZERO;

	rPos_ = AsoUtility::VECTOR_ZERO;

	cnt_ = 0;

}

void TitleScene::Update(void)
{

	// ���Ԑi�s
	//cnt_ += SceneManager::GetInstance().GetDeltaTime();
	cnt_++;

	
	//cnt_ = AsoUtility::Deg2RadF(10.0f);

	//���g�݋���
	Sphere_->Update();
	
	//pRot_.y++;

	//MATRIX mRot = MGetIdent();
	//mRot = MMult(mRot, MGetRotY(pRot_.y));

	pRot_.y = sin( static_cast<int>(cnt_)/10%50);

	
	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TUTORIAL);
	}

}

void TitleScene::Draw(void)
{

	//���w�i
	for (int y = 0; y < Application::SCREEN_SIZE_Y; y++)
	{
		for (int x = 0; x < Application::SCREEN_SIZE_X; x++)
		{
			//DrawPixel(x, y, GetColor(255, 255, 255));
			DrawPixel(x, y, GetColor(0,0,0));
		}
	}

	// ���S�`��
	DrawLogo();

	Sphere_->Draw();
}

void TitleScene::Release(void)
{
}

void TitleScene::DrawLogo(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// �^�C�g�����S2
	DrawRotaGraph(
		cx, cy ,
		1.0f, 0.0f, imgTitleLogo2_, true);

	// �^�C�g�����S
	DrawRotaGraph(
		cx, cy - 40,
		1.0f, 0.0f, imgTitleLogo_, true);

	// Push���b�Z�[�W
	std::string msg = "Push Space";
	SetFontSize(28);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);

	if (cnt_/30% 2 == 0)
	{
		DrawFormatString(cx - (width / 2), cy + 200, 0x87cefa, msg.c_str());
	}
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);
}

VECTOR TitleScene::Rot(VECTOR localP)
{

	// �Ǐ]����̍��W���擾
	VECTOR pos = rPos_;


	//��]�s��̏ꍇ
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotY(cnt_));
	mat = MMult(mat, MGetRotX(cnt_));

	//���΍��W����]
	VECTOR localPos = localP;
	VECTOR localRotPos = VTransform(localPos, mat);

	//���΍��W����_�ƂȂ���W�ɉ����A���[���h���W�ɂ���
	VECTOR rot = VAdd(pos, localRotPos);

	return rot;

}
