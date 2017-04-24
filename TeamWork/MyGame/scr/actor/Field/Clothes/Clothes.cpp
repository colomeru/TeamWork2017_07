#include "Clothes.h"

//コンストラクタ
Clothes::Clothes(IWorld* world, CLOTHES_ID clothes, int laneNum)
	:Actor(world)
	,isHit_(false), isPendulum_(false), isFriction(true)
	,fulcrum_(0, 0), rot_(90.0f), rot_spd_(0.0f), length_(300.0f), gravity_(0.3f)
{
	isHit_ = true;
}

//当たり判定処理
void Clothes::OnCollide(Actor & other, CollisionParameter colpara)
{
}

//メッセージ処理
void Clothes::OnMessage(EventMessage message, void * param)
{
}

void Clothes::Pendulum(Vector2 fulcrum, float length)
{
	float friction = 1.0f;						//摩擦
	if (isFriction) {
		friction = 1.005f;						//増加
	}
	else {
		friction = 0.995f;						//減衰
	}
	auto initialRot = rot_;						//角速度を加算する前の角度

	//現在の重りの位置
	position_.x = fulcrum.x + MathHelper::Cos(rot_) * length;
	position_.y = fulcrum.y + MathHelper::Sin(rot_) * length;

	//重力移動量を反映した重りの位置
	auto length_vec = position_ - fulcrum;
	auto t = -(length_vec.y * gravity_) / (length_vec.x * length_vec.x + length_vec.y * length_vec.y);
	auto move_weightX = position_.x + t * length_vec.x;
	auto move_weightY = position_.y + gravity_ + t * length_vec.y;

	//2つの重りの位置の角度差
	auto r = MathHelper::ATan(move_weightY - fulcrum.y, move_weightX - fulcrum.x);

	//角度差を角速度に加算
	auto sub = r - rot_;
	sub -= std::floor(sub / 360.0f) * 360.0f;
	if (sub < -180.0f) sub += 360.0f;
	if (sub > 180.0f) sub -= 360.0f;
	rot_spd_ += sub;

	//摩擦
	rot_ *= friction;

	//角速度の制限
	MathHelper::Clamp(rot_spd_, -3.0f, -3.0f);

	//角度に角速度を加算
	rot_ += rot_spd_;

	//新しい重りの位置
	position_.x = fulcrum.x + MathHelper::Cos(rot_) * length;
	position_.y = fulcrum.y + MathHelper::Sin(rot_) * length;

	//角度を画像に反映
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
