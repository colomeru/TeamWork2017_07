#pragma once

// AN^[ID
enum ACTOR_ID
{
	BEGIN_ACTOR,	
	LANE_ACTOR,
	STAGE_ACTOR,
	GOAL_ACTOR,
	PIN_ACTOR,
	HANGER_ACTOR,
	SAMPLE_ACTOR,
	BACK_GROUND_ACTOR,
	PLAYER_SWORD_ACTOR,
	PLAYER_ACTOR,
	PLAYER_HEAD_ACTOR,
	ENEMY_ACTOR,
	EFECT_ACTOR,
	CAMERA_ACTOR,
	UI_ACTOR,
	CLOTHES_DROPING_ACTOR,
	HAIRBALL_ACTOR,
	EFFECT_ACTOR,
	END_ACTOR,
};

// UIAN^[ID
enum UI_ID
{
	BEGIN_UI,

	END_UI
};

// ÕË»ID
enum COL_ID
{
	BEGIN_COL,
	TEST_COL,
	BOX_SEGMENT_COL,
	BOX_CLOTHES_COL,
	CLOTHES_BOX_COL,
	BOX_BOX_COL,
	PLAYER_PIN_COL,
	BOX_HANGER_COL,
	PHEAD_CLOTHES_COL,
	DROP_PHEAD_COL,
	TAPPER_PHEAD_COL,
	PSWORD_CLOTHES_COL,
	TAPPER_PSWORD_COL,
	PHEAD_HAIRBALL_COL,
	PSWORD_HAIRBALL_COL,
	DROP_CLOTHES_COL,
	PHEAD_GOAL_COL,
	END_COL,
};

// XvCgID
enum SPRITE_ID
{
	BEGIN_SPRITE,
	TEST_SPRITE,
	TEST2_SPRITE,
	ARROW_SPRITE,
	CIRCLE_SPRITE,
	SPHERE_SPRITE,
	HITO_SPRITE,
	TITLE_SPRITE,
	SNAKE_SPRITE,
	PLAYER_HEAD_SPRITE,
	PLAYER_HEAD_FATIGUE_SPRITE,
	BASE_CLOTHES_SPRITE,
	TEST_CLOTHES_SPRITE,
	GOAL_UI_SPRITE,
	HANGER_SPRITE,
	FLUFFY_SPRITE,
	GUM_SPRITE,
	HAIRBALL_SPRITE,
	GAMEOVER_TEXT_SPRITE,
	RETRY_TEXT_SPRITE,
	BACKTITLE_TEXT_SPRITE,
	LANE_SPRITE,
	LANE_SIDE_SPRITE,
	LANE_SIDE2_SPRITE,
	GAMECLEAR_TEXT_SPRITE,
	CHANGE_NEXTSTAGE_TEXT_SPRITE,
	CHANGE_STAGESELECT_TEXT_SPRITE,
	BACKGROUND_1_SPRITE,
	BACKGROUND_2_SPRITE,
	BACKGROUND_3_SPRITE,
	BACKGROUND_4_SPRITE,
	BACKGROUND_5_SPRITE,
	BACKGROUND_6_SPRITE,
	BACKGROUND_7_SPRITE,
	BACKGROUND_8_SPRITE,
	FENCE_SPRITE,
	WIND_SPRITE,
	GAME_START_TEXT_SPRITE,
	PAUSE_TEXT_SPRITE,
	BACK_GAMEPLAY_TEXT_SPRITE,
	CEILING_SPRITE,
	PBODY_SPRITE,
	END_SPRITE,
	OROCHI_NECK_SPRITE,
	OROCHI_HEAD_SPRITE,
	OROCHI_HEAD_SHOOT_SPRITE,
	OROCHI_HEAD_SHOOT_END_SPRITE,
	WHITE_SCREEN_SPRITE,
	BLACK_SCREEN_SPRITE,
	TITLE_START_SPRITE,
	TITLE_EXIT_SPRITE,
	SWORD_SPRITE,
	BIRDS_DROPPING_SPRITE,
	BIRD_SPRITE,
	GOAL_CLOTHES_SPRITE,
	MOVE_GOAL_CLOTHES_SPRITE,
	BASE_CLOTHES_02_SPRITE,
	TOWEL_CLOTHES_SPRITE,
	METER_SPRITE,
	NECK_CHAIN1_SPRITE,
	NECK_CHAIN2_SPRITE,
	OROCHI_DEAD_SPRITE,
	STAGE_SELECT_BACK_SPRITE,
	STAGE_SELECT_M_SPRITE,
	WWW_SPRITE,
	OROCHI_CURSOR_SPRITE,
	TO_STAGESELECT_SPRITE,
	BITECOUNT_SPRITE,
	HEADCOUNT_SPRITE,
	NOWLOADING_SPRITE,
	DOT_SPRITE,

	METAMOR_EFFECT_SPRITE,
	BITE_EFFECT_SPRITE,
	TAP_EFFECT_SPRITE,
	GET_SWORD_EFFECT_SPRITE,
	FATIGUE_EFFECT_SPRITE,
	SWORD_ATTACK_EFFECT_SPRITE,

	NUMBER_SPRITE,
	SCORE_STAR_SPRITE,

	CLOTHES_TAP_01_SPRITE,
	CLOTHES_TAP_02_SPRITE,
	CLOTHES_TAP_03_SPRITE,
	CLOTHES_TAP_04_SPRITE,
	CLOTHES_TAP_05_SPRITE,
	CLOTHES_TAP_06_SPRITE,
	CLOTHES_TAP_07_SPRITE,
	CLOTHES_TAP_08_SPRITE,

	CLOTHES_TAP_DEAD_01_SPRITE,
	CLOTHES_TAP_DEAD_02_SPRITE,
	CLOTHES_TAP_DEAD_03_SPRITE,
	CLOTHES_TAP_DEAD_04_SPRITE,
	CLOTHES_TAP_DEAD_05_SPRITE,
	CLOTHES_TAP_DEAD_06_SPRITE,
	CLOTHES_TAP_DEAD_07_SPRITE,
	CLOTHES_TAP_DEAD_08_SPRITE,
	CLOTHES_TAP_DEAD_09_SPRITE,
	CLOTHES_TAP_DEAD_10_SPRITE,
	CLOTHES_TAP_DEAD_11_SPRITE,
	CLOTHES_TAP_DEAD_12_SPRITE,
	CLOTHES_TAP_DEAD_13_SPRITE,
	CLOTHES_TAP_DEAD_14_SPRITE,
	CLOTHES_TAP_DEAD_15_SPRITE,
	CLOTHES_TAP_DEAD_16_SPRITE,
	CLOTHES_TAP_DEAD_17_SPRITE,
	CLOTHES_TAP_DEAD_18_SPRITE,

	LOAD_ANM_1_SPRITE,
	LOAD_ANM_2_SPRITE,
	LOAD_ANM_3_SPRITE,
	LOAD_ANM_4_SPRITE,
	LOAD_ANM_5_SPRITE,
	LOAD_ANM_6_SPRITE,
	LOAD_ANM_7_SPRITE,
	LOAD_ANM_8_SPRITE,

	CROW_ANM_01_SPRITE,
	CROW_ANM_02_SPRITE,
	CROW_ANM_03_SPRITE,
	CROW_ANM_04_SPRITE,
	CROW_ANM_05_SPRITE,
	CROW_ANM_06_SPRITE,
	CROW_ANM_07_SPRITE,
	CROW_ANM_08_SPRITE,

};

// fID
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

	MOVE_CURSOR_SE,
	CANCEL_SE,
	TAP_SE,
	CHANGE_HEAD_SE,
	BITE_SE,
	BITE_END_SE,
	FATIGUE_SE,
	CHECK_SE,
	HEAD_SHOOT_SE,
	WIND_SE,
	POWERFUL_WIND_SE,
	CLOTHES_WIND_SE,
	FALL_OUT_SE,
	CREATE_SWORD_SE,
	SLASH_SE,



	END_SE
};

// BGM_ID
enum BGM_ID
{
	BEGIN_BGM,

	TITLE_BGM,
	STAGE_01_BGM,
	STAGE_02_BGM,
	STAGE_03_BGM,
	STAGE_CLEAR_BGM,
	GAME_OVER_BGM,
	STAGE_SELECT_BGM,

	END_BGM
};

enum FONT_ID {
	TUTORIAL_FONT,
};

// ID
enum CLOTHES_ID
{
	TEST_CLOTHES,		//eXgp
	BASE_CLOTHES,		//ÊÌ
	GUM_CLOTHES,		//SÌ
	FLUFFY_CLOTHES,		//ÓíÓíÈ
	THIN_CLOTHES,		//¢¶n
	HANGER,				//nK[
	GOAL_CLOTHES,		//S[
	HAIRBALL,			//ÑÊ
};
