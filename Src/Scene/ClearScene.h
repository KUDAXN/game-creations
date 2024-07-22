#pragma once
#include "../Object/Common/Transform.h"
#include "SceneBase.h"

class ClearScene : public SceneBase
{
public:


	// コンストラクタ
	ClearScene(void);

	// デストラクタ
	~ClearScene(void);

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
