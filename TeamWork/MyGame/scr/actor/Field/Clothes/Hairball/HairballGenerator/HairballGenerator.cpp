#include "HairballGenerator.h"
#include "../Hairball.h"
#include "../MyGame/scr/Def.h"
#include "../MyGame/scr/game/Random.h"
#include "../MyGame/scr/scene/GamePlayDefine.h"

HairballGenerator::HairballGenerator(IWorld * world, int laneNum, Vector2 pos)
	:Actor(world)
	,is_Generate_(false)
	,generate_Count_(0)
{
	parameter_.ID = ACTOR_ID::BEGIN_ACTOR;
	parameter_.radius = 0.0f;
	parameter_.size = Vector2::Zero;
	parameter_.mat
		= Matrix::CreateScale(Vector3::Zero)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(0, 0, 0));

	laneNum_ = laneNum;
	position_ = pos;

	frequencyHairBall_[Stage::Stage1] = frequencyWind[0];
	frequencyHairBall_[Stage::Stage2] = frequencyWind[1];
	frequencyHairBall_[Stage::Stage3] = frequencyWind[2];
	frequencyHairBall_[Stage::Stage4] = frequencyWind[3];
	frequencyHairBall_[Stage::Stage5] = frequencyWind[4];
	frequencyHairBall_[Stage::Stage6] = frequencyWind[5];
	frequencyHairBall_[Stage::Stage7] = frequencyWind[6];
	frequencyHairBall_[Stage::Stage8] = frequencyWind[7];

	defGenerate_Count_[Stage::Stage1] = hairballCnt[0];
	defGenerate_Count_[Stage::Stage2] = hairballCnt[1];
	defGenerate_Count_[Stage::Stage3] = hairballCnt[2];
	defGenerate_Count_[Stage::Stage4] = hairballCnt[3];
	defGenerate_Count_[Stage::Stage5] = hairballCnt[4];
	defGenerate_Count_[Stage::Stage6] = hairballCnt[5];
	defGenerate_Count_[Stage::Stage7] = hairballCnt[6];
	defGenerate_Count_[Stage::Stage8] = hairballCnt[7];

	world_->EachActor(ACTOR_ID::PLAYER_ACTOR, [&, this](const Actor& other) {
		player_ = static_cast<Player*>(const_cast<Actor*>(&other));
	});
}

HairballGenerator::~HairballGenerator()
{
	player_ = nullptr;
}

void HairballGenerator::Update()
{
	if (player_ == nullptr)return;

	if (player_->GetLaneNum() == laneNum_) {
		int rand = Random::GetInstance().Range(-2, 3);
		position_ = Vector2(player_->GetPosition().x + WINDOW_WIDTH, player_->GetCurrentPHeadPosition().y - rand * 100); 
	}
	else
		position_.x = player_->GetPosition().x + WINDOW_WIDTH;

	if (is_Generate_) {
		GenerateHairball();
	}
}

void HairballGenerator::Draw() const
{
}

void HairballGenerator::OnUpdate()
{
}

void HairballGenerator::OnCollide(Actor & other, CollisionParameter colpara)
{
}

void HairballGenerator::OnMessage(EventMessage message, void * param)
{
	switch (message)
	{
	case EventMessage::BEGIN_WIND:
	{
		is_Generate_ = true;
		break;
	}
	case EventMessage::END_WIND:
	{
		is_Generate_ = false;
		break;
	}
	}
}

void HairballGenerator::GenerateHairball()
{
	auto currentStage = world_->GetKeepDatas().currentStage_;
	if (generate_Count_ > defGenerate_Count_[currentStage]) {
		int rand = Random::GetInstance().Range(0, 100);
		if (rand > frequencyHairBall_[currentStage]) {
			generate_Count_ = 0;
			return;
		}
		world_->Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Hairball>(world_, CLOTHES_ID::HAIRBALL, laneNum_, position_));
		generate_Count_ = 0;
	}
	generate_Count_++;
}
