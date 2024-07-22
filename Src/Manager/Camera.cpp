
#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/Transform.h"
#include "Camera.h"

Camera::Camera(void)
{
	angles_ = VECTOR();
	cameraUp_ = VECTOR();

	mode_ = MODE::NONE;
	pos_ = { 0.0f, 0.0f, 0.0f };
	targetPos_ = { 0.0f, 0.0f, 0.0f };
	rot_ = Quaternion::Identity();

}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	// �J�����̏����ݒ�
	SetDefault();
}

void Camera::Update(void)
{
}

//3D���̃J��������
void Camera::Update3D(void)
{
}

void Camera::SetBeforeDraw(void)
{

	//SetupCamera_Ortho(1000.0f);
	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FOLLOW:
		SetupCamera_Perspective(AsoUtility::Deg2RadF(60.0f));
		SetBeforeDrawFollow();
		break;
	case Camera::MODE::FOLLOW_SPRING:
		//SetupCamera_Perspective(AsoUtility::Deg2RadF(60.0f));
		SetBeforeDrawFollow_Spring();
		break;
	case Camera::MODE::FOLLOW_2D:
		SetupCamera_Ortho(1000.0f);

		//�f�o�b�O�p����
		SetBeforeDrawFollow_2D();
		break;
	case Camera::MODE::FOLLOW_3D:
		SetupCamera_Perspective(AsoUtility::Deg2RadF(60.0f));
		SetBeforeDrawFollow_3D();
		break;
	}

	// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

}

void Camera::SetBeforeDrawFixedPoint(void)
{
	// �������Ȃ�
}

void Camera::SetBeforeDrawFree(void)
{
	auto& ins = InputManager::GetInstance();

	//��]
	VECTOR axisDeg = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_UP)) {axisDeg.x += -1.0f;}
	if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x += 1.0f; }
	if (ins.IsNew(KEY_INPUT_LEFT)) {axisDeg.y += -1.0f;}
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y += 1.0f; }


	//�ړ�
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }

	//�J�������W�𒆐S�Ƃ��āA�����_����]������
	if (!AsoUtility::EqualsVZero(axisDeg))
	{
		//
		Quaternion rotPow;
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.z),AsoUtility::AXIS_Z));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.y),AsoUtility::AXIS_Y));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.x),AsoUtility::AXIS_X));

		//�J�����̉�]�ɍ���
		rot_ = rot_.Mult(rotPow);

		//�����_�̑��΍��W����]������
		VECTOR localRotPos = rot_.PosAxis(RELATIVE_C2T_POS);

		//�J�����ʒu�ɉ�]���������_�̑��΍��W�𑫂�
		//�i�����_�̃��[���h���W�̍X�V�j
		targetPos_ = VAdd(pos_, localRotPos);

		//�J�����̏�����X�V
		cameraUp_ = rot_.GetUp();

	}

	//�ړ�����
	if (!AsoUtility::EqualsVZero(moveDir))
	{
		//�J�����̌����Ă�������ɁA
		//�����_���A�J�����ʒu���ړ�

		//�ړ���������]
		VECTOR moveDirRot = VNorm(rot_.PosAxis(moveDir));
		

		//�ړ���(�����~�X�s�[�h)
		VECTOR movePow = VScale(moveDirRot, SPEED);

		//�ړ�����(���W�{�ړ���)
		pos_ = VAdd(pos_, movePow);

		targetPos_ = VAdd(targetPos_, movePow);


	}

}

void Camera::SetBeforeDrawFollow(void)
{

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = followTransform_->pos;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = followTransform_->quaRot;
	
	// �Ǐ]�Ώۂ���J�����܂ł̑��΍��W
	VECTOR relativeCPos = followRot.PosAxis(RELATIVE_F2C_POS_FOLLOW);
	
	// �J�����ʒu�̍X�V
	pos_ = VAdd(followPos, relativeCPos);
	
	// �J�����ʒu���璍���_�܂ł̑��΍��W
	VECTOR relativeTPos = followRot.PosAxis(RELATIVE_C2T_POS);
	
	// �����_�̍X�V
	targetPos_ = VAdd(pos_, relativeTPos);
	
	// �J�����̏����
	cameraUp_ = followRot.PosAxis(rot_.GetUp());
}

void Camera::SetBeforeDrawFollow_Spring(void)
{

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = followTransform_->pos;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = followTransform_->quaRot;

	//�o�l��`
	float POW_SPRING = 24.0f;
	float dampening = 2.0f * sqrt(POW_SPRING);

	//�f���^�^�C��
	float delta = SceneManager::GetInstance().GetDeltaTime();
	//float delta = 1.0f / 60.0f;

	// �Ǐ]�Ώۂ���J�����܂ł̑��΍��W
	VECTOR relativeCPos = followRot.PosAxis(RELATIVE_F2C_POS_SPRING);

	//���z�̃��[���h���W(�S�[���ƂȂ鍡�܂ł̃J�����ʒu)
	VECTOR idealPos = VAdd(followTransform_->pos, relativeCPos);
	
	// ���ۂƗ��z�̍�
	VECTOR diff = VSub(pos_, idealPos);
	
	// �� = -�o�l�̋��� �~ �o�l�̐L�� - ��R �~ �J�����̑��x
	VECTOR force = VScale(diff, -POW_SPRING);
	force = VSub(force, VScale(Velocity_, dampening));
	

	//�ړ��ʂ̍X�V
	Velocity_ = VAdd(Velocity_, VScale(force, delta));

	// �J�����ʒu�̍X�V
	//pos_ = VAdd(followPos, relativeCPos);
	pos_ = VAdd(pos_, VScale(Velocity_,delta));

	// �J�����ʒu���璍���_�܂ł̑��΍��W
	VECTOR relativeTPos = followRot.PosAxis(RELATIVE_C2T_POS);

	// �����_�̍X�V
	targetPos_ = VAdd(pos_, relativeTPos);

	// �J�����̏����
	cameraUp_ = followRot.PosAxis(rot_.GetUp());

}

#pragma region �J������2D����

void Camera::SetBeforeDrawFollow_2D(void)
{
	ProcessRotY();

	auto& ins = InputManager::GetInstance();

	//�E
	//if (ins.IsTrgDown(KEY_INPUT_RIGHT)) {
	//	Turn(-90.0f, AsoUtility::AXIS_Y);
	//	//Turn2D(-1.0f, AsoUtility::AXIS_Y, Turn2DGole(-90.0f, AsoUtility::AXIS_Y));

	//	//angle_ += AsoUtility::Deg2RadF(-90.0f);
	//}

	//������
	//if (ins.IsTrgDown(KEY_INPUT_LEFT)) {
	//	Turn(90.0f, AsoUtility::AXIS_Y);
	//	//Turn2D(1.0f, AsoUtility::AXIS_Y, Turn2DGole(90.0f, AsoUtility::AXIS_Y));

	//	//angle_ += AsoUtility::Deg2RadF(90.0f);
	//}

	// �Ǐ]����̈ʒu
	//VECTOR followPos = VAdd(followTransform_->pos, LOCALE_T_POS);
	VECTOR followPos = VAdd(VECTOR{}, LOCALE_T_POS);

	//��]�s��̏ꍇ
	MATRIX mat = MGetIdent();
	mat = MMult(mat, MGetRotY(angle_));

	//���΍��W����]
	VECTOR localPos = rot_.PosAxis(RELATIVE_F2C_POS_2DMODE);
	VECTOR localRotPos = VTransform(localPos, mat);

	//���΍��W����_�ƂȂ���W�ɉ����A���[���h���W�ɂ���
	pos_ = VAdd(followPos, localRotPos);

	// �J�����ʒu���璍���_�܂ł̑��΍��W
	VECTOR relativeTPos = rot_.PosAxis(RELATIVE_C2T_POS_2D);

	// �����_�̍X�V
	targetPos_ = VAdd(pos_, relativeTPos);

	// �J�����̏����
	//cameraUp_ = rot_.PosAxis(rot_.GetUp());
	cameraUp_ = AsoUtility::DIR_U;

}


#pragma endregion

#pragma region �J������3D����

void Camera::SetBeforeDrawFollow_3D(void)
{

	////3D���]
	//ProcessTurn(SPEED_ROT_DEG_Y);

	//// �Ǐ]�Ώۂ̈ʒu
	//VECTOR followPos = followTransform_->pos;

	//// �Ǐ]�Ώۂ���J�����܂ł̑��΍��W
	//VECTOR relativeCPos = rot_.PosAxis(RELATIVE_F2C_POS_FOLLOW);

	//// �J�����ʒu�̍X�V
	//pos_ = VAdd(followPos, relativeCPos);

	//// �J�����ʒu���璍���_�܂ł̑��΍��W
	//VECTOR relativeTPos = rot_.PosAxis(RELATIVE_C2T_POS);

	//// �����_�̍X�V
	//targetPos_ = VAdd(pos_, relativeTPos);

	//// �J�����̏����
	//cameraUp_ = rot_.PosAxis(rot_.GetUp());


	// �J��������
	//ProcessRot();

	// �Ǐ]�ΏۂƂ̑��Έʒu�𓯊�
	SyncFollow();

}

#pragma endregion

#pragma region �ۗ�

void Camera::SyncFollow2D(void)
{	// ������̈ʒu
	VECTOR pos = VAdd(followTransform_->pos, LOCALE_T_POS);

	// ���ʂ���ݒ肳�ꂽY�����A��]������
	rot_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);

	VECTOR localPos;

	// �����_(�ʏ�d�͂ł����Ƃ����Y�l��Ǐ]�ΏۂƓ����ɂ���)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// �J�����ʒu
	localPos = rot_.PosAxis(LOCAL_F2C_POS);
	pos_ = VAdd(pos, localPos);

	// �J�����̏����
	cameraUp_ = AsoUtility::DIR_U;
}

void Camera::ProcessRotY(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_RIGHT)) { angles_.y += SPEED; }
	if (ins.IsTrgDown(KEY_INPUT_LEFT)) { angles_.y -= SPEED; }

}

#pragma endregion

#pragma region ���g�p


//��]����
void Camera::MoveTurn2D(float deg, VECTOR axis, Quaternion gole)
{

	for (float i = deg*-1; i < 90.0f; i++)
	{

	//�@�f�O���[�����W�A���ɕϊ�
	float rad = AsoUtility::Deg2RadF(deg);

	//���W�A�����N�H�[�^�j�I���ɕϊ�
	//	(��]����遁��]�ʂƎ����g�p)
	//	(���L�[�����������Ƃɂ�鍡��̉�]�ʂ����)
	Quaternion rotPow;
	rotPow = Quaternion::AngleAxis(rad, axis);

	//�B���@�̉�]�ɉ�]��������(��������)
	rot_ = rot_.Mult(rotPow);


	}

}

#pragma endregion 

void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angles_;
}

Quaternion Camera::GetQuaRotOutX(void) const
{
	return rotOutX_;
}

Quaternion Camera::GetRot(void) const
{
	return rot_;
}

Camera::MODE Camera::GetCameraMode(void) const
{
	return mode_;
}

VECTOR Camera::GetSkyDomePos(void)
{
	VECTOR sPos;

	sPos = VAdd(pos_, {-100.0f,500.0f});

	return pos_;
}

void Camera::ChangeMode(MODE mode)
{

	// �J�����̏����ݒ�
	SetDefault();

	// �J�������[�h�̕ύX
	mode_ = mode;

	// �ύX���̏���������
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FOLLOW:
		break;
	case Camera::MODE::FOLLOW_SPRING:
		break;
	case Camera::MODE::FOLLOW_2D:
		break;
	case Camera::MODE::FOLLOW_3D:
		break;
	}

}

void Camera::SetDefault(void)
{

	// �J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;

	// �����_
	targetPos_ = VAdd(pos_, RELATIVE_C2T_POS);

	// �J�����̏����
	cameraUp_ = AsoUtility::DIR_U;

	
	angles_.x = AsoUtility::Deg2RadF(30.0f);
	angles_.y = 0.0f;
	angles_.z = 0.0f;

	angle_ = 0.0f;

	rot_ = Quaternion::Identity();

}

void Camera::SyncFollow(void)
{

	// ������̈ʒu
	VECTOR pos = followTransform_->pos;

	// ���ʂ���ݒ肳�ꂽY�����A��]������
	rotOutX_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);

	// ���ʂ���ݒ肳�ꂽX�����A��]������
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	VECTOR localPos;

	// �����_(�ʏ�d�͂ł����Ƃ����Y�l��Ǐ]�ΏۂƓ����ɂ���)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// �J�����ʒu
	localPos = rot_.PosAxis(LOCAL_F2C_POS);
	pos_ = VAdd(pos, localPos);

	// �J�����̏����
	cameraUp_ = AsoUtility::DIR_U;
}

void Camera::ProcessRot(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_RIGHT)) { angles_.y += SPEED / 60.0f; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { angles_.y -= SPEED / 60.0f; }
	if (LIMIT_X_UP_RAD > angles_.x) {
		if (ins.IsNew(KEY_INPUT_UP)) { angles_.x += SPEED / 60.0f; }
	}
	if (-LIMIT_X_DW_RAD < angles_.x) {
		if (ins.IsNew(KEY_INPUT_DOWN)) { angles_.x -= SPEED / 60.0f; }
	}
}

//Turn�֐����g���ĉ�]����
void Camera::ProcessTurn(float deg)
{
	auto& ins = InputManager::GetInstance();

	//�E����
	if (ins.IsNew(KEY_INPUT_RIGHT)) {
		Turn(deg, AsoUtility::AXIS_Y);
	}

	//������
	if (ins.IsNew(KEY_INPUT_LEFT)) {
		Turn(-deg, AsoUtility::AXIS_Y);
	}
}

//��]����
void Camera::Turn(float deg, VECTOR axis)
{
	//�@�f�O���[�����W�A���ɕϊ�
	float rad = AsoUtility::Deg2RadF(deg);

	//���W�A�����N�H�[�^�j�I���ɕϊ�
	//	(��]����遁��]�ʂƎ����g�p)
	//	(���L�[�����������Ƃɂ�鍡��̉�]�ʂ����)
	Quaternion rotPow;
	rotPow = Quaternion::AngleAxis(rad, axis);

	//�B���@�̉�]�ɉ�]��������(��������)
	rot_ = rot_.Mult(rotPow);
}

//debug�pUI
void Camera::DrawDebug(void)
{
	VECTOR followPos = VAdd(followTransform_->pos, LOCALE_T_POS);

	DrawFormatString(0, 10, 0xff0000, "�J�������W�@�F(%.1f, %.1f, %.1f)", pos_.x, pos_.y, pos_.z);

	DrawFormatString(0, 30, 0x00ff00, "�Ǐ]�Ώۍ��W�F(%.1f, %.1f, %.1f)", followPos.x, followPos.y, followPos.z);

	DrawFormatString(0, 50, 0x0000ff, "�����_���W�F(%.1f, %.1f, %.1f)", targetPos_.x, targetPos_.y, targetPos_.z);

	DrawFormatString(Application::SCREEN_SIZE_X - 400, 30, 0xffffff, "�J����Rot�F(%.1f,%.1f,%.1f)", rot_.x, rot_.y, rot_.z);
	DrawFormatString(Application::SCREEN_SIZE_X - 400, 50, 0xffffff, "�J����Angle�F(%.1f,%.1f,%.1f)", angles_.x, angles_.y, angles_.z);

	//DrawFormatString(0, 30, 0x00ff00, "�J�����p�x�@�F(%.1f, %.1f, %.1f)", AsoUtility::Rad2DegD(rot_.x), AsoUtility::Rad2DegD(rot_.y), AsoUtility::Rad2DegD(rot_.z));

	
}

void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}
