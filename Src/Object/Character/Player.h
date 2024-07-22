#pragma once
#include <memory>
#include "../Common/Transform.h"
#include "../../Manager/Camera.h"
#include "CharBase.h"

class Quaternion;
class AnimationController;
class Collider;
class Capsule;
class Stage;

class Player
{
public:

	static constexpr float TIME_ROT = 1.0f;

	// �ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 1.5f;

	// ���@�̉�]��
	static constexpr float SPEED_ROT_DEG_Y = 1.0f;
	static constexpr float SPEED_ROT_DEG_X = 1.0f;

	//�d�͂̑傫��
	static constexpr float GRAVITY = 9.81f;

	//����
	static constexpr float MASS = 0.5f;

	// �W�����v��
	//static constexpr float POW_JUMP = 20.0f;
	static constexpr float POW_JUMP = 11.0f;

	// �W�����v��t����
	//static constexpr float TIME_JUMP_IN = 0.3f;
	static constexpr float TIME_JUMP_IN = 0.4f;


	//�A�j���[�V����
	enum class ANIM_TYPE
	{
		IDLE,	//�ҋ@
		WALK,	//���s
		JUMP,	//�W�����v
	};

	// �R���X�g���N�^
	Player(void);

	// �f�X�g���N�^
	~Player(void);
	
	void Init(void);
	void Update(const Camera::MODE& cameraMode);
	void Update2D(void);
	void Update3D(void);
	void Draw(void);
	void Release(void);

	void SetPos(VECTOR pos);

	const Transform& GetTransform(void) const;

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(std::weak_ptr<Collider> collider);
	void ClearCollider(void);

	//�Փ˗p�J�v�Z���̎擾
	const Capsule& GetCapsule(void) const;

private:

	// ���f������̊�{���
	Transform transform_;

	//�X�e�[�W
	std::unique_ptr<Stage> stage_;

	//�A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	//��ԊǗ�
	//ANIM_TYPE state_;

	//�W�����v��
	VECTOR jumpPow_;

	// �ړ�����
	VECTOR moveDir_;

	// �ړ���
	VECTOR movePow_;

	// �ړ���̍��W
	VECTOR movedPos_;

	//�d��
	float gravityPow_;

	// ��]
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	// �W�����v����
	bool isJump_;

	// �W�����v�̓��͎�t����
	float stepJump_;

	//�Փ˃`�F�b�N
	VECTOR gravHitPosUp_;
	VECTOR gravHitPosDown_;

	// �Փ˔���ɗp������R���C�_
	std::vector<std::weak_ptr<Collider>> colliders_;

	//�����蔻��
	std::unique_ptr<Capsule> capsule_;


	//�A�j���[�V�����̏�����
	void InitAnimation(void);

	//����3D
	void ProcessTurn(void);
	void Turn(float deg, VECTOR axis);
	void Process(void);
	
	//3D����
	void Process3D(void);
	void Turn3D( VECTOR axis);
	//void Process3D(void);

	//2D����
	void ProcessMove2D(void);
	void Process2D(void);
	void Gravity2D(void);
	void Collision2D(void);

	// ��]
	void SetGoalRotate(double rotRad);

	void Rotate(void);
	Quaternion Rotate2D(void);

	void LazyRotation(float goalRot);

	//�d��
	void Gravity(void);

	//�W�����v����
	void ProcessJump(void);

	//�Փ˔���
	void Collision(void);
	void CollisionGravity(void);

	//�J�v�Z���ƏՓ˕��̓����蔻��
	void CollisionCapsule(void);
 

	// ���n���[�V�����I��
	bool IsEndLanding(void);

};

