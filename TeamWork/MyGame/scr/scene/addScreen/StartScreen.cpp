#include "StartScreen.h"
#include"../../graphic/Sprite.h"
#include"../../world/IWorld.h"
#include"screenSupport/StartCameraPointAct.h"

StartScreen::StartScreen(World* world,int maxLaneCount) :world_(world), maxLaneCount_(maxLaneCount)
{

}

void StartScreen::Init(float stageWidth)
{

	cameraPointActor_ = std::make_shared<StartCameraPointAct>(world_, stageWidth);
	world_->Add(ACTOR_ID::SAMPLE_ACTOR, cameraPointActor_);

	world_->InitializeInv(Vector2(cameraPointActor_->GetPosition().x, cameraPointActor_->GetPosition().y));
	world_->SetTarget(cameraPointActor_.get());
	
}

bool StartScreen::Update()
{
	world_->StartModeUpdate();

	return (cameraPointActor_->isArrivePoint());
}

void StartScreen::Draw() const
{
	world_->Draw(maxLaneCount_, world_->GetKeepDatas().playerLane_);
}

