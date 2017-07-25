#include "LogoScene.h"
#include "../time/Time.h"
#include "../math/MathHelper.h"
#include "../graphic/Sprite.h"
#include"../fade/FadePanel.h"

LogoScene::LogoScene() :
	nextScene_(Scene::Title),timeCount_(0)
{
	// ワールド生成
	world_ = std::make_shared<World>();
	// イベントリスナー登録
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});
}

LogoScene::~LogoScene()
{
}

void LogoScene::Initialize()
{
	timeCount_ = 0.f;
	isEnd_ = false;
	nextScene_ = Scene::Title;
	FadePanel::GetInstance().SetInTime(0.5f);
	FadePanel::GetInstance().FadeIn();
}

void LogoScene::Update()
{

	// 更新
	world_->Update();
	if (!FadePanel::GetInstance().IsClearScreen()) return;

	timeCount_ += Time::DeltaTime;

	if (timeCount_ <= 3.f)return;

	FadePanel::GetInstance().AddCollBack([=] {
		isEnd_ = true;
	});
	FadePanel::GetInstance().FadeOut();
}

void LogoScene::Draw() const
{
	DrawFillBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(0, 0, 0));
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::LOGO_SPRITE)/2;
	Sprite::GetInstance().Draw(SPRITE_ID::LOGO_SPRITE,Vector2((float)WINDOW_WIDTH,(float)WINDOW_HEIGHT)/2.f,origin,Vector2::One);
}

bool LogoScene::IsEnd() const
{
	return isEnd_;
}

Scene LogoScene::Next() const
{
	return nextScene_;
}

void LogoScene::End()
{
	// 初期化
	world_->Clear();
}

void LogoScene::handleMessage(EventMessage message, void * param)
{
}
