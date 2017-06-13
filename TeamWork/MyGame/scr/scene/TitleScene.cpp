#include "TitleScene.h"
#include "../Def.h"
#include "../camera/Camera.h"
#include "../conv/DXConverter.h"
#include "../graphic/Sprite.h"
#include "../game/Game1.h"
#include "../fade/FadePanel.h"
#include "../time/Time.h"
#include"../tween/TweenManager.h"
#include"../sound/sound.h"
#include"../actor/Field/Clothes/BaseClothes.h"

TitleScene::TitleScene() :
	nextScene_(Scene::Menu)
{
	world_ = std::make_shared<World>();

	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});

	posList_.push_back(Vector2(200.f, 750.f));
	posList_.push_back(Vector2(200.f, 930.f));
}
TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{

	isEnd_ = false;
	isPushKey_ = false;
	isStartSetPanel_ = false;
	selectPos_ = Vector2(200.f,750.f);
	selectNum_ = 0;
	timer = 0;
	alpha_.resize(2, 0.f);
	bgScreen_.Init();
	sinCount_ = 0;


	Sound::GetInstance().PlayBGM(BGM_ID::TITLE_BGM,DX_PLAYTYPE_LOOP);
	Sound::GetInstance().SetBGMVolume(BGM_ID::TITLE_BGM, 0.5f);
	//FadePanel::GetInstance().Initialize();
	FadePanel::GetInstance().SetInTime(0.5f);
	FadePanel::GetInstance().FadeIn();
	isTrigger_ = true;
}

void TitleScene::Update()
{
	sinCount_+=5; sinCount_ %= 360;
	bgScreen_.Update();
	world_->Update();

	if (!isPushKey_) {
		if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M) || GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2)) {
			SetNextPanel();

		}

		return;
	}
	
	timer += Time::DeltaTime;
	// XV
	// I—¹
	/*Camera::GetInstance().Position.Set(Vector3 (0,0,-50));
	Camera::GetInstance().Target.Set(Vector3(0,0,0));
	Camera::GetInstance().Update();
	*/
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M) || GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2)) {
		if (selectNum_ == 0) {
			FadePanel::GetInstance().AddCollBack([=]() {
				SetBackgroundColor(153, 204, 255);
				isEnd_ = true;
			});
			FadePanel::GetInstance().SetOutTime(1.0f);
			FadePanel::GetInstance().FadeOut();
		}
		else if (selectNum_ == 1) {
			GameFrame::GameEnd();
			//Escape
		}
		Sound::GetInstance().PlaySE(SE_ID::CHECK_SE);
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::UP)|| (GamePad::GetInstance().Stick().y < -0.3f&&isTrigger_)) {
		isTrigger_ = false;
		selectNum_++;
		selectNum_ %=2;
		TweenManager::GetInstance().Add(Linear, &selectPos_, posList_[selectNum_], 0.2f);
		Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::DOWN) || (GamePad::GetInstance().Stick().y>0.3f&&isTrigger_)) {
		isTrigger_ = false;
		selectNum_++;
		selectNum_ %= 2;
		TweenManager::GetInstance().Add(Linear, &selectPos_, posList_[selectNum_], 0.2f);
		Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);
	}
	if (abs(GamePad::GetInstance().Stick().y) <= 0.3f) {
		isTrigger_=true;
	}
	for (int i = 0; i < alpha_.size(); i++) {
		if (i == selectNum_) {
			alpha_[i] = MathHelper::Sin(sinCount_);
		}
		else {
			alpha_[i] = 1.f;
		}
	}
}

void TitleScene::Draw() const
{
	// 読みこんだグラフィックを画面左上に描画
	bgScreen_.Draw();


	//DrawFormatString(100, 100, GetColor(255, 255, 255), "TitleScene");
	auto drawpos = Vector2(WINDOW_WIDTH / 2, 450) ;
	auto origin = Sprite::GetInstance().GetSize(SPRITE_ID::TITLE_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::TITLE_SPRITE, drawpos, origin, Vector2::One);

	Vector2 pborigin = Sprite::GetInstance().GetSize(SPRITE_ID::PRESS_B_SPRITE) / 2;
	if(!isPushKey_)Sprite::GetInstance().Draw(SPRITE_ID::PRESS_B_SPRITE, Vector2(WINDOW_WIDTH/2, 800.0f),pborigin,MathHelper::Sin(sinCount_),Vector2::One);

	if (!isPushKey_)return;
//	alpha_

	Vector2 ssorigin = Sprite::GetInstance().GetSize(SPRITE_ID::CHANGE_STAGESELECT_TEXT_SPRITE)/2;
	Sprite::GetInstance().Draw(SPRITE_ID::CHANGE_STAGESELECT_TEXT_SPRITE, Vector2(WINDOW_WIDTH/2, 750.0f), ssorigin,alpha_.at(0),Vector2::One);
	Vector2 teorigin = Sprite::GetInstance().GetSize(SPRITE_ID::TITLE_EXIT_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::TITLE_EXIT_SPRITE, Vector2(WINDOW_WIDTH / 2, 950.0f), teorigin, alpha_.at(1), Vector2::One);
	Vector2 csorigin = Sprite::GetInstance().GetSize(SPRITE_ID::OROCHI_CURSOR_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::OROCHI_CURSOR_SPRITE, selectPos_, csorigin, 1.f, Vector2::One);

	world_->Draw();

}

bool TitleScene::IsEnd() const
{
	return isEnd_;
}

Scene TitleScene::Next() const
{
	return nextScene_;
}

void TitleScene::End()
{
	Sound::GetInstance().StopBGM();
	world_->Clear();

	//FadePanel::GetInstance().AddCollBack([=] {FadePanel::GetInstance().FadeIn(); });
	//FadePanel::GetInstance().FadeOut();
}

void TitleScene::handleMessage(EventMessage message, void * param)
{
}

void TitleScene::SetNextPanel()
{
	if (isStartSetPanel_)return;
	isStartSetPanel_ = true;
	Sound::GetInstance().PlaySE(SE_ID::CHECK_SE);

	TweenManager::GetInstance().Add(Linear, &dummy_, 1.f, 0.2f, [=] {isPushKey_ = true; sinCount_ = 0; });
}
