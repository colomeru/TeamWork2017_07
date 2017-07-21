#include "ProgressMeter.h"
#include "../../../graphic/Sprite.h"
#include "../../../math/MathHelper.h"
#include "../../../input/Keyboard.h"
#include "../../../input/GamePad.h"
#include "../../../actor/Actor.h"
#include "../../../tween/TweenManager.h"
#include "../../../debugdata/DebugDraw.h"

//コンストラクタ
ProgressMeter::ProgressMeter(World * world, int stageLength, Vector2 position) :
	world_(world), stageLen_(stageLength), meterNum_(3), prevLane_(1), dis_(30),
	meterLen_(700), meterPos_(position), nowLane_(1), pIconPos_(0, 0), pPosY_(30)
{
	Sprite& ins = Sprite::GetInstance();
	pinSize_ = ins.GetSize(SPRITE_ID::PLAYER_HEAD_SPRITE);
	pIconSize_ = ins.GetSize(SPRITE_ID::SNAKE_SPRITE);
	laneSize_ = ins.GetSize(SPRITE_ID::METER_SPRITE);
	startSize_ = ins.GetSize(SPRITE_ID::METER_START_SPRITE);
	goalSize_ = ins.GetSize(SPRITE_ID::METER_GOAL_SPRITE);

}

//デストラクタ
ProgressMeter::~ProgressMeter()
{
}

//初期化
void ProgressMeter::Initialize()
{
}

//更新
void ProgressMeter::Update()
{
	//プレイヤー情報を取得
	auto pPos = world_->GetKeepDatas().playerPos_; //プレイヤー座標
	nowLane_ = world_->GetKeepDatas().playerLane_; //現在のレーン

	if (nowLane_ != prevLane_) {
		TweenManager::GetInstance().Add(EaseOutQuad, &pPosY_, (float)nowLane_ * dis_, 1.0f);
		prevLane_ = world_->GetKeepDatas().playerLane_;
	}

	//プレイヤーアイコン座標
	pIconPos_ = Vector2(pPos.x * meterLen_ / stageLen_ + meterPos_.x, meterPos_.y + pPosY_);
	pIconPos_.x = MathHelper::Clamp(pIconPos_.x, meterPos_.x, meterPos_.x + meterLen_ - pIconSize_.x / 4);

}

//描画
void ProgressMeter::Draw() const
{
	Sprite& ins = Sprite::GetInstance();

	//スタート
	ins.Draw(SPRITE_ID::METER_START_SPRITE, Vector2(meterPos_.x - startSize_.x / 2, meterPos_.y + 35), Vector2(startSize_.x / 2, startSize_.y / 2), Vector2::One, 1.0f, false);
	//ゴール
	ins.Draw(SPRITE_ID::METER_GOAL_SPRITE, Vector2(meterPos_.x + meterLen_ + goalSize_.x / 2, meterPos_.y + 35), Vector2(goalSize_.x / 2, goalSize_.y / 2), Vector2::One, 1.0f, false);

	//メーターを描画
	for (int i = 0; i < meterNum_; i++) {
		ins.Draw(SPRITE_ID::METER_SPRITE, Vector2(meterPos_.x, meterPos_.y + i * dis_), Vector2(0, laneSize_.y / 4), Vector2(meterLen_ / laneSize_.x, 0.5f), 1.0f, false);
		if (i == nowLane_) {
			//現在プレイヤーがいるレーンのメーターに表示
			ins.Draw(SPRITE_ID::OROCHI_HEAD_SPRITE, pIconPos_, Vector2(pIconSize_.x / 4, pIconSize_.y / 1.5), Vector2(0.5f, 0.5f), 1.0f, false);
		}
	}

	//ピンを描画
	world_->EachActor(ACTOR_ID::PIN_ACTOR, [=](Actor& other) {
		PinStruct pin = { other.GetLaneNum(), other.GetPosition().x };
		Sprite::GetInstance().Draw(SPRITE_ID::PLAYER_HEAD_SPRITE, Vector2(pin.posX * meterLen_ / stageLen_ + meterPos_.x, meterPos_.y + pin.lane * dis_ + pinSize_.y / 8), Vector2(pinSize_.x / 4, pinSize_.y / 4), Vector2(0.5f, 0.5f), 180.0f);
	});

	//デバッグ表示
	if (BuildMode != 1) return;
	DebugDraw::DebugDrawFormatString(0, 80, GetColor(255, 255, 255), "nowLane_:%d", nowLane_);
	DebugDraw::DebugDrawFormatString(0, 100, GetColor(255, 255, 255), "pPosY:%f", pPosY_);
}

//終了
void ProgressMeter::End()
{
}
