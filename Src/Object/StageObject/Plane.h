#pragma once
#include "../Common/Transform.h"


class Plane
{

public:
	// �R���X�g���N�^
	Plane();

	// �f�X�g���N�^
	~Plane(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// ���f������̊�{���
	Transform transform_;

};

