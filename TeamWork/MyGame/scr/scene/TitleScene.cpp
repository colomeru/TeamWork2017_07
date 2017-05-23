﻿#include "TitleScene.h"
#include "../Def.h"
#include "../time/Time.h"
#include "../camera/Camera.h"
#include "../conv/DXConverter.h"
#include "../graphic/Sprite.h"

TitleScene::TitleScene() :
nextScene_(Scene::Menu)
{
	// ƒ[ƒ‹ƒh¶¬
	world_ = std::make_shared<World>();
	// ƒCƒxƒ“ƒgƒŠƒXƒi[“o˜^
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	Camera::GetInstance().SetRange(0.1f, 10000.0f);
	Camera::GetInstance().SetViewAngle(60.0f);
	Camera::GetInstance().Up.Set(Vector3::Up);
	Camera::GetInstance().Position.Set(Vector3(0, 0, -10));
	Camera::GetInstance().Target.Set(Vector3(0, 0, 0));
	Camera::GetInstance().Update();
	isEnd_ = false;
}

void TitleScene::Update()
{
	// XV
	world_->Update();

	// I—¹
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;
	Camera::GetInstance().Position.Set(Vector3 (0,0,-50));
	Camera::GetInstance().Target.Set(Vector3(0,0,0));
	Camera::GetInstance().Update();
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::UP)) {
		selectX_ = 850.0f;
		selectY_ = 703.0f;
		selectNum_ = 0;
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::DOWN)) {
		selectX_ = 850.0f;
		selectY_ = 768.0f;
		selectNum_ = 1;
	}

	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE)) {
		if (selectNum_ == 0) {
			isEnd_ = true;
		}
		else if (selectNum_ == 1) {
			Escape;
		}
	}
}

void TitleScene::Draw() const
{

	// 読みこんだグラフィックを画面左上に描画
	//auto pos1 = DXConverter::GetInstance().ToVECTOR(Vector3(0, 0, 0));
	//auto pos2 = DXConverter::GetInstance().ToVECTOR(Vector3(0, 0, 0));
	//DrawFormatString(100, 100, GetColor(255, 255, 255), "TitleScene");
	Sprite::GetInstance().Draw(SPRITE_ID::TITLE_START_SPRITE, Vector2(905.0f, 680.0f));
	Sprite::GetInstance().Draw(SPRITE_ID::TITLE_EXIT_SPRITE, Vector2(905.0f, 740.0f));
	Sprite::GetInstance().Draw(SPRITE_ID::SPHERE_SPRITE, Vector2(selectX_, selectY_));

	DrawFormatString(100, 100, GetColor(255, 255, 255), "TitleScene");
	auto drawpos = Vector2(WINDOW_WIDTH, WINDOW_HEIGHT) / 2;
	auto origin = Sprite::GetInstance().GetSize(SPRITE_ID::TITLE_SPRITE)/2;
	Sprite::GetInstance().Draw(SPRITE_ID::TITLE_SPRITE,drawpos,origin,Vector2::One);
	//DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);
	//DrawCapsule3D(pos1, pos2, 4.0f, 8, GetColor(255, 255, 0), GetColor(255, 255, 0), FALSE);
	// •`‰æ
	
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
	world_->Clear();
}

void TitleScene::handleMessage(EventMessage message, void * param)
{
}
