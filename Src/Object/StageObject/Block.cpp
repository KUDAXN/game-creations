#include<fstream>
#include "../../Manager/ResourceManager.h"
#include "../../Utility/AsoUtility.h"
#include "Block.h"


Block::Block()
{
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 2; x++)
		{
			position_[y][x] = 0;
		}
	}
}

Block::~Block(void)
{
}

void Block::Init(void)
{


	LoadPosition();
}

void Block::Update(void)
{
}

void Block::Draw(void)
{
}

void Block::Release(void)
{
}

VECTOR Block::GetPos(void)
{
	return VECTOR({ position_[0][0], position_[0][1],0.0f });
}

void Block::LoadPosition(void)
{

	//�t�@�C���̓Ǎ��̏���
	//-------------------------
	std::ifstream ifs = std::ifstream("Data/BlockPosData.csv");
	if (!ifs)
	{
		return;		//�t�@�C���̓ǂݍ��ݏ������s
	}

	//�t�@�C����s���f�[�^�i�[�̈�
	std::string line;		//�s�̃f�[�^
	int x = 0;				//�}�b�v�f�[�^�̓Y�����}�b�v��X���W
	int y = 0;				//�}�b�v�f�[�^�̓Y�����}�b�v��Y���W
	float chipNo = 0.0f;			//�}�b�v�`�b�v�ԍ�

	std::vector<std::string> strSplit;		//��؂�L���ŕ�������vector���

	//�t�@�C���̃f�[�^��1�s���ɓǂݍ���
	while (getline(ifs, line))
	{
		strSplit = AsoUtility::Split(line, ',');

		//�v�f��[strSplit.size()]�����[�v����
		for (int x = 0; x < strSplit.size(); x++)
		{
			//�v�f�����o���A�}�b�v�`�b�v�ԍ��ɕϊ�
			chipNo = stof(strSplit[x]);

			position_[y][x] = chipNo;
		}

		//1�s���̊i�[���I������̂Ŏ��̍s�Ɉړ�
		y++;
	}

}
