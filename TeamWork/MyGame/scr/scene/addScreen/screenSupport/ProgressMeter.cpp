#include "ProgressMeter.h"
#include "../../../graphic/Sprite.h"
#include "../../../math/MathHelper.h"
#include "../../../input/Keyboard.h"
#include "../../../input/GamePad.h"
#include "../../../actor/Actor.h"
#include "../../../tween/TweenManager.h"

//コンストラクタ
ProgressMeter::ProgressMeter(World * world, int stageLength) :
	world_(world), stageLen_(stageLength), meterNum_(3),prevLane_(1),dis_(50),
	meterLen_(800), meterPos_(300, 300), nowLane_(1), pIconPos_(0, 0),pPosY_(50)
{
	pinHandle_ = Sprite::GetInstance().GetHandle(SPRITE_ID::OROCHI_NECK_SPRITE);
	playerHandle_ = Sprite::GetInstance().GetHandle(SPRITE_ID::OROCHI_HEAD_SPRITE);
	startHandle_ = Sprite::GetInstance().GetHandle(SPRITE_ID::HITO_SPRITE);
	goalhandle_ = Sprite::GetInstance().GetHandle(SPRITE_ID::HITO_SPRITE);

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
		prevLane_ = world_->GetKeepDatas().playerLane_; //現在のレーン
	}

	//プレイヤーアイコン座標
	pIconPos_ = Vector2(pPos.x * meterLen_ / stageLen_ + meterPos_.x, meterPos_.y + pPosY_);


}

//描画
void ProgressMeter::Draw() const
{
	if (BuildMode == 1) {
		DrawFormatString(0, 80, GetColor(255, 255, 255), "nowLane_:%d", nowLane_);

	}

	//メーターを描画
	for (int i = 0; i < meterNum_; i++) {
		DrawBox(meterPos_.x, meterPos_.y + i * dis_, meterPos_.x + meterLen_, meterPos_.y + i * dis_ + 20, GetColor(0, 255, 0), 1);
		if (i == nowLane_) {
			//現在プレイヤーがいるレーンのメーターに表示
			Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, pIconPos_, Vector2(32.0f, 32.0f), Vector2::One, 1.0f, false);
		}
	}

	//スタート
	Sprite::GetInstance().Draw(SPRITE_ID::BASE_CLOTHES_SPRITE, Vector2(meterPos_.x - 200,meterPos_.y), Vector2(32.0f, 32.0f), Vector2::One, 1.0f, false);
	//ゴール
	Sprite::GetInstance().Draw(SPRITE_ID::BASE_CLOTHES_02_SPRITE, Vector2(meterPos_.x  + meterLen_ + 70, meterPos_.y), Vector2(32.0f, 32.0f), Vector2::One, 1.0f, false);

	//ピンを描画
	world_->EachActor(ACTOR_ID::PIN_ACTOR, [=](Actor& other) {
		PinStruct pin = { other.GetLaneNum(), other.GetPosition().x };
		Sprite::GetInstance().Draw(SPRITE_ID::OROCHI_HEAD_SPRITE, Vector2(pin.posX * meterLen_ / stageLen_ + meterPos_.x, meterPos_.y + pin.lane * dis_), Vector2(32.0f, 32.0f), Vector2::One, 1.0f, false);
	});

}

//終了
void ProgressMeter::End()
{
}
