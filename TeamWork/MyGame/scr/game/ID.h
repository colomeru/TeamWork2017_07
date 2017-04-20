#pragma once

// アクターID
enum ACTOR_ID
{
	BEGIN_ACTOR,	
	STAGE_ACTOR,
	SAMPLE_ACTOR,
	
	PLAYER_ACTOR,
	ENEMY_ACTOR,
	EFECT_ACTOR,
	CAMERA_ACTOR,
	END_ACTOR,
};

// UIアクターID
enum UI_ID
{
	BEGIN_UI,

	END_UI
};

// 衝突判ID
enum COL_ID
{
	BEGIN_COL,
	TEST_COL,
	BOX_SEGMENT_COL,
	BOX_CLOTHES_COL,
	END_COL,
};

// スプライトID
enum SPRITE_ID
{
	BEGIN_SPRITE,
	TEST_SPRITE,
	TEST2_SPRITE,
	ARROW_SPRITE,
	CIRCLE_SPRITE,
	SPHERE_SPRITE,
	HITO_SPRITE,
	SNAKE_SPRITE,
	END_SPRITE
};

// モデルID
enum MODEL_ID
{
	BEGIN_MODEL,
	PLANE_MODEL,
	PLAYER_MODEL,
	END_MODEL,
};

// SE_ID
enum SE_ID
{
	BEGIN_SE,

	END_SE
};

// BGM_ID
enum BGM_ID
{
	BEGIN_BGM,

	END_BGM
};

// 服ID
enum CLOTHES_ID
{
	TEST_CLOTHES,
	BASE_CLOTHES,
};
