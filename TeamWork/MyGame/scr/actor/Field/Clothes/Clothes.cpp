#include "Clothes.h"
#include "../MyGame/scr/math/MyFuncionList.h"
#include "../MyGame/scr/game/Random.h"

//�R���X�g���N�^
Clothes::Clothes(IWorld* world, CLOTHES_ID clothes, int laneNum)
	:Actor(world)
	,isHit_(false), isPendulum_(false), isFriction_(false), isWind_(false)
	,fulcrum_(0, 0), rot_(90.0f), rot_spd_(0.5f), length_(125.0f), gravity_(0.3f), friction_(1.0f)
	,count_(0),clothesState_(ClothesState::BEGIN_WIND)
{
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
	float initialRot = rot_;					//�p���x�����Z����O�̊p�x

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

	auto temp = rot_spd_ + sub;
	if (sign(rot_spd_) != sign(temp) && isFriction_) {
		friction_ *= 0.995f;
	}
	rot_spd_ = temp;
	//rot_spd_ += sub;

	//���C
	rot_ *= friction_;

	//�p���x�̐���
	//rot_spd_ = MathHelper::Clamp(rot_spd_, -2.8f, 2.8f);

	//�p�x�Ɋp���x�����Z
	rot_ += rot_spd_;

	//�V�����d��̈ʒu
	position_.x = fulcrum.x + MathHelper::Cos(rot_) * length;
	position_.y = fulcrum.y + MathHelper::Sin(rot_) * length;

	//�p�x���摜�ɔ��f
	auto angle = rot_ - 90;
	angle_ = angle;
	
	count_++;

	switch (count_)
	{
	case 300:
		clothesState_ = ClothesState::BEGIN_STRONG_WIND;
		break;
	case 600:
		clothesState_ = ClothesState::ATTENUATE_WIND;
		break;
	case 900:
		clothesState_ = ClothesState::POSSIBLE_BITE;
		break;
	case 1200:
		clothesState_ = ClothesState::END_WIND;
		break;
	default:
		break;
	}
}

void Clothes::ShakesClothes()
{
	if (isPendulum_ && isDraw_) {
		Pendulum(fulcrum_, length_);

		switch (clothesState_)
		{
		case ClothesState::BEGIN_STRONG_WIND:
			rot_spd_ = 2.8f;
			isWind_ = true;
			clothesState_ = ClothesState::STRONG_WIND;
			break;
		case ClothesState::ATTENUATE_WIND:
			isFriction_ = true;
			break;
		case ClothesState::POSSIBLE_BITE:
			isWind_ = false;
			break;
		case ClothesState::END_WIND:
			rot_spd_ = 0.5f;
			rot_ = 90.0f;
			friction_ = 1.0f;
			angle_ = 0;
			position_ = basePosition_;
			isFriction_ = false;
			count_ = 0;
			isPendulum_ = false;
			break;
		default:
			break;
		}
	}
}
