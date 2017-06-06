#include "ProgressMeter.h"
#include "../../../graphic/Sprite.h"
#include "../../../math/MathHelper.h"
#include "../../../input/Keyboard.h"
#include "../../../input/GamePad.h"
#include "../../../actor/Actor.h"
#include "../../../tween/TweenManager.h"

//�R���X�g���N�^
ProgressMeter::ProgressMeter(World * world, int stageLength, int pinPosition) :
	world_(world), stageLen_(stageLength), pin_(0), meterNum_(3),prevLane_(1),
	meterLen_(1200), meterPos_(300, 300), nowLane_(1), pIconPos_(0, 0),pPosY_(50)
{
	pinHandle_ = Sprite::GetInstance().GetHandle(SPRITE_ID::OROCHI_NECK_SPRITE);
	playerHandle_ = Sprite::GetInstance().GetHandle(SPRITE_ID::OROCHI_HEAD_SPRITE);
	startHandle_ = Sprite::GetInstance().GetHandle(SPRITE_ID::HITO_SPRITE);
	goalhandle_ = Sprite::GetInstance().GetHandle(SPRITE_ID::HITO_SPRITE);

}

//�f�X�g���N�^
ProgressMeter::~ProgressMeter()
{
}

//������
void ProgressMeter::Initialize()
{
}

//�X�V
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

	//�v���C���[�����擾
	auto pPos = world_->GetKeepDatas().playerPos_; //�v���C���[���W
	nowLane_ = world_->GetKeepDatas().playerLane_; //���݂̃��[��
	pin_.clear();
	world_->EachActor(ACTOR_ID::PIN_ACTOR, [=](Actor& other) {
		PinStruct pin = { other.GetLaneNum(), other.GetPosition().x };
		pin_.push_back(pin);
	});


	if (nowLane_ != prevLane_) {
		TweenManager::GetInstance().Add(EaseOutExpo, &pPosY_, (float)nowLane_ * 50, 2.0f);
		prevLane_ = world_->GetKeepDatas().playerLane_; //���݂̃��[��
	}

	//�v���C���[�A�C�R�����W
	pIconPos_ = Vector2(pPos.x * meterLen_ / stageLen_ + meterPos_.x, meterPos_.y + pPosY_);


}

//�`��
void ProgressMeter::Draw() const
{
	DrawFormatString(0, 80, GetColor(255, 255, 255), "nowLane_:%d", nowLane_);

	//���[�^�[��`��
	for (int i = 0; i < meterNum_; i++) {
		DrawBox(meterPos_.x, meterPos_.y + i * 50, meterPos_.x + meterLen_, meterPos_.y + i * 50 + 20, GetColor(0, 255, 0), 1);
		if (i == nowLane_) {
			//���݃v���C���[�����郌�[���̃��[�^�[�ɕ\��
			Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, pIconPos_, Vector2(32.0f, 32.0f), Vector2::One, 1.0f, false);
		}
	}
	//�s����`��
	for (int i = 0; i < pin_.size(); i++) {
		Sprite::GetInstance().Draw(SPRITE_ID::OROCHI_HEAD_SPRITE, Vector2(pin_[i].posX * meterLen_ / stageLen_ + meterPos_.x,meterPos_.y + pin_[i].lane * 50), Vector2(32.0f, 32.0f), Vector2::One, 1.0f, false);
	}

	//�X�^�[�g
	Sprite::GetInstance().Draw(SPRITE_ID::BASE_CLOTHES_SPRITE, Vector2(meterPos_.x - 200,meterPos_.y), Vector2(32.0f, 32.0f), Vector2::One, 1.0f, false);
	//�S�[��
	Sprite::GetInstance().Draw(SPRITE_ID::BASE_CLOTHES_02_SPRITE, Vector2(meterPos_.x  + meterLen_ + 70, meterPos_.y), Vector2(32.0f, 32.0f), Vector2::One, 1.0f, false);

}

//�I��
void ProgressMeter::End()
{
}

////��i�X�V
//void ProgressMeter::UpperLane()
//{
//}
//
////��i�`��
//void ProgressMeter::UpperDraw() const
//{
//}
//
////���i�X�V
//void ProgressMeter::MiddleLane()
//{
//}
//
////���i�`��
//void ProgressMeter::MiddleDraw() const
//{
//}
//
////���i�X�V
//void ProgressMeter::LowerLane()
//{
//}
//
////���i�`��
//void ProgressMeter::LowerDraw()
//{
//}
