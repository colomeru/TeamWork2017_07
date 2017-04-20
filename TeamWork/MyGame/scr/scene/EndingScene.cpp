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
	aAlpha = 1.0f;
	particlePos = Vector2(200.0f, 200.0f);
	circlePos = Vector2(400.0f, 200.0f);
	turn = false;
	x1 = 0.0f;
	x2 = 0.0f;

	arrowSize = Sprite::GetInstance().GetSize(SPRITE_ID::ARROW_SPRITE);
	particleSize = Sprite::GetInstance().GetSize(SPRITE_ID::TEST_SPRITE);
	circleSize = Sprite::GetInstance().GetSize(SPRITE_ID::CIRCLE_SPRITE);

	//振り子
	fx = 200.0f;
	fy = 100.0f;
	rot = 0.0f;
	rot_spd = 0.0f;
	length = 150.0f;
	g = 0.5f;
	friction = 0.995f;
	vec = 7;
	len = 118.0f;
	line1_Rot = -45.0f;
	
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

	//四角の当たり判定
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

	//円の当たり判定
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




	//振り子
	//現在の重りの位置
	auto px = fx + MathHelper::Cos(rot) * length;
	auto py = fy + MathHelper::Sin(rot) * length;

	//重力移動量を反映した重りの位置
	auto vx = px - fx;
	auto vy = py - fy;
	auto t = -(vy * g) / (vx * vx + vy * vy);
	auto gx = px + t * vx;
	auto gy = py + g + t * vy;

	//２つの重りの位置の確度差
	auto r = MathHelper::ATan(gy - fy, gx - fx);

	//角度差を角速度に加算
	auto sub = r - rot;
	sub -= std::floor(sub / 360.0f) * 360.0f;
	if (sub < -180.0f) sub += 360.0f;
	if (sub > 180.0f) sub -= 360.0f;
	rot_spd += sub;

	//摩擦
	rot_spd *= friction;

	//角度に角速度を加算
	rot += rot_spd;

	//新しい重りの位置
	px = fx + MathHelper::Cos(rot) * length;
	py = fy + MathHelper::Sin(rot) * length;

	//重りの座標
	spherePos.x = px;
	spherePos.y = py;

	//
	rot2 = rot - 90.0f;

	//line1
	line1_Pos_In.x = spherePos.x + MathHelper::Cos(line1_Rot + rot2) * 32.0f;
	line1_Pos_In.y = spherePos.y + MathHelper::Sin(line1_Rot + rot2) * 32.0f;
	line1_Pos_Out.x = line1_Pos_In.x + MathHelper::Cos(line1_Rot + rot2) * len;
	line1_Pos_Out.y = line1_Pos_In.y + MathHelper::Sin(line1_Rot + rot2) * len;
	//line2
	line2_Pos_In.x = spherePos.x + MathHelper::Cos(line2_Rot + rot2) * 32.0f;
	line2_Pos_In.y = spherePos.y + MathHelper::Sin(line2_Rot + rot2) * 32.0f;
	line2_Pos_Out.x = line2_Pos_In.x + MathHelper::Cos(line2_Rot + rot2) * len;
	line2_Pos_Out.y = line2_Pos_In.y + MathHelper::Sin(line2_Rot + rot2) * len;
	//line3
	line3_Pos_In.x = spherePos.x + MathHelper::Cos(line3_Rot + rot2) * 32.0f;
	line3_Pos_In.y = spherePos.y + MathHelper::Sin(line3_Rot + rot2) * 32.0f;
	line3_Pos_Out.x = line3_Pos_In.x + MathHelper::Cos(line3_Rot + rot2) * len;
	line3_Pos_Out.y = line3_Pos_In.y + MathHelper::Sin(line3_Rot + rot2) * len;
	//line4
	line4_Pos_In.x = spherePos.x + MathHelper::Cos(line4_Rot + rot2) * 32.0f;
	line4_Pos_In.y = spherePos.y + MathHelper::Sin(line4_Rot + rot2) * 32.0f;
	line4_Pos_Out.x = line4_Pos_In.x + MathHelper::Cos(line4_Rot + rot2) * len;
	line4_Pos_Out.y = line4_Pos_In.y + MathHelper::Sin(line4_Rot + rot2) * len;
	//line5
	line5_Pos_In.x = spherePos.x + MathHelper::Cos(line5_Rot + rot2) * 32.0f;
	line5_Pos_In.y = spherePos.y + MathHelper::Sin(line5_Rot + rot2) * 32.0f;
	line5_Pos_Out.x = line5_Pos_In.x + MathHelper::Cos(line5_Rot + rot2) * len;
	line5_Pos_Out.y = line5_Pos_In.y + MathHelper::Sin(line5_Rot + rot2) * len;
	//line6
	line6_Pos_In.x = spherePos.x + MathHelper::Cos(line6_Rot + rot2) * 32.0f;
	line6_Pos_In.y = spherePos.y + MathHelper::Sin(line6_Rot + rot2) * 32.0f;
	line6_Pos_Out.x = line6_Pos_In.x + MathHelper::Cos(line6_Rot + rot2) * len;
	line6_Pos_Out.y = line6_Pos_In.y + MathHelper::Sin(line6_Rot + rot2) * len;
	//line7
	line7_Pos_In.x = spherePos.x + MathHelper::Cos(line7_Rot + rot2) * 32.0f;
	line7_Pos_In.y = spherePos.y + MathHelper::Sin(line7_Rot + rot2) * 32.0f;
	line7_Pos_Out.x = line7_Pos_In.x + MathHelper::Cos(line7_Rot + rot2) * len;
	line7_Pos_Out.y = line7_Pos_In.y + MathHelper::Sin(line7_Rot + rot2) * len;
	//line8
	line8_Pos_In.x = spherePos.x + MathHelper::Cos(line8_Rot + rot2) * 32.0f;
	line8_Pos_In.y = spherePos.y + MathHelper::Sin(line8_Rot + rot2) * 32.0f;
	line8_Pos_Out.x = line8_Pos_In.x + MathHelper::Cos(line8_Rot + rot2) * len;
	line8_Pos_Out.y = line8_Pos_In.y + MathHelper::Sin(line8_Rot + rot2) * len;

	fulcrum = { line1_Pos_Out,
		line2_Pos_Out ,
		line3_Pos_Out ,
		line4_Pos_Out ,
		line5_Pos_Out ,
		line6_Pos_Out ,
		line7_Pos_Out ,
		line8_Pos_Out
	};

	//circleの当たり判定
	Vector2 absA = spherePos - arrowPos;
	float distanceS = sqrt((absA.x * absA.x) + (absA.y * absA.y));
	if (distanceS < 32.0f ||
		rot_spd < 0 && Keyboard::GetInstance().KeyStateDown(KEYCODE::RIGHT) ||
		rot_spd > 0 && Keyboard::GetInstance().KeyStateDown(KEYCODE::LEFT))
	{
		aAlpha = 0.5f; //circleに当たっていれば半透明
		friction = 1.02f; //摩擦を減らす
	}
	else
	{
		aAlpha = 1.0f; //circleに当たっていなければ不透明
		friction = 0.98f;
	}
	//スピード制限
	if (rot_spd > 4.0f) rot_spd = 4.0f;
	if (rot_spd < -4.0f) rot_spd = -4.0f;

	//支点を移動
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::RSHIFT))
	{
		vec += 1;
		if (vec > 7) vec = 0;
		fx = fulcrum[vec].x;
		fy = fulcrum[vec].y;
		line1_Rot -= 45.0f;
		rot += 45.0f;
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::LSHIFT))
	{
		vec -= 1;
		if (vec < 0) vec = 7;
		fx = fulcrum[vec].x;
		fy = fulcrum[vec].y;
		line1_Rot += 45.0f;
		rot -= 45.0f;
	}
	//line1_Rot = -45.0f;
	line2_Rot = line1_Rot + 45.0f;
	line3_Rot = line2_Rot + 45.0f;
	line4_Rot = line3_Rot + 45.0f;
	line5_Rot = line4_Rot + 45.0f;
	line6_Rot = line5_Rot + 45.0f;
	line7_Rot = line6_Rot + 45.0f;
	line8_Rot = line7_Rot + 45.0f;

}

void EndingScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "EndingScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);
	DrawFormatString(0, 40, GetColor(255, 255, 255), "座標 %f %f", arrowPos.x, arrowPos.y);
	DrawFormatString(0, 60, GetColor(255, 255, 255), "X:%f Y:%f", absH, absV);
	DrawFormatString(0, 80, GetColor(255, 255, 255), "rad:%f", rad);
	DrawFormatString(0, 100, GetColor(255, 255, 255), "rot:%f", rot);
	DrawFormatString(0, 120, GetColor(255, 255, 255), "rot_spd:%f", rot_spd);
	DrawFormatString(0, 140, GetColor(255, 255, 255), "friction:%f", friction);
	DrawFormatString(0, 160, GetColor(255, 255, 255), "line1_Rot:%f", line1_Rot);
	DrawFormatString(0, 180, GetColor(255, 255, 255), "vec:%d", vec);


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

	Sprite::GetInstance().Draw(SPRITE_ID::ARROW_SPRITE, arrowPos, aAlpha);
	Sprite::GetInstance().Draw(SPRITE_ID::HITO_SPRITE, spherePos, Vector2(16, 32), Vector2::One, rot2 + line2_Rot);
	DrawCircle(spherePos.x, spherePos.y, 32, GetColor(255, 255, 255), 0, 1);
	DrawCircle(fx, fy, 16, GetColor(255, 0, 0), 0, 1); //支点
	DrawLine(line1_Pos_In.x, line1_Pos_In.y, line1_Pos_Out.x, line1_Pos_Out.y, GetColor(0, 255, 0), 1);
	DrawLine(line2_Pos_In.x, line2_Pos_In.y, line2_Pos_Out.x, line2_Pos_Out.y, GetColor(255, 0, 0), 1);
	DrawLine(line3_Pos_In.x, line3_Pos_In.y, line3_Pos_Out.x, line3_Pos_Out.y, GetColor(0, 0, 255), 1);
	DrawLine(line4_Pos_In.x, line4_Pos_In.y, line4_Pos_Out.x, line4_Pos_Out.y, GetColor(255, 255, 0), 1);
	DrawLine(line5_Pos_In.x, line5_Pos_In.y, line5_Pos_Out.x, line5_Pos_Out.y, GetColor(255, 0, 255), 1);
	DrawLine(line6_Pos_In.x, line6_Pos_In.y, line6_Pos_Out.x, line6_Pos_Out.y, GetColor(0, 255, 255), 1);
	DrawLine(line7_Pos_In.x, line7_Pos_In.y, line7_Pos_Out.x, line7_Pos_Out.y, GetColor(255, 165, 0), 1);
	DrawLine(line8_Pos_In.x, line8_Pos_In.y, line8_Pos_Out.x, line8_Pos_Out.y, GetColor(255, 255, 255), 1);


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