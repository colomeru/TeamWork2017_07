#include "EndingScene.h"
#include "../time/Time.h"
#include "../camera/Camera.h"
#include "../conv/DXConverter.h"
#include "../graphic/Sprite.h"
#include "../input/Keyboard.h"
#include <cmath>
#include <math.h>
#include "../math/MathHelper.h"
#include "../stageGenerator/Stage1/Stage1.h"
#include "../input//GamePad.h"
#include "../game//Random.h"


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
	rotDirection = true;
	r1 = rot;
	r2 = 0.0f;
	v1 = Vector2::Zero;
	v2 = Vector2::Zero;
	f = 1.0f;

	arrowSize = Sprite::GetInstance().GetSize(SPRITE_ID::ARROW_SPRITE);
	particleSize = Sprite::GetInstance().GetSize(SPRITE_ID::TEST_SPRITE);
	circleSize = Sprite::GetInstance().GetSize(SPRITE_ID::CIRCLE_SPRITE);

	stageManager.Add(Stage::Stage1, std::make_shared<Stage1>(world_.get(), std::string("Stage1")));
	stageManager.SetStage(Stage::Stage1);

	//振り子
	for (int i = 0; i < 8; i++)
	{
		neckLen[i] = Random::GetInstance().Range(360.0f, 362.0f);
	}

	fx = 200.0f;
	fy = 100.0f;
	rot = 0.0f;
	rot_spd = 0.0f;
	r = 32.0f;
	vec = 0;
	length = neckLen[vec];
	g = 0.5f;
	friction = 0.995f;
	vec = 0;
	lineRot[0] = -90.0f;
	stageLen = stageManager.GetStageSize(Stage::Stage1).x;
	meterLen = 800.0f;
	meterPos = Vector2(200.0f, 100.0f);

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

	//四角の当たり判定
	centerPos = Vector2(particlePos.x + particleSize.x / 2, particlePos.y + particleSize.y / 2);

	absH = abs(centerPos.x - arrowPos.x);
	absV = abs(centerPos.y - arrowPos.y);

	distanceH = particleSize.x / 2;
	distanceV = particleSize.y / 2;

	if (absH < distanceH && absV < distanceV) alpha = 0.5f;
	else alpha = 1.0f;

	//円の当たり判定
	cCenterPos = Vector2(circlePos.x + circleSize.x / 2, circlePos.y + circleSize.y / 2);
	absC = cCenterPos - arrowPos;
	distanceC = sqrt((absC.x * absC.x) + (absC.y * absC.y));
	if (distanceC < circleSize.x / 2) cAlpha = 0.5f;
	else cAlpha = 1.0f;


	Pendulum();
	
}

void EndingScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "EndingScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);
	DrawFormatString(0, 40, GetColor(255, 255, 255), "座標 %f %f", arrowPos.x, arrowPos.y);
	DrawFormatString(0, 60, GetColor(255, 255, 255), "X:%f Y:%f", absH, absV);
	DrawFormatString(0, 100, GetColor(255, 255, 255), "rot:%f", rot);
	DrawFormatString(0, 120, GetColor(255, 255, 255), "rot_spd:%f", rot_spd);
	DrawFormatString(0, 140, GetColor(255, 255, 255), "friction:%f", friction);
	DrawFormatString(0, 180, GetColor(255, 255, 255), "vec:%d", vec);
	DrawFormatString(0, 200, GetColor(255, 255, 255), "spherePos x:%f y:%f", spherePos.x, spherePos.y);
	for (int i = 0; i < 8; i++)
	{
		DrawFormatString(0, 220 + 20 * i, GetColor(255, 255, 255), "neckLen%d:%f", i, neckLen[i]);
	}
	DrawFormatString(0, 380, GetColor(255, 255, 255), "r:%f", r);
	DrawFormatString(0, 400, GetColor(255, 255, 255), "limit:%f", spdLimit);
	if (rotDirection && rot < 90) DrawFormatString(0, 420, GetColor(255, 255, 255), "左に加速できます！");
	else if (!rotDirection && rot > 90) DrawFormatString(0, 420, GetColor(255, 255, 255), "右に加速できます！");
	else DrawFormatString(0, 420, GetColor(255, 255, 255), "加速できません！");


	// 描画
	world_->Draw();

	//DrawCapsule3D(VGet(0.0f, 0.0f, 0.0f), VGet(0.0f, 0.0f, 0.0f), size, 4, GetColor(255, 0, 0), GetColor(0, 0, 255), false);

	//DrawLine3D(VGet(10.0f, 0.0f, 0.0f), VGet(-10.0f, 0.0f, 0.0f), GetColor(0, 255, 0));

	//DrawTriangle3D(VGet(-5.0f, 5.0f, 0.0f), VGet(5.0f, 5.0f, 0.0f), VGet(0.0f, -5.0f, 0.0f), GetColor(0, 0, 255), false);

	//DrawSphere3D(VGet(5.0f, 0.0f, 0.0f), size, 4, GetColor(255, 0, 0), GetColor(0, 0, 255), false);

	//DrawCone3D(VGet(-5.0f, 5.0f, 0.0f), VGet(-5.0f, -2.0f, 0.0f), 2.0f, 4, GetColor(0, 255, 0), GetColor(255, 255, 255), false);

	//Sprite::GetInstance().Draw(SPRITE_ID::TEST2_SPRITE, Vector2(0, 400));
	//Sprite::GetInstance().Draw(SPRITE_ID::TEST_SPRITE, particlePos, alpha);
	//Sprite::GetInstance().Draw(SPRITE_ID::CIRCLE_SPRITE, circlePos, cAlpha);

	//Sprite::GetInstance().Draw(SPRITE_ID::ARROW_SPRITE, arrowPos, aAlpha);
	Sprite::GetInstance().Draw(SPRITE_ID::HITO_SPRITE, spherePos, Vector2(16, 32), Vector2::One, rot2 + lineRot[2]);
	DrawCircle(spherePos.x, spherePos.y, (int)r, GetColor(255, 255, 255), 0, 1);
	DrawCircle(fx, fy, 16, GetColor(255, 0, 0), 0, 1); //支点に円を表示
	DrawLine(inPos[0].x, inPos[0].y, outPos[0].x, outPos[0].y, GetColor(0, 255, 0), 1); //緑
	DrawLine(inPos[1].x, inPos[1].y, outPos[1].x, outPos[1].y, GetColor(255, 0, 0), 1); //赤
	DrawLine(inPos[2].x, inPos[2].y, outPos[2].x, outPos[2].y, GetColor(0, 0, 255), 1); //青
	DrawLine(inPos[3].x, inPos[3].y, outPos[3].x, outPos[3].y, GetColor(255, 255, 0), 1); //黄色
	DrawLine(inPos[4].x, inPos[4].y, outPos[4].x, outPos[4].y, GetColor(255, 0, 255), 1); //紫
	DrawLine(inPos[5].x, inPos[5].y, outPos[5].x, outPos[5].y, GetColor(0, 255, 255), 1); //水色
	DrawLine(inPos[6].x, inPos[6].y, outPos[6].x, outPos[6].y, GetColor(255, 165, 0), 1); //オレンジ
	DrawLine(inPos[7].x, inPos[7].y, outPos[7].x, outPos[7].y, GetColor(255, 255, 255), 1); //白

	DrawBox(meterPos.x, meterPos.y, meterPos.x + meterLen, meterPos.y + 20, GetColor(0, 255, 0), 1);
	Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, Vector2(spherePos.x * meterLen / stageLen + meterPos.x, meterPos.y), Vector2(32.0f, 32.0f), Vector2::One, 1.0f, turn);

	DrawLine(spherePos.x, spherePos.y, v1.x, v1.y, GetColor(0, 255, 0), 1);
	DrawLine(fx, fy, v2.x, v2.y, GetColor(0, 255, 0), 1);
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

void EndingScene::Pendulum()
{
	//Aボタンで加速方向に跳ぶ
	float a;
	if (rotDirection) a = rot + 90.0f;
	else a = rot - 90.0f;
	v1 = Vector2(spherePos.x + MathHelper::Cos(a) * ((neckLen[vec] - r) * MathHelper::Abs(rot_spd)) / 2.0f, spherePos.y + MathHelper::Sin(a) * ((neckLen[vec] - r) * MathHelper::Abs(rot_spd)) / 2.0f);
	v2 = Vector2(fx + MathHelper::Cos(a) * ((neckLen[vec] - r) * MathHelper::Abs(rot_spd)) / 2.0f, fy + MathHelper::Sin(a) * ((neckLen[vec] - r) * MathHelper::Abs(rot_spd)) / 2.0f);
	if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM1))
	{
		spherePos += Vector2((v1.x - spherePos.x) * 0.1f, (v1.y - spherePos.y) * 0.1f + f);
		fx += (v2 - Vector2(fx, fy)).x * 0.1f;
		fy += (v2 - Vector2(fx, fy)).y * 0.1f + f;
		f += 0.5f;
		if (v1.y > 0 || v2.y > 0) rot_spd = -rot_spd;
	}
	if (GamePad::GetInstance().ButtonTriggerUp(PADBUTTON::NUM1)) f = 1.0f;
	if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM1)) return;

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
	auto rDiff = MathHelper::ATan(gy - fy, gx - fx);

	//角度差を角速度に加算
	auto sub = rDiff - rot;
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

	//角度調整
	rot2 = rot - 90.0f;

	//首の内側と外側の座標を求め、外側の座標を支点に設定する
	for (int i = 0; i < 8; i++)
	{
		inPos[i].x = spherePos.x + MathHelper::Cos(lineRot[i] + rot2);
		inPos[i].y = spherePos.y + MathHelper::Sin(lineRot[i] + rot2);
		outPos[i].x = inPos[i].x + MathHelper::Cos(lineRot[i] + rot2) * neckLen[i];
		outPos[i].y = inPos[i].y + MathHelper::Sin(lineRot[i] + rot2) * neckLen[i];
		fulcrum[i] = outPos[i];
	}

	//1フレーム前のrotと比較
	r2 = rot;
	if (r1 < r2) //右に回っていれば
	{
		rotDirection = true;
		r1 = rot;
	}
	else if (r1 > r2) //左に回っていれば
	{
		rotDirection = false;
		r1 = rot;
	}

	//circleの当たり判定
	Vector2 absA = spherePos - arrowPos;
	float distanceS = sqrt((absA.x * absA.x) + (absA.y * absA.y));
	if (distanceS < 32.0f ||
		(rot_spd < 0 && !rotDirection && (Keyboard::GetInstance().KeyStateDown(KEYCODE::RIGHT) || GamePad::GetInstance().ButtonStateDown(PADBUTTON::RIGHT))) ||
		(rot_spd > 0 && rotDirection && (Keyboard::GetInstance().KeyStateDown(KEYCODE::LEFT) || GamePad::GetInstance().ButtonStateDown(PADBUTTON::LEFT))))
	{
		aAlpha = 0.5f; //circleに当たっていれば半透明
		friction = 1.015f; //摩擦を減らす
	}
	else
	{
		aAlpha = 1.0f; //circleに当たっていなければ不透明
		friction = 0.985f; //摩擦を戻す
	}

	//スピード制限
	//spdLimit = sqrt(2 * g * (MathHelper::Sin(90.0f) * neckLen[vec])) / (neckLen[vec] * 0.02f);
	//rot_spd = 3.0f;
	rot_spd = MathHelper::Clamp(rot_spd, -spdLimit, spdLimit);
	//if (rot_spd > 4.0f) rot_spd = 2.0f;
	//if (rot_spd < -4.0f) rot_spd = -2.0f;

	//支点を移動
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::RSHIFT) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2))
	{
		vec += 1;
		if (vec > 7) vec = 0;
		length = neckLen[vec];
		fx = fulcrum[vec].x;
		fy = fulcrum[vec].y;
		lineRot[0] -= 45.0f;
		rot += 45.0f;
		turn = false;
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::LSHIFT) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM3))
	{
		vec -= 1;
		if (vec < 0) vec = 7;
		length = neckLen[vec];
		fx = fulcrum[vec].x;
		fy = fulcrum[vec].y;
		lineRot[0] += 45.0f;
		rot -= 45.0f;
		turn = true;
	}

	//45度ずつ8方向に首を伸ばす
	for (int i = 1; i < 8; i++)
	{
		lineRot[i] = lineRot[i - 1] + 45.0f; //1本目の首に45度ずつ足していく
	}

	//頂点で正規化
	if (rot > 270.0f) rot = -90.0f;
	if (rot < -90) rot = 270.0f;


	//LB/RBで左右どちらかの首を伸ばし、もう片方の首を縮ませる
	if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM6))
	{
		if (vec == 7)
		{
			if (neckLen[vec - 1] <= r + 20.0f) return; //これ以上短くならない
			neckLen[0] += 5.0f;
			neckLen[vec - 1] -= 5.0f;
		}
		else
		{
			if ((vec != 0 && neckLen[vec - 1] <= r + 20.0f) || (vec == 0 && neckLen[7] <= r + 20.0f)) return;
			neckLen[vec + 1] += 5.0f;
			if (vec == 0) neckLen[7] -= 5.0f;
			else neckLen[vec - 1] -= 5.0f;
		}
	}
	else if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM5))
	{
		if (vec == 0)
		{
			if (neckLen[vec + 1] <= r + 20.0f) return;
			neckLen[7] += 5.0f;
			neckLen[vec + 1] -= 5.0f;
		}
		else
		{
			if ((vec != 7 && neckLen[vec + 1] <= r + 20.0f) || (vec == 7 && neckLen[0] <= r + 20.0f)) return;
			neckLen[vec - 1] += 5.0f;
			if (vec == 7) neckLen[0] -= 5.0f;
			else neckLen[vec + 1] -= 5.0f;
		}
	}
}