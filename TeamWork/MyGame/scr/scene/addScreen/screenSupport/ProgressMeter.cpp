#include "ProgressMeter.h"
#include "../../../graphic/Sprite.h"
#include "../../../math/MathHelper.h"
#include "../../../input/Keyboard.h"
#include "../../../input/GamePad.h"
#include "../../../actor/Actor.h"
#include "../../../tween/TweenManager.h"

//�R���X�g���N�^
ProgressMeter::ProgressMeter(World * world, int stageLength) :
	world_(world), stageLen_(stageLength), meterNum_(3),prevLane_(1),dis_(50),
	meterLen_(800), meterPos_(WINDOW_WIDTH - 1100,50), nowLane_(1), pIconPos_(0, 0),pPosY_(50)
{
	pinHandle_ = Sprite::GetInstance().GetHandle(SPRITE_ID::OROCHI_NECK_SPRITE);
	pIconHandle_ = Sprite::GetInstance().GetHandle(SPRITE_ID::OROCHI_HEAD_SPRITE);
	laneHandle_ = Sprite::GetInstance().GetHandle(SPRITE_ID::METER_SPRITE);
	startHandle_ = Sprite::GetInstance().GetHandle(SPRITE_ID::HITO_SPRITE);
	goalHandle_ = Sprite::GetInstance().GetHandle(SPRITE_ID::HITO_SPRITE);

	pinSize_ = Sprite::GetInstance().GetSize(SPRITE_ID::SNAKE_SPRITE);
	pIconSize_ = Sprite::GetInstance().GetSize(SPRITE_ID::SNAKE_SPRITE);
	laneSize_ = Sprite::GetInstance().GetSize(SPRITE_ID::METER_SPRITE);
	startSize_ = Sprite::GetInstance().GetSize(SPRITE_ID::TEST_SPRITE);
	goalSize_ = Sprite::GetInstance().GetSize(SPRITE_ID::TEST_SPRITE);

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
	//�v���C���[�����擾
	auto pPos = world_->GetKeepDatas().playerPos_; //�v���C���[���W
	nowLane_ = world_->GetKeepDatas().playerLane_; //���݂̃��[��

	if (nowLane_ != prevLane_) {
		TweenManager::GetInstance().Add(EaseOutQuad, &pPosY_, (float)nowLane_ * dis_, 1.0f);
		prevLane_ = world_->GetKeepDatas().playerLane_; //���݂̃��[��
	}

	//�v���C���[�A�C�R�����W
	pIconPos_ = Vector2(pPos.x * meterLen_ / stageLen_ + meterPos_.x, meterPos_.y + pPosY_);

}

//�`��
void ProgressMeter::Draw() const
{
	if (BuildMode == 1) {
		DrawFormatString(0, 80, GetColor(255, 255, 255), "nowLane_:%d", nowLane_);

	}

	//���[�^�[��`��
	for (int i = 0; i < meterNum_; i++) {
		DrawBox(meterPos_.x, meterPos_.y + i * dis_, meterPos_.x + meterLen_, meterPos_.y + i * dis_ + 20, GetColor(0, 255, 0), 1);
		Sprite::GetInstance().Draw(SPRITE_ID::METER_SPRITE, Vector2(meterPos_.x,meterPos_.y + i * dis_), Vector2(0,/*laneSize_.y / 2*/0), Vector2(meterLen_ / laneSize_.x,1.0f), 1.0f, false);
		if (i == nowLane_) {
			//���݃v���C���[�����郌�[���̃��[�^�[�ɕ\��
			Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, pIconPos_, Vector2(pIconSize_.x / 2, pIconSize_.y / 2), Vector2::One, 1.0f, false);
		}
	}

	//�X�^�[�g
	Sprite::GetInstance().Draw(SPRITE_ID::TEST_SPRITE, Vector2(meterPos_.x - (startSize_.x - 20),meterPos_.y), Vector2().Zero, Vector2::One, 1.0f, false);
	//�S�[��
	Sprite::GetInstance().Draw(SPRITE_ID::TEST_SPRITE, Vector2(meterPos_.x  + (meterLen_ + 20), meterPos_.y), Vector2().Zero, Vector2::One, 1.0f, false);

	//�s����`��
	world_->EachActor(ACTOR_ID::PIN_ACTOR, [=](Actor& other) {
		PinStruct pin = { other.GetLaneNum(), other.GetPosition().x };
		Sprite::GetInstance().Draw(SPRITE_ID::OROCHI_HEAD_SPRITE, Vector2(pin.posX * meterLen_ / stageLen_ + meterPos_.x, meterPos_.y + pin.lane * dis_), Vector2(pinSize_.x / 2,pinSize_.y / 2), Vector2::One, 1.0f, false);
	});

}

//�I��
void ProgressMeter::End()
{
}
