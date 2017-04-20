#include "Player_Head.h"

#include "../../input/Keyboard.h"
#include "../../conv/DXConverter.h"
#include "../../graphic/Model.h"

Player_Head::Player_Head(IWorld * world, Player* targetP, Vector2 pos,int myNumber)
	:Actor(world,targetP)
	, isHit_(false), isBitePoint_(false), player_(targetP), myNumber_(myNumber), isHitOnce(true)
{
	parameter_.ID = ACTOR_ID::PLAYER_ACTOR;
	parameter_.radius = 32.0f;
	parameter_.size = Vector2(32.0f, 64.0f);
	parameter_.HP = 10;

	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3::Zero);


	position_ = pos;

	stopPos_ = position_;

}

Player_Head::~Player_Head()
{
}

void Player_Head::Update()
{
	if (!isHitOnce) {
		isBitePoint_ = false;
	}
	isHitOnce = false;
	auto basePos = player_->GetHeadPos(myNumber_);
	Vector2 vel = basePos-player_->GetPosition();


	Vector2 bPlusLngPos = vel*player_->GetHeadLengthChangeToPosMult(myNumber_);

	position_ = basePos + bPlusLngPos;
	
	
	//自分,相手のID,Colの種類
	world_->SetCollideSelect(shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::BOX_CLOTHES_COL);
	
	//velocity_ = Vector2::Zero;
	//float speed = 0.0f;
	//isHit_ = false;
	//auto pos = parameter_.mat.Translation();

	//world_->SetCollideSelect(shared_from_this(), ACTOR_ID::ENEMY_ACTOR, COL_ID::TEST_COL);

	////行列にangleをかける
	//parameter_.mat += Matrix::CreateRotationZ(angle_);
	//parameter_.mat.NormalizeRotationMatrix();

	////正面への移動量を追加(DXライブラリだからZにマイナス)
	////velocity_ += parameter_.mat.Backward() * speed;
	////velocity_ += parameter_.mat.Up() * jumpVec;
	////velocityをpositionに追加
	//pos += Vector3(velocity_.x, velocity_.y);

	////if (pos.y < 0)pos.y = 0;

	//position_ += velocity_;
	//parameter_.mat.Translation(pos);
	if (player_->GetCurHead() == myNumber_) {
		if (isHit_) {
			position_ = stopPos_;
		}
	}
	else {
		isHit_ = false;
	}

}

void Player_Head::Draw() const
{
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

	DrawBox(pos1.x, pos1.y, pos4.x, pos4.y, GetColor(255, 0, 0), TRUE);

	//DrawLine(pos.x - seg.x, pos.y-seg.y, pos.x + seg.x, pos.y+seg.y, GetColor(255, 255, 255));
	//DrawFormatString(0, 700, GetColor(255, 255, 255), "position x:%f y:%f z:%f", position_.x, position_.y);
	//DrawFormatString(0, 80, GetColor(255, 255, 255), "angle %f", angle_);
	DrawFormatString(drawPos_.x, drawPos_.y, GetColor(255, 255, 255), "%d", myNumber_);

	if (isBitePoint_) {
		DrawFormatString(400, 400, GetColor(255, 255, 255), "true");
	}
	else{
		DrawFormatString(400, 400, GetColor(255, 255, 255), "false");

	}
}

void Player_Head::OnUpdate()
{
}

void Player_Head::OnCollide(Actor& other, CollisionParameter colpara)
{
	isBitePoint_ = true;
	isHitOnce = true;
	if (player_->GetCurHead()!=myNumber_||isHit_|| !player_->GetIsBiteMode())return;

	isHit_ = true;
	isBitePoint_ = false;
	stopPos_ = position_;

	player_->CurHeadBite(stopPos_);
}

void Player_Head::NonCollide(Actor & other, CollisionParameter colpara)
{

}

void Player_Head::OnMessage(EventMessage message, void * param)
{
}
