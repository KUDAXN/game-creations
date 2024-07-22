#pragma once
#include "../Object/Common/Transform.h"
#include "SceneBase.h"

class ClearScene : public SceneBase
{
public:


	// �R���X�g���N�^
	ClearScene(void);

	// �f�X�g���N�^
	~ClearScene(void);

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
