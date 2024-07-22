#pragma once

#include "../Object/Common/Transform.h"


class SpaceDome
{
public:

	// �R���X�g���N�^
	SpaceDome(const VECTOR& follow);

	// �f�X�g���N�^
	~SpaceDome(void);

	void Init(void);
	void Update(VECTOR pos);
	void Draw(void);
	void Release(void);

private:

	//�Ǐ]�Ώ�
	const VECTOR& follow_;

	// ���f������̊�{���
	Transform transform_;

};

