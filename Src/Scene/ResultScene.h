#pragma once
#include "../Object/Common/Transform.h"
#include "SceneBase.h"

class ResultScene : public SceneBase
{
public:


	// �R���X�g���N�^
	ResultScene(void);

	// �f�X�g���N�^
	~ResultScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//���S
	int Logo_;

	// ���S�`��
	void DrawLogo(void);
};
