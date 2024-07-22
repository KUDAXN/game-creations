#pragma once
class Grid
{

	// ���̒���
	static constexpr float LEN = 1500.0f;

	// ���̒����̔���
	static constexpr float HLEN = LEN / 2.0f;

	// ���̊Ԋu
	static constexpr float TERM = 50.0f;

	// ���̐�
	static const int NUM = static_cast<int>(LEN / TERM);

	// ���̐��̔���
	static const int HNUM = NUM / 2;

public:

	// �R���X�g���N�^
	Grid(void);

	// �f�X�g���N�^
	~Grid(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);


};

