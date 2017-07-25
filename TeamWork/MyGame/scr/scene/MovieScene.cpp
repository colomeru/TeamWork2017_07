#include "MovieScene.h"
#include "../time/Time.h"
#include "../Def.h"
#include "../fade/FadePanel.h"
#include "../tween/TweenManager.h"


const float MovieEndTime = 93.2f;

MovieScene::MovieScene() :
	nextScene_(Scene::Title), timer_(0.0f)
{
}
MovieScene::~MovieScene()
{
}

void MovieScene::Initialize()
{
	WorkFolder::SetWorkFolder("res/Movie/");
	movieView_.Load(MOVIE_FILE);
	movieView_.Play(DEMO_MOVIE);
	timer_ = 0;
	isEnd_ = false;		
	FadePanel::GetInstance().Initialize();
	FadePanel::GetInstance().SetInTime(0.0f);
	FadePanel::GetInstance().FadeIn();
}

void MovieScene::Update()
{
	timer_ += Time::DeltaTime;
	
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)|| timer_ >= MovieEndTime) {
		timer_ = 0;
		FadePanel::GetInstance().AddCollBack([=]() {
			SetBackgroundColor(153, 204, 255);
			isEnd_ = true;
		});
		FadePanel::GetInstance().SetOutTime(1.0f);
		FadePanel::GetInstance().FadeOut();
	}	
}

void MovieScene::Draw() const
{
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
	movieView_.Delete();
}

void MovieScene::handleMessage(EventMessage message, void * param)
{
}
