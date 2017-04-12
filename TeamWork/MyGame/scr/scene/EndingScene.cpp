#include "EndingScene.h"
#include "../time/Time.h"
#include "../camera/Camera.h"
#include "../conv/DXConverter.h"
#include "../graphic/Sprite.h"
#include "../input/Keyboard.h"

EndingScene::EndingScene() :
nextScene_(Scene::Title)
{
	// ワールド生成
	world_ = std::make_shared<World>();
	// イベントリスナー登録
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});
}

EndingScene::~EndingScene()
{
}

void EndingScene::Initialize()
{
	isEnd_ = false;

	Camera::GetInstance().SetRange(0.1f, 10000.0f);
	Camera::GetInstance().SetViewAngle(60.0f);
	Camera::GetInstance().Up.Set(Vector3::Up);
	Camera::GetInstance().Position.Set(Vector3(0.0f, 0.0f, -10.0f));
	Camera::GetInstance().Target.Set(Vector3(0.0f, 0.0f, 0.0f));
	Camera::GetInstance().Update();

	size = 1.0f;
	horizontal = 0.0f;
	vertical = 0.0f;
}

void EndingScene::Update()
{
	// 更新
	world_->Update();

	// 終了
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;

	Camera::GetInstance().Position.Set(Vector3(0.0f, 0.0f, -10.0f));
	Camera::GetInstance().Target.Set(Vector3(0.0f, 0.0f, 0.0f));
	Camera::GetInstance().Update();

	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::A))
	{
		size += 0.01f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::S))
	{
		size -= 0.01f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::RIGHT))
	{
		horizontal += 5.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::LEFT))
	{
		horizontal -= 5.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::UP))
	{
		vertical -= 5.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::DOWN))
	{
		vertical += 5.0f;
	}

}

void EndingScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "EndingScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);
	DrawFormatString(0, 40, GetColor(255, 255, 255), "座標" + (int)horizontal + (int)vertical);

	// 描画
	world_->Draw();

	DrawCapsule3D(VGet(0.0f,0.0f,0.0f),VGet(0.0f,0.0f,0.0f), size, 4, GetColor(255,0,0),GetColor(0,0,255),FALSE);

	DrawLine3D(VGet(10.0f, 0.0f, 0.0f), VGet(-10.0f, 0.0f, 0.0f), GetColor(0, 255, 0));

	DrawTriangle3D(VGet(-5.0f, 5.0f, 0.0f), VGet(5.0f, 5.0f, 0.0f),VGet(0.0f, -5.0f, 0.0f),GetColor(0,0,255),FALSE);

	DrawSphere3D(VGet(5.0f, 0.0f, 0.0f),size, 4, GetColor(255, 0, 0), GetColor(0, 0, 255), FALSE);

	DrawCone3D(VGet(-5.0f, 5.0f, 0.0f), VGet(-5.0f, -2.0f, 0.0f), 2.0f, 4,GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);

	Sprite::GetInstance().Draw(SPRITE_ID::TEST_SPRITE,Vector2(0.0f,200.0f));
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::Z))return;
	Sprite::GetInstance().Draw(SPRITE_ID::TEST2_SPRITE, Vector2(horizontal, vertical));

	
}

bool EndingScene::IsEnd() const
{
	return isEnd_;
}

Scene EndingScene::Next() const
{
	return nextScene_;
}

void EndingScene::End()
{
	// 初期化
	world_->Clear();
}

void EndingScene::handleMessage(EventMessage message, void * param)
{
}
