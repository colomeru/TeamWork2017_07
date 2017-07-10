#include "GamePlayScene.h"
#include "../math/MathHelper.h"
#include "../time/Time.h"
#include "../fade/FadePanel.h"
#include"../sound/sound.h"
#include"../debug/DebugDraw.h"
#include "../actor/SampleActor.h"
#include"../camera/Camera.h"
#include"../conv/DXConverter.h"
#include"../actor/player/Player.h"
#include"../actor/Field/Clothes/BaseClothes.h"
#include"../actor/Field/Clothes/TestClothes.h"
#include"../actor/Field/ClothesLine.h"
#include"../actor/Field/ClothesPin.h"
#include"../myData/MyTestCamera.h"
#include"../collision/MyCol.h"
#include"../stageGenerator/Stage1/Stage1.h"
#include"../stageGenerator/Stage1/Stage2.h"
#include"../stageGenerator/Stage1/Stage3.h"
#include"../stageGenerator/Stage1/Stage4.h"
#include"../stageGenerator/Stage1/Stage5.h"
#include"../stageGenerator/Stage1/Stage6.h"
#include"../stageGenerator/Stage1/Stage7.h"
#include"../stageGenerator/Stage1/Stage8.h"
#include"../game/Random.h"
#include"GamePlayDefine.h"
#include"../actor/Field/Enemys/EnemyGenerator.h"
#include"../actor/Field/Enemys/EnemyCharas/ClothesTapper.h"
#include"../tween/TweenManager.h"
#include"../actor/Effects/PlayerEffect/PlayerMetamorEffect.h"
#include"../cheat/CheatData.h"
#include"../fade/FadePanel.h"

GamePlayScene::GamePlayScene() :
	nextScene_(Scene::Credit), windTime_(defWindTime[0]), maxLaneCount(3),
	gameOverScreen_(), gameClearScreen_(), pauseScreen_(), stageLen_(0.f),
	gamePlayMode_(0), currentStage_(Stage::Stage2), stageEffectScreen_()
{
	// ワールド生成
	world_ = std::make_shared<World>();
	// イベントリスナー登録
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});

	updateFunctionMap_[6] = std::bind(&GamePlayScene::nextSwitchUpdate, this);
	updateFunctionMap_[5] = std::bind(&GamePlayScene::nextUpdate, this);
	updateFunctionMap_[4] = std::bind(&GamePlayScene::pauseUpdate, this);
	updateFunctionMap_[3] = std::bind(&GamePlayScene::clearUpdate, this);
	updateFunctionMap_[2] = std::bind(&GamePlayScene::overUpdate, this);
	updateFunctionMap_[1] = std::bind(&GamePlayScene::baseUpdate, this);
	updateFunctionMap_[0] = std::bind(&GamePlayScene::startUpdate, this);

	nextStageList_[Stage::Stage1] = Stage::Stage2;
	nextStageList_[Stage::Stage2] = Stage::Stage3;
	nextStageList_[Stage::Stage3] = Stage::Stage4;
	nextStageList_[Stage::Stage4] = Stage::Stage5;
	nextStageList_[Stage::Stage5] = Stage::Stage6;
	nextStageList_[Stage::Stage6] = Stage::Stage7;
	nextStageList_[Stage::Stage7] = Stage::Stage8;
	nextStageList_[Stage::Stage8] = Stage::Stage1;

	stageBGMList_[Stage::Stage1] = BGM_ID::STAGE_01_BGM;
	stageBGMList_[Stage::Stage2] = BGM_ID::STAGE_02_BGM;
	stageBGMList_[Stage::Stage3] = BGM_ID::STAGE_03_BGM;
	stageBGMList_[Stage::Stage4] = BGM_ID::STAGE_01_BGM;
	stageBGMList_[Stage::Stage5] = BGM_ID::STAGE_02_BGM;
	stageBGMList_[Stage::Stage6] = BGM_ID::STAGE_03_BGM;
	stageBGMList_[Stage::Stage7] = BGM_ID::STAGE_01_BGM;
	stageBGMList_[Stage::Stage8] = BGM_ID::STAGE_02_BGM;

	defWindTime_[Stage::Stage1] = defWindTime[0];
	defWindTime_[Stage::Stage2] = defWindTime[1];
	defWindTime_[Stage::Stage3] = defWindTime[2];
	defWindTime_[Stage::Stage4] = defWindTime[3];
	defWindTime_[Stage::Stage5] = defWindTime[4];
	defWindTime_[Stage::Stage6] = defWindTime[5];
	defWindTime_[Stage::Stage7] = defWindTime[6];
	defWindTime_[Stage::Stage8] = defWindTime[7];

	stagenum_[Stage::Stage1] = 0;
	stagenum_[Stage::Stage2] = 1;
	stagenum_[Stage::Stage3] = 2;
	stagenum_[Stage::Stage4] = 3;
	stagenum_[Stage::Stage5] = 4;
	stagenum_[Stage::Stage6] = 5;
	stagenum_[Stage::Stage7] = 6;
	stagenum_[Stage::Stage8] = 7;

	bgScreen_ = BackgroundScreen(world_.get());
	changeScreen_=LaneChangeScreen(world_.get());
	uiScreen_ = UIScreen(world_.get());

	stageGeneratorManager.Add(Stage::Stage1, std::make_shared<Stage1>(world_.get(), std::string("Test"),frequencyWind[0],frequencyHairBall[0],hairballCnt[0]));
	stageGeneratorManager.Add(Stage::Stage2, std::make_shared<Stage2>(world_.get(), std::string("Stage1"), frequencyWind[1], frequencyHairBall[1], hairballCnt[1]));
	stageGeneratorManager.Add(Stage::Stage3, std::make_shared<Stage3>(world_.get(), std::string("Stage2"), frequencyWind[2], frequencyHairBall[2], hairballCnt[2]));
	stageGeneratorManager.Add(Stage::Stage4, std::make_shared<Stage4>(world_.get(), std::string("Stage3"), frequencyWind[3], frequencyHairBall[3], hairballCnt[3]));
	stageGeneratorManager.Add(Stage::Stage5, std::make_shared<Stage5>(world_.get(), std::string("Stage4"), frequencyWind[4], frequencyHairBall[4], hairballCnt[4]));
	stageGeneratorManager.Add(Stage::Stage6, std::make_shared<Stage6>(world_.get(), std::string("Stage5"), frequencyWind[5], frequencyHairBall[5], hairballCnt[5]));
	stageGeneratorManager.Add(Stage::Stage7, std::make_shared<Stage7>(world_.get(), std::string("Stage6"), frequencyWind[6], frequencyHairBall[6], hairballCnt[6]));
	stageGeneratorManager.Add(Stage::Stage8, std::make_shared<Stage8>(world_.get(), std::string("Stage7"), frequencyWind[7], frequencyHairBall[7], hairballCnt[7]));

}

GamePlayScene::~GamePlayScene()
{
	ply1 = nullptr;
}

void GamePlayScene::Initialize()
{
	changeCount_ = 600;
	currentStage_ = CheatData::getInstance().GetSelectStage();

	isEnd_ = false;
	//シーン遷移系の初期化
	{
		gamePlayMode_ = 0;
		nextScene_ = Scene::Title;
		pauseScreen_.Init();
		gameOverScreen_.Init();
		gameClearScreen_.Init();
		allClearScreen_.Init();
		bgScreen_.Init(currentStage_);
	}
	world_->Initialize();

	//ステージの最大レーン数(後々MapGeneratorからレーン数を受け取れるようにする)
	int stageLaneSize = 3;
	ply1 = std::make_shared<Player>(world_.get(), stageLaneSize, 1);
	world_->Add(ACTOR_ID::PLAYER_ACTOR, ply1);
	world_->PushStackActor(ply1);

	stageGeneratorManager.SetStage(currentStage_);
	stageLen_ = stageGeneratorManager.GetStageSize(currentStage_).x;
	enemGenerator_ = std::make_shared<EnemyGenerator>(world_.get(),currentStage_);
	world_->Add(ACTOR_ID::ENEMY_ACTOR, enemGenerator_);
	maxLaneCount = stageLaneSize;
	world_->Add(ACTOR_ID::LANE_ACTOR, std::make_shared<ClothesLine>(world_.get(), 0, stageGeneratorManager.GetStageSize(currentStage_)+Vector2(200,0), Vector2(0, 0)));
	world_->Add(ACTOR_ID::LANE_ACTOR, std::make_shared<ClothesLine>(world_.get(), 1, stageGeneratorManager.GetStageSize(currentStage_) + Vector2(200, 0), Vector2(0, 0)));
	world_->Add(ACTOR_ID::LANE_ACTOR, std::make_shared<ClothesLine>(world_.get(), 2, stageGeneratorManager.GetStageSize(currentStage_) + Vector2(200, 0), Vector2(0, 0)));

	world_->SetMaxSize((int)stageLen_-WINDOW_WIDTH/2);
	
	startScreen_ = StartScreen(world_.get(), maxLaneCount);
	
	startScreen_.Init(stageLen_);
	uiScreen_.Init(currentStage_, stageLen_);

	stageEffectScreen_.Init(currentStage_);
	
	Sound::GetInstance().StopBGM();
	Sound::GetInstance().PlayBGM(stageBGMList_[currentStage_],DX_PLAYTYPE_LOOP);
	FadePanel::GetInstance().SetInTime(1.0f, 0.5f);
	FadePanel::GetInstance().FadeIn();

	world_->GetCanChangedKeepDatas().currentStage_ = currentStage_;
}

void GamePlayScene::Update()
{
	updateFunctionMap_[gamePlayMode_]();
	if (!world_->GetIsFreeCamY_()) {
		if (!world_->GetIsCamChangeMode()) {
			changeScreen_.End();
		}
	}
	uiScreen_.Update(ply1->GetPosition());
		
}

void GamePlayScene::Draw() const
{
	bgScreen_.Draw();
	// 描画
	world_->Draw(maxLaneCount, world_->GetKeepDatas().playerLane_);

	stageEffectScreen_.Draw();

	uiScreen_.Draw();

	if (gamePlayMode_ == 2) {
		gameOverScreen_.Draw();
	}
	else if (gamePlayMode_ == 3) {
		if (currentStage_ == Stage::Stage8) {
			allClearScreen_.Draw();
		}
		else {
			gameClearScreen_.Draw();
		}
	}
	else if (gamePlayMode_ == 4) {
		pauseScreen_.Draw();
	}

	DebugDraw::DebugDrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);
}

bool GamePlayScene::IsEnd() const
{
	return isEnd_;
}

Scene GamePlayScene::Next() const
{
	return nextScene_;
}

void GamePlayScene::End()
{
	// 初期化
	ply1 = nullptr;
	world_->Clear();
	bgScreen_.End();

	Sound::GetInstance().StopBGM();

	TweenManager::GetInstance().Clear();

}

void GamePlayScene::handleMessage(EventMessage message, void * param)
{
	switch (message) {
	case EventMessage::GAME_CLEAR_FLAG:{
		setNextMode(5);
		break;
	}
	case EventMessage::GOAL_FLAG: {
		world_->PopStackActor();
		break;
	}
	case EventMessage::LANE_CHANGE_FALL: {

		break;
	}
	case EventMessage::LANE_CHANGE_END: {
		changeScreen_.End();

		break;
	}
	case EventMessage::START_LANE_CHANGE:{
		if (world_->GetKeepDatas().nextLane_ > 0) {
			changeScreen_.Init(WindDir::UP);
		}
		else {
			changeScreen_.Init(WindDir::DOWN);
		}
		break;
	}
	case EventMessage::BEGIN_WIND: {
		stageEffectScreen_.StartEffect();
		Sound::GetInstance().PlaySE(SE_ID::WIND_SE);
		break;
	}
	case EventMessage::STRONG_WIND: {
		Sound::GetInstance().PlaySE(SE_ID::POWERFUL_WIND_SE);
		break;
	}
	case EventMessage::TAPPER_DEAD: {
		enemGenerator_->StartTapperResurrectTimer();
		break;
	}
	case EventMessage::PLAY_NEXT_STAGE: {
		setNextMode(3);
		break;
	}
	case EventMessage::ADD_SCORE: {
		uiScreen_.AddScore((int)param);
		break;
	}
	case EventMessage::PLAYER_DEAD: {
		setNextMode(2);
	}
	default:
		break;
	}
}

void GamePlayScene::baseUpdate()
{
	// 更新
	world_->Update();
	ply1->deadLine();

	int randT = Random::GetInstance().Range(0, 3);
	windTime_ -= randT;
	if (windTime_ <= 0) {
		world_->sendMessage(EventMessage::BEGIN_WIND);
		bgScreen_.addBGCharacters();
		windTime_ = defWindTime_[currentStage_];
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::H) || GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM8)) {
		setNextMode(4);
		TweenManager::GetInstance().StopAll();

	}
	if (BuildMode==1&&Keyboard::GetInstance().KeyTriggerDown(KEYCODE::L)) {
		setNextMode(3);
	}

	bgScreen_.Update();
	changeScreen_.Update();
	stageEffectScreen_.Update();
}

void GamePlayScene::pauseUpdate()
{
	PauseScreen::returnGameType backType;
	if (pauseScreen_.Update(nextScene_, backType)) {
		if (nextScene_ == Scene::GamePlay) {
			if (backType == PauseScreen::returnGameType::Resume) {
				setNextMode(1);
				TweenManager::GetInstance().Play();
			}
			else {
				FadePanel::GetInstance().AddCollBack([=]() { End(); Initialize(); });
				FadePanel::GetInstance().FadeOut();

			}
		}
		else {
			FadePanel::GetInstance().AddCollBack([=]() {isEnd_ = true; });
			FadePanel::GetInstance().FadeOut();
		}
	}

}

void GamePlayScene::overUpdate()
{
	if (gameOverScreen_.Update(nextScene_)) {
		if (nextScene_ == Scene::GamePlay) {
			FadePanel::GetInstance().AddCollBack([=]() { End(); Initialize(); });
		}
		else {
			FadePanel::GetInstance().AddCollBack([=]() { isEnd_ = true; });
		}
		FadePanel::GetInstance().FadeOut();
	}

}

void GamePlayScene::clearUpdate()
{
	if (currentStage_ != Stage::Stage8) {
		if (gameClearScreen_.Update(nextScene_)) {
			if (nextScene_ == Scene::GamePlay) {
				if (currentStage_ == Stage::Stage8) {
					nextScene_ = Scene::Credit2;
					FadePanel::GetInstance().AddCollBack([=]() { isEnd_ = true; });

				}
				else {
					FadePanel::GetInstance().AddCollBack([=]() {
						currentStage_ = nextStageList_[currentStage_];
						End();
						Initialize();
					});
				}
			}
			else {
				FadePanel::GetInstance().AddCollBack([=]() { isEnd_ = true; });
			}
			FadePanel::GetInstance().FadeOut();

		}
	}
	else {
		if (allClearScreen_.Update(nextScene_)) {
			FadePanel::GetInstance().AddCollBack([=]() { isEnd_ = true; });
			FadePanel::GetInstance().FadeOut();
		}
	}
}
void GamePlayScene::nextUpdate()
{

	world_->Update();
	bgScreen_.Update();
	changeScreen_.Update();
	stageEffectScreen_.Update();
	
	if(world_->GetKeepDatas().camPosY_<=-WINDOW_HEIGHT)changeCount_--;
	if (changeCount_ <= 0) setNextMode(3);
	bgScreen_.DownCeil();

}

void GamePlayScene::nextSwitchUpdate()
{
}

void GamePlayScene::setNextMode(int mode) {
	gamePlayMode_ = mode;
	switch (gamePlayMode_)
	{
	case 2: {
		TweenManager::GetInstance().StopAll();
		gameOverScreen_.Init();
		Sound::GetInstance().PlayBGM(BGM_ID::GAME_OVER_BGM);

		break;
	}
	case 3: {
		CheatData::getInstance().SetClearData(stagenum_[currentStage_], true);
		if(currentStage_!=Stage::Stage8)
			CheatData::getInstance().SetSelectStage(nextStageList_[currentStage_]);
		
		if (currentStage_ == Stage::Stage8) {
			allClearScreen_.Init();
			allClearScreen_.SetScore(uiScreen_.GetScore(), ply1->GetPHeadLiveCount(), currentStage_);

		}
		else {
			gameClearScreen_.Init();
			gameClearScreen_.SetScore(uiScreen_.GetScore(), ply1->GetPHeadLiveCount(), currentStage_);
		}
		Sound::GetInstance().SetBGMVolume(BGM_ID::STAGE_CLEAR_BGM, 0.7f);
		Sound::GetInstance().PlayBGM(BGM_ID::STAGE_CLEAR_BGM);
		break;
	}
	case 4: {
		TweenManager::GetInstance().StopAll();
		pauseScreen_.Init();
		Sound::GetInstance().PlaySE(SE_ID::CHECK_SE);
	}
	default:
		break;
	}
}

void GamePlayScene::startUpdate()
{
	if (startScreen_.Update()) {
		setNextMode(1);
		world_->SetTarget(ply1.get());
	}
}
