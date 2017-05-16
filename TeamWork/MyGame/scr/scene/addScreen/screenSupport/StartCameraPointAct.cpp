#include "StartCameraPointAct.h"

StartCameraPointAct::StartCameraPointAct(IWorld * world, float startPosx):
	Actor(world)
{
	position_ = Vector2(startPosx, 0);

	world_->GetCanChangedKeepDatas().SetStartPointPos(position_);
}

StartCameraPointAct::~StartCameraPointAct()
{
}

void StartCameraPointAct::Update()
{
}

void StartCameraPointAct::StartOnlyUpdate()
{
	position_.x -= 20.f;

	world_->GetCanChangedKeepDatas().SetStartPointPos(position_);

}

void StartCameraPointAct::Draw() const
{
}

void StartCameraPointAct::OnUpdate()
{
}

void StartCameraPointAct::OnCollide(Actor &, CollisionParameter colpara)
{
}

void StartCameraPointAct::OnMessage(EventMessage message, void * param)
{
}
