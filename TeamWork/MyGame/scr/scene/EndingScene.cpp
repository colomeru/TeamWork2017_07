#include "EndingScene.h"
#include "../time/Time.h"
#include "../camera/Camera.h"
#include "../conv/DXConverter.h"
#include "../graphic/Sprite.h"
#include "../input/Keyboard.h"
#include <cmath>
#include <math.h>
#include "../math/MathHelper.h"


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
	alpha = 1.0f;
	cAlpha = 1.0f;
	particlePos = Vector2(200.0f, 200.0f);
	circlePos = Vector2(400.0f, 200.0f);
	turn = false;

	arrowSize = Sprite::GetInstance().GetSize(SPRITE_ID::ARROW_SPRITE);
	particleSize = Sprite::GetInstance().GetSize(SPRITE_ID::TEST_SPRITE);
	circleSize = Sprite::GetInstance().GetSize(SPRITE_ID::CIRCLE_SPRITE);

	//ここから振り子
	fx = 200.0f;
	fy = 100.0f;
	rot = 0.0f;
	rot_spd = 0.0f;
	length = 150.0f;
	g = 0.5f;

	len = 100.0f;
	lineRot = -45.0f;
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
		arrowPos.x += 5.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::LEFT))
	{
		arrowPos.x -= 5.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::UP))
	{
		arrowPos.y -= 5.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::DOWN))
	{
		arrowPos.y += 5.0f;
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::Z) && turn == false)
	{
		turn = true;
	}
	else if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::Z) && turn == true)
	{
		turn = false;
	}

	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::D))
	{
		Sprite::GetInstance().DeleteAll();
	}

	centerPos = Vector2(particlePos.x + particleSize.x / 2, particlePos.y + particleSize.y / 2);

	absH = abs(centerPos.x - arrowPos.x);
	absV = abs(centerPos.y - arrowPos.y);

	distanceH = particleSize.x / 2;
	distanceV = particleSize.y / 2;

	if (absH < distanceH && absV < distanceV)
	{
		alpha = 0.5f;
	}
	else
	{
		alpha = 1.0f;
	}

	cCenterPos = Vector2(circlePos.x + circleSize.x / 2, circlePos.y + circleSize.y / 2);
	absC = cCenterPos - arrowPos;
	distanceC = sqrt((absC.x * absC.x) + (absC.y * absC.y));
	if (distanceC < circleSize.x / 2)
	{
		cAlpha = 0.5f;
	}
	else
	{
		cAlpha = 1.0f;
	}
	

	//ここから振り子
	//rot += rot_spd;
	//spherePos.x = fx + MathHelper::Cos(rot * MathHelper::Pi / 180) * length;
	//spherePos.y = fy + MathHelper::Sin(rot * MathHelper::Pi / 180) * length;

	//現在の重りの位置
	//rad = rot * MathHelper::Pi / 180;
	auto px = fx + MathHelper::Cos(rot) * length;
	auto py = fy + MathHelper::Sin(rot) * length;

	//重力移動量を反映した重りの位置
	auto vx = px - fx;
	auto vy = py - fy;
	auto t = -(vy * g) / (vx * vx + vy * vy);
	auto gx = px + t * vx;
	auto gy = py + g + t * vy;

	//２つの重りの位置の確度差
	auto r = MathHelper::ATan(gy - fy, gx - fx) /** 180 / MathHelper::Pi*/;

	//角度差を角速度に加算
	auto sub = r - rot;
	sub -= std::floor(sub / 360.0f) * 360.0f;
	if (sub < -180.0f) sub += 360.0f;
	if (sub > 180.0f) sub -= 360.0f;
	rot_spd += sub;

	//摩擦
	rot_spd *= 0.995f;

	//角度に角速度を加算
	rot += rot_spd;

	//新しい重りの位置
	//rad = rot * MathHelper::Pi / 180;
	//px = fx + MathHelper::Cos(rad) * length;
	//py = fy + MathHelper::Sin(rad) * length;
	px = fx + MathHelper::Cos(rot) * length;
	py = fy + MathHelper::Sin(rot) * length;

	//重りの座標
	spherePos.x = px;
	spherePos.y = py;
	rot2 = rot - 90.0f;
	linePos_1.x = spherePos.x + MathHelper::Cos(lineRot + rot2) * 32.0f;
	linePos_1.y = spherePos.y + MathHelper::Sin(lineRot + rot2) * 32.0f;
	linePos_2.x = linePos_1.x + MathHelper::Cos(lineRot + rot2) * len;
	linePos_2.y = linePos_1.y + MathHelper::Sin(lineRot + rot2) * len;

	
}

void EndingScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "EndingScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);
	DrawFormatString(0, 40, GetColor(255, 255, 255), "座標 %f %f", arrowPos.x, arrowPos.y);
	DrawFormatString(0, 60, GetColor(255, 255, 255), "X:%f Y:%f", absH, absV);
	DrawFormatString(0, 80, GetColor(255, 255, 255), "rad:%f", rad);
	DrawFormatString(0, 100, GetColor(255, 255, 255), "rot:%f", rot);


	// 描画
	world_->Draw();

	DrawCapsule3D(VGet(0.0f, 0.0f, 0.0f), VGet(0.0f, 0.0f, 0.0f), size, 4, GetColor(255, 0, 0), GetColor(0, 0, 255), false);

	DrawLine3D(VGet(10.0f, 0.0f, 0.0f), VGet(-10.0f, 0.0f, 0.0f), GetColor(0, 255, 0));

	DrawTriangle3D(VGet(-5.0f, 5.0f, 0.0f), VGet(5.0f, 5.0f, 0.0f), VGet(0.0f, -5.0f, 0.0f), GetColor(0, 0, 255), false);

	DrawSphere3D(VGet(5.0f, 0.0f, 0.0f), size, 4, GetColor(255, 0, 0), GetColor(0, 0, 255), false);

	DrawCone3D(VGet(-5.0f, 5.0f, 0.0f), VGet(-5.0f, -2.0f, 0.0f), 2.0f, 4, GetColor(0, 255, 0), GetColor(255, 255, 255), false);

	Sprite::GetInstance().Draw(SPRITE_ID::TEST2_SPRITE, Vector2(0, 400));
	Sprite::GetInstance().Draw(SPRITE_ID::TEST_SPRITE, particlePos, alpha);
	Sprite::GetInstance().Draw(SPRITE_ID::CIRCLE_SPRITE, circlePos, cAlpha);

	Sprite::GetInstance().Draw(SPRITE_ID::ARROW_SPRITE, arrowPos, 1.0f,turn);
	Sprite::GetInstance().Draw(SPRITE_ID::HITO_SPRITE, spherePos,Vector2(16,32),Vector2::One,rot2);
	DrawCircle(spherePos.x, spherePos.y,32,GetColor(255,255,255),0,1);
	DrawLine(linePos_1.x, linePos_1.y, linePos_2.x, linePos_2.y, GetColor(0, 255, 0), 1);

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