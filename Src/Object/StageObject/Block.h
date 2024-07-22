#pragma once
#include "../Common/Transform.h"


class Block
{

public:
	// コンストラクタ
	Block();

	// デストラクタ
	~Block(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void);

private:

	// モデル制御の基本情報
	
	//モデルの配置位置
	float position_[5][2];


	void LoadPosition(void);


};

