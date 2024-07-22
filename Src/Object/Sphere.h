#pragma once
#include "Common/Transform.h"
#include "../Manager/Camera.h"


class Sphere
{
public:

	// 移動スピード
	static constexpr float SPEED_MOVE = 10.0f;

	// 自機の回転量
	static constexpr float SPEED_ROT_DEG_Y = 1.0f;
	static constexpr float SPEED_ROT_DEG_X = 1.0f;

	// コンストラクタ
	Sphere(void);

	// デストラクタ
	~Sphere(void);

	void Init(VECTOR pos);
	void Update(void);
	void Draw(void);
	void Release(void);

	const Transform& GetTransform(void) const;

	const void SetPos(VECTOR pos);

private:

	// モデル制御の基本情報
	Transform transform_;

	//3D球体の位置
	VECTOR pos_;

	//球体の角度
	VECTOR rot_;

	int count_;

	// 操作
	void ProcessTurn(void);
	void Turn(float deg, VECTOR axis);

};

