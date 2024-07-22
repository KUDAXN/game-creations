#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "Sphere.h"

Sphere::Sphere(void)
{
	pos_ = AsoUtility::VECTOR_ONE;

	rot_ = AsoUtility::VECTOR_ZERO;

	count_ = 0;
}

Sphere::~Sphere(void)
{
}

void Sphere::Init(VECTOR pos)
{
	//pos_ = AsoUtility::VECTOR_ONE;

	//rot_ = AsoUtility::VECTOR_ZERO;

	//count_ = 0;

	//���f������̊�{���
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER));

	float scale = 1.0f;
	transform_.scl = { scale,scale,scale };
	transform_.pos = pos;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion();
	transform_.Update();
}

void Sphere::Update(void)
{
	//pos_.y += sin(10);

	//rot_.y++;

	//MATRIX mRot = MGetIdent();
	//mRot = MMult(mRot, MGetRotY(rot_.y));


	//rot_ = VAdd(rot_, AsoUtility::AXIS_Y);

	////��]����(�ړ��̑O��)
	//ProcessTurn();

	////�O���������擾
	//VECTOR forward = transform_.GetForward();

	////�ړ���(�����~�X�s�[�h)
	//VECTOR movePow = VScale(forward, SPEED_MOVE);

	////�ړ�����(���W�{�ړ���)
	//transform_.pos = VAdd(transform_.pos, movePow);


	transform_.Update();

}

void Sphere::Draw(void)
{
	MV1DrawModel(transform_.modelId);

	//DrawSphere3D(pos_, 80.0f, 8, GetColor(0, 0, 225), GetColor(0, 0, 0), false);
	MV1SetWireFrameDrawFlag(transform_.modelId, true);
}

void Sphere::Release(void)
{
}

const Transform& Sphere::GetTransform(void) const
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return transform_;
}

const void Sphere::SetPos(VECTOR pos)
{
	transform_.pos = pos;
}
