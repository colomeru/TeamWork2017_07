#include "TestClothes.h"

TestClothes::TestClothes(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos)
	:Clothes(world, clothes, laneNum)
{
	clothes_ID = CLOTHES_ID::TEST_CLOTHES;
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(0, 0, 0));

	laneNum_ = laneNum;

	position_ = pos;

}

TestClothes::~TestClothes()
{
}

void TestClothes::Update()
{
	isHit_ = false;
}

void TestClothes::Draw() const
{
	DrawBox(position_.x - 100, position_.y - 100, position_.x + 100, position_.y + 100, GetColor(255, 255, 0), TRUE);
}

void TestClothes::OnUpdate()
{
}

void TestClothes::OnCollide(Actor * other, CollisionParameter colpara)
{
}

void TestClothes::OnMessage(EventMessage message, void * param)
{
}
