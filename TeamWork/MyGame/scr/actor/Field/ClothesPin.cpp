#include "ClothesPin.h"
#include "../MyGame/scr/graphic/Sprite.h"
#include "../MyGame/scr/input/Keyboard.h"
#include "../../graphic/Sprite.h"
#include "Clothes\Clothes.h"
#include "../../Def.h"

ClothesPin::ClothesPin(IWorld * world, int laneNum, Vector2 pos, Actor* clothes, Vector2 fulcrum)
	:Actor(world, clothes)
{
	parameter_.ID = ACTOR_ID::PIN_ACTOR;
	parameter_.radius = 32.0f;
	parameter_.size = Vector2(50.f, 50.f);
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
	DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, GetColor(255, 255, 255));
	DrawLine(pos1.x, pos1.y, pos3.x, pos3.y, GetColor(255, 255, 255));
	DrawLine(pos2.x, pos2.y, pos4.x, pos4.y, GetColor(255, 255, 255));
	DrawLine(pos3.x, pos3.y, pos4.x, pos4.y, GetColor(255, 255, 255));

	//Vector2 crcOrigin = Sprite::GetInstance().GetSize(SPRITE_ID::TEST_SPRITE) / 2;
	//Sprite::GetInstance().Draw(SPRITE_ID::TEST_SPRITE, drawPos_, crcOrigin, spriteAlpha_, Vector2::One, angle_);
	//DrawBox(pos1.x, pos1.y, pos4.x, pos4.y, GetColor(255, 255, 0), TRUE);
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::PLAYER_HEAD_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::PLAYER_HEAD_SPRITE, drawPos_, origin, 1.f, Vector2::One, angle_);

	if (BuildMode != 1)return;

}

void ClothesPin::OnUpdate()
{
}

void ClothesPin::OnCollide(Actor * other, CollisionParameter colpara)
{
}

void ClothesPin::OnMessage(EventMessage message, void * param)
{
}
