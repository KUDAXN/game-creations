#include<fstream>
#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "Character/Player.h"
#include "Stage.h"


Stage::Stage(Player& player) : player_(player)	//�������w��q
{
	block_ = nullptr;
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{

	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::STAGE1));
	//float scale = 30.0f;
	float scale = 0.3f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 10.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(90.0f),
		0.0f);
	transform_.quaRotLocal = Quaternion();


	// �����蔻��(�R���C�_)�쐬
	transform_.MakeCollider(Collider::TYPE::STAGE);

	transform_.Update();

	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 2; x++)
		{
			objBlock_[y][x] = 0;
			objPlane_[y][x] = 0;
			objWall_[y][x] = 0;
		}
	}

}

void Stage::Update(void)
{
	transform_.Update();
}

void Stage::Draw(void)
{
	int i = MV1DrawModel(transform_.modelId);

	//DrawDebug();
	//DrawFormatString(0, 90, 0xff0099, "�X�e�[�W���W�F(%.1f,%.1f,%.1f)", transform_.pos.x, transform_.pos.y, transform_.pos.z);
	
}

void Stage::DrawDebug(void)
{
	//�f�o�b�O�p
	///-----------------------------------------------------------------------------------------------------
	DrawString(0, 280, "Block", 0x000000);
	DrawFormatString(0, 300, 0xff000000, "Lt�F(%.1f , %.1f)", Block_.LtPos.x, Block_.LtPos.y);
	DrawFormatString(0, 320, 0xff000000, "Rb�F(%.1f , %.1f)", Block_.RbPos.x, Block_.RbPos.y);

	DrawString(0, 350, "Plane", 0x000000);
	DrawFormatString(0, 370, 0xff000000, "Lt�F(%.1f , %.1f)", Plane_.LtPos.x, Plane_.LtPos.y);
	DrawFormatString(0, 390, 0xff000000, "Rb�F(%.1f , %.1f)", Plane_.RbPos.x, Plane_.RbPos.y);

	DrawString(0, 420, "Wall", 0x000000);
	DrawFormatString(0, 440, 0xff000000, "Lt�F(%.1f , %.1f)", Wall_.LtPos.x, Wall_.LtPos.y);
	DrawFormatString(0, 460, 0xff000000, "Rb�F(%.1f , %.1f)", Wall_.RbPos.x, Wall_.RbPos.y);

}

void Stage::Release(void)
{
}

VECTOR Stage::GetColc(void)
{
	return block_->GetPos();
}

VECTOR Stage::GetCol(void)
{
	return Plane_.LtPos;
}

VECTOR Stage::GetCold(void)
{
	return Plane_.RbPos;
}

Transform Stage::GetTransform(void) const
{
	return transform_;
}

void Stage::LoadUsedCsvCol()
{

	//�t�@�C���̓Ǎ��̏���
	//-------------------------
	std::ifstream ifs = std::ifstream("Data/CollisionData.csv");
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
			
			if (y < 2)
			{
				objBlock_[y][x] = chipNo;
			}
			else if (y < 4)
			{
				objPlane_[y-2][x] = chipNo;
			}
			else if (y < 6)
			{
				objWall_[y - 4][x] = chipNo;
			}
		}

		//1�s���̊i�[���I������̂Ŏ��̍s�Ɉړ�
		y++;
	}

	x = 0;
	y = 0;
	bool isLoop = true;

	while (isLoop)
	{

		if (y == 0 && x == 0)
		{
			Block_.LtPos.x = objBlock_[y][x];
			Plane_.LtPos.x = objPlane_[y][x];
			Wall_.LtPos.x = objWall_[y][x];
			x++;
		}
		else if (y == 0 && x == 1)
		{
			Block_.LtPos.y = objBlock_[y][x];
			Plane_.LtPos.y = objPlane_[y][x];
			Wall_.LtPos.y = objWall_[y][x];
			y++;
			x = 0;
		}
		if (y == 1 && x == 0)
		{
			Block_.RbPos.x = objBlock_[y][x];
			Plane_.RbPos.x = objPlane_[y][x];
			Wall_.RbPos.x = objWall_[y][x];
			x++;
		}
		else if (y == 1 && x == 1)
		{
			Block_.RbPos.y = objBlock_[y][x];
			Plane_.RbPos.y = objPlane_[y][x];
			Wall_.RbPos.y = objWall_[y][x];
			isLoop = false;
		}
	}

}
