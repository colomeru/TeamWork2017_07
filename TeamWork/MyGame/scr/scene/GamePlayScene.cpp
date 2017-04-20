#include "GamePlayScene.h"
#include "../math/MathHelper.h"
#include "../time/Time.h"
#include "../fade/FadePanel.h"

#include "../actor/SampleActor.h"
#include"../camera/Camera.h"
#include"../conv/DXConverter.h"
#include"../actor/player/Player3.h"
#include"../actor/player/Player2.h"
#include"../actor/Field/Clothes/BaseClothes.h"

#include"../myData/MyTestCamera.h"
#include"../collision/MyCol.h"

GamePlayScene::GamePlayScene() :
nextScene_(Scene::Credit)//, posit(0,0,0), camera_pos_(0, 100, -100),target_(0, 0, 0)

{
	// ワールド生成
	world_ = std::make_shared<World>();
	// イベントリスナー登録
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});
}

GamePlayScene::~GamePlayScene()
{
}

void GamePlayScene::Initialize()
{
	isEnd_ = false;

	// フェードパネル初期化
	FadePanel::GetInstance().Initialize();

	// アクター生成
	world_->Add(ACTOR_ID::SAMPLE_ACTOR, std::make_shared<SampleActor>(world_.get()));

	//Vector3 position_ = posit;
	//Vector3 target_ = posit;
	//RangeF range = RangeF(0.f, 1000.f);
	//float angle = 60.f;
	
	//camera_ = std::make_shared<TPSCamera>(world_.get(), position_, target_, range, angle, Vector3::Up, 0);
	//world_->Add(ACTOR_ID::CAMERA_ACTOR,camera_);

	//world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<Stage>(world_.get()));
	
	ply1 = std::make_shared<Player3>(world_.get());
	ply2 = std::make_shared<Player2>(world_.get());
	world_->Add(ACTOR_ID::PLAYER_ACTOR, ply1);
	world_->Add(ACTOR_ID::ENEMY_ACTOR, ply2);
	world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<BaseClothes>(world_.get(), CLOTHES_ID::BASE_CLOTHES, Vector2(500,200)));
	world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<BaseClothes>(world_.get(), CLOTHES_ID::BASE_CLOTHES, Vector2(850, 200)));
	world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<BaseClothes>(world_.get(), CLOTHES_ID::BASE_CLOTHES, Vector2(1200, 200)));

	//本番用
	//world_->Add(ACTOR_ID::CAMERA_ACTOR, std::make_shared<TPSCamera>(world_.get()));
	//テスト用
	//world_->Add(ACTOR_ID::CAMERA_ACTOR, std::make_shared<MyTestCamera>(world_.get()));


	//Camera::GetInstance().SetRange(0.f, 1000.f);
	//Camera::GetInstance().SetViewAngle(60.f);
	//Camera::GetInstance().Up.Set(Vector3::Up);
	//Camera::GetInstance().Position.Set(camera_pos_);
	//Camera::GetInstance().Target.Set(target_);
	//Camera::GetInstance().Update();

	world_->InitializeInv(Vector2(ply1->GetPosition().x, ply1->GetPosition().y));
	world_->SetTarget(ply1.get());
}

void GamePlayScene::Update()
{
	// 更新
	world_->Update();
	// 終了
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;


	//Camera::GetInstance().Position.Set(camera_pos_);
	//Camera::GetInstance().Target.Set(target_);
	//Camera::GetInstance().Update();

}

void GamePlayScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "GamePlayScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);

	DrawFormatString(700, 600, GetColor(255, 255, 255), "%f", ply1->GetAngle());

	CollisionParameter param= IsHit_OBB_Segment(*ply1->GetActor(), *ply2->GetActor());
	if (param.colFrag) {
		DrawFormatString(0, 500, GetColor(255, 255, 255), "%f:%f", param.colPos.x,param.colPos.y);
		DrawCircle(param.colPos.x, param.colPos.y, 32, GetColor(255, 0, 0));
	}
	else
	{
		DrawFormatString(0, 500, GetColor(255, 255, 255), "dame");

	}
	// 描画
	world_->Draw();

	DrawLine(ply1->GetParameter().mat.Translation().x, ply1->GetParameter().mat.Translation().y, ply2->GetParameter().mat.Translation().x, ply2->GetParameter().mat.Translation().y,GetColor(255,255,255));

	//VECTOR pos1 = DXConverter::GetInstance().ToVECTOR(posit);
	//VECTOR pos2 = DXConverter::GetInstance().ToVECTOR(posit);
	
	//DrawCapsule3D(pos1, pos2, 1, 16, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);

}

bool GamePlayScene::IsEnd() const
{
	return isEnd_;
}

Scene GamePlayScene::Next() const
{
	return nextScene_;
}

void GamePlayScene::End()
{
	// 初期化
	world_->Clear();
}

void GamePlayScene::handleMessage(EventMessage message, void * param)
{
}
