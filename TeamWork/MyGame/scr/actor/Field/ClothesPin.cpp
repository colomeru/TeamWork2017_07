#include "ClothesPin.h"
#include "../MyGame/scr/graphic/Sprite.h"
#include "../MyGame/scr/input/Keyboard.h"

ClothesPin::ClothesPin(IWorld * world, int laneNum, Vector2 pos)
	:Actor(world)
{
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	parameter_.radius = 32.0f;
	parameter_.size = Vector2(50.f, 50.f);
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(0, 0, 0));

	laneNum_ = laneNum;
	position_ = pos;
	
	colFuncMap_[COL_ID::PLAYER_PIN_COL] = std::bind(&CollisionFunction::IsHit_OBB_OBB, colFunc_, std::placeholders::_1, std::placeholders::_2);

}

ClothesPin::~ClothesPin()
{

}

void ClothesPin::Update()
{
	//ShakesClothes();
	if (isCheckCol_ && isUpdate_) {
		world_->SetCollideSelect(shared_from_this(), ACTOR_ID::PLAYER_HEAD_ACTOR, COL_ID::PLAYER_PIN_COL);
	}

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
	//����,�E��,����,�E��
	auto pos1 = Vector3(pos.x + box1.x, pos.y + box1.y);
	auto pos2 = Vector3(pos.x + box2.x, pos.y + box2.y);
	auto pos3 = Vector3(pos.x + box3.x, pos.y + box3.y);
	auto pos4 = Vector3(pos.x + box4.x, pos.y + box4.y);
	//Model::GetInstance().Draw(MODEL_ID::PLAYER_MODEL, parameter_.mat);
	DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, GetColor(255, 255, 255));
	DrawLine(pos1.x, pos1.y, pos3.x, pos3.y, GetColor(255, 255, 255));
	DrawLine(pos2.x, pos2.y, pos4.x, pos4.y, GetColor(255, 255, 255));
	DrawLine(pos3.x, pos3.y, pos4.x, pos4.y, GetColor(255, 255, 255));

	DrawBox(pos1.x, pos1.y, pos4.x, pos4.y, GetColor(255, 255, 0), TRUE);
}

void ClothesPin::OnUpdate()
{
}

void ClothesPin::OnCollide(Actor * other, CollisionParameter colpara)
{
}

void ClothesPin::OnMessage(EventMessage message, void * param)
{
	//switch (message)
	//{
	//case EventMessage::BEGIN_WIND:
	//	basePosition_ = position_;
	//	isPendulum_ = true;
	//	break;
	//case EventMessage::STRONG_WIND:
	//	rot_spd_ = 2.8f;
	//	isWind_ = true;
	//	break;
	//case EventMessage::ATTENUATE_WIND:
	//	rot_spd_ = 0.0f;
	//	isFriction_ = true;
	//	break;
	//case EventMessage::END_WIND:
	//	rot_spd_ = 0.5f;
	//	rot_ = 90.0f;
	//	friction_ = 1.0f;
	//	angle_ = 0;
	//	position_ = basePosition_;
	//	isPendulum_ = false;
	//	isFriction_ = false;
	//	isWind_ = false;
	//	break;
	//}
}