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

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//�O���b�h��
	Grid* grid_;

	//�X�e�[�W
	Stage* stage_;
	Stage2D* stage2_;
	
	//�X�J�C�h�[��
	SpaceDome* skyDome_;
	SpaceDome* skyDomeMap_;

	//�v���C���[�V�b�v
	Player* player_;

	//�J�������[�h�̐؂�ւ��t���O
	int cameraMode_;

	//�����܂ł��ꂽ��
	bool isMiddle_;

	//�Փ˔���
	void Collision(int mode);

	bool DeathCheck(void);

	//�Q�[���N���A
	bool isClear(void);

	void ReSpawn(void);

	void OrtSave(void);

	//�f�o�b�O�p�֐�
	void DrawDebug(void);

};

