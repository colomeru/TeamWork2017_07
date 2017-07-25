#include "GamePlayScene.h"
#include "../math/MathHelper.h"
#include "../time/Time.h"
#include "../fade/FadePanel.h"
#include"../sound/sound.h"
#include"../debugdata/DebugDraw.h"
#include"../camera/Camera.h"
#include"../conv/DXConverter.h"
#include"../actor/player/Player.h"
#include"../actor/Field/Clothes/BaseClothes.h"
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
#include"../input/InputChecker.h"

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

	updateFunctionMap_[0] = std::bind(&GamePlayScene::startUpdate, this);
	updateFunctionMap_[1] = std::bind(&GamePlayScene::baseUpdate, this);
	updateFunctionMap_[2] = std::bind(&GamePlayScene::overUpdate, this);
	updateFunctionMap_[3] = std::bind(&GamePlayScene::clearUpdate, this);
	updateFunctionMap_[4] = std::bind(&GamePlayScene::pauseUpdate, this);
	updateFunctionMap_[5] = std::bind(&GamePlayScene::nextUpdate, this);
	updateFunctionMap_[6] = std::bind(&GamePlayScene::nextSwitchUpdate, this);

	changeModeFunctionMap_[0] = [this] {ToStartMode(); };
	changeModeFunctionMap_[1] = [this] {ToBaseMode(); };
	changeModeFunctionMap_[2] = [this] {ToOverMode(); };
	changeModeFunctionMap_[3] = [this] {ToClearMode(); };
	changeModeFunctionMap_[4] = [this] {ToPauseMode(); };
	changeModeFunctionMap_[5] = [this] {ToNextMode(); };
	changeModeFunctionMap_[6] = [this] {ToSwitchMode(); };

	stageBGMList_[Stage::Stage1] = BGM_ID::STAGE_01_BGM;
	stageBGMList_[Stage::Stage2] = BGM_ID::STAGE_02_BGM;
	stageBGMList_[Stage::Stage3] = BGM_ID::STAGE_03_BGM;
	stageBGMList_[Stage::Stage4] = BGM_ID::STAGE_01_BGM;
	stageBGMList_[Stage::Stage5] = BGM_ID::STAGE_02_BGM;
	stageBGMList_[Stage::Stage6] = BGM_ID::STAGE_03_BGM;
	stageBGMList_[Stage::Stage7] = BGM_ID::STAGE_01_BGM;
	stageBGMList_[Stage::Stage8] = BGM_ID::STAGE_02_BGM;

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

	InitWindTime();

	world_->GetCanChangedKeepDatas().currentStage_ = currentStage_;
}

void GamePlayScene::Update()
{
	Sound::GetInstance().SetAllBGMVolume(0.0f);

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


	switch (gamePlayMode_)
	{
	case 2: {
		OverDraw();
		break;
	}
	case 3: {
		ClearDraw();
		break;
	}
	case 4: {
		PauseDraw();
	}
	default:
		break;
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
	windTimer_.Action();
	if (InputChecker::GetInstance().KeyTriggerDown(InputChecker::Input_Key::Start)) {
		setNextMode(4);
		TweenManager::GetInstance().StopAll();

	}
#ifndef NDEBUG
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::L)) {
		setNextMode(3);
	}
#endif

	bgScreen_.Update();
	changeScreen_.Update();
	stageEffectScreen_.Update();
}

void GamePlayScene::pauseUpdate()
{
	PauseScreen::returnGameType backType;
	if (!pauseScreen_.Update(nextScene_, backType)) return;
	
	if (nextScene_ == Scene::GamePlay) {
		pause_Check_Resume(backType);
	}
	else {
		FadePanel::GetInstance().AddCollBack([=]() {isEnd_ = true; });
		FadePanel::GetInstance().FadeOut();
	}

}

void GamePlayScene::overUpdate()
{
	if (!gameOverScreen_.Update(nextScene_)) return;
	if (nextScene_ == Scene::GamePlay) {
		FadePanel::GetInstance().AddCollBack([=]() { End(); Initialize(); });
	}
	else {
		FadePanel::GetInstance().AddCollBack([=]() { isEnd_ = true; });
	}
	FadePanel::GetInstance().FadeOut();
}

void GamePlayScene::clearUpdate()
{
	if (currentStage_ == Stage::Stage8) {
		clear_All_Update();
	}
	else {
		clear_Normal_Update();
	}
	return;

}
void GamePlayScene::nextUpdate()
{
	world_->Update();
	bgScreen_.Update();
	changeScreen_.Update();
	stageEffectScreen_.Update();
	
	if(world_->GetKeepDatas().camPosY_<=-WINDOW_HEIGHT)clearTimer_.Action();
	
	bgScreen_.DownCeil();

}

void GamePlayScene::nextSwitchUpdate()
{
}

void GamePlayScene::BeginWind()
{
	world_->sendMessage(EventMessage::BEGIN_WIND);
	bgScreen_.addBGCharacters();
	ResetWindTime();
}

void GamePlayScene::InitWindTime()
{
	windTimer_.Initialize();
	windTimer_.AddEmpty(defWindTime[(int)currentStage_]);
	windTimer_.Add([this] {BeginWind(); });
}

void GamePlayScene::ResetWindTime()
{
	windTimer_.InnerInitialize();
	windTimer_.AddEmpty(defWindTime[(int)currentStage_]);
	windTimer_.Add([this] {BeginWind(); });
}

void GamePlayScene::pause_Check_Resume(PauseScreen::returnGameType backType)
{
	if (backType == PauseScreen::returnGameType::Resume) {
		setNextMode(1);
		TweenManager::GetInstance().Play();
	}
	else {
		FadePanel::GetInstance().AddCollBack([=]() { End(); Initialize(); });
		FadePanel::GetInstance().FadeOut();
	}

}

void GamePlayScene::clear_Normal_Update()
{
	if(!gameClearScreen_.Update(nextScene_))return;

	if (nextScene_ == Scene::GamePlay) {
		FadePanel::GetInstance().AddCollBack([=]() {
			currentStage_ = (Stage)((int)currentStage_ + 1);
			End();
			Initialize();
		});
	}
	else {
		FadePanel::GetInstance().AddCollBack([=]() { isEnd_ = true; });
	}
	FadePanel::GetInstance().FadeOut();
}

void GamePlayScene::clear_All_Update()
{
	if(!allClearScreen_.Update(nextScene_))return;

	FadePanel::GetInstance().AddCollBack([=]() { isEnd_ = true; });
	FadePanel::GetInstance().FadeOut();
	
}

void GamePlayScene::ToStartMode()
{
}

void GamePlayScene::ToBaseMode()
{
}

void GamePlayScene::ToOverMode()
{
	TweenManager::GetInstance().StopAll();
	gameOverScreen_.Init();
	Sound::GetInstance().PlayBGM(BGM_ID::GAME_OVER_BGM);

}

void GamePlayScene::ToClearMode()
{
	CheatData::getInstance().SetClearData((int)currentStage_, true);

	if (currentStage_ == Stage::Stage8) {
		allClearScreen_.Init();
		allClearScreen_.SetScore(uiScreen_.GetScore(), ply1->GetPHeadLiveCount(), currentStage_);
	}
	else {
		CheatData::getInstance().SetSelectStage(AddStageNum(currentStage_));

		gameClearScreen_.Init();
		gameClearScreen_.SetScore(uiScreen_.GetScore(), ply1->GetPHeadLiveCount(), currentStage_);
	}
	Sound::GetInstance().SetBGMVolume(BGM_ID::STAGE_CLEAR_BGM, 0.7f);
	Sound::GetInstance().PlayBGM(BGM_ID::STAGE_CLEAR_BGM);

}

void GamePlayScene::ToPauseMode()
{
	TweenManager::GetInstance().StopAll();
	pauseScreen_.Init();
	Sound::GetInstance().PlaySE(SE_ID::CHECK_SE);
}

void GamePlayScene::ToNextMode()
{
	windTimer_.Initialize();
	windTimer_.AddEmpty(changeCount_);
	windTimer_.Add([this] {setNextMode(3); });
}

void GamePlayScene::ToSwitchMode()
{
}

Stage GamePlayScene::AddStageNum(Stage current)
{
	int nextStage = ((int)current + 1);
	nextStage %= (int)Stage::STAGE_COUNT;
	return (Stage)nextStage;
}

void GamePlayScene::OverDraw() const
{
	gameOverScreen_.Draw();
}

void GamePlayScene::ClearDraw() const
{
	if (currentStage_ == Stage::Stage8) {
		allClearScreen_.Draw();
	}
	else {
		gameClearScreen_.Draw();
	}
}

void GamePlayScene::PauseDraw() const
{
	pauseScreen_.Draw();
}

void GamePlayScene::setNextMode(int mode) {
	gamePlayMode_ = mode;

	changeModeFunctionMap_[gamePlayMode_]();

}

void GamePlayScene::startUpdate()
{
	if (startScreen_.Update()) {
		setNextMode(1);
		world_->SetTarget(ply1.get());
	}
}
