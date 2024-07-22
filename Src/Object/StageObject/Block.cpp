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

	//ファイルの読込の準備
	//-------------------------
	std::ifstream ifs = std::ifstream("Data/BlockPosData.csv");
	if (!ifs)
	{
		return;		//ファイルの読み込み準備失敗
	}

	//ファイル一行分データ格納領域
	std::string line;		//行のデータ
	int x = 0;				//マップデータの添え字マップのX座標
	int y = 0;				//マップデータの添え字マップのY座標
	float chipNo = 0.0f;			//マップチップ番号

	std::vector<std::string> strSplit;		//区切り記号で分割したvector情報

	//ファイルのデータを1行毎に読み込む
	while (getline(ifs, line))
	{
		strSplit = AsoUtility::Split(line, ',');

		//要素数[strSplit.size()]分ループする
		for (int x = 0; x < strSplit.size(); x++)
		{
			//要素を取り出し、マップチップ番号に変換
			chipNo = stof(strSplit[x]);

			position_[y][x] = chipNo;
		}

		//1行分の格納が終わったので次の行に移動
		y++;
	}

}
