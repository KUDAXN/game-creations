#pragma once
#include "../Object/Common/Transform.h"
#include "SceneBase.h"

class SpaceDome;
class Sphere;


class TutorialScene : public SceneBase
{

public:

	// �R���X�g���N�^
	TutorialScene(void);

	// �f�X�g���N�^
	~TutorialScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//�X�J�C�h�[���p
	Transform spaceDomeTran_;

	//�`���[�g���A����ʕ`��
	int imgTutorial_;

	//�_�ŗp�ϐ�
	int cnt_;

	// ���S�`��
	void DrawText(void);

};
