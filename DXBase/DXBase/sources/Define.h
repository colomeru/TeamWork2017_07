#pragma once

#include "Math/Math.h"

/* ウインドウ系統 */

//// スクリーンのサイズ倍率
//const static float		SCREEN_RATE = 200;

const static char*		WINDOW_NAME = "window";
// スクリーンのサイズ
const static Vector2	SCREEN_SIZE = Vector2(1920, 1080);
// スクリーンのアスペクト比
const static float		SCREEN_ASPECT = SCREEN_SIZE.x / SCREEN_SIZE.y;
// ウインドウの表示倍率(おそらくデバック時のみ)
const static float		WINDOW_RATE = 0.5f;
// ウインドウモードにするかしないか(TRUE : FALSE)
const static int		WINDOW_MODE = TRUE;


//追加する定数は自分の名前の下に書いてください。
//競合が起こる可能性が高いので扱いには注意

/*** 矢嶋 ***/
const static Vector2	BOSSBODY_POSITION = Vector2(SCREEN_SIZE.x - 96 * 3, SCREEN_SIZE.y + 96 * 10);



/*** 片岡 ***/




/*** 宮内 ***/

const static float CHIPSIZE = 96;

const static float TURNLESS_FLOOR_SIZE = 1;

const static float RIGHTLEFTRANGE = 3;

const static float UPDOWNRANGE = 3;

static const int FlashTempo = 3;

static const float LRMoveSpeed = 2;

static const float UDMoveSpeed = 2;

static const int CountPos = 500;

/*** 永野 ***/

/* 一般的なパラメータ */
// 重力の値
static const float		GRAVITY = 9.8f;
// 符号(+)
static const int		SIGN_PLUS = 1;
// 符号(-)
static const int		SIGN_MINUS = -1;

/* プレイヤーパラメータ系統 */
// プレイヤーのデフォルトの開始位置
static const Vector2	START_POS = Vector2(700, 1500);
// プレイヤーの表示位置
const static Vector2	PLAYER_SCREEN_POSITION = Vector2(SCREEN_SIZE.x * 0.5f, SCREEN_SIZE.y * 0.75f);
// プレイヤーの移動速度
const static float		PLAYER_SPEED = 10.0f;
// プレイヤーの本体の半径
const static float		PLAYER_RADIUS = CHIPSIZE * 0.75f;
// プレイヤーの本体の衝突の半径
const static float		PLAYER_COLLIDER_RADIUS = PLAYER_RADIUS * 1.25f;
// プレイヤーの通常の長さ
const static float		PLAYER_MAX_NORMAL_LENGTH = CHIPSIZE * 2;
// プレイヤーが伸びた場合の最大値
const static float		PLAYER_MAX_STRETCH_LENGTH = CHIPSIZE * 3.5f;
// プレイヤーが伸びた場合の最大値
const static float		PLAYER_SWIM_LENGTH = PLAYER_MAX_STRETCH_LENGTH * 0.3f;
// プレイヤーの発射速度
const static float		PLAYER_LAUNCH_POWER = 120.0f;
// プレイヤーのジャンプ速度
const static float		PLAYER_JUMP_POWER = 30.0f;
// プレイヤーの仰け反り速度
const static float		PLAYER_LEANBACK_POWER = 10.0f;
// プレイヤーの無敵時間
const static float		PLAYER_INV_TIME = 2.5f;
// プレイヤーの死亡までの時間
const static float		PLAYER_DEAD_LIMIT = 5.0f;
// プレイヤーの硬直時間
const static float		PLAYER_FREEZE_TIME = 0.5f;
// プレイヤーコネクタの分割数
const static int		PLAYER_CNTR_DIV_NUM = 7;
// プレイヤーコネクタの分割数
const static float		PLAYER_MIN_DIV_LENGTH = PLAYER_MAX_NORMAL_LENGTH / static_cast<float>(PLAYER_CNTR_DIV_NUM + 1);
// プレイヤーコネクタの分割数
const static float		PLAYER_MAX_DIV_LENGTH = PLAYER_MAX_STRETCH_LENGTH / static_cast<float>(PLAYER_CNTR_DIV_NUM + 1);
// プレイヤーの坂速度
const static float		SLIP_SPEED = 70.0f;
// プレイヤーのバネ剛性
const static float		SPRING_STIFFNESS = 4.0f;
// プレイヤーのバネ摩擦力
const static float		SPRING_FRICTION = 0.1f;
// プレイヤーのバネ質量
const static float		SPRING_MASS = 0.8f;











