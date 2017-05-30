#include "ClothesTapper.h"
#include"../../../../game/Random.h"
#include"../../../../math/EasingManager.h"

static const float maxTime = 5.0f;

ClothesTapper::ClothesTapper(IWorld * world, int laneNum, Vector2 pos):
	Enemys(world,laneNum,pos), timeCount_(0.f),targetPos_(Vector2::Zero)
{
	//auto ePx = std::make_shared<EaseNode>(&position_.x, EaseType::Linear, 0.f, -200.f, 5.f, [this]() {SetNextTapPos(); });
	////auto ePy = std::make_shared<EaseNode>(&position_.y, EaseType::Linear, position_.y, 5.f, 5.f, [this]() {SetNextTapPos(); });
	//EasingManager::GetInstance().Add(ePx);
	////EasingManager::GetInstance().Add(ePy);
}

ClothesTapper::~ClothesTapper()
{
}

void ClothesTapper::Update()
{
	timeCount_ += 0.0016f;
	if (timeCount_ > maxTime) {
		timeCount_ = 0.f;
		SetNextTapPos();
	}
	


}

void ClothesTapper::Draw() const
{
}

void ClothesTapper::OnUpdate()
{
}

void ClothesTapper::OnCollide(Actor & other, CollisionParameter colpara)
{
}

void ClothesTapper::OnMessage(EventMessage message, void * param)
{
}

void ClothesTapper::SetNextTapPos(const Vector2 & pos)
{
	targetPos_ = Vector2(world_->GetKeepDatas().playerPos_.x, 0.f);
	targetPos_.x += Random::GetInstance().Range(0.f, WINDOW_WIDTH / 2.f);

	if (pos.x != 0.0f&&pos.y!=0.0f) {
		targetPos_ = pos;
	}
}
