#include "MovieScene.h"
#include "../camera/Camera.h"
#include "../conv/DXConverter.h"
#include "../time/Time.h"
#include "../Def.h"


MovieScene::MovieScene() :
	nextScene_(Scene::Title)
{
	world_ = std::make_shared<World>();

	world_->AddEventMessageListener([=](EventMessage msg, void* param) {
		handleMessage(msg, param);
	});

}

MovieScene::~MovieScene()
{
}

void MovieScene::Initialize()
{
	WorkFolder::SetWorkFolder("res/Movie/");
	movieView_.Load("‹C—´_PV.avi");
	movieView_.Play("‹C—´_PV");
	isEnd_ = false;
}

void MovieScene::Update()
{
	world_->Update();


	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;
}

void MovieScene::Draw() const
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "MovieScene");
	world_->Draw();
	movieView_.Draw();
}

bool MovieScene::IsEnd() const
{
	return isEnd_;
}

Scene MovieScene::Next() const
{
	return nextScene_;
}

void MovieScene::End()
{
	world_->Clear();
}

void MovieScene::handleMessage(EventMessage message, void * param)
{
}
