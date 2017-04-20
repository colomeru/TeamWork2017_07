#include "PlayerLine.h"
#include "../Def.h"
#include "../math/MathHelper.h"

PlayerLine::PlayerLine(IWorld * world) :
	Actor(world),
	pos_(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2)
{

}

PlayerLine::~PlayerLine()
{
}

void PlayerLine::Update()
{
	auto length_ = 100;
	auto length2_ = 10;

	head1_.x = 50 * MathHelper::Cos(45);
	head1_.y = 50 * MathHelper::Sin(45);
	head1_ += pos_;
	line1_.x = head1_.x + length_;
	line1_.y = head1_.y + length_;

	head2_.x = 50 * MathHelper::Cos(135);
	head2_.y = 50 * MathHelper::Sin(135);
	head2_ += pos_;
	line2_.x = head2_.x - length2_;
	line2_.y = head2_.y + length2_;

	head3_.x = 50 * MathHelper::Cos(225);
	head3_.y = 50 * MathHelper::Sin(225);
	head3_ += pos_;
	line3_.x = head3_.x - length_;
	line3_.y = head3_.y - length_; 

	head4_.x = 50 * MathHelper::Cos(315);
	head4_.y = 50 * MathHelper::Sin(315);
	head4_ += pos_;
	line4_.x = head4_.x + length_;
	line4_.y = head4_.y - length_;

	head5_.x = 50 * MathHelper::Cos(0);
	head5_.y = 50 * MathHelper::Sin(0);
	head5_ += pos_;
	line5_.x = head5_.x + length_ * sqrt(2);
	line5_.y = head5_.y;

	head6_.x = 50 * MathHelper::Cos(90);
	head6_.y = 50 * MathHelper::Sin(90);
	head6_ += pos_;
	line6_.x = head6_.x;
	line6_.y = head6_.y + length_ * sqrt(2);

	head7_.x = 50 * MathHelper::Cos(180);
	head7_.y = 50 * MathHelper::Sin(180);
	head7_ += pos_;
	line7_.x = head7_.x - length_ * sqrt(2);
	line7_.y = head7_.y;

	head8_.x = 50 * MathHelper::Cos(270);
	head8_.y = 50 * MathHelper::Sin(270);
	head8_ += pos_;
	line8_.x = head8_.x;
	line8_.y = head8_.y - length_ * sqrt(2);
}

void PlayerLine::Draw() const
{	
	DrawCircle(pos_.x,pos_.y, 50, GetColor(255,0,0), 0);
	DrawCircle(head1_.x, head1_.y, 2, GetColor(0, 255, 0), 0);
	DrawCircle(head2_.x, head2_.y, 2, GetColor(0, 255, 0), 0);
	DrawCircle(head3_.x, head3_.y, 2, GetColor(0, 255, 0), 0);
	DrawCircle(head4_.x, head4_.y, 2, GetColor(0, 255, 0), 0);
	DrawCircle(head5_.x, head5_.y, 2, GetColor(0, 255, 0), 0);
	DrawCircle(head6_.x, head6_.y, 2, GetColor(0, 255, 0), 0);
	DrawCircle(head7_.x, head7_.y, 2, GetColor(0, 255, 0), 0);
	DrawCircle(head8_.x, head8_.y, 2, GetColor(0, 255, 0), 0);
	
	DrawLine(pos_.x,pos_.y, head1_.x, head1_.y, GetColor(0, 0, 255), 1);
	DrawLine(head1_.x, head1_.y, line1_.x, line1_.y , GetColor(0, 255, 255), 1);
	
	DrawLine(pos_.x, pos_.y, head2_.x, head2_.y, GetColor(0, 0, 255), 1);
	DrawLine(head2_.x, head2_.y, line2_.x, line2_.y, GetColor(100, 0, 255), 1);

	DrawLine(pos_.x, pos_.y, head3_.x, head3_.y, GetColor(0, 0, 255), 1);
	DrawLine(head3_.x, head3_.y, line3_.x, line3_.y, GetColor(100, 0, 255), 1);

	DrawLine(pos_.x, pos_.y, head4_.x, head4_.y, GetColor(0, 0, 255), 1);
	DrawLine(head4_.x, head4_.y, line4_.x, line4_.y, GetColor(100, 0, 255), 1);

	DrawLine(pos_.x, pos_.y, head5_.x, head5_.y, GetColor(0, 0, 255), 1);
	DrawLine(head5_.x, head5_.y, line5_.x, line5_.y, GetColor(100, 0, 255), 1);

	DrawLine(pos_.x, pos_.y, head6_.x, head6_.y, GetColor(0, 0, 255), 1);
	DrawLine(head6_.x, head6_.y, line6_.x, line6_.y, GetColor(100, 0, 255), 1);

	DrawLine(pos_.x, pos_.y, head7_.x, head7_.y, GetColor(0, 0, 255), 1);
	DrawLine(head7_.x, head7_.y, line7_.x, line7_.y, GetColor(100, 0, 255), 1);

	DrawLine(pos_.x, pos_.y, head8_.x, head8_.y, GetColor(0, 0, 255), 1);
	DrawLine(head8_.x, head8_.y, line8_.x, line8_.y, GetColor(100, 0, 255), 1);

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
