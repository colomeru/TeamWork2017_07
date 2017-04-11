#include "MyGame.h"
#include "Time.h"
#include "../Input/InputMgr.h"

#include "../Define.h"

#include "../Scene/LoadingScene/LoadingScene.h"
#include "../Scene/GamePlay/GamePlayScene.h"
#include "../Scene/FirstGamePlay/FirstGamePlayScene.h"
#include "../Scene/GameOver/GameOverScene.h"
//#include "../Scene/KataokaScene/KataokaScene.h"
#include "../Scene/StageClear/StageClearScene.h"
#include "../Scene/GameClear/GameClearScene.h"
#include "../Scene/BossStage/BossStage.h"
#include"../Scene/MainMenu/MainMenuScene.h"
#include"../Scene/Credit/CreditScene.h"
#include"../Scene/TeamLogoScene/TeamLogoScene.h"
#include"../Scene/GameEnd/GameEndScene.h"
#include "../Scene/Tutoreal/TutorealScene.h"
#include "../Scene/BonusStage/BonusStage.h"
MyGame::MyGame() : 
	Game(WINDOW_NAME, static_cast<int>(SCREEN_SIZE.x), static_cast<int>(SCREEN_SIZE.y), WINDOW_RATE, WINDOW_MODE),
	dataKeeper_(), 
	sceneMgr_(&dataKeeper_){
}

MyGame::~MyGame(){}

void MyGame::start(){
	sceneMgr_.add(Scene::Loading, std::make_shared<LoadingScene>(&dataKeeper_));
	sceneMgr_.add(Scene::MainMenu, std::make_shared<MainMenuScene>(&dataKeeper_));
	sceneMgr_.add(Scene::FirstGamePlay, std::make_shared<FirstGamePlayScene>(&dataKeeper_));
	sceneMgr_.add(Scene::GamePlay, std::make_shared<GamePlayScene>(&dataKeeper_));
	sceneMgr_.add(Scene::GameOver, std::make_shared<GameOverScene>(&dataKeeper_));
	sceneMgr_.add(Scene::GameClear, std::make_shared<GameClearScene>(&dataKeeper_));
	sceneMgr_.add(Scene::StageClear, std::make_shared<StageClearScene>(&dataKeeper_));
	sceneMgr_.add(Scene::BossStage01, std::make_shared<BossStage>(&dataKeeper_));
	//sceneMgr_.add(Scene::Kataoka, std::make_shared<KataokaScene>(&dataKeeper_));
	sceneMgr_.add(Scene::Credit, std::make_shared<CreditScene>(&dataKeeper_));
	sceneMgr_.add(Scene::GameEnd, std::make_shared<GameEndScene>(&dataKeeper_));
	sceneMgr_.add(Scene::Logo, std::make_shared<TeamLogoScene>(&dataKeeper_));
	sceneMgr_.add(Scene::Tutoreal, std::make_shared<TutorealScene>(&dataKeeper_));
	sceneMgr_.add(Scene::Bonus, std::make_shared<BonusStage>(&dataKeeper_));
	sceneMgr_.change(Scene::Loading);
}

void MyGame::update(){
	Time::GetInstance().update();
	InputMgr::GetInstance().GetHitKeyFrameAll();

	sceneMgr_.update();
}

void MyGame::draw(){
	sceneMgr_.draw();

	Time::GetInstance().draw_fps();
}

void MyGame::end(){
	sceneMgr_.end();
}