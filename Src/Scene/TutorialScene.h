#pragma once
#include "../Object/Common/Transform.h"
#include "SceneBase.h"

class SpaceDome;
class Sphere;


class TutorialScene : public SceneBase
{

public:

	// コンストラクタ
	TutorialScene(void);

	// デストラクタ
	~TutorialScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//スカイドーム用
	Transform spaceDomeTran_;

	//チュートリアル画面描画
	int imgTutorial_;

	//点滅用変数
	int cnt_;

	// ロゴ描画
	void DrawText(void);

};
