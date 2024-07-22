#pragma once
#include "../Object/Common/Transform.h"
#include "SceneBase.h"

class SpaceDome;
class Sphere;


class TitleScene : public SceneBase
{

public:
	static constexpr VECTOR Triangle1 = {0.0f,200.0f,0.0f};
	static constexpr VECTOR Triangle2 = {};
	static constexpr VECTOR Triangle3 = {};


	// �R���X�g���N�^
	TitleScene(void);

	// �f�X�g���N�^
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//�X�J�C�h�[���p
	Transform spaceDomeTran_;

	//�X�J�C�h�[��
	SpaceDome* sky_;

	//�v���C���[
	Sphere* Sphere_;

	VECTOR pRot_;

	VECTOR rPos_;


	int cnt_;

	// �^�C�g�����S
	int imgTitleLogo_;

	// �^�C�g�����S2
	int imgTitleLogo2_;

	// ���S�`��
	void DrawLogo(void);

	//��]
	VECTOR Rot(VECTOR localP);

};
