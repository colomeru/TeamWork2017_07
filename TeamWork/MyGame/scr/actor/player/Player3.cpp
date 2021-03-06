#include "Player3.h"
#include "../../input/Keyboard.h"
#include "../../conv/DXConverter.h"
#include "../../graphic/Model.h"
#include "../../math/MathHelper.h"

Player3::Player3(IWorld * world)
	:Actor(world)
	, isHit_(false), jumpVec(0), fulcrum_(500.0f, 200.0f), rot_(90.0f), rot_spd_(3.0f), length_(300.0f), gravity_(0.5f)
{
	parameter_.ID = ACTOR_ID::PLAYER_ACTOR;
	parameter_.radius = 32.0f;
	parameter_.size = Vector2(96.0f, 96.0f);
	parameter_.HP = 10;
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(0, 0, 0));

	auto pos = parameter_.mat.Translation();
}

Player3::~Player3()
{
}

void Player3::Update()
{
	velocity_ = Vector2::Zero;
	float speed = 0.0f;
	isHit_ = false;
	jumpVec -= 0.1f;
	auto pos = parameter_.mat.Translation();

	Pendulum(fulcrum_, length_);

	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::UP)) {
		velocity_.y -= 1.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::DOWN)) {
		velocity_.y += 1.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::RIGHT)) {
		velocity_.x += 1.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::LEFT)) {
		velocity_.x -= 1.0f;
	}

	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::X)) {
		angle_ -= 1.0f;
	}
	else if (Keyboard::GetInstance().KeyStateDown(KEYCODE::Z)) {
		angle_ += 1.0f;
	}

	world_->SetCollideSelect(shared_from_this(), ACTOR_ID::ENEMY_ACTOR, COL_ID::TEST_COL);

	//行列にangleをかける
	parameter_.mat += Matrix::CreateRotationZ(angle_);
	parameter_.mat.NormalizeRotationMatrix();

	//正面への移動量を追加(DXライブラリだからZにマイナス)
	//velocity_ += parameter_.mat.Backward() * speed;
	//velocity_ += parameter_.mat.Up() * jumpVec;
	//velocityをpositionに追加
	pos += Vector3(velocity_.x, velocity_.y);
	position_ += velocity_;

	//if (pos.y < 0)pos.y = 0;

	parameter_.mat.Translation(pos);

}

void Player3::Draw() const
{
	//auto pos_1 = DXConverter::GetInstance().ToVECTOR(parameter_.mat.Translation());
	//auto pos_2 = DXConverter::GetInstance().ToVECTOR(parameter_.mat.Translation() + Vector3(0, 10, 0));
	//DrawCapsule3D(pos_1, pos_2, 5.0f, 4, GetColor(255, 0, 0), GetColor(255, 0, 0), true);

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
	DrawCircle(pos.x, pos.y, 50, GetColor(255, 255, 255), false);

	//DrawLine(pos.x - seg.x, pos.y - seg.y, pos.x + seg.x, pos.y + seg.y, GetColor(255, 255, 255));
	//DrawFormatString(500, 60, GetColor(255, 255, 255), "position x:%f y:%f z:%f", pos.x, pos.y);
	//DrawFormatString(500, 80, GetColor(255, 255, 255), "angle %f", angle_);

	DrawFormatString(500, 120, GetColor(255, 255, 255), "rot_spd_ %f", rot_spd_);
	DrawFormatString(500, 140, GetColor(255, 255, 255), "rot_ %f", rot_);

}

void Player3::OnUpdate()
{
}

void Player3::OnCollide(Actor & other, CollisionParameter colpara)
{
	isHit_ = true;
}

void Player3::OnMessage(EventMessage message, void * param)
{
}

void Player3::Pendulum(Vector2 fulcrum, float length)
{
	float friction = 0.998f;								//摩擦

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

	//角度に角速度を加算
	rot_ += rot_spd_;

	//新しい重りの位置
	position_.x = fulcrum.x + MathHelper::Cos(rot_) * length;
	position_.y = fulcrum.y + MathHelper::Sin(rot_) * length;
}
