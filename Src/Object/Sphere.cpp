#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "Sphere.h"

Sphere::Sphere(void)
{
	pos_ = AsoUtility::VECTOR_ONE;

	rot_ = AsoUtility::VECTOR_ZERO;

	count_ = 0;
}

Sphere::~Sphere(void)
{
}

void Sphere::Init(VECTOR pos)
{
	//pos_ = AsoUtility::VECTOR_ONE;

	//rot_ = AsoUtility::VECTOR_ZERO;

	//count_ = 0;

	//モデル制御の基本情報
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER));

	float scale = 1.0f;
	transform_.scl = { scale,scale,scale };
	transform_.pos = pos;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion();
	transform_.Update();
}

void Sphere::Update(void)
{
	//pos_.y += sin(10);

	//rot_.y++;

	//MATRIX mRot = MGetIdent();
	//mRot = MMult(mRot, MGetRotY(rot_.y));


	//rot_ = VAdd(rot_, AsoUtility::AXIS_Y);

	////回転操作(移動の前に)
	//ProcessTurn();

	////前方方向を取得
	//VECTOR forward = transform_.GetForward();

	////移動量(方向×スピード)
	//VECTOR movePow = VScale(forward, SPEED_MOVE);

	////移動処理(座標＋移動量)
	//transform_.pos = VAdd(transform_.pos, movePow);


	transform_.Update();

}

void Sphere::Draw(void)
{
	MV1DrawModel(transform_.modelId);

	//DrawSphere3D(pos_, 80.0f, 8, GetColor(0, 0, 225), GetColor(0, 0, 0), false);
	MV1SetWireFrameDrawFlag(transform_.modelId, true);
}

void Sphere::Release(void)
{
}

const Transform& Sphere::GetTransform(void) const
{
	// TODO: return ステートメントをここに挿入します
	return transform_;
}

const void Sphere::SetPos(VECTOR pos)
{
	transform_.pos = pos;
}
