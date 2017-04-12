#include "Player.h"
#include "../../input/Keyboard.h"
#include "../../conv/DXConverter.h"
#include "../../graphic/Model.h"

Player::Player(IWorld * world)
	:Actor(world)
	,angle_(0.0f)
	,isHit_(false)
{
	parameter_.ID = ACTOR_ID::PLAYER_ACTOR;
	parameter_.radius = 3.0f;
	parameter_.size = Vector3(3.0f, 3.0f, 3.0f);
	parameter_.HP = 10;

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
	isHit_ = false;

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

	world_->SetCollideSelect(shared_from_this(), ACTOR_ID::ENEMY_ACTOR, COL_ID::TEST_COL);
	
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
	//auto pos_1 = DXConverter::GetInstance().ToVECTOR(parameter_.mat.Translation());
	//auto pos_2 = DXConverter::GetInstance().ToVECTOR(parameter_.mat.Translation() + Vector3(0, 10, 0));
	//DrawCapsule3D(pos_1, pos_2, 5.0f, 4, GetColor(255, 0, 0), GetColor(255, 0, 0), true);

	Model::GetInstance().Draw(MODEL_ID::PLAYER_MODEL, parameter_.mat);

	auto pos = parameter_.mat.Translation();

	DrawFormatString(0, 60, GetColor(255, 255, 255), "position x:%f y:%f z:%f", pos.x, pos.y, pos.z);
	DrawFormatString(0, 80, GetColor(255, 255, 255), "angle %f", angle_);

}

void Player::OnUpdate()
{
}

void Player::OnCollide(Actor * other, CollisionParameter colpara)
{
	isHit_ = true;
}

void Player::OnMessage(EventMessage message, void * param)
{
}
