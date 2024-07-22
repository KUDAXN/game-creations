#pragma once
#include "../Common/Transform.h"


class Plane
{

public:
	// コンストラクタ
	Plane();

	// デストラクタ
	~Plane(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// モデル制御の基本情報
	Transform transform_;

};

