#pragma once
#include "../Common/Transform.h"


class Block
{

public:
	// �R���X�g���N�^
	Block();

	// �f�X�g���N�^
	~Block(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void);

private:

	// ���f������̊�{���
	
	//���f���̔z�u�ʒu
	float position_[5][2];


	void LoadPosition(void);


};

