#include "Content.h"
#include "../game/WorkFolder.h"
#include "../graphic/Model.h"
#include "../graphic/Sprite.h"
//#include "../sound/Sound.h"
#include "../Def.h"
#include "../math/MathHelper.h"
#include "ID.h"

// âÊëúÇì«Ç›çûÇﬁ
void Content::LoadSprite(Sprite& sprite, Model& model)
{
	WorkFolder::SetWorkFolder("res/Sprite/");
	sprite.Load("particle.bmp",SPRITE_ID::TEST_SPRITE);
	sprite.Load("3219116.png", SPRITE_ID::TEST2_SPRITE);
	sprite.Load("arrow.png",SPRITE_ID::ARROW_SPRITE);
	sprite.Load("title_background.png", SPRITE_ID::TITLE_SPRITE);
	sprite.Load("circle.png", SPRITE_ID::CIRCLE_SPRITE);
	sprite.Load("sphere.png", SPRITE_ID::SPHERE_SPRITE);
	sprite.Load("êl.png", SPRITE_ID::HITO_SPRITE);
	sprite.Load("snake.png", SPRITE_ID::SNAKE_SPRITE);
	sprite.Load("head.bmp", SPRITE_ID::PLAYER_HEAD_SPRITE);
	sprite.Load("headFatigue.bmp", SPRITE_ID::PLAYER_HEAD_FATIGUE_SPRITE);
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
	
}

// ÇRÇcÉÇÉfÉãÇì«Ç›çûÇﬁ
void Content::LoadModel(Model& model, bool async)
{
	WorkFolder::SetWorkFolder("res/Model/");

	model.Load("plane.mv1", MODEL_ID::PLANE_MODEL);
	model.Load("cube.mv1", MODEL_ID::PLAYER_MODEL);

	WorkFolder::SetWorkFolder("res/Model2D/");
}

// âπäyÇì«Ç›çûÇﬁ
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