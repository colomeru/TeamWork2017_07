#include "Enemy.h"
#include "../../conv/DXConverter.h"

Enemy::Enemy(IWorld * world)
	:Actor(world)
	,isHit_(false)
{
	parameter_.radius = 3.0f;
	parameter_.size = Vector2(3.0f, 3.0f);
	parameter_.ID = ACTOR_ID::ENEMY_ACTOR;

	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationY(0.0f)
		* Matrix::CreateTranslation(Vector3(0.0f, 0.0f, 10.0f));
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	if (isHit_) {
		Dead();
	}

	isHit_ = false;
}

void Enemy::Draw() const
{
	auto pos_1 = DXConverter::GetInstance().ToVECTOR(parameter_.mat.Translation());
	auto pos_2 = DXConverter::GetInstance().ToVECTOR(parameter_.mat.Translation() + Vector3(0.0f, 5.0f, 0.0f));

	DrawCapsule3D(pos_1, pos_2, parameter_.radius, 4, GetColor(0, 255, 0), GetColor(0, 255, 0), true);
}

void Enemy::OnUpdate()
{
}

void Enemy::OnCollide(Actor & other, CollisionParameter colpara)
{
	isHit_ = true;
}

void Enemy::OnMessage(EventMessage message, void * param)
{
}
