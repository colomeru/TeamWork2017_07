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
#include "../scene/MenuScene.h"
#include "../scene/addScreen/MenuScreen.h"
#include "../Def.h"

const int resWidth = 30;
const int correctionHeight = 30;
const float correctionWidth = 1.8f;

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

	//Camera::GetInstance().SetRange(0.1f, 10000.0f);
	//Camera::GetInstance().SetViewAngle(60.0f);
	//Camera::GetInstance().Up.Set(Vector3::Up);
	//Camera::GetInstance().Position.Set(Vector3(0.0f, 0.0f, -10.0f));
	//Camera::GetInstance().Target.Set(Vector3(0.0f, 0.0f, 0.0f));
	//Camera::GetInstance().Update();

	//size = 1.0f;
	//horizontal = 0.0f;
	//vertical = 0.0f;
	//alpha = 1.0f;
	//cAlpha = 1.0f;
	//aAlpha = 1.0f;
	//particlePos = Vector2(200.0f, 200.0f);
	//circlePos = Vector2(400.0f, 200.0f);
	turn = false;
	rotDirection = true;
	//r1 = rot;
	r2 = 0.0f;
	v1 = Vector2::Zero;
	v2 = Vector2::Zero;
	f = 1.0f;

	//arrowSize = Sprite::GetInstance().GetSize(SPRITE_ID::ARROW_SPRITE);
	//particleSize = Sprite::GetInstance().GetSize(SPRITE_ID::TEST_SPRITE);
	//circleSize = Sprite::GetInstance().GetSize(SPRITE_ID::CIRCLE_SPRITE);



	//振り子
	for (int i = 0; i < 8; i++)
	{
		neckLen[i] = Random::GetInstance().Range(100.0f, 102.0f);
	}

	//fx = 200.0f;
	//fy = 100.0f;
	//rot = 90.0f;
	//rot_spd = 0.0f;
	r = 32.0f;
	vec = 0;
	length = neckLen[vec];
	g = 0.5f;
	friction = 0.995f;
	vec = 0;
	lineRot[0] = -90.0f;
	meterLen = 800.0f;
	meterPos = Vector2(200.0f, 100.0f);

	dRot = 90.0f;
	dRot_spd = 0.0f;

	oneLength = 100.0f;
	fNum = 7;
	//mRot.clear();
	//mRot_spd.clear();
	//mLimit.clear();
	//fPos.clear();
	//multiplePos.clear();

	//fPos.push_back(Vector2(800.0f, 300.0f));
	//mRot.push_back(135.0f);
	//mLimit.push_back(0.0f);
	//mRot_spd.push_back(0.0f);
	//multiplePos.push_back(Vector2(0.0f, 0.0f));
	neckLengh = 736.0f;

	//int s = neckLengh / oneLength + 1;
	//correctionLens.clear();
	//for (int i = 0; i < s; i++) {
	//	correctionLens.push_back(1.0f);
	//}
	//if (neckLengh - oneLength > 0) {
	//	float h = MathHelper::Mod(neckLengh - oneLength, oneLength) / oneLength;
	//	correctionLens.push_back(h);
	//}
	//fNum = s;

	//for (int i = 0; i < fNum - 1; i++)
	//{
	//	auto px = fPos[i].x + MathHelper::Cos(mRot[i]) * 100;
	//	auto py = fPos[i].y + MathHelper::Sin(mRot[i]) * 100;

	//	mRot.push_back(135.0f);
	//	mRot_spd.push_back(0.0f);
	//	mLimit.push_back(0.0f);
	//	fPos.push_back(Vector2(px, py));
	//	multiplePos.push_back(Vector2(0.0f, 0.0f));
	//}
	MultipleInit(neckLengh, Vector2(800.0f, 300.0f), 0.0f,150.0f);
	Multiple();


	//変形描画
	//gHandle = LoadGraph("res//Sprite//head.bmp");
	hNeck = Sprite::GetInstance().GetHandle(SPRITE_ID::OROCHI_NECK_SPRITE);
	hHead = Sprite::GetInstance().GetHandle(SPRITE_ID::OROCHI_HEAD_SPRITE);

	gPos1[0] = Vector2(800, 400);
	gPos1[1] = Vector2(gPos1[0].x + 50, gPos1[0].y);
	gPos1[2] = Vector2(gPos1[0].x + 50, gPos1[0].y + 100);
	gPos1[3] = Vector2(gPos1[0].x, gPos1[0].y + 100);

	gPos2[0] = Vector2(gPos1[3].x, gPos1[3].y);
	gPos2[1] = Vector2(gPos1[2].x, gPos1[2].y);
	gPos2[2] = Vector2(gPos2[0].x + 50, gPos2[0].y + 100);
	gPos2[3] = Vector2(gPos2[0].x, gPos2[0].y + 100);

	posNum = 0;
	//for (int i = 0; i < fNum; i++) {
	//	for (int j = 0; j < 4; j++) {
	//		if (j < 2) {
	//			pos[i][j].x = fPos[i].x + MathHelper::Cos(mRot[i]) * 25;
	//			pos[i][j].y = fPos[i].y + MathHelper::Sin(mRot[i]) * 25;
	//		}
	//		else {
	//			pos[i][j].x = multiplePos[i].x + MathHelper::Cos(mRot[i]) * 25;
	//			pos[i][j].y = multiplePos[i].y + MathHelper::Sin(mRot[i]) * 25;
	//		}
	//	}
	//}
	DeformationDraw();

	player_ = std::make_shared<Player>(world_.get());
	world_->Add(ACTOR_ID::PLAYER_ACTOR, player_);
	world_->PushStackActor(player_);


	world_->InitializeInv(Vector2(player_->GetPosition().x, player_->GetPosition().y));
	world_->SetTarget(player_.get());

	stageManager.Add(Stage::Stage1, std::make_shared<Stage1>(world_.get(), std::string("Test0"), 60));
	stageManager.SetStage(Stage::Stage1);
	stageLen = stageManager.GetStageSize(Stage::Stage1).x;

	meter_ = ProgressMeter(world_.get(), stageLen);
	meter_.Initialize();
}

void EndingScene::Update()
{
	// 更新
	world_->Update();

	// 終了
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;
	//Camera::GetInstance().Position.Set(Vector3(0.0f, 0.0f, -10.0f));
	//Camera::GetInstance().Target.Set(Vector3(0.0f, 0.0f, 0.0f));
	//Camera::GetInstance().Update();

	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::LSHIFT) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM4))
	{
		//MultipleInit(neckLengh, Vector2(800.0f, 300.0f), 0.0f);
	}
	MultipleInit(neckLengh, Vector2(800.0f, 300.0f), 0.0f,150.0f);


	//四角の当たり判定
	//centerPos = Vector2(particlePos.x + particleSize.x / 2, particlePos.y + particleSize.y / 2);

	//absH = abs(centerPos.x - arrowPos.x);
	//absV = abs(centerPos.y - arrowPos.y);

	//distanceH = particleSize.x / 2;
	//distanceV = particleSize.y / 2;

	//if (absH < distanceH && absV < distanceV) alpha = 0.5f;
	//else alpha = 1.0f;

	////円の当たり判定
	//cCenterPos = Vector2(circlePos.x + circleSize.x / 2, circlePos.y + circleSize.y / 2);
	//absC = cCenterPos - arrowPos;
	//distanceC = sqrt((absC.x * absC.x) + (absC.y * absC.y));
	//if (distanceC < circleSize.x / 2) cAlpha = 0.5f;
	//else cAlpha = 1.0f;


	//Pendulum();

	//Double(spherePos);

	Multiple();

	DeformationDraw();

	meter_.Update();
}

void EndingScene::Draw() const
{
	if (BuildMode == 1) {
		DrawFormatString(0, 00, GetColor(255, 255, 255), "EndingScene");
		DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);
		//DrawFormatString(0, 40, GetColor(255, 255, 255), "座標 %f %f", arrowPos.x, arrowPos.y);
		//DrawFormatString(0, 60, GetColor(255, 255, 255), "X:%f Y:%f", absH, absV);
		//DrawFormatString(0, 100, GetColor(255, 255, 255), "rot:%f", rot);
		//DrawFormatString(0, 120, GetColor(255, 255, 255), "rot_spd:%f", rot_spd);
		DrawFormatString(0, 140, GetColor(255, 255, 255), "friction:%f", friction);
		DrawFormatString(0, 180, GetColor(255, 255, 255), "vec:%d", vec);
		DrawFormatString(0, 200, GetColor(255, 255, 255), "spherePos x:%f y:%f", spherePos.x, spherePos.y);
		for (int i = 0; i < 8; i++)
		{
			DrawFormatString(0, 220 + 20 * i, GetColor(255, 255, 255), "neckLen%d:%f", i, neckLen[i]);
		}
		DrawFormatString(0, 380, GetColor(255, 255, 255), "r:%f", r);
		DrawFormatString(0, 400, GetColor(255, 255, 255), "limit:%f", spdLimit);
		//if (rotDirection && rot < 90) DrawFormatString(0, 420, GetColor(255, 255, 255), "左に加速できます！");
		//else if (!rotDirection && rot > 90) DrawFormatString(0, 420, GetColor(255, 255, 255), "右に加速できます！");
		//else DrawFormatString(0, 420, GetColor(255, 255, 255), "加速できません！");
		DrawFormatString(0, 440, GetColor(255, 255, 255), "dRot:%f", dRot);
		DrawFormatString(0, 460, GetColor(255, 255, 255), "dRot_spd:%f", dRot_spd);
		DrawFormatString(0, 480, GetColor(255, 255, 255), "dFriction:%f", dFriction);
		DrawFormatString(0, 500, GetColor(255, 255, 255), "dSub:%f", dSub);
		DrawFormatString(0, 520, GetColor(255, 255, 255), "any:%f", any);
		DrawFormatString(0, 540, GetColor(255, 255, 255), "any1:%f", any1);
		DrawFormatString(0, 560, GetColor(255, 255, 255), "any2:%f", any2);
		for (int i = 0; i < 8; i++)
		{
			DrawFormatString(0, 580 + 20 * i, GetColor(255, 255, 255), "outPos%d:%f %f", i, outPos[i].x, outPos[i].y);
		}
		for (int i = 0; i < fPos_.size(); i++)
		{
			DrawFormatString(0, 740 + 20 * i, GetColor(255, 255, 255), "mLimit%d:%f", i, mLimit[i]);
			DrawFormatString(0, 840 + 20 * i, GetColor(255, 255, 255), "mRot_spd%d:%f", i, mRot_spd[i]);
			DrawFormatString(1500, 0 + 20 * i, GetColor(255, 255, 255), "mRot%d:%f", i, mRot[i]);

		}

		int a = drawPoints.size();
		DrawFormatString(1500, 160, GetColor(255, 255, 255), "trial:%d", a);
		DrawFormatString(1500, 180, GetColor(255, 255, 255), "neckLength:%f", neckLengh);
		DrawFormatString(1500, 200, GetColor(255, 255, 255), "mRot_size:%d", mRot.size());
		DrawFormatString(1500, 220, GetColor(255, 255, 255), "fNum:%d", fNum);
		DrawFormatString(1500, 240, GetColor(255, 255, 255), "drawPoints_size:%d", drawPoints.size());

	}




	//baseClothes
	//DrawModiGraph(gPos1[0].x, gPos1[0].y, gPos1[1].x, gPos1[1].y, gPos1[2].x, gPos1[2].y, gPos1[3].x, gPos1[3].y, gHandle, true);
	//DrawModiGraph(gPos2[0].x, gPos2[0].y, gPos2[1].x, gPos2[1].y, gPos2[2].x, gPos2[2].y, gPos2[3].x, gPos2[3].y, gHandle, true);


	DrawCircle(gPos1[posNum].x, gPos1[posNum].y, 16, GetColor(255, 0, 0), 0, 1); //支点に円を表示
	//DrawGraph(0, 0, gHandle, true);



	//Sprite::GetInstance().Draw(SPRITE_ID::TEST2_SPRITE, Vector2(0, 400));
	//Sprite::GetInstance().Draw(SPRITE_ID::TEST_SPRITE, particlePos, alpha);
	//Sprite::GetInstance().Draw(SPRITE_ID::CIRCLE_SPRITE, circlePos, cAlpha);

	//Sprite::GetInstance().Draw(SPRITE_ID::ARROW_SPRITE, arrowPos, aAlpha);
	//Sprite::GetInstance().Draw(SPRITE_ID::HITO_SPRITE, spherePos, Vector2(16, 32), Vector2::One, rot2 + lineRot[2]);
	//DrawCircle(spherePos.x, spherePos.y, (int)r, GetColor(255, 255, 255), 0, 1);
	//DrawCircle(fx, fy, 16, GetColor(255, 0, 0), 0, 1); //支点に円を表示
	//DrawLine(inPos[0].x, inPos[0].y, outPos[0].x, outPos[0].y, GetColor(0, 255, 0), 1); //緑
	//DrawLine(inPos[1].x, inPos[1].y, outPos[1].x, outPos[1].y, GetColor(255, 0, 0), 1); //赤
	//DrawLine(inPos[2].x, inPos[2].y, outPos[2].x, outPos[2].y, GetColor(0, 0, 255), 1); //青
	//DrawLine(inPos[3].x, inPos[3].y, outPos[3].x, outPos[3].y, GetColor(255, 255, 0), 1); //黄色
	//DrawLine(inPos[4].x, inPos[4].y, outPos[4].x, outPos[4].y, GetColor(255, 0, 255), 1); //紫
	//DrawLine(inPos[5].x, inPos[5].y, outPos[5].x, outPos[5].y, GetColor(0, 255, 255), 1); //水色
	//DrawLine(inPos[6].x, inPos[6].y, outPos[6].x, outPos[6].y, GetColor(255, 165, 0), 1); //オレンジ
	//DrawLine(inPos[7].x, inPos[7].y, outPos[7].x, outPos[7].y, GetColor(255, 255, 255), 1); //白

	//進行度メーター
	DrawBox(meterPos.x, meterPos.y, meterPos.x + meterLen, meterPos.y + 20, GetColor(0, 255, 0), 1);
	Sprite::GetInstance().Draw(SPRITE_ID::SNAKE_SPRITE, Vector2(spherePos.x * meterLen / stageLen + meterPos.x, meterPos.y), Vector2(32.0f, 32.0f), Vector2::One, 1.0f, turn);

	//DrawLine(spherePos.x, spherePos.y, v1.x, v1.y, GetColor(0, 255, 0), 1);
	//DrawLine(fx, fy, v2.x, v2.y, GetColor(0, 255, 0), 1);

	//ここからDouble
	Sprite::GetInstance().Draw(SPRITE_ID::HITO_SPRITE, doublePos, Vector2(16, 32), Vector2::One, lineRot[2] + rot2/*dRot - 90.0f*/);
	DrawCircle(doublePos.x, doublePos.y, (int)r, GetColor(255, 255, 255), 0, 1);
	DrawLine(anyPos1.x, anyPos1.y, doublePos.x, doublePos.y, GetColor(239, 117, 188), 1); //ピンク
	//DrawCircle(fx, fy, 16, GetColor(255, 0, 0), 0, 1); //支点に円を表示

	//ここからMultiple
	for (int i = 0; i < fPos_.size(); i++)
	{
		Sprite::GetInstance().Draw(SPRITE_ID::HITO_SPRITE, multiplePos[i], Vector2(16, 32), Vector2::One, mRot[i] - 90.0f);
		//Sprite::GetInstance().Draw(SPRITE_ID::LANE_SPRITE, multiplePos[i], Vector2(64, 25), Vector2::One, mRot[i]);
		//DrawModiGraph(pos[i][0].x, pos[i][0].y, pos[i][1].x, pos[i][1].y, pos[i][2].x, pos[i][2].y, pos[i][3].x, pos[i][3].y, gHandle, true);

		DrawCircle(multiplePos[i].x, multiplePos[i].y, (int)r, GetColor(255, 255, 255), 0, 1);
		DrawLine(fPos_[i].x, fPos_[i].y, multiplePos[i].x, multiplePos[i].y, GetColor(239, 117, 188), 1); //ピンク
		//DrawBox(fPos[i].x - 20.0f, fPos[i].y, multiplePos[i].x + 20.0f, multiplePos[i].y, GetColor(239, 117, 188), 1); //ピンク
	}


	int i = 1;
	//for (auto& p : drawPoints) {
	//	if (i == drawPoints.size())
	//		DrawModiGraph(p.p0.x, p.p0.y, p.p1.x, p.p1.y, p.p2.x, p.p2.y, p.p3.x, p.p3.y, hHead, true);
	//	else if (i == 1) {
	//		//DrawRectRotaGraph(p.p1.x,p.p1.y, 0, 0, 41, 76 * correctionLens.back(),1.0f,mRot[fNum], hNeck, true, false);
	//	}
	//	else
	//		DrawRectModiGraph(p.p0.x, p.p0.y, p.p1.x, p.p1.y, p.p2.x, p.p2.y, p.p3.x, p.p3.y,0,0,41,76 * correctionLens[correctionLens.size() - 1 - i],hNeck,1);

	//	i++;
	//}
	for (int i = drawPoints.size() - 1; i > 0; i--) {
		auto p = drawPoints[i];
		DrawRectModiGraph(p.p0.x, p.p0.y, p.p1.x, p.p1.y, p.p2.x, p.p2.y, p.p3.x, p.p3.y, 0, 0, 41.0f, 76.0f * correctionLens[i], hNeck, 1);
	}
	auto p = drawPoints[0];
	DrawRectModiGraph(p.p0.x, p.p0.y, p.p1.x, p.p1.y, p.p2.x, p.p2.y, p.p3.x, p.p3.y, 0, 0, 63, 91 * correctionLens[0], hHead, 1);


	DrawLine(300, 1000, 300, 1000 - neckLengh, GetColor(255, 0, 0), 1);
	for (int i = 0; i < neckLengh / 100; i++) {
		DrawCircle(300, 1000 - i * 100, 16, GetColor(0, 0, 255), 0, 1); //支点に円を表示
	}

	//41:76
	//DrawRectGraph(350, 600, 0, 0, 41, h, hNeck, true, false);

	// 描画
	world_->Draw();

	meter_.Draw();

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
{	// 初期化
	world_->Clear();
	meter_.End();
}

void EndingScene::handleMessage(EventMessage message, void * param)
{
}

//void EndingScene::Pendulum()
//{
//	//Aボタンで加速方向に跳ぶ
//	float a;
//	if (rotDirection) a = rot + 90.0f;
//	else a = rot - 90.0f;
//	v1 = Vector2(spherePos.x + MathHelper::Cos(a) * ((neckLen[vec] - r) * MathHelper::Abs(rot_spd)) / 2.0f, spherePos.y + MathHelper::Sin(a) * ((neckLen[vec] - r) * MathHelper::Abs(rot_spd)) / 2.0f);
//	v2 = Vector2(fx + MathHelper::Cos(a) * ((neckLen[vec] - r) * MathHelper::Abs(rot_spd)) / 2.0f, fy + MathHelper::Sin(a) * ((neckLen[vec] - r) * MathHelper::Abs(rot_spd)) / 2.0f);
//	if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM1))
//	{
//		spherePos += Vector2((v1.x - spherePos.x) * 0.1f, (v1.y - spherePos.y) * 0.1f + f);
//		fx += (v2 - Vector2(fx, fy)).x * 0.1f;
//		fy += (v2 - Vector2(fx, fy)).y * 0.1f + f;
//		f += 0.5f;
//		if (v1.y > 0 || v2.y > 0) rot_spd = -rot_spd;
//	}
//	if (GamePad::GetInstance().ButtonTriggerUp(PADBUTTON::NUM1)) f = 1.0f;
//	if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM1)) return;
//
//	//振り子
//	//現在の重りの位置
//	auto px = fx + MathHelper::Cos(rot) * length;
//	auto py = fy + MathHelper::Sin(rot) * length;
//
//	//重力移動量を反映した重りの位置
//	auto vx = px - fx;
//	auto vy = py - fy;
//	auto t = -(vy * g) / (vx * vx + vy * vy);
//	auto gx = px + t * vx;
//	auto gy = py + g + t * vy;
//
//	//２つの重りの位置の確度差
//	auto rDiff = MathHelper::ATan(gy - fy, gx - fx);
//
//	//角度差を角速度に加算
//	auto sub = rDiff - rot;
//	sub -= std::floor(sub / 360.0f) * 360.0f;
//	if (sub < -180.0f) sub += 360.0f;
//	if (sub > 180.0f) sub -= 360.0f;
//	rot_spd += sub;
//
//	//摩擦
//	rot_spd *= friction;
//
//	//角度に角速度を加算
//	rot += rot_spd;
//
//	//新しい重りの位置
//	px = fx + MathHelper::Cos(rot) * length;
//	py = fy + MathHelper::Sin(rot) * length;
//
//	//重りの座標
//	spherePos.x = px;
//	spherePos.y = py;
//
//	//角度調整
//	rot2 = rot - 90.0f;
//
//	//首の内側と外側の座標を求め、外側の座標を支点に設定する
//	for (int i = 0; i < 8; i++)
//	{
//		inPos[i].x = spherePos.x + MathHelper::Cos(lineRot[i] + rot2);
//		inPos[i].y = spherePos.y + MathHelper::Sin(lineRot[i] + rot2);
//		outPos[i].x = inPos[i].x + MathHelper::Cos(lineRot[i] + rot2) * neckLen[i];
//		outPos[i].y = inPos[i].y + MathHelper::Sin(lineRot[i] + rot2) * neckLen[i];
//		fulcrum[i] = outPos[i];
//	}
//
//	//1フレーム前のrotと比較
//	r2 = rot;
//	if (r1 < r2) //右に回っていれば
//	{
//		rotDirection = true;
//		r1 = rot;
//	}
//	else if (r1 > r2) //左に回っていれば
//	{
//		rotDirection = false;
//		r1 = rot;
//	}
//
//	//circleの当たり判定
//	Vector2 absA = spherePos - arrowPos;
//	float distanceS = sqrt((absA.x * absA.x) + (absA.y * absA.y));
//	if (distanceS < 32.0f ||
//		(rot_spd < 0 && !rotDirection && (Keyboard::GetInstance().KeyStateDown(KEYCODE::RIGHT) || GamePad::GetInstance().ButtonStateDown(PADBUTTON::RIGHT))) ||
//		(rot_spd > 0 && rotDirection && (Keyboard::GetInstance().KeyStateDown(KEYCODE::LEFT) || GamePad::GetInstance().ButtonStateDown(PADBUTTON::LEFT))))
//	{
//		aAlpha = 0.5f; //circleに当たっていれば半透明
//		friction = 1.015f; //摩擦を減らす
//	}
//	else
//	{
//		aAlpha = 1.0f; //circleに当たっていなければ不透明
//		friction = 0.985f; //摩擦を戻す
//	}
//
//	if (spherePos.y < fy)
//	{
//		friction = 0.985;
//	}
//
//	//スピード制限
//	spdLimit = sqrt(2 * g * (MathHelper::Sin(90.0f) * neckLen[vec])) / (neckLen[vec] * 0.02f);
//	//spdLimit = 3.0f / (neckLen[vec] * ((neckLen[vec] * 800.0f) * 0.0002f));
//	//rot_spd = 3.0f;
//	rot_spd = MathHelper::Clamp(rot_spd, -spdLimit, spdLimit);
//
//	//支点を移動
//	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::RSHIFT) ||
//		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2))
//	{
//		vec += 1;
//		if (vec > 7) vec = 0;
//		length = neckLen[vec];
//		fx = fulcrum[vec].x;
//		fy = fulcrum[vec].y;
//		lineRot[0] -= 45.0f;
//		rot += 45.0f;
//		turn = false;
//	}
//	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::LSHIFT) ||
//		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM3))
//	{
//		vec -= 1;
//		if (vec < 0) vec = 7;
//		length = neckLen[vec];
//		fx = fulcrum[vec].x;
//		fy = fulcrum[vec].y;
//		lineRot[0] += 45.0f;
//		rot -= 45.0f;
//		turn = true;
//	}
//
//	//45度ずつ8方向に首を伸ばす
//	for (int i = 1; i < 8; i++)
//	{
//		lineRot[i] = lineRot[i - 1] + 45.0f; //1本目の首に45度ずつ足していく
//	}
//
//	//頂点で正規化
//	if (rot > 270.0f) rot -= 360.0f;
//	if (rot < -90) rot += 360.0f;
//
//	//LB/RBで左右どちらかの首を伸ばし、もう片方の首を縮ませる
//	if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM6))
//	{
//		if (vec == 7)
//		{
//			if (neckLen[vec - 1] <= r + 20.0f) return; //これ以上短くならない
//			neckLen[0] += 5.0f;
//			neckLen[vec - 1] -= 5.0f;
//		}
//		else
//		{
//			if ((vec != 0 && neckLen[vec - 1] <= r + 20.0f) || (vec == 0 && neckLen[7] <= r + 20.0f)) return;
//			neckLen[vec + 1] += 5.0f;
//			if (vec == 0) neckLen[7] -= 5.0f;
//			else neckLen[vec - 1] -= 5.0f;
//		}
//	}
//	else if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM5))
//	{
//		if (vec == 0)
//		{
//			if (neckLen[vec + 1] <= r + 20.0f) return;
//			neckLen[7] += 5.0f;
//			neckLen[vec + 1] -= 5.0f;
//		}
//		else
//		{
//			if ((vec != 7 && neckLen[vec + 1] <= r + 20.0f) || (vec == 7 && neckLen[0] <= r + 20.0f)) return;
//			neckLen[vec - 1] += 5.0f;
//			if (vec == 7) neckLen[0] -= 5.0f;
//			else neckLen[vec + 1] -= 5.0f;
//		}
//	}
//
//	if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM4))
//	{
//		rot_spd += spdLimit;
//	}
//}

//void EndingScene::Double(Vector2 fPos)
//{
//	//振り子
//	//現在の重りの位置
//	dRot = rot;
//	auto px = fPos.x + MathHelper::Cos(dRot) * length;
//	auto py = fPos.y + MathHelper::Sin(dRot) * length;
//
//	//重力移動量を反映した重りの位置
//	auto vx = px - fPos.x;
//	auto vy = py - fPos.y;
//	auto t = -(vy * g) / (vx * vx + vy * vy);
//	auto gx = px + t * vx;
//	auto gy = py + g + t * vy;
//
//	//２つの重りの位置の確度差
//	auto rDiff = MathHelper::ATan(gy - fPos.y, gx - fPos.x);
//
//	//角度差を角速度に加算
//	auto sub = rDiff - dRot;
//	sub -= std::floor(sub / 360.0f) * 360.0f;
//	if (sub < -180.0f) sub += 360.0f;
//	if (sub > 180.0f) sub -= 360.0f;
//	dRot_spd = dRot_spd + sub;
//
//	////上の振り子との角度差を角速度に加算
//	auto dSub = rot - dRot;
//	dSub -= std::floor(dSub / 360.0f) * 360.0f;
//	if (dSub < -180.0f) dSub += 360.0f;
//	if (dSub > 180.0f) dSub -= 360.0f;
//	dRot_spd = dRot_spd + dSub;
//
//	any = rot - dRot;
//	any1 = rot;
//	any2 = dRot;
//	anyPos1 = fPos;
//
//	//摩擦
//	//if (doublePos.y < spherePos.y) dFriction = 0.985; //上の振り子の重りより高く上がったら摩擦を増やす
//	if (dRot <45.0f || dRot > 135.0f) dFriction = 0.985; //上の振り子より45度以上大きく振れたら摩擦を増やす
//	else dFriction = friction; //上記以外は上の振り子の摩擦を代入
//	dRot_spd *= dFriction;
//
//	//スピード制限
//	float dLimit = sqrt(2 * g * (MathHelper::Sin(90.0f) * neckLen[vec])) / (neckLen[vec] * 0.02f);
//	//dRot_spd = MathHelper::Clamp(dRot_spd, -dLimit, dLimit);
//	//dRot_spd = rot_spd;
//	//if (dRot_spd > spdLimit) dRot_spd = spdLimit;
//	//if (dRot_spd < -dRot_spd) dRot_spd = -spdLimit;
//
//	//角度に角速度を加算
//	dRot += dRot_spd;
//
//	//新しい重りの位置
//	px = fPos.x + MathHelper::Cos(dRot + rot2) * length;
//	py = fPos.y + MathHelper::Sin(dRot + rot2) * length;
//
//	//重りの座標
//	doublePos.x = px;
//	doublePos.y = py;
//
//	//頂点で正規化
//	if (dRot > 270.0f) dRot -= 360.0f;
//	if (dRot < -90) dRot += 360.0f;
//
//	//支点を移動
//	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::RSHIFT) ||
//	//	GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2))
//	//{
//	//	dRot -= 45.0f;
//	//}
//	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::LSHIFT) ||
//	//	GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM3))
//	//{
//	//	dRot += 45.0f;
//	//}
//
//	//1フレーム前のrotと比較
//	//r2 = rot;
//	//if (r1 < r2) //右に回っていれば
//	//{
//	//	rotDirection = true;
//	//	r1 = rot;
//	//}
//	//else if (r1 > r2) //左に回っていれば
//	//{
//	//	rotDirection = false;
//	//	r1 = rot;
//	//}
//}

void EndingScene::Multiple()
{
	//振り子
	//float oneLength = 100.0f;

	//現在の重りの位置
	for (int i = 0; i < fPos_.size(); i++)
	{
		auto px = fPos_[i].x + MathHelper::Cos(mRot[i]) * (oneLength);
		auto py = fPos_[i].y + MathHelper::Sin(mRot[i]) * (oneLength);

		//重力移動量を反映した重りの位置
		auto vx = px - fPos_[i].x;
		auto vy = py - fPos_[i].y;
		auto t = -(vy * g) / (vx * vx + vy * vy);
		auto gx = px + t * vx;
		auto gy = py + g + t * vy;

		//２つの重りの位置の確度差
		auto rDiff = MathHelper::ATan(gy - fPos_[i].y, gx - fPos_[i].x);

		//角度差を角速度に加算
		auto sub = rDiff - mRot[i];
		sub -= std::floor(sub / 360.0f) * 360.0f;
		if (sub < -180.0f) sub += 360.0f;
		if (sub > 180.0f) sub -= 360.0f;

		if (i == 0)mRot_spd[i] = mRot_spd[i] + sub;
		else mRot_spd[i] = (mRot_spd[i - 1] + sub) * 0.8f;


		//anyPos2[i] = fPos[i];


		//角度に角速度を加算
		if (i == 0) mRot[i] += mRot_spd[i];
		else mRot[i] = mRot[0] + mRot_spd[i];

		//新しい重りの位置
		px = fPos_[i].x + MathHelper::Cos(mRot[i] /*+ rot2*/) * (oneLength);
		py = fPos_[i].y + MathHelper::Sin(mRot[i] /*+ rot2*/) * (oneLength);

		//重りの座標
		multiplePos[i].x = px;
		multiplePos[i].y = py;

		//if (fPos.size() - 1 > i) {
		//	fPos[i + 1].x = px;
		//	fPos[i + 1].y = py;
		//}
		//else {
		//}
		if (i > 0) fPos_[i] = multiplePos[i - 1];

		//頂点で正規化
		if (mRot[i] > 270.0f) mRot[i] -= 360.0f;
		if (mRot[i] < -90) mRot[i] += 360.0f;


		//摩擦
		//if (multiplePos[i].y < spherePos.y) mFriction = 0.985; //上の振り子の重りより高く上がったら摩擦を増やす
		//else if (i > 0 && (multiplePos[i].y < multiplePos[i - 1].y)) mFriction = friction;
		//////if (mRot <45.0f || mRot > 135.0f) mFriction = 0.985; //上の振り子より45度以上大きく振れたら摩擦を増やす
		//else mFriction = friction; //上記以外は上の振り子の摩擦を代入
		////mFriction = friction;

		if ((mRot_spd[0] < 0 /*&& !rotDirection*/ && (Keyboard::GetInstance().KeyStateDown(KEYCODE::RIGHT) || GamePad::GetInstance().ButtonStateDown(PADBUTTON::RIGHT))) ||
			(mRot_spd[0] > 0 /*&& rotDirection*/ && (Keyboard::GetInstance().KeyStateDown(KEYCODE::LEFT) || GamePad::GetInstance().ButtonStateDown(PADBUTTON::LEFT))))
		{
			mFriction = 1.015f; //摩擦を減らす
		}
		else
		{
			mFriction = 0.985f; //摩擦を戻す
		}

		mRot_spd[i] *= mFriction;

		//支点を移動
		if (Keyboard::GetInstance().KeyStateDown(KEYCODE::RSHIFT) ||
			GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM6))
		{
			neckLengh += 1.0f;
		}
		if (Keyboard::GetInstance().KeyStateDown(KEYCODE::LSHIFT) ||
			GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM5))
		{
			neckLengh -= 1.0f;
		}
	}
}

//変形描画
void EndingScene::DeformationDraw()
{
	//
	drawPoints.clear();

	DrawPos p;
	for (int i = 0; i < fPos_.size(); i++) {
		Vector2 v = fPos_[i] - multiplePos[i];
		Vector2 n = Vector2(-v.y, v.x).Normalize();
		if (i == 0) {
			p.p0 = fPos_[i] + n * resWidth * correctionWidth * 1.2f;
			p.p1 = fPos_[i] - n * resWidth * correctionWidth * 1.2f;
			if (v.Length() > 0) {
				p.p2 = multiplePos[i] - n * resWidth * correctionWidth * 1.2f;
				p.p3 = multiplePos[i] + n * resWidth * correctionWidth * 1.2f;
				p.p2 -= v.Normalize() * correctionHeight * correctionWidth;
				p.p3 -= v.Normalize() * correctionHeight * correctionWidth;
				p.p0 += v.Normalize() * correctionHeight * correctionWidth;
				p.p1 += v.Normalize() * correctionHeight * correctionWidth;
				drawPoints.push_back(p);
			}
		}
		else {
			p.p0 = fPos_[i] + n * resWidth;
			p.p1 = fPos_[i] - n * resWidth;
			if (v.Length() > 0) {
				p.p2 = p.p1 - v.Normalize() * 76.0f * correctionLens[i];
				p.p3 = p.p0 - v.Normalize() * 76.0f * correctionLens[i];
				p.p0 += v.Normalize() * correctionHeight;
				p.p1 += v.Normalize() * correctionHeight;
				p.p2 -= v.Normalize() * correctionHeight;
				p.p3 -= v.Normalize() * correctionHeight;
				drawPoints.push_back(p);
			}
		}
	}
}

void EndingScene::MultipleInit(float len, const Vector2& fPos, float rot,float radius)
{
	mRot.clear();
	mRot_spd.clear();
	mLimit.clear();
	fPos_.clear();
	multiplePos.clear();

	int s = len / oneLength;
	correctionLens.clear();
	vector<float> data(s + 1, 0.0f);
	correctionLens = data;
	correctionLens.front() = 1.0f;
	len -= oneLength;
	len -= radius;
	int i;
	for (i = 1; i < len / oneLength; i++) {
		correctionLens[i] = 1.0f;

	}
	float h = (MathHelper::Mod(len - oneLength, oneLength) + correctionHeight * 2.0f) / (oneLength + correctionHeight * 2.0f);

	correctionLens[i] = h;
	fNum = s;

	fPos_.push_back(fPos);
	mRot.push_back(rot);
	mLimit.push_back(0.0f);
	mRot_spd.push_back(0.0f);
	multiplePos.push_back(Vector2(0.0f, 0.0f));
	for (int i = 0; i < fNum; i++) {
		auto px = fPos_[i].x + MathHelper::Cos(mRot[i]) * 100;
		auto py = fPos_[i].y + MathHelper::Sin(mRot[i]) * 100;

		mRot.push_back(rot);
		mRot_spd.push_back(0.0f);
		mLimit.push_back(0.0f);
		fPos_.push_back(Vector2(px, py));
		multiplePos.push_back(Vector2(0.0f, 0.0f));
	}
}

