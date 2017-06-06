#include "ProgressMeter.h"
#include "../../../graphic/Sprite.h"
#include "../../../math/MathHelper.h"
#include "../../../input/Keyboard.h"
#include "../../../input/GamePad.h"

//コンストラクタ
ProgressMeter::ProgressMeter(World * world, int stageLength, int pinPosition) :
	world_(world), stageLen_(stageLength), pinPos_(pinPosition), meterNum_(3),
	meterLen_(1200), meterPos_(100, 100), nowLane_(1),pIconPos_(0,0)
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
	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::UP) ||
	//	GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::UP))
	//{
	//	nowLane_ -= 1;
	//}
	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::DOWN) ||
	//	GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::DOWN))
	//{
	//	nowLane_ += 1;
	//}
	//nowLane_ = MathHelper::Clamp(nowLane_, 0, 2);

	//プレイヤー情報を取得
	auto pPos = world_->GetKeepDatas().playerPos_; //プレイヤー座標
	nowLane_ = world_->GetKeepDatas().playerLane_; //現在のレーン
	

	//プレイヤーアイコン座標
	pIconPos_ = Vector2(pPos.x * meterLen_ / stageLen_ + meterPos_.x, meterPos_.y + nowLane_ * 50);


}

//描画
void ProgressMeter::Draw() const
{
	DrawFormatString(0, 80, GetColor(255, 255, 255), "nowLane_:%d",nowLane_);

	for (int i = 0; i < meterNum_; i++) {
		DrawBox(meterPos_.x, meterPos_.y + i * 50, meterPos_.x + meterLen_, meterPos_.y + i * 50 + 20, GetColor(0, 255, 0), 1);
		if (i == nowLane_) {
			//現在プレイヤーがいるレーンのメーターに表示
			Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, pIconPos_, Vector2(32.0f, 32.0f), Vector2::One, 1.0f, false);

		}

	}
}

//終了
void ProgressMeter::End()
{
}

////上段更新
//void ProgressMeter::UpperLane()
//{
//}
//
////上段描画
//void ProgressMeter::UpperDraw() const
//{
//}
//
////中段更新
//void ProgressMeter::MiddleLane()
//{
//}
//
////中段描画
//void ProgressMeter::MiddleDraw() const
//{
//}
//
////下段更新
//void ProgressMeter::LowerLane()
//{
//}
//
////下段描画
//void ProgressMeter::LowerDraw()
//{
//}
