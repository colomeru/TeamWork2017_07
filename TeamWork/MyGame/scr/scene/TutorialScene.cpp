
#include "TutorialScene.h"
#include "../time/Time.h"
#include "../conv/DXConverter.h"
#include "../fade/FadePanel.h"

#include "../actor/enemy/Enemy.h"
#include "../graphic/Model.h"
#include "../math/MathHelper.h"
#include "../stageGenerator/Stage.h"
#include "../stageGenerator/Stage1/Stage1.h"
#include "../actor/Field/Clothes/Hanger/Hanger.h"
#include "../graphic/Sprite.h"
#include "../actor/Field/Clothes/Hairball/HairballGenerator/HairballGenerator.h"
#include "../game/Random.h"
#include "GamePlayDefine.h"
#include"../actor/Field/ClothesLine.h"
#include"../tween/TweenManager.h"
#include"../actor/Field/Enemys/TutorialManager.h"
#include"../actor/Effects/PlayerEffect/CursorEffect.h"
#include"../sound/sound.h"
#include"../graphic/FontManager.h"
#include"../input/InputChecker.h"

static int maxTextCount[maxTutorialNum]{
	4,
	1,
	1,
	1,
	3
};

TutorialScene::TutorialScene() :
	nextScene_(Scene::Menu), sinCount_(0), isDrawCtrl_(false), isNext_(false),stageTextNum_(0), isUpdate_(false)
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
	StageNameList_[3] = "Tutorial4";
	StageNameList_[4] = "Tutorial5";

	TextAddList_[0] = ("_1");
	TextAddList_[1] = ("_2");
	TextAddList_[2] = ("_3");
	TextAddList_[3] = ("_4");

	setLockFuncList_.push_back([this](int i) {SetLock1(i); });
	setLockFuncList_.push_back([this](int i) {SetLock2(i); });
	setLockFuncList_.push_back([this](int i) {SetLock3(i); });
	setLockFuncList_.push_back([this](int i) {SetLock4(i); });
	setLockFuncList_.push_back([this](int i) {SetLock5(i); });

	stageTexts_[0] = "身体を振ってみよう";
	stageTexts_[1] = "首を伸ばしてみよう";
	stageTexts_[2] = "首を逆側に伸ばしてみよう";
	stageTexts_[3] = "頭を切り替えてみよう";
	stageTexts_[4] = "下に落ちてみよう";
	stageTexts_[5] = "上に登ってみよう";
	stageTexts_[6] = "布団叩きを切ってみよう";
	stageTexts_[7] = "ゴールを目指そう";
	stageTexts_[8] = "";
	stageTexts_[9] = "";

}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Initialize()
{
	Sound::GetInstance().StopBGM();
	Sound::GetInstance().PlayBGM(BGM_ID::STAGE_01_BGM, DX_PLAYTYPE_LOOP);
	
	stageTextNum_ = 0;

	currentTutorialNum_ = 0;
	ResetLockNum();
	SceneInit();
	isDrawCtrl_ = false;
	isUpdate_ = false;
}

void TutorialScene::SceneInit()
{
	isNext_ = false;
	sinCount_ = 0;
	isDrawCtrl_ = false;
	isUpdate_ = false;	
	isAlreadyPutButton_ = false;
	isEnd_ = false;
	world_->Initialize();

	Vector2 pPos = Vector2(0.f, 200.f);
	if(currentTutorialNum_==0)pPos = Vector2(600.f, 200.f);
	player_ = std::make_shared<Player>(world_.get(),3,1, pPos);
	world_->Add(ACTOR_ID::PLAYER_ACTOR, player_);
	world_->PushStackActor(player_);

	Stage1 stage(world_.get(), StageNameList_[currentTutorialNum_]);
	if (currentTutorialNum_ == maxTutorialNum - 1) stage.LoadStage();
	else stage.CreateClothes();

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

	SetLockList(currentTutorialNum_, tutorialLockNum_);

	timeCount_ = 0.f;

	switch (currentTutorialNum_)
	{
	case 0: {
		stageTextNum_ = tutorialLockNum_;
		
		break;
	}
	case 1: {
		stageTextNum_ = maxTextCount[0];
		
		break;
	}
	case 2: {
		stageTextNum_ = maxTextCount[0]+ maxTextCount[1];

		break;
	}
	case 3: {
		stageTextNum_ = maxTextCount[0] + maxTextCount[1]+ maxTextCount[2];

		break;
	}
	case 4: {
		stageTextNum_ = maxTextCount[0] + maxTextCount[1] + maxTextCount[2]+ maxTextCount[3]+ tutorialLockNum_;

		break;
	}
	default:
		break;
	} 
	world_->Update();
}

void TutorialScene::Update()
{
	sinCount_+=3; sinCount_ %= 360;

	// 更新
	if(isUpdate_)world_->Update();
	player_->deadLine();

	//ロック関係
	if (player_->GetRot() >= 180.f || player_->GetRot() <= 0.f) {
		UnLock(UnLockType::FullStick);
	}
	else if (player_->GetRot() <= 40.f) {
		UnLock(UnLockType::HalfFullRightStick);

	}
	else if (player_->GetRot() >= 140.f) {
		UnLock(UnLockType::HalfFullLeftStick);

	}
	else if (player_->GetRot() >= 120.f || player_->GetRot() <= 60.f) {
		UnLock(UnLockType::Stick);
	}
	else {
		ReLockPendulum();
	}
	if (!player_->GetIsBiteMode()) {
		ReLockBite();
	}
	if (!player_->GetIsSwordActive()) {
		ReLockUseSword();
	}

	bgScreen_.Update();
	if (textScreen_.TutorialUpdate() && !isAlreadyPutButton_) {
		if (tutorialLockNum_ < maxTextCount[currentTutorialNum_]) {
			isAlreadyPutButton_ = true;
			player_->SetUseKey(true);
			if (currentTutorialNum_ >= maxTutorialNum - 1 &&tutorialLockNum_ >= 1) {
				isUpdate_ = true;
			}
			else {
				isDrawCtrl_ = true;
				isUpdate_ = true;
			}
		}
	}
	if (IsCanSceneLock()) {
		SceneLock();
	}
	if (!FadePanel::GetInstance().IsClearScreen()) return;

	// 終了
	if (InputChecker::GetInstance().KeyTriggerDown(InputChecker::Input_Key::Start))
	{
		if (!isNext_) {
			FadePanel::GetInstance().AddCollBack([=]() {Sound::GetInstance().StopBGM(); isEnd_ = true; });
			if (!FadePanel::GetInstance().IsAction())FadePanel::GetInstance().FadeOut();
		}
	}

}

void TutorialScene::Draw() const
{
	bgScreen_.Draw();
	// 描画
	world_->Draw(3, world_->GetKeepDatas().playerLane_);

	textScreen_.Draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (128));
	if (stageTexts_[stageTextNum_].size()>=3) {
		DrawBox(100, 0, stageTexts_[stageTextNum_].size() * 60, 100, GetColor(128, 128, 128), TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	FontManager::GetInstance().DrawTextApplyFont(200, 20, GetColor(0, 0, 0), FONT_ID::NAME_FONT, stageTexts_[stageTextNum_]);



	if (isDrawCtrl_) {

		int gamepadX = 1300;

		if (currentTutorialNum_ >= maxTutorialNum - 1) {

		}
		else {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, (128));
			DrawBox(1200, 500, WINDOW_WIDTH - 50, 900, GetColor(128, 128, 128), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			Sprite::GetInstance().Draw(SPRITE_ID::GAMEPAD_SPRITE, Vector2(gamepadX, WINDOW_HEIGHT / 2), Vector2::Zero, 1.f, Vector2::One*1.5f);
			for (auto i : lockList_) {
				if (!i.isLock) {
					Sprite::GetInstance().Draw(i.ctrl, Vector2(gamepadX, WINDOW_HEIGHT / 2),Vector2::Zero, abs(MathHelper::Sin((float)sinCount_)), Vector2::One*1.5f);
					return;
				}
			}
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (abs(MathHelper::Sin(sinCount_)) * 255));
	if(!player_->GetUseKey()&& !player_->GetIsClearMode())FontManager::GetInstance().DrawTextApplyFont(1550, 250, GetColor(0, 0, 0), FONT_ID::TUTORIAL_FONT, "Bで進むよ");
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

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
	
	FadePanel::GetInstance().SetOutTime(0.5f);

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
	case EventMessage::GOAL_FLAG: {
		UnLock(UnLockType::ClearStage);
		break;
	}
	case EventMessage::CHANGE_HEAD_KEY: {
		UnLock(UnLockType::ChangeHeadKey);
		break;
	}
	case EventMessage::GAME_CLEAR_FLAG: {
		UnLock(UnLockType::EndStage);
		break;
	}
	case EventMessage::CHANGE_HEAD: {
		UnLock(UnLockType::ChangeHead);
		break;
	}
	case EventMessage::LANE_CHANGE_FALL: {
		UnLock(UnLockType::ChangeLaneFall);
		break;
	}
	case EventMessage::TAPPER_DEAD:{
		enemGenerator_->StartTapperResurrectTimer();
		
		UnLock(UnLockType::UseSword);
		UnLock(UnLockType::KillTapper);
		break;
	}
	case EventMessage::LANE_CHANGE_END: {
		UnLock(UnLockType::ChangeLane);

		if (currentTutorialNum_ >= maxTutorialNum - 1 && tutorialLockNum_ == 0) {
			if (player_->GetLaneNum() != 0) {
				for (auto& i : lockList_) {
					i.isLock = false;
				}
			}
		}

		break;
	}
	case EventMessage::LANE_CHANGE_DOWN_END: {
		ReLockUpLane();
		break;
	}
	case EventMessage::USE_SWORD: {
		UnLock(UnLockType::UseSword);
		break;
	}
	case EventMessage::LANE_CHANGE_UP_END: {
		UnLock(UnLockType::ChangeLaneUp);

		if (currentTutorialNum_ >= maxTutorialNum - 1 && tutorialLockNum_ == 0) {
			if (player_->GetLaneNum() == 0) {
				for (int i = 0; i < 5; i++) {
						lockList_[i].isLock = true;
				}
			}
		}
		break;
	}
	case EventMessage::PLAY_NEXT_STAGE:
		break;
	case EventMessage::ADD_SCORE: {
		UnLock(UnLockType::BiteClothes);
		ReLockChangeHead();
		break;
	}
	case EventMessage::NECK_SHOOT: {
		UnLock(TutorialScene::PlayerShoot);
		break;
	}
	case EventMessage::NECK_BACK_SHOOT: {
		UnLock(TutorialScene::PlayerBackShoot);
		break;
	}
	case EventMessage::NECK_SHOOT_END: {
		ReLockNeckShoot();
		break;
	}
	case EventMessage::PLAYER_DEAD:
	{
		FadePanel::GetInstance().AddCollBack([=]() { End(); SceneInit(); });
		if (!FadePanel::GetInstance().IsAction())FadePanel::GetInstance().FadeOut();
		break;
	}
	default:
		break;
	}
}

void TutorialScene::SceneLock()
{
	isAlreadyPutButton_ = false;
	isDrawCtrl_ = false;
	isUpdate_ = false;
	for (auto& i : lockList_) {
		i.isLock = false;
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
	textScreen_.Init(StageNameList_[currentTutorialNum_] + TextAddList_[tutorialLockNum_] + ".txt");

	if (textScreen_.TutorialUpdate()) {
		player_->SetUseKey(true);

	}

	//ここ関数化しよう
	switch (currentTutorialNum_)
	{
	case 0: {
		stageTextNum_ = tutorialLockNum_;

		break;
	}
	case 1: {
		stageTextNum_ = maxTextCount[0];

		break;
	}
	case 2: {
		stageTextNum_ = maxTextCount[0] + maxTextCount[1];

		break;
	}
	case 3: {
		stageTextNum_ = maxTextCount[0] + maxTextCount[1] + maxTextCount[2];

		break;
	}
	case 4: {
		stageTextNum_ = maxTextCount[0] + maxTextCount[1] + maxTextCount[2] + maxTextCount[3] + tutorialLockNum_;

		break;
	}
	default:
		break;
	}

}

void TutorialScene::UnLock(UnLockType type)
{
	for (auto& i : lockList_) {
		if (!i.isLock) {
			if (i.type == type) {
				i.isLock = true;
			}
			return;
		}
	}
}

bool TutorialScene::IsCanSceneLock() const
{
	if (!player_->GetIsClearMode()) {
		if (!player_->GetUseKey())return false;
	}
	else {
		if(timeCount_ >= 1.f)return false;
		else return true;
	}

	for (auto i : lockList_) {
		if (!i.isLock)return false;
	}
	return true;
}


void TutorialScene::SetLockList(int currentTutorial,int tutorialLockNum)
{
	lockList_.clear();

	player_->GetEditableUseKey().SetKeyLock(false);
	player_->GetEditableUseKey().SetStickLock(false);

	setLockFuncList_[currentTutorial](tutorialLockNum);

}

void TutorialScene::SetLock1(int tutorialLockNum)
{
	switch (tutorialLockNum)
	{
	case 0: {
		player_->GetEditableUseKey().SetKeyLock(true);
		
		lockList_.push_back(LockList(UnLockType::HalfFullRightStick, false, SPRITE_ID::GAMEPAD_STICK_SPRITE));
		lockList_.push_back(LockList(UnLockType::HalfFullLeftStick, false,SPRITE_ID::GAMEPAD_STICK_SPRITE));
		break;
	}
	case 1: {
		for (auto& rs : player_->GetEditableRot_Speed()) {
			rs = MathHelper::Clamp(rs, 10.f, 10.f);
		}
		player_->GetEditableUseKey().SetStickLock(true);
		player_->GetEditableUseKey().SetKeyLock(InputChecker::Input_Key::X, true);

		lockList_.push_back(LockList(UnLockType::PlayerShoot, false,SPRITE_ID::GAMEPAD_B_SPRITE));
		lockList_.push_back(LockList(UnLockType::BiteClothes, false,SPRITE_ID::GAMEPAD_B_SPRITE));
		break;
	}
	case 2: {
		player_->GetEditableUseKey().SetKeyLock(InputChecker::Input_Key::B,true);

		lockList_.push_back(LockList(UnLockType::PlayerBackShoot, false, SPRITE_ID::GAMEPAD_X_SPRITE));
		lockList_.push_back(LockList(UnLockType::BiteClothes, false, SPRITE_ID::GAMEPAD_X_SPRITE));
		break;
	}
	case 3: {
		lockList_.push_back(LockList(UnLockType::ChangeHead, false, SPRITE_ID::GAMEPAD_B_SPRITE));
		lockList_.push_back(LockList(UnLockType::ChangeHeadKey, false, SPRITE_ID::GAMEPAD_STICK_SPRITE));
		lockList_.push_back(LockList(UnLockType::BiteClothes, false, SPRITE_ID::GAMEPAD_B_SPRITE));
		break;
	}
	default:
		lockList_.push_back(LockList(UnLockType::Dummy, true));
		break;
	}
}

void TutorialScene::SetLock2(int tutorialLockNum)
{
	switch (tutorialLockNum)
	{
	case 0: {

		lockList_.push_back(LockList(UnLockType::ChangeHead, false,SPRITE_ID::GAMEPAD_B_SPRITE));
		lockList_.push_back(LockList(UnLockType::ChangeLaneFall, false));
		lockList_.push_back(LockList(UnLockType::BiteClothes, false,SPRITE_ID::GAMEPAD_B_SPRITE));
		break;
	}
	default:
		lockList_.push_back(LockList(UnLockType::Dummy, true));
		break;
	}
}

void TutorialScene::SetLock3(int tutorialLockNum)
{
	switch (tutorialLockNum)
	{
	case 0: {

		lockList_.push_back(LockList(UnLockType::FullStick, false, SPRITE_ID::GAMEPAD_STICK_SPRITE));
		lockList_.push_back(LockList(UnLockType::ChangeLaneUp, false, SPRITE_ID::GAMEPAD_B_SPRITE));
		lockList_.push_back(LockList(UnLockType::BiteClothes, false, SPRITE_ID::GAMEPAD_B_SPRITE));
		break;
	}

	default:
		lockList_.push_back(LockList(UnLockType::Dummy, true));
		break;
	}

}

void TutorialScene::SetLock4(int tutorialLockNum)
{
	switch (tutorialLockNum)
	{
	case 0: {
		player_->GetEditableUseKey().SetKeyLock(true);
		player_->GetEditableUseKey().SetKeyLock(InputChecker::Input_Key::A,false);


		lockList_.push_back(LockList(UnLockType::UseSword, false, SPRITE_ID::GAMEPAD_A_SPRITE));
		lockList_.push_back(LockList(UnLockType::KillTapper, false, SPRITE_ID::GAMEPAD_STICK_SPRITE));
		break;
	}
	default:
		lockList_.push_back(LockList(UnLockType::Dummy, true));
		break;
	}

}

void TutorialScene::SetLock5(int tutorialLockNum)
{
	switch (tutorialLockNum)
	{
	case 0: {
		world_->sendMessage(EventMessage::TUTORIAL_GOAL_FLASH);


		lockList_.push_back(LockList(UnLockType::ChangeHead, false, SPRITE_ID::GAMEPAD_B_SPRITE));
		lockList_.push_back(LockList(UnLockType::PlayerShoot, false, SPRITE_ID::GAMEPAD_B_SPRITE));
		lockList_.push_back(LockList(UnLockType::BiteClothes, false, SPRITE_ID::GAMEPAD_B_SPRITE));
		lockList_.push_back(LockList(UnLockType::FullStick, false, SPRITE_ID::GAMEPAD_STICK_SPRITE));
		lockList_.push_back(LockList(UnLockType::ChangeLaneUp, false, SPRITE_ID::GAMEPAD_B_SPRITE));
		lockList_.push_back(LockList(UnLockType::BiteClothes, false, SPRITE_ID::GAMEPAD_B_SPRITE));
		lockList_.push_back(LockList(UnLockType::Stick , false, SPRITE_ID::GAMEPAD_STICK_SPRITE));
		lockList_.push_back(LockList(UnLockType::ClearStage, false,SPRITE_ID::GAMEPAD_B_SPRITE));
		break;
	}
	case 1: {
		lockList_.push_back(LockList(UnLockType::EndStage, false));
		timeCount_ = 10.f;
		TweenManager::GetInstance().Add(Linear, &timeCount_, 0.f, 5.f);
		break;
	}
	case 2: {
		lockList_.push_back(LockList(UnLockType::Dummy, true));
		break;
	}
	default:
		lockList_.push_back(LockList(UnLockType::Dummy, true));
		break;
	}

}

void TutorialScene::ReLockUpLane()
{
	for (int i = lockList_.size() - 1; i > -1; i--) {
		if (lockList_[i].type == UnLockType::ChangeLaneUp) {
			if (lockList_[i].isLock) {
				lockList_[i].isLock = false;
			}
			return;
		}
	}

}

void TutorialScene::ReLockNeckShoot()
{
	for (int i = lockList_.size()-1; i > -1; i--) {
		if (lockList_[i].type == UnLockType::PlayerShoot) {
			if (lockList_[i].isLock) {
				lockList_[i].isLock = false;
			}
			return;
		}
	}

}

void TutorialScene::ReLockPendulum()
{
	for (int i = lockList_.size() - 1; i > -1; i--) {
		if (lockList_[i].isLock) {

			if (lockList_[i].type == UnLockType::Stick|| lockList_[i].type == UnLockType::FullStick) {
				lockList_[i].isLock = false;
			}
			return;
		}
	}

}

void TutorialScene::ReLockBite()
{
	for (int i = lockList_.size() - 1; i > -1; i--) {
		if (lockList_[i].isLock) {
			if (lockList_[i].type == UnLockType::BiteClothes) {
				lockList_[i].isLock = false;
			}
			return;
		}
	}

}

void TutorialScene::ReLockChangeHead()
{
	for (int i = lockList_.size() - 1; i > -1; i--) {
		if (lockList_[i].isLock) {
			if (lockList_[i].type == UnLockType::ChangeHead) {
				lockList_[i].isLock = false;
			}
			return;
		}
	}
}

void TutorialScene::ReLockUseSword()
{
	for (int i = lockList_.size() - 1; i > -1; i--) {
		if (lockList_[i].isLock) {
			if (lockList_[i].type == UnLockType::UseSword) {
				lockList_[i].isLock = false;
			}
			return;
		}
	}
}


void TutorialScene::ChangeNextTutorial()
{
	if (currentTutorialNum_ >= maxTutorialNum - 1) {
		if (!isNext_) {
			isUpdate_ = true;
			isNext_ = true;
			FadePanel::GetInstance().AddCollBack([=]() { Sound::GetInstance().StopBGM(); isEnd_ = true; });
			if (!FadePanel::GetInstance().IsAction()) FadePanel::GetInstance().FadeOut();
		}
	}
	else {
		if (!isNext_) {
			isUpdate_ = true;
			isNext_ = true;
			FadePanel::GetInstance().AddCollBack([=]() { End(); addCurrentNum(); ResetLockNum(); SceneInit(); });
			FadePanel::GetInstance().SetOutTime(0.5f, 1.0f);
			FadePanel::GetInstance().FadeOut();
		}
	}
}

void TutorialScene::ResetLockNum()
{
	tutorialLockNum_ = 0;
}
