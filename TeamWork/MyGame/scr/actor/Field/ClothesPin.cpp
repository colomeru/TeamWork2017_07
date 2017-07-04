#include "ClothesPin.h"
#include "../MyGame/scr/graphic/Sprite.h"
#include "../MyGame/scr/input/Keyboard.h"
#include "../../graphic/Sprite.h"
#include "Clothes\Clothes.h"
#include "../../Def.h"
#include"../player/Player_Head.h"
#include"../Effects/PlayerEffect/ResurrectStayEffect.h"
#include"../../sound/sound.h"

ClothesPin::ClothesPin(IWorld * world, int laneNum, Vector2 pos, Actor* clothes, Vector2 fulcrum)
	:Actor(world, clothes)
{
	parameter_.ID = ACTOR_ID::PIN_ACTOR;
	parameter_.radius = 32.0f;
	parameter_.size = Vector2(50.0f, 50.0f);
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(0, 0, 0));

	laneNum_ = laneNum;
	position_ = pos;

	pos_ = pos;
	fulcrum_ = static_cast<Clothes*>(parent_)->GetFulcrum();

	//マトリクス情報
	Matrix mat =
		Matrix::CreateTranslation(Vector3(pos_.x, pos_.y, 0))
		* Matrix::CreateRotationZ(parent_->GetAngle())
		* Matrix::CreateTranslation(Vector3(fulcrum_.x, fulcrum_.y, 0));
	
	SetPose(mat);

	angle_ = 0.f;

	colFuncMap_[COL_ID::PLAYER_PIN_COL] = std::bind(&CollisionFunction::IsHit_Circle_Circle, colFunc_, std::placeholders::_1, std::placeholders::_2);

}

ClothesPin::~ClothesPin()
{

}

void ClothesPin::Update()
{
	if (isCheckCol_ && isUpdate_) {
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::PLAYER_HEAD_ACTOR, COL_ID::PLAYER_PIN_COL);
	}

	angle_ = parent_->GetAngle();
	//マトリクス情報
	Matrix mat =
		Matrix::CreateTranslation(Vector3(pos_.x, pos_.y, 0))
		* Matrix::CreateRotationZ(angle_)
		* Matrix::CreateTranslation(Vector3(fulcrum_.x, fulcrum_.y, 0));

	SetPose(mat);
}

void ClothesPin::Draw() const
{
	auto drawPos = GetDrawPosVect(position_);
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::PLAYER_HEAD_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::PLAYER_HEAD_SPRITE, drawPos, origin, parent_->GetParameter().spriteAlpha_, Vector2::One, angle_ + 180);

	if (BuildMode != 1)return;
	auto is = Matrix::CreateRotationZ(angle_);
	auto pos = drawPos;
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
	DrawLine((int)pos1.x, (int)pos1.y, (int)pos2.x, (int)pos2.y, GetColor(255, 255, 255));
	DrawLine((int)pos1.x, (int)pos1.y, (int)pos3.x, (int)pos3.y, GetColor(255, 255, 255));
	DrawLine((int)pos2.x, (int)pos2.y, (int)pos4.x, (int)pos4.y, GetColor(255, 255, 255));
	DrawLine((int)pos3.x, (int)pos3.y, (int)pos4.x, (int)pos4.y, GetColor(255, 255, 255));


}

void ClothesPin::OnUpdate()
{
}

void ClothesPin::OnCollide(Actor& other, CollisionParameter colpara)
{
	if (parameter_.isDead)return;
	if (colpara.colID == COL_ID::PLAYER_PIN_COL) {
		if (!static_cast<Player_Head*>(&other)->ResurrectHead()) {
			world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<ResurrectStayEffect>(world_, position_));
			Sound::GetInstance().PlaySE(SE_ID::RESURRECT_SE);
		}
		ClearThis();
		return;
	}

}

void ClothesPin::OnMessage(EventMessage message, void * param)
{
}
