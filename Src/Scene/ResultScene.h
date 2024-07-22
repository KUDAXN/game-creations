#pragma once
#include "../Object/Common/Transform.h"
#include "SceneBase.h"

class ResultScene : public SceneBase
{
public:


	// コンストラクタ
	ResultScene(void);

	// デストラクタ
	~ResultScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//ロゴ
	int Logo_;

	// ロゴ描画
	void DrawLogo(void);
};
