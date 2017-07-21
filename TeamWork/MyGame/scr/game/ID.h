#pragma once

// アクターID
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
	DUMMY_ACTOR,
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
	TITLE_SPRITE,
	SNAKE_SPRITE,
	PLAYER_HEAD_SPRITE,
	PLAYER_HEAD_FATIGUE_SPRITE,
	BASE_CLOTHES_SPRITE,
	BASE_CLOTHES_02_SPRITE,
	BASE_CLOTHES_03_SPRITE,
	BASE_CLOTHES_04_SPRITE,
	BASE_CLOTHES_05_SPRITE,
	BASE_CLOTHES_06_SPRITE,
	BASE_CLOTHES_07_SPRITE,
	BASE_CLOTHES_08_SPRITE,
	BASE_CLOTHES_09_SPRITE,
	BASE_CLOTHES_10_SPRITE,
	BASE_CLOTHES_11_SPRITE,
	BASE_CLOTHES_12_SPRITE,
	BASE_CLOTHES_13_SPRITE,
	BASE_CLOTHES_14_SPRITE,
	BASE_CLOTHES_15_SPRITE,
	BASE_CLOTHES_16_SPRITE,
	BASE_CLOTHES_17_SPRITE,
	BASE_CLOTHES_18_SPRITE,
	BASE_CLOTHES_19_SPRITE,
	TEST_CLOTHES_SPRITE,
	GOAL_UI_SPRITE,
	HANGER_SPRITE,
	UPHANGER_SPRITE,
	UPHANGER_CODE_SPRITE,
	FLUFFY_SPRITE,
	FLUFFY_02_SPRITE,
	FLUFFY_03_SPRITE,
	FLUFFY_04_SPRITE,
	FLUFFY_05_SPRITE,
	FLUFFY_06_SPRITE,
	GUM_SPRITE,
	HAIRBALL_SPRITE,
	NOT_SHAKE_CLOTHES_SPRITE,
	NOT_SHAKE_CLOTHES_02_SPRITE,
	NOT_SLASH_CLOTHES_SPRITE,
	NOT_SLASH_CLOTHES_02_SPRITE,
	GAMEOVER_TEXT_SPRITE,
	RETRY_TEXT_SPRITE,
	BACKTITLE_TEXT_SPRITE,
	LANE_SPRITE,
	GAMEPLAY_LANE_SPRITE,
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
	PRESS_B_SPRITE,
	TITLE_POLE_SPRITE,
	RETRY_PAUSE_TEXT_SPRITE,
	BAD_SPRITE,
	LOGO_SPRITE,
	METAMOR_EFFECT_SPRITE,
	BITE_EFFECT_SPRITE,
	TAP_EFFECT_SPRITE,
	GET_SWORD_EFFECT_SPRITE,
	FATIGUE_EFFECT_SPRITE,
	SWORD_ATTACK_EFFECT_SPRITE,
	METER_START_SPRITE,
	METER_GOAL_SPRITE,
	TITLE_SELECT_SPRITE,
	RESURRECT_EFFECT_SPRITE,
	ARROW_EFFECT_SPRITE,
	BITE_RANGE_SPRITE,
	BITE_RANGE_TOP_SPRITE,
	GAMEPAD_SPRITE,
	GAMEPAD_A_SPRITE,
	GAMEPAD_B_SPRITE,
	GAMEPAD_X_SPRITE,	
	GAMEPAD_RB_SPRITE,
	GAMEPAD_LB_SPRITE,
	GAMEPAD_STICK_SPRITE,

	STAGE_SELECT_NIGHT1_SPRITE,
	STAGE_SELECT_NIGHT2_SPRITE,
	STAGE_SELECT_NIGHT3_SPRITE,
	STAR_SPRITE,
	STAGE_SELECT_1_SPRITE,
	STAGE_SELECT_2_SPRITE,
	STAGE_SELECT_3_SPRITE,
	STAGE_SELECT_4_SPRITE,
	STAGE_SELECT_5_SPRITE,
	STAGE_SELECT_6_SPRITE,
	STAGE_SELECT_7_SPRITE,
	STAGE_SELECT_8_SPRITE,
	STAGE_SELECT_TRAINING_SPRITE,

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

	WHITE_ANM_01_SPRITE,
	WHITE_ANM_02_SPRITE,
	WHITE_ANM_03_SPRITE,
	WHITE_ANM_04_SPRITE,
	WHITE_ANM_05_SPRITE,
	WHITE_ANM_06_SPRITE,
	WHITE_ANM_07_SPRITE,
	WHITE_ANM_08_SPRITE,

	END_SPRITE,

	CREDIT_SAITO_SPRITE,
	CREDIT_MIYAUCHI_SPRITE,
	CREDIT_SIM_SPRITE,
	CREDIT_SUGIURA_SPRITE,
	CREDIT_KUWABARA_SPRITE,
	CREDIT_NEGISHI_SPRITE,
	CREDIT_NAGAO_SPRITE,

	CREDIT_MPL_SPRITE,
	CREDIT_MPG_SPRITE,
	CREDIT_SPG_SPRITE,
	CREDIT_DESIGNER_SPRITE,
	CREDIT_TOWEL_SPRITE,
	CREDIT_FUTON_SPRITE,
	CREDIT_CHAIN_SPRITE,
	CREDIT_PG_SPRITE,

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
	RESURRECT_SE,
	BAD_SE,


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
	CREDIT_BGM,

	END_BGM
};

enum FONT_ID {
	TUTORIAL_FONT,
	NAME_FONT
};

// 服ID
enum CLOTHES_ID
{
	BASE_CLOTHES = 1,		//普通の服
	FLUFFY_CLOTHES,			//ふわふわな服
	THIN_CLOTHES,			//薄い生地
	NOT_SHAKE_CLOTHES,		//揺れない服
	NOT_SLASH_CLOTHES,		//切れない服
	HANGER,					//ハンガー
	TUTORIAL_CLOTHES,		//チュートリアル用の服
	HAIRBALL,				//毛玉
	GOAL_CLOTHES,			//ゴール
	START_CLOTHES,			//スタート用の服
	TEXT_CLOTHES,			//テキストの服

	GUM_CLOTHES,		//ゴムの服
	TEST_CLOTHES,		//テスト用
};
