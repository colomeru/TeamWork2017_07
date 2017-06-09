#include "MovieScene.h"
#include "../time/Time.h"
#include "../Def.h"


const float MovieEndTime = 94.0f;

MovieScene::MovieScene() :
	nextScene_(Scene::Title)
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
	timer = 0;
	isEnd_ = false;		
}

void MovieScene::Update()
{
	timer += Time::DeltaTime;
	
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE)||timer >= MovieEndTime) {
		timer = 0;
		movieView_.Stop(DEMO_MOVIE);
		isEnd_ = true;
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
}

void MovieScene::handleMessage(EventMessage message, void * param)
{
}
