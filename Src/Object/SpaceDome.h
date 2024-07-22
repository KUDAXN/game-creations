#pragma once

#include "../Object/Common/Transform.h"


class SpaceDome
{
public:

	// コンストラクタ
	SpaceDome(const VECTOR& follow);

	// デストラクタ
	~SpaceDome(void);

	void Init(void);
	void Update(VECTOR pos);
	void Draw(void);
	void Release(void);

private:

	//追従対象
	const VECTOR& follow_;

	// モデル制御の基本情報
	Transform transform_;

};

