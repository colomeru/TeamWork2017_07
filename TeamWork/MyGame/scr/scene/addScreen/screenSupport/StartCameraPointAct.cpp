#include "StartCameraPointAct.h"
#include"../../../input/GamePad.h"
#include"../../../input/Keyboard.h"
#include"../../GamePlayDefine.h"

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
	position_.x -= startBaseSpeed;

	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::M)||GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM2)) {
		position_.x  -= startAddSpeed;
	}

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
