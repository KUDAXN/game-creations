#include "../../Manager/SceneManager.h"
#include "../../Scene/GameScene.h"
#include "CharBase.h"

CharBase::CharBase(int baseModelId)
{
	baseModelId_ = baseModelId;
}

CharBase::~CharBase(void)
{
}

void CharBase::Init(void)
{
	SetParam();
	Update();
}

void CharBase::SetParam(void)
{
	transform_.SetModel(baseModelId_);

	//float scale = 10.0f;
	float scale = 0.2f;
	transform_.scl = { scale,scale,scale };
	transform_.pos = { 0.0f,3.0f,0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion();

	// �ړ��X�s�[�h
	speed_ = 1.5f;
	
	// �����͐������
	isAlive_ = true;

	// �A�j���[�V�������A�^�b�`����
	animAttachNo_ = MV1AttachAnim(transform_.modelId, 10);
	
	// �A�^�b�`���Ă���A�j���[�V�����̑��Đ����Ԃ��擾����
	animTotalTime_ = MV1GetAttachAnimTotalTime(transform_.modelId, animAttachNo_);
	
	// �Đ����̃A�j���[�V��������
	stepAnim_ = 0.0f;

	// �A�j���[�V�������x
	speedAnim_ = 30.0f;

	// HP�̐ݒ�
	hp_ = hpMax_ = 2;
	
	// �Փ˔���p�̋��̔��a
	collisionRadius_ = 35.0f;
	
	// �Փ˔���p�̋��̒��S�̒������W
	collisionLocalPos_ = { 0.0f, 50.0f, 0.0f };


	transform_.Update();
}

void CharBase::Update(void)
{
	if (!isAlive_)
	{
		//�������Ă��Ȃ���Ε`�悵�Ȃ�
		return;
	}
		//// �e���ړ�������

		////�ړ��ʂ̌v�Z(�����~�X�s�[�h)
		//VECTOR movePow;
		//movePow = VScale(dir_, speed_);

		////�ړ�����(���W+�ړ���)
		//pos_ = VAdd(pos_, movePow);

#pragma region �A�j���[�V��������

		// �A�j���[�V�����Đ�
		
		// �o�ߎ��Ԃ̎擾
		float deltaTime = SceneManager::GetInstance().GetDeltaTime();
		
		// �A�j���[�V�������Ԃ̐i�s
		stepAnim_ += (speedAnim_ * deltaTime);
		if (stepAnim_ > animTotalTime_)
		{
			// ���[�v�Đ�
			stepAnim_ = 0.0f;
		}
		
		// �Đ�����A�j���[�V�������Ԃ̐ݒ�
		MV1SetAttachAnimTime(baseModelId_, animAttachNo_, stepAnim_);

#pragma endregion

		//// �R�c���f���̑傫����ݒ�
		//MV1SetScale(baseModelId_, transform_.scl);

		//// �R�c���f���̈ʒu
		//MV1SetPosition(baseModelId_, transform_.pos);

		//// �R�c���f���̌���
		//MV1SetRotationXYZ(baseModelId_, transform_.rot);

		transform_.Update();

}

void CharBase::Draw(void)
{
	if (!isAlive_)
	{
		return;
	}

	//�f�o�b�O�p:�Փ˔���̋��̕`��
	//DrawSphere3D(VAdd(pos_,collisionLocalPos_), collisionRadius_, 10, 0xff0000, 0xff0000, false);

	// ���[�h���ꂽ�R�c���f������ʂɕ`��
	MV1DrawModel(transform_.modelId);
}

void CharBase::Release(void)
{
	// ���[�h���ꂽ�R�c���f����������������
	MV1DeleteModel(transform_.modelId);
}

VECTOR CharBase::GetPos(void)
{
	return transform_.pos;
}

void CharBase::SetPos(VECTOR pos)
{
	transform_.pos = pos;
}

bool CharBase::IsAlive(void)
{
	return isAlive_;
}

void CharBase::SetAlive(bool alive)
{
	isAlive_ = alive;
}

void CharBase::Damage(int damage)
{
	hp_ -= damage;
	if (hp_ <= 0)
	{
		SetAlive(false);
	}
}

VECTOR CharBase::GetCollisionPos(void)
{
	return VAdd(transform_.pos, collisionLocalPos_);
}

float CharBase::GetCollisionRadius(void)
{
	return collisionRadius_;
}
