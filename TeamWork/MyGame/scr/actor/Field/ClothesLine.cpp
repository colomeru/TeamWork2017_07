#include "ClothesLine.h"
#include "../MyGame/scr/graphic/Sprite.h"
#include "../MyGame/scr/input/Keyboard.h"
#include"../../Def.h"

ClothesLine::ClothesLine(IWorld * world, int laneNum, const Vector2& endPos, Vector2 pos):endPos_(endPos)
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
	position_ = pos +Vector2(-200.f, -150.f);

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
void ClothesLine::StartOnlyLateUpdate()
{
	isDraw_ = true;
	//position_.x = (world_->GetKeepDatas().startPointPos_.x - 200.f) + (WINDOW_WIDTH / 2.f);
}
void ClothesLine::Draw() const
{
	Vector2 pPos = world_->GetKeepDatas().playerPos_;

	float sizex = Sprite::GetInstance().GetSize(SPRITE_ID::LANE_SPRITE).x;
	
	float drawx = (float)((int)world_->GetInv().Translation().x %(int)sizex);
	drawx -= 300.0f;
	Vector2 drawVec = Vector2(drawx, drawPos_.y);

	Vector2 LaneSorigin = Sprite::GetInstance().GetSize(SPRITE_ID::LANE_SIDE_SPRITE) / 2;
	Vector2 drawEndPos = drawPos_ + Vector2(endPos_.x, 0.f);//-position_.y);// +LaneSorigin.y);
	Vector2 vecaddpos = Vector2(0, LaneSorigin.y);

	for (int i = 0; i < 6; i++) {
		Vector2 drawLPos = drawVec + Vector2(sizex / 2, 0.f)*i;
		if ((drawEndPos).x <= drawLPos.x + sizex){
			Vector2 lastdp = drawLPos;// -Vector2(0.0f, Sprite::GetInstance().GetSize(SPRITE_ID::LANE_SPRITE).y / 2);
			Vector2 lastep = drawEndPos + vecaddpos+Vector2(20.0f, Sprite::GetInstance().GetSize(SPRITE_ID::LANE_SPRITE).y/2);
			DrawModiGraph((int)lastdp.x, (int)lastdp.y, (int)lastep.x, (int)lastdp.y, (int)lastep.x, (int)lastep.y, (int)lastdp.x, (int)lastep.y, Sprite::GetInstance().GetHandle(SPRITE_ID::LANE_SPRITE), TRUE);
			//Sprite::GetInstance().Draw(SPRITE_ID::LANE_SIDE_SPRITE, drawPos_ + vecaddpos, LaneSorigin, spriteAlpha_, Vector2::One, 0.0f);
			break;
	}
		Sprite::GetInstance().Draw(SPRITE_ID::LANE_SPRITE, drawLPos);
	}
	//Sprite::GetInstance().Draw(SPRITE_ID::LANE_SPRITE, BGSize + bgPos);
	
	Sprite::GetInstance().Draw(SPRITE_ID::LANE_SIDE_SPRITE, drawPos_+ vecaddpos, LaneSorigin, spriteAlpha_, Vector2::One, 0.0f);
	Sprite::GetInstance().Draw(SPRITE_ID::LANE_SIDE2_SPRITE, drawEndPos+ vecaddpos, LaneSorigin,spriteAlpha_,Vector2::One,0.0f);

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
