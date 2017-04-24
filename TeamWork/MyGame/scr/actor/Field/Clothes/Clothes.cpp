#include "Clothes.h"

//�R���X�g���N�^
Clothes::Clothes(IWorld* world, CLOTHES_ID clothes, int laneNum)
	:Actor(world)
	,isHit_(false), isPendulum_(false), isFriction(true)
	,fulcrum_(0, 0), rot_(90.0f), rot_spd_(0.0f), length_(300.0f), gravity_(0.3f)
{
	isHit_ = true;
}

//�����蔻�菈��
void Clothes::OnCollide(Actor & other, CollisionParameter colpara)
{
}

//���b�Z�[�W����
void Clothes::OnMessage(EventMessage message, void * param)
{
}

void Clothes::Pendulum(Vector2 fulcrum, float length)
{
	float friction = 1.0f;						//���C
	if (isFriction) {
		friction = 1.005f;						//����
	}
	else {
		friction = 0.995f;						//����
	}
	auto initialRot = rot_;						//�p���x�����Z����O�̊p�x

	//���݂̏d��̈ʒu
	position_.x = fulcrum.x + MathHelper::Cos(rot_) * length;
	position_.y = fulcrum.y + MathHelper::Sin(rot_) * length;

	//�d�͈ړ��ʂ𔽉f�����d��̈ʒu
	auto length_vec = position_ - fulcrum;
	auto t = -(length_vec.y * gravity_) / (length_vec.x * length_vec.x + length_vec.y * length_vec.y);
	auto move_weightX = position_.x + t * length_vec.x;
	auto move_weightY = position_.y + gravity_ + t * length_vec.y;

	//2�̏d��̈ʒu�̊p�x��
	auto r = MathHelper::ATan(move_weightY - fulcrum.y, move_weightX - fulcrum.x);

	//�p�x�����p���x�ɉ��Z
	auto sub = r - rot_;
	sub -= std::floor(sub / 360.0f) * 360.0f;
	if (sub < -180.0f) sub += 360.0f;
	if (sub > 180.0f) sub -= 360.0f;
	rot_spd_ += sub;

	//���C
	rot_ *= friction;

	//�p���x�̐���
	MathHelper::Clamp(rot_spd_, -3.0f, -3.0f);

	//�p�x�Ɋp���x�����Z
	rot_ += rot_spd_;

	//�V�����d��̈ʒu
	position_.x = fulcrum.x + MathHelper::Cos(rot_) * length;
	position_.y = fulcrum.y + MathHelper::Sin(rot_) * length;

	//�p�x���摜�ɔ��f
	//auto angle = MathHelper::ToRadians(rot_);
	//if (rot_ > initialRot) {
	//	angle_ = angle;
	//}
	//else {
	//	angle_ = -angle;
	//}
	
}

void Clothes::ShakesClothes()
{
	if (isPendulum_) {
		Pendulum(fulcrum_, length_);
	}
}
