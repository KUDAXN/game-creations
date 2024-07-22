#pragma once
#include<vector>
#include "Common/Transform.h"
#include "StageObject/Block.h"

class Player;

class Stage2D
{
	//���̒���
	static constexpr float LEN = 500.0f;

public:

	//�u���b�N�ʒu���
	struct pBlock
	{
		VECTOR cPos = {};
		VECTOR LtPos = {};
		VECTOR RbPos = {};
	};

	//struct name
	//{
	//	pBlock 	Block_;
	//	pBlock	Plane_;
	//	pBlock	Wall_;
	//};

	//�u���b�N��
	enum class BLOCK_NAME
	{
		BLOCK,
		PLANE,
		WALL,
	};

	//�X�e�[�W��
	enum class STAGE_NAME
	{
		MAIN_STAGE,
	};

	// �R���X�g���N�^
	Stage2D(Player& player);

	// �f�X�g���N�^
	~Stage2D(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	VECTOR GetColc(void);
	VECTOR GetCol(void);
	VECTOR GetCold(void);

	Transform GetTransform(void) const;

private:

	//���Z�b�g�n�_�̃G�t�F�N�g
	int effectResetResId_;
	int effectResetPlayId_;

	//�Ǐ]�Ώ�
	//const Transform& follow_;

	//�v���C���[
	Player& player_;

	//�u���b�N
	std::unique_ptr < Block > block_;

	// ���f������̊�{���
	Transform transform_;

	//�I�u�W�F�N�g�̓����蔻��p���W
	float objBlock_[2][2];
	float objPlane_[2][2];
	float objWall_[2][2];
	pBlock 	Block_;
	pBlock	Plane_;
	pBlock	Wall_;

	//�f�o�b�O
	void DrawDebug(void);
};


