#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"

class Transform;

class Camera
{

public:

	// �J�����X�s�[�h�FNEAR
	static constexpr float SPEED = 1.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_FAR = 15000.0f;

	// �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// �J�����ʒu���璍���_�܂ł̑��΍��W
	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -100.0f, 500.0f };

	//�Ǐ]�Ώۂ̈ʒu����Ǐ]�ʒu�܂ł̑��΍��W
	static constexpr VECTOR LOCALE_T_POS = { 0.0f,25.0f,0.0f };

	// �J�����ʒu���璍���_�܂ł̑��΍��W(2D)
	//static constexpr VECTOR RELATIVE_C2T_POS_2D_R = { -500.0f, -100.0f, 0.0f };		//�E���猩��
	//static constexpr VECTOR RELATIVE_C2T_POS_2D_R = { -500.0f, 25.0f, 0.0f };		//�E���猩��
	//static constexpr VECTOR RELATIVE_C2T_POS_2D_F = { 0.0f, -100.0f, -500.0f };		//�O���猩��
	//static constexpr VECTOR RELATIVE_C2T_POS_2D_L = { 500.0f, -100.0f, 0.0f };		//�����猩��
	//static constexpr VECTOR RELATIVE_C2T_POS_2D_B = { 0.0f, -100.0f, 500.0f };		//�ォ�猩��
	static constexpr VECTOR RELATIVE_C2T_POS_2D = { 0.0f, 0.0f, 500.0f };		//-z���W���猩��


	// �Ǐ]�Ώۂ���J�����ʒu�܂ł̑��΍��W(���S�Ǐ])
	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 25.0f, -80.0f };

	// �Ǐ]�Ώۂ���J�����ʒu�܂ł̑��΍��W(�o�l�t��)
	static constexpr VECTOR RELATIVE_F2C_POS_SPRING = { 0.0f, 40.0f, 150.0f };
	
	// �Ǐ]�Ώۂ���J�����ʒu�܂ł̑��΍��W(2D���[�h)
	//static constexpr VECTOR RELATIVE_F2C_POS_2DMODE_R = { 600.0f, 25.0f, 0.0f };		//�E���猩��
	//static constexpr VECTOR RELATIVE_F2C_POS_2DMODE_F = { 0.0f, 25.0f, 100.0f };		//�O���猩��
	//static constexpr VECTOR RELATIVE_F2C_POS_2DMODE_L = { -100.0f, 25.0f, 0.0f };		//�����猩��
	//static constexpr VECTOR RELATIVE_F2C_POS_2DMODE_B = { 0.0f, 25.0f, -100.0f };		//�ォ�猩��
	static constexpr VECTOR RELATIVE_F2C_POS_2DMODE = { 0.0f, 0.0f, -600.0f };		//-z���W���猩��

	//�Ǐ]�ʒu���璍���_�܂ł̑��΍��W(3D)
	static constexpr VECTOR LOCAL_F2T_POS = {0.0f,0.0f,125.0f};

	//�Ǐ]�ʒu����J�����ʒu�܂ł̑��΍��W(3D)
	static constexpr VECTOR LOCAL_F2C_POS = {0.0f,25.0f,-250.0f};

	//�J������Y����]��
	static constexpr float SPEED_ROT_DEG_Y = 1.0f;

	// �J������X��]����x�p
	static constexpr float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = 15.0f * (DX_PI_F / 180.0f);

	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,	// ��_�J����
		FREE,			//�t���[���[�h
		FOLLOW,			//�Ǐ]���[�h
		FOLLOW_SPRING,	//�Ǐ]���[�h(�o�l�t��)
		FOLLOW_2D,		//�Q�c���[�h
		FOLLOW_3D,		//�Ǐ]���[�h(3D)
	};

	Camera(void);
	~Camera(void);

	void Init(void);

	void Update(void);
	void Update3D(void);


	void SetBeforeDraw(void);

	void SetBeforeDrawFixedPoint(void);		//��_�J����
	void SetBeforeDrawFree(void);			//�t���[�J����
	void SetBeforeDrawFollow(void);			//�Ǐ]�J����
	void SetBeforeDrawFollow_Spring(void);	//�Ǐ]�J����(�΂˕t��)

	//�Ǐ]�J�����F2D
	void SetBeforeDrawFollow_2D(void);

	//�Ǐ]�J�����F3D
	void SetBeforeDrawFollow_3D(void);
	void ProcessTurn3D(void);

	void Draw(void);
	void Release(void);

	//�J�����ʒu���擾����
	VECTOR GetPos(void) const;
	//�J�����̑���p�x
	VECTOR GetAngles(void) const;

	//X��]�𔲂����J�����p�x
	Quaternion GetQuaRotOutX(void) const;
	//�J������]���擾����
	Quaternion GetRot(void) const;

	//���݂̃J�������[�h���擾����
	MODE GetCameraMode(void) const;

	//�X�J�C�h�[���̐ݒ�Ŏg�����W
	VECTOR GetSkyDomePos(void);

	// �J�������[�h�̕ύX
	void ChangeMode(MODE mode);

	//�Ǐ]�Ώۂ̐ݒ�
	void SetFollow(const Transform* follow);

	//(�f�o�b�O�p)
	void DrawDebug(void);

private:

	//�Ǐ]�Ώ�
	const Transform* followTransform_;

	// �J�������[�h
	MODE mode_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �J�����p�x(rad)
	VECTOR angles_;

	// X����]�������p�x
	Quaternion rotOutX_;

	// �J�����̊p�x
	Quaternion rot_;

	// �J�����̒����_
	VECTOR targetPos_;

	// �J�����̏����
	VECTOR cameraUp_;


	//���x
	VECTOR Velocity_;

	//2D���_�ړ�
	float angle_;

	// �J�����������ʒu�ɖ߂�
	void SetDefault(void);

	// �Ǐ]�ΏۂƂ̈ʒu���������
	void SyncFollow(void);

	// �J��������
	void ProcessRot(void);

	// �Ǐ]�ΏۂƂ̈ʒu���������(2D)
	void SyncFollow2D(void);

	// �J��������(XZ����)
	void ProcessRotY(void);


	//��]����
	void ProcessTurn(float deg);

	//��]��(�x)�Ɖ�]���ŉ�]����
	void Turn(float deg, VECTOR axis);

	//������]
	void MoveTurn2D(float deg, VECTOR axis, Quaternion gole);

	//��]�ڕW
	Quaternion Turn2DGole(float deg, VECTOR axis);

};

