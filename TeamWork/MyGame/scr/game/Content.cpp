#include "Content.h"
#include "../game/WorkFolder.h"
#include "../graphic/Model.h"
#include "../graphic/Sprite.h"
//#include "../sound/Sound.h"
#include "../Def.h"
#include "../math/MathHelper.h"
#include "ID.h"

// 画像を読み込む
void Content::LoadSprite(Sprite& sprite, Model& model)
{
	WorkFolder::SetWorkFolder("res/Sprite/");
}

// ３Ｄモデルを読み込む
void Content::LoadModel(Model& model, bool async)
{
	WorkFolder::SetWorkFolder("res/Model/");

	model.Load("plane.mv1", MODEL_ID::PLANE_MODEL);

	WorkFolder::SetWorkFolder("res/Model2D/");
}

// 音楽を読み込む
void Content::LoadSound(Sound& sound)
{
	WorkFolder::SetWorkFolder("res/Sound/");

}

void Content::EnableASync()
{
	SetUseASyncLoadFlag(TRUE);
}
void Content::DisableASync()
{
	SetUseASyncLoadFlag(FALSE);
}

template <typename T>
int Content::GetASyncLoadCount(T& content)
{
	return (content.GetCount() - GetASyncLoadNum() + 1);
}
//template <>
//int Content::GetASyncLoadCount(Sound& content)
//{
//	return ((content.GetCountBGM() + content.GetCountSE()) - GetASyncLoadNum() + 1);
//}
//int Content::GetASyncLoadAllCount(Model& model, Sprite& sprite, Sound& sound)
//{
//	auto count = model.GetCount() + sprite.GetCount() + sound.GetCountBGM() + sound.GetCountSE();
//	return (count - GetASyncLoadNum() + 1);
//}
bool Content::IsASync()
{
	return GetASyncLoadNum() > 0;
}