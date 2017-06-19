
#include "TutorialScene.h"
#include "../time/Time.h"
#include "../conv/DXConverter.h"
#include "../fade/FadePanel.h"

#include "../actor/player/Player3.h"
#include "../actor/enemy/Enemy.h"
#include "../graphic/Model.h"
#include "../math/MathHelper.h"
#include "../stageGenerator/Stage.h"
#include "../stageGenerator/Stage1/Stage1.h"
#include "../actor/Field/Clothes/TestClothes.h"
#include "../actor/Field/Clothes/Hanger/Hanger.h"
#include "../graphic/Sprite.h"
#include "../actor/Field/Clothes/Hairball/HairballGenerator/HairballGenerator.h"
#include "../game/Random.h"
#include "GamePlayDefine.h"
#include"../actor/Field/ClothesLine.h"
#include"../tween/TweenManager.h"
#include"../actor/Field/Enemys/TutorialManager.h"

static int maxTextCount[maxTutorialNum]{
	2,
	1,
	1
};

TutorialScene::TutorialScene() :
	nextScene_(Scene::Menu), dummy_(0)
{
	// ワールド生成
	world_ = std::make_shared<World>();
	// イベントリスナー登録
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});

	bgScreen_ = BackgroundScreen(world_.get());
	textScreen_ = TutorialTextScreen(world_.get());

	StageNameList_[0] = "Tutorial1";
	StageNameList_[1] = "Tutorial2";
	StageNameList_[2] = "Tutorial3";

	TextAddList_[0] = ("_1");
	TextAddList_[1] = ("_2");
	TextAddList_[2] = ("_3");

	setLockFuncList_.push_back([this](int i) {SetLock1(i); });
	setLockFuncList_.push_back([this](int i) {SetLock2(i); });
	setLockFuncList_.push_back([this](int i) {SetLock3(i); });
}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Initialize()
{
	currentTutorialNum_ = 0;
	SceneInit();
}

void TutorialScene::SceneInit()
{

	tutorialLockNum_ = 0;
	isEnd_ = false;
	world_->Initialize();

	player_ = std::make_shared<Player>(world_.get());
	world_->Add(ACTOR_ID::PLAYER_ACTOR, player_);
	world_->PushStackActor(player_);

	Stage1 stage(world_.get(), StageNameList_[currentTutorialNum_], 0);
	stage.CreateClothes();

	world_->InitializeInv(Vector2(player_->GetPosition().x, player_->GetPosition().y));
	world_->SetTarget(player_.get());

	bgScreen_.Init(Stage::Stage1);
	textScreen_.Init(StageNameList_[currentTutorialNum_]+ TextAddList_[tutorialLockNum_] +".txt");

	world_->Add(ACTOR_ID::LANE_ACTOR, std::make_shared<ClothesLine>(world_.get(), 0, stage.GetStageSize() + Vector2(150, 0), Vector2(0, 0)));
	world_->Add(ACTOR_ID::LANE_ACTOR, std::make_shared<ClothesLine>(world_.get(), 1, stage.GetStageSize() + Vector2(150, 0), Vector2(0, 0)));
	world_->Add(ACTOR_ID::LANE_ACTOR, std::make_shared<ClothesLine>(world_.get(), 2, stage.GetStageSize() + Vector2(150, 0), Vector2(0, 0)));

	FadePanel::GetInstance().SetInTime(0.5f);
	FadePanel::GetInstance().FadeIn();

	enemGenerator_ = std::make_shared<TutorialManager>(world_.get(), currentTutorialNum_);
	world_->Add(ACTOR_ID::ENEMY_ACTOR, enemGenerator_);

	player_->Update();
	player_->SetUseKey(false);
	//player_->SetIsTutorialTextWriting(true);

	SetLockList(currentTutorialNum_, tutorialLockNum_);
}

void TutorialScene::Update()
{
	// 更新
	world_->Update();
	player_->deadLine();

	bgScreen_.Update();
	if (textScreen_.TutorialUpdate()) {
		player_->SetUseKey(true);
		//player_->SetIsTutorialTextWriting(false);
	}
	if (IsCanSceneLock()) {
		SceneLock();
	}
	if (!FadePanel::GetInstance().IsClearScreen()) return;

	//風テスト
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::H)) {
		world_->sendMessage(EventMessage::BEGIN_WIND);
	}

	//Camera::GetInstance().Position.Set(camera_pos_);
	//Camera::GetInstance().Target.Set(target_);
	//Camera::GetInstance().Update();


	// 終了
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::H) || GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM8))
	{
		FadePanel::GetInstance().AddCollBack([=]() { isEnd_ = true; });
		FadePanel::GetInstance().FadeOut();
	}

}

void TutorialScene::Draw() const
{
	bgScreen_.Draw();
	// 描画
	world_->Draw(3, world_->GetKeepDatas().playerLane_);

	textScreen_.Draw();

	if (BuildMode != 1) return;
	DrawFormatString(0, 00, GetColor(255, 255, 255), "CreditScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);


}

bool TutorialScene::IsEnd() const
{
	return isEnd_;
}

Scene TutorialScene::Next() const
{
	return nextScene_;
}

void TutorialScene::End()
{
	// 初期化
	world_->Clear();
	bgScreen_.End();

	TweenManager::GetInstance().Clear();
	
	//FadePanel::GetInstance().AddCollBack([=] {FadePanel::GetInstance().FadeIn(); });
	//FadePanel::GetInstance().FadeOut();
}

void TutorialScene::handleMessage(EventMessage message, void * param)
{
	switch (message)
	{
	case EventMessage::BEGIN_WIND:
		UnLock(UnLockType::StartWind);
		break;
	case EventMessage::STRONG_WIND:
		break;
	case EventMessage::ATTENUATE_WIND:
		break;
	case EventMessage::END_WIND:
		break;
	case EventMessage::START_LANE_CHANGE:
		break;
	case EventMessage::GOAL_FLAG:
		break;
	case EventMessage::GAME_CLEAR_FLAG:
		break;
	case EventMessage::TAPPER_DEAD:{
		enemGenerator_->StartTapperResurrectTimer();
		
		UnLock(UnLockType::KillTapper);
		break;
	}
	case EventMessage::LANE_CHANGE_END :{
		UnLock(UnLockType::ChangeLane);
		break;
	}
	case EventMessage::USE_SWORD: {
		UnLock(UnLockType::UseSword);
		break;
	}
	case EventMessage::LANE_CHANGE_UP_END: {
		UnLock(UnLockType::ChangeLaneUp);

		break;
	}
	case EventMessage::PLAY_NEXT_STAGE:
		break;
	case EventMessage::ADD_SCORE: {
		UnLock(UnLockType::BiteClothes);
		break;
	}
	case EventMessage::PLAYER_DEAD:
	{
		FadePanel::GetInstance().AddCollBack([=]() { End(); SceneInit(); });
		FadePanel::GetInstance().FadeOut();
		break;
	}
	default:
		break;
	}
}

void TutorialScene::SceneLock()
{
	for (auto& i : lockList_) {
		i.second = false;
	}

	tutorialLockNum_++;
	//テキストの最大値を超えたら、次のチュートリアルへ
	if (tutorialLockNum_ >= maxTextCount[currentTutorialNum_]) {
		ChangeNextTutorial();
		return;
	}
	tutorialLockNum_ =min(tutorialLockNum_,maxTextCount[currentTutorialNum_]-1);
	
	SetLockList(currentTutorialNum_, tutorialLockNum_);

	player_->SetUseKey(false);
	//player_->SetIsTutorialTextWriting(true);
	textScreen_.Init(StageNameList_[currentTutorialNum_] + TextAddList_[tutorialLockNum_] + ".txt");

	if (textScreen_.TutorialUpdate()) {
		player_->SetUseKey(true);
		//player_->SetIsTutorialTextWriting(false);
	}
}

void TutorialScene::UnLock(UnLockType type)
{
	for (auto& i : lockList_) {
		if (!i.second) {
			if (i.first == type) {
				i.second = true;
			}
			return;
		}
	}
}

bool TutorialScene::IsCanSceneLock() const
{
	if (!player_->GetUseKey())return false;
	for (auto i : lockList_) {
		if (!i.second)return false;
	}
	return true;
}


void TutorialScene::SetLockList(int currentTutorial,int tutorialLockNum)
{
	lockList_.clear();

	setLockFuncList_[currentTutorial](tutorialLockNum);

}

void TutorialScene::SetLock1(int tutorialLockNum)
{
	switch (tutorialLockNum)
	{
	case 0: {
		lockList_.push_back(std::pair<UnLockType, bool>(UnLockType::BiteClothes, false));
		break;
	}
	case 1: {
		lockList_.push_back(std::pair<UnLockType, bool>(UnLockType::ChangeLaneUp, false));
		break;
	}
	default:
		lockList_.push_back(std::pair<UnLockType, bool>(UnLockType::Dummy, true));
		break;
	}
}

void TutorialScene::SetLock2(int tutorialLockNum)
{
	switch (tutorialLockNum)
	{
	case 0: {
		lockList_.push_back(std::pair<UnLockType, bool>(UnLockType::UseSword, false));
		lockList_.push_back(std::pair<UnLockType, bool>(UnLockType::KillTapper, false));
		break;
	}
	default:
		lockList_.push_back(std::pair<UnLockType, bool>(UnLockType::Dummy, true));
		break;
	}
}

void TutorialScene::SetLock3(int tutorialLockNum)
{
	switch (tutorialLockNum)
	{
	case 0: {
		lockList_.push_back(std::pair<UnLockType, bool>(UnLockType::StartWind, false));
		lockList_.push_back(std::pair<UnLockType, bool>(UnLockType::StartWind, false));
		lockList_.push_back(std::pair<UnLockType, bool>(UnLockType::StartWind, false));
		break;
	}
	default:
		lockList_.push_back(std::pair<UnLockType, bool>(UnLockType::Dummy, true));
		break;
	}

}

void TutorialScene::ChangeNextTutorial()
{
	if (currentTutorialNum_ >= maxTutorialNum -1) {
		FadePanel::GetInstance().AddCollBack([=]() { isEnd_ = true; });
		FadePanel::GetInstance().FadeOut();
	}
	else {
		FadePanel::GetInstance().AddCollBack([=]() { End(); addCurrentNum(); SceneInit(); });
		FadePanel::GetInstance().FadeOut();
	}
}
