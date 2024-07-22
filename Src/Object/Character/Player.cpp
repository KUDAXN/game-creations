#include <string>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Common/Collider.h"
#include"../Common/Capsule.h"
#include"../Stage.h"
#include "Player.h"

Player::Player(void)
{
	stage_ = nullptr;
	animationController_ = nullptr;

	isJump_ = false;
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	auto& resouce = ResourceManager::GetInstance();

	//���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::HUMAN));

	//float scale = 10.0f;
	float scale = 0.2f;
	transform_.scl = { scale,scale,scale };
	//transform_.pos = { 0.0f,0.0f,0.0f };
	//transform_.pos = { 580.0f,300.0f,0.0f };
	transform_.pos = { -600.0f,0.0f,0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion();
	transform_.quaRotLocal = transform_.GetDir({0.0f,AsoUtility::Deg2RadF(180.0f),0.0f});
	transform_.Update();

	//�Փ˃`�F�b�N
	gravHitPosUp_ = AsoUtility::VECTOR_ZERO;
	gravHitPosDown_ = AsoUtility::VECTOR_ZERO;

	//�A�j���[�V�����̏�����
	InitAnimation();

	//���
	//state_ = ANIM_TYPE::IDLE;

	capsule_ = std::make_unique<Capsule>(transform_);
	capsule_->SetLocalPosTop({0.0f,30.0f,0.0f});
	capsule_->SetLocalPosDown({0.0f,7.0f,0.0f});
	capsule_->SetRadius(7.0f);
}

void Player::Update(const Camera::MODE& cameraMode)
{

	//�J�������[�h�ɂ���Ĉړ����@��ς���
	switch (cameraMode)
	{
	case Camera::MODE::FOLLOW_2D:

		Update2D();

		// ���ݍ��W���N�_�Ɉړ�����W�����߂�
		movedPos_ = VAdd(transform_.pos, movePow_);

		// �ړ�
		transform_.pos = movedPos_;

		//Gravity2D();

		break;

	case Camera::MODE::FOLLOW_3D:

		Update3D();

		break;

	default:
		break;
	}

	Collision();

	//�d�͂ɂ��ړ�
	Gravity();

	//�W�����v
	ProcessJump();


	auto& ins = InputManager::GetInstance();

	//�A�j���[�V�����̍Đ�
	animationController_->Update();

	transform_.Update();

}

void Player::Update2D(void)
{

	//Process2D();
	ProcessMove2D();

	//�J�����̊p�x�ɉ�������]
	Rotate2D();

	//��]������
	transform_.quaRot = Quaternion();
	transform_.quaRot = transform_.quaRot.Mult(Rotate2D());
}

void Player::Update3D(void)
{
	Process3D();

	// �ړ������ɉ�������]
	Rotate();

	//��]������
	transform_.quaRot = Quaternion();
	transform_.quaRot = transform_.quaRot.Mult(playerRotY_);
}

void Player::Draw(void)
{
	MV1DrawModel(transform_.modelId);

	//�f�o�b�O�p

	//DrawFormatString(0, 110, 0x00ff99, "�A�j���[�V�����o�ߎ��ԁF(%.1f)", stepAnim_);
	//capsule_->Draw();

}

void Player::Release(void)
{
}

void Player::SetPos(VECTOR pos)
{
	transform_.pos = pos;
}

const Transform& Player::GetTransform(void) const
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return transform_;
}

void Player::AddCollider(std::weak_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void Player::ClearCollider(void)
{
	colliders_.clear();
}

const Capsule& Player::GetCapsule(void) const
{
	return *capsule_;
}

void Player::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "Anim_Idle.mv1",20.0f);
	animationController_->Add((int)ANIM_TYPE::WALK, path + "Walk.mv1",30.0f);
	//animationController_->Add((int)ANIM_TYPE::JUMP, path + "Jumping.mv1",20.0f);
	animationController_->Add((int)ANIM_TYPE::JUMP, path + "Jumping.mv1",40.0f);

	animationController_->Play((int)ANIM_TYPE::IDLE);
}

#pragma region ����3D

void Player::ProcessTurn(void)
{
	auto& ins = InputManager::GetInstance();

	//�E����
	if (ins.IsNew(KEY_INPUT_RIGHT)) {
		Turn(SPEED_ROT_DEG_Y,AsoUtility::AXIS_Y);
	}
	
	//������
	if (ins.IsNew(KEY_INPUT_LEFT)) {
		Turn(-SPEED_ROT_DEG_Y,AsoUtility::AXIS_Y);
	}
	
	if (ins.IsNew(KEY_INPUT_UP)) {
		Turn(-SPEED_ROT_DEG_X,AsoUtility::AXIS_X);
	}
	
	if (ins.IsNew(KEY_INPUT_DOWN)) {
		Turn(SPEED_ROT_DEG_X,AsoUtility::AXIS_X);
	}

}

void Player::Turn(float deg, VECTOR axis)
{
	//�@�f�O���[�����W�A���ɕϊ�
	float rad = AsoUtility::Deg2RadF(deg);

	//���W�A�����N�H�[�^�j�I���ɕϊ�
	//	(��]����遁��]�ʂƎ����g�p)
	//	(���L�[�����������Ƃɂ�鍡��̉�]�ʂ����)
	Quaternion rotPow;
	rotPow = Quaternion::AngleAxis(rad, axis);

	//�B���@�̉�]�ɉ�]��������(��������)
	transform_.quaRot = Quaternion::Euler({ 0.0f,1.0f,0.0f });
	transform_.quaRot = transform_.quaRot.Mult(rotPow);

}

void Player::Process(void)
{
	auto& ins = InputManager::GetInstance();
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	//��]����(�ړ��̑O��)
	ProcessTurn();

	//�O���������擾
	VECTOR forward = transform_.GetForward();

	//�ړ���(�����~�X�s�[�h)
	VECTOR movePow = VScale(forward, SPEED_MOVE);

	//�ړ�����(���W�{�ړ���)
	transform_.pos = VAdd(transform_.pos, movePow);


	transform_.Update();
}

#pragma endregion

#pragma region 3D����

void Player::Process3D(void)
{

	auto& ins = InputManager::GetInstance();

	//�J����
	Camera* camera = SceneManager::GetInstance().GetCamera();

	//X����]���������A�d�͕����ɐ����ȃJ����(XZ����)���擾
	Quaternion cameraRot = camera->GetQuaRotOutX();

	//�ړ��ʂ��[��
	movePow_ = AsoUtility::VECTOR_ZERO;

	VECTOR dir = AsoUtility::VECTOR_ZERO;
	double rotRad = 0.0;


	//�ړ��������擾
	//�J���������̍��E�ړ�
	if (ins.IsNew(KEY_INPUT_A)) {
		dir = cameraRot.GetLeft();
		rotRad = AsoUtility::Deg2RadD(-90.0);
	}
	if (ins.IsNew(KEY_INPUT_D)) {
		dir = cameraRot.GetRight();
		rotRad = AsoUtility::Deg2RadD(90.0);
	}
	
	//�J���������̑O��ړ�
	if (ins.IsNew(KEY_INPUT_W)) {
		dir = cameraRot.GetForward();
		rotRad = AsoUtility::Deg2RadD(0.0);
	}
	if (ins.IsNew(KEY_INPUT_S)) {
		dir = cameraRot.GetBack();
		rotRad = AsoUtility::Deg2RadD(180.0);
		//dir = transform_.GetBack();
	}

	//Turn3D(dir);

	if (!AsoUtility::EqualsVZero(dir) && (isJump_ || IsEndLanding()))
	{

		moveDir_ = dir;
		movePow_ = VScale(dir, SPEED_MOVE);

		//�ړ���(�����~�X�s�[�h)
		//VECTOR movePow = VScale(dir, SPEED_MOVE);

		//�ړ�����(���W�{�ړ���)
		//transform_.pos = VAdd(transform_.pos, movePow);

		SetGoalRotate(rotRad);


		if (!isJump_ && IsEndLanding())
		{
			//�����A�j���[�V�����̍Đ�
			animationController_->Play((int)ANIM_TYPE::WALK);
		}
	}
	else
	{
		if (!isJump_ && IsEndLanding())
		{

			//�ҋ@�A�j���[�V�����̍Đ�
			animationController_->Play((int)ANIM_TYPE::IDLE);
		}
	}

	//transform_.Update();
}

void Player::SetGoalRotate(double rotRad)
{
	//�J����
	Camera* camera = SceneManager::GetInstance().GetCamera();

	//���݂̃J�����p�x(rad)
	VECTOR cameraRot = camera->GetAngles();

	//�ڕW�Ƃ���p�x�����߂�
	Quaternion axis =
		Quaternion::AngleAxis(
			(double)cameraRot.y + rotRad, AsoUtility::AXIS_Y);

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// �������l
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}
	goalQuaRot_ = axis;
}

void Player::Rotate(void)
{
	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();
	// ��]�̋��ʕ��
	//playerRotY_ = Quaternion::Slerp(playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
	playerRotY_ = Quaternion::Slerp(playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

Quaternion Player::Rotate2D(void)
{
	//�J����
	Camera* camera = SceneManager::GetInstance().GetCamera();

	stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();

	// ��]�̋��ʕ��
	return  Quaternion::Slerp(camera->GetRot(), goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);

}

void Player::Turn3D( VECTOR axis)
{
	////�@�f�O���[�����W�A���ɕϊ�
	//float rad = AsoUtility::Deg2RadF(deg);

	////���W�A�����N�H�[�^�j�I���ɕϊ�
	////	(��]����遁��]�ʂƎ����g�p)
	////	(���L�[�����������Ƃɂ�鍡��̉�]�ʂ����)
	//Quaternion rotPow;
	//rotPow = Quaternion::AngleAxis(rad, axis);

	//-----------------------------------------------------------------------------

	Quaternion rotPow;
	//rotPow = rotPow.Mult(
	//	Quaternion::AngleAxis(
	//		AsoUtility::Deg2RadF(axis.z), AsoUtility::AXIS_Z));
	rotPow = rotPow.Mult(
		Quaternion::AngleAxis(
			AsoUtility::Deg2RadF(axis.y), AsoUtility::AXIS_Y));
	//rotPow = rotPow.Mult(
	//	Quaternion::AngleAxis(
	//		AsoUtility::Deg2RadF(axis.x), AsoUtility::AXIS_X));

	//------------------------------------------------------------------------------

	//		// XYZ�̉�]�s��
	//	// XZ���ʈړ��ɂ���ꍇ�́AXZ�̉�]���l�����Ȃ��悤�ɂ���
	//MATRIX mat = MGetIdent();
	////mat = MMult(mat, MGetRotX(angles_.x));
	//mat = MMult(mat, MGetRotY(cameraAngles.y));
	////mat = MMult(mat, MGetRotZ(angles_.z));

	//// ��]�s����g�p���āA�x�N�g������]������
	//VECTOR moveDir = VTransform(axis, mat);

	//// �����~�X�s�[�h�ňړ��ʂ�����āA���W�ɑ����Ĉړ�
	//pos_ = VAdd(pos_, VScale(moveDir, movePow));

	//// �������p�x�ɕϊ�����(XZ���� Y��)
	//float angle = atan2f(dir.x, dir.z);
	//// �J�����̊p�x����Ƃ��A�������̊p�x��������
	////rot_.y = cameraAngles.y + angle;

	//// �J�����̊p�x����Ƃ��A�������̊p�x�ւ������߂Â�
	//LazyRotation(cameraAngles.y + angle);

	//---------------------------------------------------------------------------------

	//�B���@�̉�]�ɉ�]��������(��������)
	transform_.quaRot = transform_.quaRot.Mult(rotPow);
}

#pragma endregion

#pragma region 2D���[�h�̑���

//�ړ�����
void Player::ProcessMove2D(void)
{

	//�J����
	Camera* camera = SceneManager::GetInstance().GetCamera();

	auto& ins = InputManager::GetInstance();

	//�ړ��ʂ��[��
	movePow_ = AsoUtility::VECTOR_ZERO;
	VECTOR dir = AsoUtility::VECTOR_ZERO;
	double rotRad = 0.0;

	//��ʉE�Ɉړ�
	if (ins.IsNew(KEY_INPUT_D)) {


		dir = VECTOR{ 1.0f,0.0f,0.0f };

		//dir = camera->GetRot().GetForward();
		//double rad = camera->GetAngles().y + AsoUtility::Deg2RadD(90.0);

		//auto cameraRot = Quaternion::ToEuler( camera->GetRot());
		auto cameraRot = camera->GetRot().GetForward();

		VECTOR pRot = Quaternion::ToEuler(playerRotY_);

		auto rot = AsoUtility::Deg2RadD(90.0);


		rotRad = rot;

	}
	//���Ɉړ�
	if (ins.IsNew(KEY_INPUT_A)) {

		dir = VECTOR{ -1.0f,0.0f,0.0f };

		//dir = camera->GetRot().GetBack();
		rotRad = AsoUtility::Deg2RadD(-90.0);

	}

	if (!AsoUtility::EqualsVZero(dir))
	{

		moveDir_ = dir;
		movePow_ = VScale(dir, SPEED_MOVE);

		//�ړ���(�����~�X�s�[�h)
		//VECTOR movePow = VScale(dir, SPEED_MOVE);

		//�ړ�����(���W�{�ړ���)
		//transform_.pos = VAdd(transform_.pos, movePow);

		SetGoalRotate(rotRad);

		//�����A�j���[�V�����̍Đ�
		animationController_->Play((int)ANIM_TYPE::WALK);
	}
	else
	{
		//�ҋ@�A�j���[�V�����̍Đ�
		animationController_->Play((int)ANIM_TYPE::IDLE);
	}


}

void Player::Process2D(void)
{
	////�ړ�����
	//VECTOR moveDir = ProcessMove2D();

	////�ړ���(�����~�X�s�[�h)
	//VECTOR movePow = VScale(moveDir, SPEED_MOVE);


	////�ړ�����
	//moveDir_ = ProcessMove2D();

	//movePow_ = VScale(moveDir_, SPEED_MOVE);

	////�ړ�����(���W�{�ړ���)
	//transform_.pos = VAdd(transform_.pos, movePow);


	//transform_.Update();
}

void Player::Gravity2D(void)
{
	//�������ւ̏d��
	jumpPow_.y -= GRAVITY * SceneManager::GetInstance().GetDeltaTime();

	//float speed;
	//transform_.pos = VAdd(transform_.pos, VScale(transform_.rot, speed));

	//�d�͕���
	VECTOR dirGravity = AsoUtility::DIR_D;

	//�d��
	VECTOR grv = VScale(AsoUtility::DIR_D, GRAVITY);

	// �����o�ϐ� jumpPow_ �ɏd�͌v�Z���s��(�����x)
	jumpPow_ = VAdd(jumpPow_, grv);

	//����
	float dot = VDot(dirGravity, jumpPow_);

	if (dot >= 0.0f)
	{
		//�d�͕����Ɣ��Ε���(�}�C�i�X)�łȂ���΁A�W�����v�͂��Ȃ���
		jumpPow_ = grv;
	}

	Collision2D();

}

void Player::Collision2D(void)
{
	//�����̓����蔻��
		//�ݒu����(�����̏Փ˔���)

	//�������W(���S)
	VECTOR footPos = transform_.pos;	//���S
	//Vector2 footPos = pos_.ToVector2();	//���S
	//footPos.y += SIZE_Y / 2;			//��������

	//�������W(�E)
	//Vector2 footPosR = GetColPos(COL_LR::R, COL_TD::D);
	//Vector2 footPosR = pos_.ToVector2();
	//footPosR.y += 16;

	//�������W(��)
	//Vector2 footPosL = GetColPos(COL_LR::L, COL_TD::D);
	//Vector2 footPosL = pos_.ToVector2();
	//footPosL.y -= 16;

	//if (sceneGame_->IsCollisionStage(footPos)
	//	|| sceneGame_->IsCollisionStage(footPosR)
	//	|| sceneGame_->IsCollisionStage(footPosL))
	//if (sceneGame_->IsCollisionStage(footPos))
	//{
	//	//�����蔻��ӏ��̂ǂ������ǂɂ߂荞�񂾎�
	//	//�@�����̒��S���W����}�b�v�̂ǂ��ɂ��邩��T��
	//	Vector2 mapPos = sceneGame_->World2MapPos(footPos);
	//	//�A�߂荞�񂾃u���b�N�̏�
	//	transform_.pos.y = mapPos.y * Stage::CHIP_SIZE_Y - 1 - hitBox_.y - hitPos_.y;

	//	//�n�ʂɂ���ꍇ��
	//	isJump_ = false;
	//	SetJumpPower(0.0f);

	//	// �ڒn������W�����v�J�E���^�����ɖ߂�
	//	cntJumpInput_ = 0;

	//}

	
}

#pragma endregion

//�d�͏���
void Player::Gravity(void)
{
	//float speed;
	//transform_.pos = VAdd(transform_.pos, VScale(transform_.rot, speed));

	//�d�͕���
	VECTOR dirGravity = AsoUtility::DIR_D;

	//�d��
	VECTOR grv = VScale(dirGravity, GRAVITY);

	// �����o�ϐ� jumpPow_ �ɏd�͌v�Z���s��(�����x)
	jumpPow_ = VAdd(jumpPow_, grv);

	//����
	float dot = VDot(dirGravity, jumpPow_);

	if (dot >= 0.0f)
	{
		//�d�͕����Ɣ��Ε���(�}�C�i�X)�łȂ���΁A�W�����v�͂��Ȃ���
		jumpPow_ = grv;
	}

}

void Player::ProcessJump(void)
{
	bool isHit = CheckHitKey(KEY_INPUT_BACKSLASH);
	//bool isHit = CheckHitKey(KEY_INPUT_RSHIFT);

	// �W�����v
	if (isHit && (isJump_ || IsEndLanding()))
	{
		if (!isJump_)
		{
			// ���䖳���W�����v
			//animationController_->Play((int)ANIM_TYPE::JUMP);
			// ���̌�A�������̃W�����v�p�^�[���������܂�1

			// ���[�v���Ȃ��W�����v
			//animationController_->Play((int)ANIM_TYPE::JUMP,false);

			//�؂���A�j���[�V����
			//animationController_->Play(
			//	(int)ANIM_TYPE::JUMP, false, 13.0f, 24.0f);

			//�������A�j���[�V����
			animationController_->Play(
				(int)ANIM_TYPE::JUMP, true, 13.0f, 25.0f);

			animationController_->SetEndLoop(23.0f, 25.0f, 20.0f);

		}

		isJump_ = true;

		// �W�����v�̓��͎�t���Ԃ��w���X 
		stepJump_ += SceneManager::GetInstance().GetDeltaTime();

		if (stepJump_ < TIME_JUMP_IN)
		{
			jumpPow_ = VScale(AsoUtility::DIR_U, POW_JUMP);
		}
	}

	// �{�^���𗣂�����W�����v�͂ɉ��Z���Ȃ�
	if (!isHit)
	{
		stepJump_ = TIME_JUMP_IN;
	}

}

void Player::Collision(void)
{
	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);

	//�Փ�(�J�v�Z��)
	CollisionCapsule();

	// �Փ�(�d��)
	CollisionGravity();

	// �ړ�
	transform_.pos = movedPos_;
}

void Player::CollisionGravity(void)
{
	// �W�����v�ʂ����Z
	movedPos_ = VAdd(movedPos_, jumpPow_);

	// �d�͕���
	VECTOR dirGravity = AsoUtility::DIR_D;

	// �d�͕����̔���
	VECTOR dirUpGravity = AsoUtility::DIR_U;

	// �d�͂̋���
	float gravityPow = GRAVITY;

	//�v���C���[�̂Ђ������瑫�����܂ł̐���(���C)�����
	float checkPow = 10.0f;
	//�n�_
	gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * 2.0f));
	//�I�_
	gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
	for (const auto c : colliders_)
	{
		// �n�ʂƂ̏Փ�
		auto hit = MV1CollCheck_Line(c.lock()->modelId_, -1, gravHitPosUp_, gravHitPosDown_);
		//if (hit.HitFlag > 0)
		if (hit.HitFlag > 0 && VDot(dirGravity, jumpPow_) > 0.9f)
		{
			// �n�ʂƏՓ˂��Ă���
			// �����߂������ƃW�����v�͂̑ł��������������܂��傤

			//�Փ˒n�_����A������Ɉړ�
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));

			//�W�����v���Z�b�g
			jumpPow_ = AsoUtility::VECTOR_ZERO;
			stepJump_ = 0.0f;

			if (isJump_)
			{
				// ���n���[�V����
				//animationController_->Play(
				//	(int)ANIM_TYPE::JUMP, false, 29.0f, 45.0f, false, true);
				// ���n���[�V����
				animationController_->Play(
					(int)ANIM_TYPE::JUMP, false, 30.0f, 40.0f, false, true);
			}

			isJump_ = false;

			//VECTOR vec = VSub(gravHitPosDown_, hit.HitPosition);
			//movedPos_ = VAdd(movedPos_,VScale(vec,-1.0f));
			//jumpPow_ = VAdd(jumpPow_, VScale(dirUpGravity,gravityPow));
		}
	}
}

void Player::CollisionCapsule(void)
{
	// �J�v�Z�����ړ�������
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = Capsule(*capsule_, trans);

	// �J�v�Z���Ƃ̏Փ˔���
	for (const auto c : colliders_)
	{
		auto hits = MV1CollCheck_Capsule(
			c.lock()->modelId_, -1,
			cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());

		// �Փ˂��������̃|���S���ƏՓˉ������܂ŁA
		// �v���C���[�̈ʒu���ړ�������
		for (int i = 0; i < hits.HitNum; i++)
		{
			auto hit = hits.Dim[i];

			// �n�ʂƈقȂ�A�Փˉ���ʒu���s���Ȃ��߁A���x���ړ�������
			// ���̎��A�ړ�����������́A�ړ��O���W�Ɍ����������ł�������A
			// �Փ˂����|���S���̖@�������������肷��
			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{

				// �ēx�A���f���S�̂ƏՓˌ��o����ɂ́A���������߂���̂ŁA
				// �ŏ��̏Փ˔���Ō��o�����Փ˃|���S��1���ƏՓ˔�������
				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(),
					hit.Position[0], hit.Position[1], hit.Position[2]);
				if (pHit)
				{

					// �@���̕����ɂ�����Ƃ����ړ�������
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));

					// �J�v�Z�����ꏏ�Ɉړ�������
					trans.pos = movedPos_;
					trans.Update();
					continue;
				}

				break;
			}
		}

		// ���o�����n�ʃ|���S�����̌�n��
		MV1CollResultPolyDimTerminate(hits);
	}
}

bool Player::IsEndLanding(void)
{
	bool ret = true;

	// �A�j���[�V�������W�����v�ł͂Ȃ�
	if (animationController_->GetPlayType() != (int)ANIM_TYPE::JUMP)
	{
		return ret;
	}
	// �A�j���[�V�������I�����Ă��邩
	if (animationController_->IsEnd())
	{
		return ret;
	}

	return false;
}

