#include "StartCameraPointAct.h"
#include"../../../input/GamePad.h"
#include"../../../input/Keyboard.h"
#include"../../GamePlayDefine.h"
#include"../../../math/Easing.h"

StartCameraPointAct::StartCameraPointAct(IWorld * world, float startPosx):
	Actor(world), timeCount_(0.f)
{
	position_ = Vector2(startPosx, 0);
	defPosx_ = startPosx;
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
	timeCount_ += 0.016f;

	//position_.x -= startBaseSpeed;


	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::M)||GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM2)) {
		timeCount_ += startAddTime;
	}
	timeCount_ = min(startEase, timeCount_);

	position_.x = Easing::EaseInOutQuad(timeCount_, defPosx_, -defPosx_, startEase);
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
