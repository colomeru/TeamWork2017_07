#include "ClothesLine.h"
#include "../MyGame/scr/graphic/Sprite.h"
#include "../MyGame/scr/input/Keyboard.h"
#include"../../Def.h"

ClothesLine::ClothesLine(IWorld * world, int laneNum, const Vector2& endPos, Vector2 pos):endPos_(endPos)
	,Actor(world)
{
	parameter_.ID = ACTOR_ID::UI_ACTOR;
	parameter_.radius = 32.0f;
	parameter_.size = Vector2((float)STAGE_TIP_SIZE, 50.f);
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(0, 0, 0));

	laneNum_ = laneNum;
	position_ = pos +Vector2(-200.f, -150.f);
	addPos_ = pos;
}

ClothesLine::~ClothesLine()
{
}

void ClothesLine::Update()
{	
}
void ClothesLine::LateUpdate()
{
	isDraw_ = true;
	isUpdate_ = true;
}
void ClothesLine::StartOnlyLateUpdate()
{
	isDraw_ = true;
}
void ClothesLine::Draw() const
{
	Vector2 pPos = world_->GetKeepDatas().playerPos_;

	float sizex = (int)Sprite::GetInstance().GetSize(SPRITE_ID::GAMEPLAY_LANE_SPRITE).x;
	
	float drawx = (float)((int)world_->GetInv().Translation().x %(int)sizex);
	drawx -= 250.0f;
	drawx += addPos_.x;
	Vector2 drawVec = Vector2(drawx, drawPos_.y);

	Vector2 LaneSorigin = Sprite::GetInstance().GetSize(SPRITE_ID::LANE_SIDE_SPRITE) / 2;
	Vector2 drawEndPos = drawPos_ + Vector2(endPos_.x, 0.f);//-position_.y);// +LaneSorigin.y);
	Vector2 vecaddpos = Vector2(0, LaneSorigin.y);

	for (int i = 0; i < 6; i++) {
		Vector2 drawLPos = drawVec + Vector2(sizex / 2.f, 0.f)*i;
		if ((drawEndPos).x <= drawLPos.x + sizex){
			Vector2 lastdp = drawLPos;// -Vector2(0.0f, Sprite::GetInstance().GetSize(SPRITE_ID::LANE_SPRITE).y / 2);
			Vector2 lastep = drawEndPos + vecaddpos+Vector2(20.0f, (float)Sprite::GetInstance().GetSize(SPRITE_ID::GAMEPLAY_LANE_SPRITE).y/2.f);
			DrawModiGraph((int)lastdp.x, (int)lastdp.y, (int)lastep.x, (int)lastdp.y, (int)lastep.x, (int)lastep.y, (int)lastdp.x, (int)lastep.y, Sprite::GetInstance().GetHandle(SPRITE_ID::LANE_SPRITE), TRUE);
			break;
	}
		Sprite::GetInstance().Draw(SPRITE_ID::GAMEPLAY_LANE_SPRITE, drawLPos);
	}
	
	Sprite::GetInstance().Draw(SPRITE_ID::LANE_SIDE_SPRITE, drawPos_+ vecaddpos+Vector2(-20.0f,0.0f), LaneSorigin, parameter_.spriteAlpha_, Vector2::One, 0.0f);
	Sprite::GetInstance().Draw(SPRITE_ID::LANE_SIDE2_SPRITE, drawEndPos+ vecaddpos, LaneSorigin, parameter_.spriteAlpha_,Vector2::One,0.0f);

}

void ClothesLine::OnUpdate()
{
}

void ClothesLine::OnCollide(Actor * other, CollisionParameter colpara)
{
}

void ClothesLine::OnMessage(EventMessage message, void * param)
{
}
