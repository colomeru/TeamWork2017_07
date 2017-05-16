#include "ClothesLine.h"
#include "../MyGame/scr/graphic/Sprite.h"
#include "../MyGame/scr/input/Keyboard.h"
#include"../../Def.h"

ClothesLine::ClothesLine(IWorld * world, int laneNum, int blockSize, Vector2 pos):blockSize_(blockSize)
	,Actor(world)
{
	parameter_.ID = ACTOR_ID::UI_ACTOR;
	parameter_.radius = 32.0f;
	parameter_.size = Vector2(STAGE_TIP_SIZE, 50.f);
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(0, 0, 0));

	laneNum_ = laneNum;
	position_ = pos +Vector2(0, -150.f);

}

ClothesLine::~ClothesLine()
{
}

void ClothesLine::Update()
{
	//ShakesClothes();
	//position_.x = (world_->GetKeepDatas().playerPos_.x-200.f)+(WINDOW_WIDTH/2.f);
}
void ClothesLine::LateUpdate()
{
	isDraw_ = true;
	isUpdate_ = true;
}
void ClothesLine::StartOnlyUpdate()
{
	//position_.x = (world_->GetKeepDatas().startPointPos_.x - 200.f) + (WINDOW_WIDTH / 2.f);
}
void ClothesLine::Draw() const
{
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::LANE_SPRITE)/2;
	for (int i = 0; i < blockSize_; i++) {
		Sprite::GetInstance().Draw(SPRITE_ID::LANE_SPRITE, drawPos_+(i*STAGE_TIP_SIZE), origin, spriteAlpha_, Vector2::One);
	}
	//auto is = Matrix::CreateRotationZ(angle_);
	//auto pos = drawPos_;
	//auto sizeVec = Vector3((parameter_.size.x / 2), (parameter_.size.y / 2));

	//auto box1 = Vector3(-sizeVec.x, -sizeVec.y)*is;
	//auto box2 = Vector3(+sizeVec.x, -sizeVec.y)*is;
	//auto box3 = Vector3(-sizeVec.x, +sizeVec.y)*is;
	//auto box4 = Vector3(+sizeVec.x, +sizeVec.y)*is;
	////auto seg = Vector3(+sizeVec.x, 0)*is;
	////¶ã,‰Eã,¶‰º,‰E‰º
	//auto pos1 = Vector3(pos.x + box1.x, pos.y + box1.y);
	//auto pos2 = Vector3(pos.x + box2.x, pos.y + box2.y);
	//auto pos3 = Vector3(pos.x + box3.x, pos.y + box3.y);
	//auto pos4 = Vector3(pos.x + box4.x, pos.y + box4.y);
	////Model::GetInstance().Draw(MODEL_ID::PLAYER_MODEL, parameter_.mat);
	//DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, GetColor(255, 255, 255));
	//DrawLine(pos1.x, pos1.y, pos3.x, pos3.y, GetColor(255, 255, 255));
	//DrawLine(pos2.x, pos2.y, pos4.x, pos4.y, GetColor(255, 255, 255));
	//DrawLine(pos3.x, pos3.y, pos4.x, pos4.y, GetColor(255, 255, 255));

	//DrawBox(pos1.x, pos1.y, pos4.x, pos4.y, GetColor(255, 255, 0), TRUE);
}

void ClothesLine::OnUpdate()
{
}

void ClothesLine::OnCollide(Actor * other, CollisionParameter colpara)
{
}

void ClothesLine::OnMessage(EventMessage message, void * param)
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
