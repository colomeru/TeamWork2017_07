#include "Player_Head.h"

#include "../../input/Keyboard.h"
#include "../../conv/DXConverter.h"
#include "../../graphic/Model.h"
#include"../Field/Clothes/Clothes.h"
#include"../Field/ClothesPin.h"

Player_Head::Player_Head(IWorld * world, Player* targetP, Vector2 pos, int myNumber)
	:Actor(world, targetP)
	, isHit_(false), isBitePoint_(false), player_(targetP), myNumber_(myNumber), isHitOnce(true), posAddVect_(Vector2::Zero), fatigueCheckColor_(0),isBiteSlipWind_(false)
{
	spriteId_ = SPRITE_ID::OROCHI_HEAD_SPRITE;

	parameter_.ID = ACTOR_ID::PLAYER_HEAD_ACTOR;
	parameter_.radius = Sprite::GetInstance().GetSize(spriteId_).x / 2;
	parameter_.size = Sprite::GetInstance().GetSize(spriteId_);
	parameter_.HP = 10;

	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3::Zero);


	position_ = pos;


	colFuncMap_[COL_ID::PHEAD_CLOTHES_COL] = std::bind(&CollisionFunction::IsHit_Circle_Capsules, colFunc_, std::placeholders::_1, std::placeholders::_2);
	colFuncMap_[COL_ID::BOX_HANGER_COL] = std::bind(&CollisionFunction::IsHit_PHead_Hanger, colFunc_, std::placeholders::_1, std::placeholders::_2);
}

Player_Head::~Player_Head()
{
}

void Player_Head::Update()
{
	//自分が死んでたら更新を行わない
	if (player_->GetPHeadDead(myNumber_))return;
	//服を噛んでいる時は、頭の色を赤く、離すと元の色に戻していく
	if (player_->GetIsResistMode() && player_->GetCurHead() == myNumber_)fatigueCheckColor_ += 10.0f;
	else if (player_->GetIsBiteMode() && player_->GetCurHead() == myNumber_)fatigueCheckColor_ = MathHelper::Lerp(0.f, 255.f, 1 - player_->GetSlipCount() / defSlipCount);
	else {
		fatigueCheckColor_ -= 2;
		fatigueCheckColor_ = max(fatigueCheckColor_, 0);
	}
	//Vector2 posAddP = position_;
	
	//風に吹かれた服に当たってかつ吹かれていない服につかめてない場合のみ落ちる
	//if (isBiteSlipWind_) {
	//	player_->SetMode(MODE_SHOOT);
	//	isBiteSlipWind_ = false;
	//}
	//
	//stopPos_ = position_;
	if (player_->GetCurHead() == myNumber_)player_->SetStopPos(position_);
	auto basePos = player_->GetHeadPos(myNumber_);
	
	//プレイヤーから各ヘッドまでの長さ、(32,32のLength)*自分の首の長さ
	Vector2 vel = basePos - player_->GetPosition();
	//自分の首の向き*自分の首に設定されている長さ
	if (player_->GetCurHead() == myNumber_&&(player_->GetIsSlipped()|| world_->GetIsCamChangeMode())) {
		//vel = player_->GetHeadPosAddVect();
		position_ = player_->GetSlipHeadPoint();
	}
	else {
		Vector2 bPlusLngPos = vel*player_->GetHeadLengthChangeToPosMult(myNumber_);
		//スリップしたかを調べて
		//if (player_->GetCurHead() == myNumber_&&player_->GetIsSlipped()) {
			//Playerがスリップ状態に入った時点でposAddVectを指定してもらい、その位置に首を固定する
			//position_ = basePos + posAddVect_;
		//}
		//else {
			//通常時は首の長さに対応した位置に補正する
		position_ = basePos + bPlusLngPos;
		//}
	}

	if (player_->GetCurHead() == myNumber_) {
		if (player_->GetIsBiteMode())	position_ = player_->GetStopPos();
	}
	else {
		isHit_ = false;
	}

	Vector3 toMatPos = Vector3(position_.x, position_.y, 0);

	parameter_.mat.Translation(toMatPos);


	if (player_->GetIsShootModeEnd()&&player_->GetCurHead()==myNumber_) {
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::PHEAD_CLOTHES_COL);
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::HANGER_ACTOR, COL_ID::BOX_HANGER_COL);
	}
	
}

void Player_Head::Draw() const
{
	if (player_->GetPHeadDead(myNumber_))return;
	//auto pos_1 = DXConverter::GetInstance().ToVECTOR(parameter_.mat.Translation());
	//auto pos_2 = DXConverter::GetInstance().ToVECTOR(parameter_.mat.Translation() + Vector3(0, 10, 0));
	//DrawCapsule3D(pos_1, pos_2, 5.0f, 4, GetColor(255, 0, 0), GetColor(255, 0, 0), true);

	//Model::GetInstance().Draw(MODEL_ID::PLAYER_MODEL, parameter_.mat);

	auto is = Matrix::CreateRotationZ(angle_);
	auto pos = drawPos_;
	auto sizeVec = Vector3((parameter_.size.x / 2), (parameter_.size.y / 2));

	auto box1 = Vector3(-sizeVec.x, -sizeVec.y)*is;
	auto box2 = Vector3(+sizeVec.x, -sizeVec.y)*is;
	auto box3 = Vector3(-sizeVec.x, +sizeVec.y)*is;
	auto box4 = Vector3(+sizeVec.x, +sizeVec.y)*is;
	//auto seg = Vector3(+sizeVec.x, 0)*is;

	//左上,右上,左下,右下
	auto pos1 = Vector3(pos.x + box1.x, pos.y + box1.y);
	auto pos2 = Vector3(pos.x + box2.x, pos.y + box2.y);
	auto pos3 = Vector3(pos.x + box3.x, pos.y + box3.y);
	auto pos4 = Vector3(pos.x + box4.x, pos.y + box4.y);
	//Model::GetInstance().Draw(MODEL_ID::PLAYER_MODEL, parameter_.mat);
	//DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, GetColor(255, 255, 255));
	//DrawLine(pos1.x, pos1.y, pos3.x, pos3.y, GetColor(255, 255, 255));
	//DrawLine(pos2.x, pos2.y, pos4.x, pos4.y, GetColor(255, 255, 255));
	//DrawLine(pos3.x, pos3.y, pos4.x, pos4.y, GetColor(255, 255, 255));

	float angle = (float)(((int)MathAngle(player_->GetPosition()- position_))%360);
	if (getIsCurrentHead()) {
		if (angle > 90 && 180 > angle)angle = 90;
		else if (angle < 270 && 180 <= angle) {
			angle = 270;
		}
		Vector2 headOrigin = Sprite::GetInstance().GetSize(spriteId_) / 2;
		Sprite::GetInstance().Draw(spriteId_, drawPos_, headOrigin, spriteAlpha_,Vector2::One, angle,true,false);
		Sprite::GetInstance().Draw(SPRITE_ID::PLAYER_HEAD_FATIGUE_SPRITE, drawPos_, headOrigin, ((float)fatigueCheckColor_ / 255.f)*spriteAlpha_, Vector2::One);
	}
	else {
		Vector2 headOrigin = Sprite::GetInstance().GetSize(SPRITE_ID::PLAYER_HEAD_SPRITE) / 2;
		Sprite::GetInstance().Draw(SPRITE_ID::PLAYER_HEAD_SPRITE, drawPos_, headOrigin, spriteAlpha_, Vector2::One, angle, true, false);
		//Sprite::GetInstance().Draw(SPRITE_ID::PLAYER_HEAD_SPRITE, drawPos_, headOrigin, spriteAlpha_, Vector2::One);
		Sprite::GetInstance().Draw(SPRITE_ID::PLAYER_HEAD_FATIGUE_SPRITE, drawPos_, headOrigin, ((float)fatigueCheckColor_ / 255.f)*spriteAlpha_, Vector2::One);

	}
	//DrawBox(pos1.x, pos1.y, pos4.x, pos4.y, GetColor(0, 255, 0), TRUE);
	//if (player_->GetCurHead() == myNumber_) {
	//SetDrawBlendMode(BLEND_MODE::Alpha, fatigueCheckColor_);
	//DrawBox(pos1.x, pos1.y, pos4.x, pos4.y, GetColor(255, 0, 0), TRUE);
	//SetDrawBlendMode(BLEND_MODE::NoBlend, 0);
	//}
	//DrawLine(pos.x - seg.x, pos.y-seg.y, pos.x + seg.x, pos.y+seg.y, GetColor(255, 255, 255));
	//if (myNumber_ == player_->GetCurHead())DrawFormatString(300, 700, GetColor(255, 255, 255), "position x:%f y:%f z:%f", position_.x, position_.y);
	//DrawFormatString(0, 80, GetColor(255, 255, 255), "angle %f", angle_);
	//if (myNumber_ == player_->GetCurHead())DrawFormatString(250, 250, GetColor(255, 255, 255), "%d", fatigueCheckColor_);
	//if (myNumber_ == player_->GetCurHead())DrawFormatString(350, 350, GetColor(255, 255, 255), "%f:%f", stopPos_.x,stopPos_.y);
	//DrawFormatString(drawPos_.x, drawPos_.y, GetColor(255, 255, 255), "%d", myNumber_);


	DrawLine(drawPos_.x, drawPos_.y, player_->GetDrawPos().x, player_->GetDrawPos().y, GetColor(255, 255, 255));
}

void Player_Head::OnUpdate()
{
}

void Player_Head::OnCollide(Actor& other, CollisionParameter colpara)
{
	//結構いらなくなってるから消せるとこけす
	if (colpara.colID == COL_ID::PLAYER_PIN_COL) {
		player_->ResurrectHead();
		static_cast<ClothesPin*>(&other)->ClearThis();
		return;
	}

	if (player_->GetPHeadDead(myNumber_))return;

	isBitePoint_ = true;


	if (player_->GetCurHead() != myNumber_) {
		return;
	}


	//Clothes* otherClothes = static_cast<Clothes*>(&other);

	//if (colpara.colID==COL_ID::BOX_BOX_COL) {
	//	//服が風に吹かれていたらくっつかない
	//	if (static_cast<Clothes*>(&other)->GetIsWind()) {
	//		if (!isHitOnce) {
	//			isBiteSlipWind_ = true;
	//			return;
	//		}
	//	}
	//	else {
	//		isBiteSlipWind_ = false;
	//		isHitOnce = true;
	//	}
	//}

	//if (isHit_ || !(player_->GetIsShootModeEnd()))return;

	//isHit_ = true;
	//isBitePoint_ = false;

	//player_->CurHeadBite(stopPos_);
	
	//if(colpara.colID == COL_ID::BOX_BOX_COL|| colpara.colID == COL_ID::BOX_HANGER_COL)
	//player_->SetOtherClothesID_(static_cast<Clothes*>(&other)->GetClothesID());
	
}

void Player_Head::OnMessage(EventMessage message, void * param)
{
}
