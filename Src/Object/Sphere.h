#pragma once
#include "Common/Transform.h"
#include "../Manager/Camera.h"


class Sphere
{
public:

	// �ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 10.0f;

	// ���@�̉�]��
	static constexpr float SPEED_ROT_DEG_Y = 1.0f;
	static constexpr float SPEED_ROT_DEG_X = 1.0f;

	// �R���X�g���N�^
	Sphere(void);

	// �f�X�g���N�^
	~Sphere(void);

	void Init(VECTOR pos);
	void Update(void);
	void Draw(void);
	void Release(void);

	const Transform& GetTransform(void) const;

	const void SetPos(VECTOR pos);

private:

	// ���f������̊�{���
	Transform transform_;

	//3D���̂̈ʒu
	VECTOR pos_;

	//���̂̊p�x
	VECTOR rot_;

	int count_;

	// ����
	void ProcessTurn(void);
	void Turn(float deg, VECTOR axis);

};

