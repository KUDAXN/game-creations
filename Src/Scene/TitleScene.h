#pragma once
#include "../Object/Common/Transform.h"
#include "SceneBase.h"

class SpaceDome;
class Sphere;


class TitleScene : public SceneBase
{

public:
	static constexpr VECTOR Triangle1 = {0.0f,200.0f,0.0f};
	static constexpr VECTOR Triangle2 = {};
	static constexpr VECTOR Triangle3 = {};


	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//スカイドーム用
	Transform spaceDomeTran_;

	//スカイドーム
	SpaceDome* sky_;

	//プレイヤー
	Sphere* Sphere_;

	VECTOR pRot_;

	VECTOR rPos_;


	int cnt_;

	// タイトルロゴ
	int imgTitleLogo_;

	// タイトルロゴ2
	int imgTitleLogo2_;

	// ロゴ描画
	void DrawLogo(void);

	//回転
	VECTOR Rot(VECTOR localP);

};
