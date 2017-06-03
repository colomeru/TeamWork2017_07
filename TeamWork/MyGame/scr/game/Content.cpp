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
	sprite.Load("particle.bmp",SPRITE_ID::TEST_SPRITE);
	sprite.Load("3219116.png", SPRITE_ID::TEST2_SPRITE);
	sprite.Load("arrow.png",SPRITE_ID::ARROW_SPRITE);
	sprite.Load("title_background.png", SPRITE_ID::TITLE_SPRITE);
	sprite.Load("circle.png", SPRITE_ID::CIRCLE_SPRITE);
	sprite.Load("sphere.png", SPRITE_ID::SPHERE_SPRITE);
	sprite.Load("人.png", SPRITE_ID::HITO_SPRITE);
	sprite.Load("snake.png", SPRITE_ID::SNAKE_SPRITE);
	sprite.Load("clip.png", SPRITE_ID::PLAYER_HEAD_SPRITE);
	sprite.Load("orochi_4.png", SPRITE_ID::PLAYER_HEAD_FATIGUE_SPRITE);
	sprite.Load("baseClothes.png", SPRITE_ID::BASE_CLOTHES_SPRITE);
	sprite.Load("testClothes.bmp", SPRITE_ID::TEST_CLOTHES_SPRITE);
	sprite.Load("goal.bmp", SPRITE_ID::TEST_GOAL_SPRITE);
	sprite.Load("hanger.png", SPRITE_ID::HANGER_SPRITE);
	sprite.Load("fluffyClothes.png", SPRITE_ID::FLUFFY_SPRITE);
	sprite.Load("gumClothes.png", SPRITE_ID::GUM_SPRITE);
	sprite.Load("hairball.png", SPRITE_ID::HAIRBALL_SPRITE);
	sprite.Load("gameoverText.png", SPRITE_ID::GAMEOVER_TEXT_SPRITE);
	sprite.Load("retry.png", SPRITE_ID::RETRY_TEXT_SPRITE);
	sprite.Load("backtitle.png", SPRITE_ID::BACKTITLE_TEXT_SPRITE);
	sprite.Load("laneSP.bmp", SPRITE_ID::LANE_SPRITE);
	sprite.Load("gameclearText.png", SPRITE_ID::GAMECLEAR_TEXT_SPRITE);
	sprite.Load("nextstage.png", SPRITE_ID::CHANGE_NEXTSTAGE_TEXT_SPRITE);
	sprite.Load("stageselect.png", SPRITE_ID::CHANGE_STAGESELECT_TEXT_SPRITE);
	sprite.Load("background.png", SPRITE_ID::BACKGROUND_SPRITE);
	sprite.Load("fence.png", SPRITE_ID::FENCE_SPRITE);
	sprite.Load("wind.png", SPRITE_ID::WIND_SPRITE);
	sprite.Load("gamestartText.png", SPRITE_ID::GAME_START_TEXT_SPRITE);
	sprite.Load("pauseText.png", SPRITE_ID::PAUSE_TEXT_SPRITE);
	sprite.Load("backgameplay.png", SPRITE_ID::BACK_GAMEPLAY_TEXT_SPRITE);
	sprite.Load("ceiling.png", SPRITE_ID::CEILING_SPRITE);
	sprite.Load("pbody.png", SPRITE_ID::PBODY_SPRITE);
	sprite.Load("orochi_neck2.png", SPRITE_ID::OROCHI_NECK_SPRITE);
	sprite.Load("orochi2.png", SPRITE_ID::OROCHI_HEAD_SPRITE);
	sprite.Load("sword.png", SPRITE_ID::SWORD_SPRITE);
	sprite.Load("birdsDropping.png", SPRITE_ID::BIRDS_DROPPING_SPRITE);
	sprite.Load("bird.png", SPRITE_ID::BIRD_SPRITE);
	sprite.Load("whitescreen.png", SPRITE_ID::WHITE_SCREEN_SPRITE);
	sprite.Load("blackscreen.png", SPRITE_ID::BLACK_SCREEN_SPRITE);
	sprite.Load("testGoalClothes.png", SPRITE_ID::GOAL_CLOTHES_SPRITE);
	//非結合腕アニメーションリソース
	sprite.Load("tataki/tatakianime_00001.png", SPRITE_ID::CLOTHES_TAP_01_SPRITE);
	sprite.Load("tataki/tatakianime_00002.png", SPRITE_ID::CLOTHES_TAP_02_SPRITE);
	sprite.Load("tataki/tatakianime_00003.png", SPRITE_ID::CLOTHES_TAP_03_SPRITE);
	sprite.Load("tataki/tatakianime_00004.png", SPRITE_ID::CLOTHES_TAP_04_SPRITE);
	sprite.Load("tataki/tatakianime_00005.png", SPRITE_ID::CLOTHES_TAP_05_SPRITE);
	sprite.Load("tataki/tatakianime_00006.png", SPRITE_ID::CLOTHES_TAP_06_SPRITE);
	sprite.Load("tataki/tatakianime_00007.png", SPRITE_ID::CLOTHES_TAP_07_SPRITE);
	sprite.Load("tataki/tatakianime_00008.png", SPRITE_ID::CLOTHES_TAP_08_SPRITE);
}

// ３Ｄモデルを読み込む
void Content::LoadModel(Model& model, bool async)
{
	WorkFolder::SetWorkFolder("res/Model/");

	model.Load("plane.mv1", MODEL_ID::PLANE_MODEL);
	model.Load("cube.mv1", MODEL_ID::PLAYER_MODEL);

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