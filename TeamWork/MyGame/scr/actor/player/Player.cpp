#include "Player.h"
#include "../../input/Keyboard.h"
#include "../../conv/DXConverter.h"

Player::Player(IWorld * world)
	:Actor(world)
	,angle_(0.0f)
{
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationY(0.0f)
		* Matrix::CreateTranslation(Vector3::Zero);

	auto pos = parameter_.mat.Translation();
}

Player::~Player()
{
}

void Player::Update()
{
	velocity_ = Vector3::Zero;
	angle_ = 0;
	float speed = 0.0f;

	auto pos = parameter_.mat.Translation();

	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::UP)) {
		speed = 1.0f;
	}

	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::RIGHT)) {
		angle_ += 1.0f;
	}
	else if (Keyboard::GetInstance().KeyStateDown(KEYCODE::LEFT)) {
		angle_ -= 1.0f;
	}
	
	//行列にangleをかける
	parameter_.mat *= Matrix::CreateFromAxisAngle(GetPose().Up(), angle_);
	parameter_.mat.NormalizeRotationMatrix();

	//正面への移動量を追加(DXライブラリだからZにマイナス)
	velocity_ += parameter_.mat.Backward() * speed;
	//velocityをpositionに追加
	pos += velocity_;
	parameter_.mat.Translation(pos);
}

void Player::Draw() const
{
	auto pos_1 = DXConverter::GetInstance().ToVECTOR(parameter_.mat.Translation());
	auto pos_2 = DXConverter::GetInstance().ToVECTOR(parameter_.mat.Translation() + Vector3(0, 10, 0));

	DrawCapsule3D(pos_1, pos_2, 5.0f, 4, GetColor(255, 0, 0), GetColor(255, 0, 0), true);

	auto pos = parameter_.mat.Translation();

	DrawFormatString(0, 60, GetColor(255, 255, 255), "position x:%f y:%f z:%f", pos.x, pos.y, pos.z);
	DrawFormatString(0, 80, GetColor(255, 255, 255), "angle %f", angle_);
}

void Player::OnUpdate()
{
}

void Player::OnCollide(Actor * other, CollisionParameter colpara)
{
}

void Player::OnMessage(EventMessage message, void * param)
{
}
