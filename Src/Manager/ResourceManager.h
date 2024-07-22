#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		SKYDOME,			//スカイドーム
		RESET,		//セーブ地点の誇張
		SPEECH_BALLOON,		//吹き出し
		PLAYER,				//球体
		STAGE1,				//ステージモデル
		STAGE2,				//ステージモデル(2D用)
		BLOCK,				//ステージ(細い足場)
		PLANE,				//ステージ(広い足場)
		WALL,				//ステージ(壁)
		HUMAN,					//操作キャラ
		HUMAN_IDLE,				//操作キャラ(待機)
		HUMAN_WALK,				//操作キャラ(歩行)
		HUMAN_JUMP,				//操作キャラ(ジャンプ)
		TITLE,					//タイトル
		TITLE2,					//タイトル2
		TL_TAB,					//切り替え説明
		TL_BSRASH,				//ジャンプ説明
		TL_WASD,				//移動説明
		TL_RE,					//リセット説明
		TL_LR,					//始点切り替え説明
		GAMEOVER,				//ゲームオーバー
		GAMECLEAR,				//ゲームクリア
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	Resource Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource*> loadedMap_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);

	// デストラクタも同様
	~ResourceManager(void) = default;

	// 内部ロード
	Resource* _Load(SRC src);

};

