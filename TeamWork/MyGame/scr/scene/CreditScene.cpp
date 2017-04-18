#include "CreditScene.h"
#include "../time/Time.h"
#include "../conv/DXConverter.h"

#include "../actor/player/Player3.h"
#include "../actor/enemy/Enemy.h"
#include "../graphic/Model.h"
#include "../math/MathHelper.h"
#include "../stageGenerate/Stage.h"
#include "../stageGenerate/Stage1/Stage1.h"

CreditScene::CreditScene() :
nextScene_(Scene::Ending),
camera_pos_(0, 100, -100),
target_(0, 0, 0),
weight_pos_(0.0f, 0.0f),
fulcrum_(500.0f, 300.0f),
rot_(90.0f),
rot_spd_(3.0f),
length_(200.0f),
gravity_(0.2f)
{
	// ワールド生成
	world_ = std::make_shared<World>();
	// イベントリスナー登録
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});

	stageGenerateManager.Add(Stage::Stage1, std::make_shared<Stage1>(world_.get(), std::string("Stage1")));
	stageGenerateManager.SetStage(Stage::Stage1);
}

CreditScene::~CreditScene()
{
}

void CreditScene::Initialize()
{
	isEnd_ = false;

	Camera::GetInstance().SetRange(0.1f, 10000.0f);
	Camera::GetInstance().SetViewAngle(60.0f);
	Camera::GetInstance().Up.Set(Vector3::Up);
	Camera::GetInstance().Position.Set(camera_pos_);
	Camera::GetInstance().Target.Set(target_);
	Camera::GetInstance().Update();


	//world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<Stage>(world_.get()));
	world_->Add(ACTOR_ID::PLAYER_ACTOR, std::make_shared<Player3>(world_.get()));
	//world_->Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Enemy>(world_.get()));
}

void CreditScene::Update()
{
	// 更新
	world_->Update();

	Camera::GetInstance().Position.Set(camera_pos_);
	Camera::GetInstance().Target.Set(target_);
	Camera::GetInstance().Update();

	weight_pos_ = pendulum(fulcrum_, length_);

	// 終了
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;
}

void CreditScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "CreditScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);

	DrawFormatString(0, 60, GetColor(255, 255, 255), "座標 x :%f y :%f", weight_pos_.x, weight_pos_.y);

	DrawCircle(fulcrum_.x, fulcrum_.y, 10, GetColor(255, 0, 0), true);
	//DrawCircle(weight_pos_.x, weight_pos_.y, 50, GetColor(255, 255, 255), true);

	// 描画
	world_->Draw();
}

bool CreditScene::IsEnd() const
{
	return isEnd_;
}

Scene CreditScene::Next() const
{
	return nextScene_;
}

void CreditScene::End()
{
	// 初期化
	world_->Clear();
}

void CreditScene::handleMessage(EventMessage message, void * param)
{
}

Vector2 CreditScene::pendulum(Vector2 fulcrum, float length)
{
	Vector2 result_;										//結果(移動後)
	Vector2 result_2;										//結果(移動前)
	float friction = 0.998f;								//摩擦

	//現在の重りの位置
	weight_pos_.x = fulcrum.x + MathHelper::Cos(rot_) * length;
	weight_pos_.y = fulcrum.y + MathHelper::Sin(rot_) * length;
	result_2 = weight_pos_;

	//重力移動量を反映した重りの位置
	auto length_vec = weight_pos_ - fulcrum;
	auto t = -(length_vec.y * gravity_) / (length_vec.x * length_vec.x + length_vec.y * length_vec.y);
	auto move_weightX = weight_pos_.x + t * length_vec.x;
	auto move_weightY = weight_pos_.y + gravity_ + t * length_vec.y;

	//2つの重りの位置の角度差
	auto r = MathHelper::ATan(move_weightY - fulcrum.y, move_weightX - fulcrum.x);

	//角度差を角速度に加算
	auto sub = r - rot_;
	sub -= std::floor(sub / 360.0f) * 360.0f;
	if (sub < -180.0f) sub += 360.0f;
	if (sub > 180.0f) sub -= 360.0f;
	rot_spd_ += sub;

	//摩擦
	rot_ *= friction;

	//角度に角速度を加算
	rot_ += rot_spd_;

	//新しい重りの位置
	weight_pos_.x = fulcrum.x + MathHelper::Cos(rot_) * length;
	weight_pos_.y = fulcrum.y + MathHelper::Sin(rot_) * length;

 	result_ = weight_pos_;

	return result_;
}
