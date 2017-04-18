#include "PlayerLine.h"
#include "../Def.h"
#include "../math/MathHelper.h"

PlayerLine::PlayerLine(IWorld * world) :
	Actor(world),
	pos_(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
	head1_(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
	line1_(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2)
{

}

PlayerLine::~PlayerLine()
{
}

void PlayerLine::Update()
{
	auto length_ = 50;

	head1_.x = 50 * MathHelper::Cos(45);
	head1_.y = 50 * MathHelper::Sin(45);
	head1_ += pos_;
	line1_.x = head1_.x + length_;
	line1_.y = head1_.y + length_;

	head2_.x = 50 * MathHelper::Cos(135);
	head2_.y = 50 * MathHelper::Sin(135);
	head2_ += pos_;

}

void PlayerLine::Draw() const
{
	
	DrawCircle(pos_.x,pos_.y, 50, GetColor(255,0,0), 0);
	DrawCircle(head1_.x, head1_.y ,5, GetColor(0, 255, 0), 0);
	DrawCircle(head2_.x, head2_.y, 5, GetColor(0, 255, 0), 0);

	DrawLine(pos_.x,pos_.y, head1_.x, head1_.y, GetColor(0, 0, 255), 1);
	DrawLine(head1_.x, head1_.y, line1_.x, line1_.y , GetColor(0, 255, 255), 1);
	
	DrawLine(pos_.x, pos_.y, head2_.x, head2_.y, GetColor(0, 0, 255), 1);

}

void PlayerLine::OnUpdate()
{
}

void PlayerLine::OnCollide(Actor * other, CollisionParameter colpara)
{
}

void PlayerLine::OnMessage(EventMessage message, void * param)
{
}
