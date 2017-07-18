#include "Player_Head.h"

#include "../../input/Keyboard.h"
#include "../../conv/DXConverter.h"
#include "../../graphic/Model.h"
#include"../Field/Clothes/Clothes.h"
#include"../Field/ClothesPin.h"
#include"../Effects/PlayerEffect/PlayerFatigueEffect.h"
#include"../../sound/sound.h"
#include"../../time/Time.h"

Player_Head::Player_Head(IWorld * world, Player* targetP, Vector2 pos, int myNumber)
	:Actor(world, targetP), biteSpriteTimer_(0.0f)
	, isHit_(false), isBitePoint_(false), player_(targetP), myNumber_(myNumber), isHitOnce(true), posAddVect_(Vector2::Zero), fatigueCheckColor_(0),isBiteSlipWind_(false), isAlreadyCreateSplash_(false)
{
	spriteId_ = SPRITE_ID::OROCHI_HEAD_SPRITE;

	parameter_.ID = ACTOR_ID::PLAYER_HEAD_ACTOR;
	parameter_.radius = Sprite::GetInstance().GetSize(spriteId_).x / 3.f;
	parameter_.size = Sprite::GetInstance().GetSize(spriteId_);
	parameter_.HP = 10;

	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3::Zero);


	position_ = pos;


	colFuncMap_[COL_ID::PHEAD_CLOTHES_COL] = std::bind(&CollisionFunction::IsHit_Circle_Capsules, colFunc_, std::placeholders::_1, std::placeholders::_2);
	colFuncMap_[COL_ID::BOX_HANGER_COL] = std::bind(&CollisionFunction::IsHit_PHead_Hanger, colFunc_, std::placeholders::_1, std::placeholders::_2);
	colFuncMap_[COL_ID::PHEAD_GOAL_COL] = std::bind(&CollisionFunction::IsHit_PHead_Goal, colFunc_, std::placeholders::_1, std::placeholders::_2);

}

Player_Head::~Player_Head()
{
}

void Player_Head::Update()
{
	//服を噛んでいる時は、頭の色を赤く、離すと元の色に戻していく
	if (player_->GetIsClearMode() && player_->GetCurHead() == myNumber_)fatigueCheckColor_ -= 10;
	if (player_->GetIsResistMode() && player_->GetCurHead() == myNumber_)fatigueCheckColor_ += 10;
	else if (player_->GetIsBiteMode() && player_->GetCurHead() == myNumber_)fatigueCheckColor_ = (int)MathHelper::Lerp(0.f, 255.f, 1 - player_->GetSlipCount() / defSlipCount);
	else {
		fatigueCheckColor_ -= 2;
		isAlreadyCreateSplash_ = false;
	}

	if (player_->GetCurHead()==myNumber_&&fatigueCheckColor_ >= 200)CreateFatigueEffect();

	fatigueCheckColor_ = MathHelper::Clamp(fatigueCheckColor_, 0,255);
	
	biteSpriteTimer_ -= Time::DeltaTime;
	biteSpriteTimer_ = max(biteSpriteTimer_,0.0f);

	if (player_->GetCurHead() == myNumber_)player_->SetStopPos(position_);
	auto basePos = player_->GetHeadPos(myNumber_);
	
	//プレイヤーから各ヘッドまでの長さ、(32,32のLength)*自分の首の長さ
	Vector2 vel = basePos - player_->GetPosition();
	//自分の首の向き*自分の首に設定されている長さ
	if (player_->GetCurHead() == myNumber_&&(player_->GetIsSlipped()|| world_->GetIsCamChangeMode()||player_->GetIsClearShoot())) {
		position_ = player_->GetSlipHeadPoint();
	}
	else {
		Vector2 bPlusLngPos = vel*player_->GetHeadLengthChangeToPosMult(myNumber_);
		position_ = basePos + bPlusLngPos;
	}

	if (player_->GetCurHead() == myNumber_) {
		if (player_->GetIsBiteMode()||player_->GetIsClearBite())	position_ = player_->GetStopPos();
	}
	else {
		isHit_ = false;
	}

	Vector3 toMatPos = Vector3(position_.x, position_.y, 0);

	parameter_.mat.Translation(toMatPos);

	if (player_->GetPHeadDead(myNumber_))return;
	if (player_->GetIsShootModeEnd()&&player_->GetCurHead()==myNumber_) {
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::PHEAD_CLOTHES_COL);
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::HANGER_ACTOR, COL_ID::BOX_HANGER_COL);
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::GOAL_ACTOR, COL_ID::PHEAD_GOAL_COL);
	}
	
}

void Player_Head::Draw() const
{
	if (player_->GetPHeadDead(myNumber_)) {
		if (player_->GetIsClearMode())return;

		if (player_->GetCurHead() == myNumber_) {
			auto pos = GetDrawPosVect(position_);
			auto vect= position_ - player_->GetPosition();
			vect = vect.Normalize();
			pos += vect * 20.f;
			Vector2 badOrigin = Sprite::GetInstance().GetSize(BAD_SPRITE) / 2;

			Sprite::GetInstance().Draw(BAD_SPRITE, pos, badOrigin, parameter_.spriteAlpha_, Vector2::One, 0, true,player_->IsLookBack() );
		}
		return;
	}

	auto is = Matrix::CreateRotationZ(angle_);
	auto pos = GetDrawPosVect(position_);
	auto sizeVec = Vector3((parameter_.size.x / 2), (parameter_.size.y / 2));

	auto box1 = Vector3(-sizeVec.x, -sizeVec.y)*is;
	auto box2 = Vector3(+sizeVec.x, -sizeVec.y)*is;
	auto box3 = Vector3(-sizeVec.x, +sizeVec.y)*is;
	auto box4 = Vector3(+sizeVec.x, +sizeVec.y)*is;

	//左上,右上,左下,右下
	auto pos1 = Vector3(pos.x + box1.x, pos.y + box1.y);
	auto pos2 = Vector3(pos.x + box2.x, pos.y + box2.y);
	auto pos3 = Vector3(pos.x + box3.x, pos.y + box3.y);
	auto pos4 = Vector3(pos.x + box4.x, pos.y + box4.y);

	float angle = (float)(((int)MathAngle(player_->GetPosition()- position_))%360);
	if (getIsCurrentHead()) {
		//float angle = (float)(((int)MathAngle(player_->GetHeadPos() - position_)) % 360);
		angle = player_->GetRot()-90.f;
		if(player_->PlayerModeChecker(Player_Mode::MODE_BITE)) {
			angle = MathHeadRotation_Bite();
		}
		else {
			angle = MathHeadRotation_Fall();
		}
		if (angle > 90 && 180 > angle)angle = 90;
		else if ((angle < 270 && 180 <= angle)||angle<=-90.f) {
			angle = 270;
		}
		Vector2 headOrigin = Sprite::GetInstance().GetSize(spriteId_) / 2;

		if (player_->GetIsShootMode())
		{
			Sprite::GetInstance().Draw(SPRITE_ID::OROCHI_HEAD_SHOOT_SPRITE, pos, headOrigin, parameter_.spriteAlpha_, Vector2::One, angle, true, false);
		}
		else if (biteSpriteTimer_ > 0.01f) {
			Sprite::GetInstance().Draw(SPRITE_ID::OROCHI_HEAD_SHOOT_END_SPRITE, pos, headOrigin, parameter_.spriteAlpha_, Vector2::One, angle, true, false);

		}
		else {
			Sprite::GetInstance().Draw(spriteId_, pos, headOrigin, parameter_.spriteAlpha_, Vector2::One, angle, true, false);
			Sprite::GetInstance().Draw(SPRITE_ID::PLAYER_HEAD_FATIGUE_SPRITE, pos, headOrigin, ((float)fatigueCheckColor_ / 255.f)*parameter_.spriteAlpha_, Vector2::One, angle, true, false);
		}
	}
	else {
		Vector2 headOrigin = Sprite::GetInstance().GetSize(SPRITE_ID::PLAYER_HEAD_SPRITE) / 2;
		Sprite::GetInstance().Draw(SPRITE_ID::PLAYER_HEAD_SPRITE, pos, headOrigin, parameter_.spriteAlpha_, Vector2::One, angle, true, false);
	}



	
}

void Player_Head::OnUpdate()
{
}

void Player_Head::OnCollide(Actor& other, CollisionParameter colpara)
{
	if (player_->GetPHeadDead(myNumber_))return;

	isBitePoint_ = true;


	if (player_->GetCurHead() != myNumber_) {
		return;
	}
}

void Player_Head::OnMessage(EventMessage message, void * param)
{
}

void Player_Head::UpdatePos()
{
	if (player_->GetCurHead() == myNumber_)player_->SetStopPos(position_);
	auto basePos = player_->GetHeadPos(myNumber_);

	//プレイヤーから各ヘッドまでの長さ、(32,32のLength)*自分の首の長さ
	Vector2 vel = basePos - player_->GetPosition();
	//自分の首の向き*自分の首に設定されている長さ
	if (player_->GetCurHead() == myNumber_ && (player_->GetIsSlipped() || world_->GetIsCamChangeMode() || player_->GetIsClearShoot())) {
		position_ = player_->GetSlipHeadPoint();
	}
	else {
		Vector2 bPlusLngPos = vel*player_->GetHeadLengthChangeToPosMult(myNumber_);
		position_ = basePos + bPlusLngPos;
	}

}

void Player_Head::CreateFatigueEffect()
{
	if (isAlreadyCreateSplash_)return;

	world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerFatigueEffect>(world_,position_,this));
	Sound::GetInstance().PlaySE(SE_ID::FATIGUE_SE);
	isAlreadyCreateSplash_ = true;
}

float Player_Head::MathHeadRotation_Bite()const
{
	return player_->GetRot() - 90.f;
}

float Player_Head::MathHeadRotation_Fall()const
{
	return (float)(((int)MathAngle(player_->GetHeadPos() - position_)) % 360);

}

