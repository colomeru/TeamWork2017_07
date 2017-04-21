#include "BaseClothes.h"

#include "../../../input/Keyboard.h"
#include "../../../conv/DXConverter.h"
#include "../../../graphic/Model.h"

BaseClothes::BaseClothes(IWorld * world, CLOTHES_ID clothes, int laneNum, Vector2 pos)
	:Clothes(world, clothes, laneNum)
{
	clothes_ID = CLOTHES_ID::BASE_CLOTHES;
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	parameter_.radius = 32.0f;
	parameter_.size = Vector2(200, 200.f);
	parameter_.HP = 10;
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(0, 0, 0));
	parameter_.ClothSegmentPoints_.push_back(Vector2(-100.f, 100.f));
	parameter_.ClothSegmentPoints_.push_back(Vector2(	0.f, 100.f));
	parameter_.ClothSegmentPoints_.push_back(Vector2( 100.f, 100.f));

	laneNum_ = laneNum;

	//auto pos = parameter_.mat.Translation();

	position_ = pos;
}

BaseClothes::~BaseClothes()
{
}

void BaseClothes::Update()
{
	velocity_ = Vector2::Zero;
	float speed = 0.0f;
	isHit_ = false;
	auto pos = parameter_.mat.Translation();

	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::W)) {
		velocity_.y -= 1.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::S)) {
		velocity_.y += 1.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::D)) {
		velocity_.x += 1.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::A)) {
		velocity_.x -= 1.0f;
	}

	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::V)) {
		angle_ -= 1.0f;
	}
	else if (Keyboard::GetInstance().KeyStateDown(KEYCODE::C)) {
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

void BaseClothes::Draw() const
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

	DrawBox(pos1.x, pos1.y, pos4.x, pos4.y, GetColor(0, 255, 0), TRUE);
	//DrawLine(pos.x - seg.x, pos.y - seg.y, pos.x + seg.x, pos.y + seg.y, GetColor(255, 255, 255));
	DrawFormatString(500, 60, GetColor(255, 255, 255), "position x:%f y:%f z:%f", position_.x, position_.y);
	DrawFormatString(500, 80, GetColor(255, 255, 255), "angle %f", angle_);

}

void BaseClothes::OnUpdate()
{
}

void BaseClothes::OnCollide(Actor * other, CollisionParameter colpara)
{
	isHit_ = true;
}

void BaseClothes::OnMessage(EventMessage message, void * param)
{
}
