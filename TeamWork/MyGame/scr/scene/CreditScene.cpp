#include "CreditScene.h"
#include "../conv/DXConverter.h"
#include "../fade/FadePanel.h"
#include "../math/MathHelper.h"
#include "../stageGenerator/Stage.h"
#include "../stageGenerator/Stage1/Stage1.h"
#include "../graphic/Sprite.h"
#include"../actor/Field/ClothesLine.h"
#include "../tween/TweenManager.h"
#include "addScreen/screenSupport/CreditTextGenerator.h"
#include "../time/Time.h"
#include "../Def.h"
#include "../actor/player/CreditPlayer.h"
#include "../scene/addScreen/screenSupport/CreditText.h"
#include "../sound/sound.h"
#include "../debugdata/DebugDraw.h"
#include "../actor/player/PlayerNeck/PlayerNeckPendulumSupport.h"
#include "../input/InputChecker.h"

//背景色
const Vector3 BgColor[9]{ Vector3(155, 204, 255) ,Vector3(51, 204, 255) ,Vector3(0, 153, 255) ,
						  Vector3(0, 153, 235) ,  Vector3(204, 153, 0) , Vector3(153, 102, 51) ,
						  Vector3(0, 51, 102) , Vector3(0, 0, 102) ,  Vector3(155, 204, 255) };

//コンストラクタ
CreditScene::CreditScene() :
	nextScene_(Scene::Title), isUseKey_(true), bgColorNum_(0), from_(0.0f), bgColor_(BgColor[0]), sceneTimer_(0.0f),
	waiting_(true), mulInit_(true)
{
	// ワールド生成
	world_ = std::make_shared<World>();
	// イベントリスナー登録
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});

	bgScreen_ = BackgroundScreen(world_.get());
}

//デストラクタ
CreditScene::~CreditScene()
{

}

//初期化
void CreditScene::Initialize()
{
	isEnd_ = false;

	world_->Initialize();

	player_ = std::make_shared<CreditPlayer>(world_.get());

	world_->Add(ACTOR_ID::PLAYER_ACTOR, player_);
	world_->PushStackActor(player_);

	std::string filename = "Tutorial1";
	Stage1 stage(world_.get(), filename);

	world_->InitializeInv(Vector2(player_->GetPosition().x, player_->GetPosition().y));

	world_->SetTarget(player_.get());

	bgScreen_.Init(Stage::Stage1);

	world_->Add(ACTOR_ID::LANE_ACTOR, std::make_shared<ClothesLine>(world_.get(), 1, stage.GetStageSize() + Vector2(WINDOW_WIDTH + 100, 0), Vector2(-150, 100)));

	//開始地点
	pHeadPos_ = startPos_;
	operate_ = true;
	sceneTimer_ = 0.0f;
	mulInit_ = true;
	waiting_ = true;

	//カラス
	whitePos_ = { player_->GetCurrentPHeadPosition() + Corr + WhiteCorr,
				  Vector2(WINDOW_WIDTH / 2.0f, -300.0f) + WhiteCorr };
	dWhitePos_ = whitePos_[0];
	//アニメーション読み込み
	int crowIdNum = WHITE_ANM_01_SPRITE;
	for (int i = 0; i < 8; i++) {
		anmManager_.Add((SPRITE_ID)(crowIdNum + i));
	}
	anmManager_.SetIsRepeat(true);

	//背景色
	SetBackgroundColor(bgColor_.x, bgColor_.y, bgColor_.z);
	TweenManager::GetInstance().Loop(Linear, &from_, 1.0f, 6.0f, [=]() {ChangeBackGroundColor(); });

	Sound::GetInstance().PlayBGM(BGM_ID::CREDIT_BGM, DX_PLAYTYPE_LOOP);

	world_->Add(ACTOR_ID::SAMPLE_ACTOR, std::make_shared<CreditTextGenerator>(world_.get(), Vector2(500, 100)));

	//フェードイン
	FadePanel::GetInstance().SetInTime(0.5f);
	FadePanel::GetInstance().FadeIn();
}

//更新
void CreditScene::Update()
{
	world_->Update();

	world_->SetScrollPos(Vector2::Zero);

	bgScreen_.Update();

	anmManager_.Update();

	whitePos_[0] = player_->GetCurrentPHeadPosition() + Corr + WhiteCorr;

	PlayerRestart();

	//フェード中は以下の処理をスキップ
	if (!FadePanel::GetInstance().IsClearScreen()) return;

	sceneTimer_ += Time::DeltaTime;
	if (sceneTimer_ >= SceneTime) {
		//シーン遷移
		FadePanel::GetInstance().AddCollBack([=]() { isEnd_ = true; });
		FadePanel::GetInstance().FadeOut();
	}

	//背景色セット
	SetBackgroundColor(bgColor_.x, bgColor_.y, bgColor_.z);
}

//描画
void CreditScene::Draw() const
{
	//背景
	//bgScreen_.Draw();
	Sprite::GetInstance().Draw(CREDIT_BG_SPRITE, Vector2::Zero);
	Sprite::GetInstance().Draw(FENCE_SPRITE, Vector2::Zero);

	// 描画
	world_->Draw(3, world_->GetKeepDatas().playerLane_);

	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::BIRD_SPRITE);
	anmManager_.Draw(dWhitePos_, origin, Vector2::One, 1.0f);

	if (BuildMode != 1) return;
	DebugDraw::DebugDrawFormatString(0, 00, GetColor(255, 255, 255), "CreditScene");
	DebugDraw::DebugDrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);
	if (operate_) DebugDraw::DebugDrawFormatString(0, 40, GetColor(255, 255, 255), "操作できます！");
	else DebugDraw::DebugDrawFormatString(0, 40, GetColor(255, 255, 255), "操作できません！");
	DebugDraw::DebugDrawFormatString(0, 60, GetColor(255, 255, 255), "sceneTimer_:[%f]", sceneTimer_);
	DebugDraw::DebugDrawFormatString(0, 80, GetColor(255, 255, 255), "bgColorNum_:%d", bgColorNum_);
}

//終了しているか？
bool CreditScene::IsEnd() const
{
	return isEnd_;
}

//次
Scene CreditScene::Next() const
{
	return nextScene_;
}

//終了時処理
void CreditScene::End()
{
	// 初期化
	world_->Clear();
	player_.reset();
	credit_.reset();
	bgScreen_.End();
	TweenManager::GetInstance().Cancel(&from_);
	TweenManager::GetInstance().Cancel(&bgColor_);

	Sound::GetInstance().StopBGM();
}

//メッセージ処理
void CreditScene::handleMessage(EventMessage message, void * param)
{
	switch (message)
	{
	case EventMessage::PLAYER_POS_RESET:
	{
		//スタート地点に復帰
		RestartSet();
		break;
	}
	default:
		break;
	}
}

//画面外か？
bool CreditScene::ScreenOut() const
{
	if (player_->GetCurrentPHeadPosition().x < -300.0f - Corr.x || player_->GetCurrentPHeadPosition().x > WINDOW_WIDTH + 300.0f - Corr.x ||
		player_->GetCurrentPHeadPosition().y < -300.0f - Corr.y || player_->GetCurrentPHeadPosition().y > WINDOW_HEIGHT + 300.0f - Corr.y)
		return true;
	else
		return false;
}

//プレイヤーリスタート
void CreditScene::PlayerRestart()
{
	//操作不能時
	if (!operate_) {
		NotOperate();
	}

	//画面外に出たら
	if (ScreenOut() && operate_) {
		RestartSet();
	}

	//スタート位置に戻ったら
	Vector2 dis = player_->GetCurrentPHeadPosition() - startPos_;
	if (dis.Length() <= 2.0f && !operate_) {
		PlayerStart();
	}

	//待機中
	if (waiting_ && (isUseKey_.KeyTriggerDown(InputChecker::Input_Key::B) || InputChecker::GetInstance().KeyTriggerDown(InputChecker::Input_Key::X))) {
		TweenManager::GetInstance().Add(EaseInQuart, &dWhitePos_, whitePos_[1], 2.0f);
		waiting_ = false;
	}
}

//リスタートセット
void CreditScene::RestartSet()
{
	operate_ = false;
	player_->AllResurrectHead();
	player_->SetIsBiteMode(false);
	player_->CurHeadBite(pHeadPos_);
	player_->PHeadLengthReset();
	pHeadPos_ = player_->GetCurrentPHeadPosition(); //プレイヤー座標
	player_->SetOtherClothesID_(CLOTHES_ID::FLUFFY_CLOTHES);
	TweenManager::GetInstance().Add(EaseOutQuart, &pHeadPos_, startPos_, 2.0f);
	//復帰中は操作を受け付けない
	int flag = 0;
	world_->sendMessage(EventMessage::OPERATE_FLAG, (void*)flag);

	Sound::GetInstance().PlaySE(SE_ID::CANCEL_SE);
}

//操作不能時
void CreditScene::NotOperate()
{
	player_->SetIsBiteMode(true);
	player_->setCurPHeadSPos(pHeadPos_);
	player_->GetCurrentHead()->SetPose(Matrix::CreateTranslation(Vector3(pHeadPos_.x, pHeadPos_.y, 0)));
	dWhitePos_ = whitePos_[0];
	if (mulInit_) {
		player_->MultipleInit(110.0f, player_->GetCurrentPHeadPosition(), player_->GetRot(), 60.0f);
		mulInit_ = false;
	}
}

//スタート時
void CreditScene::PlayerStart()
{
	player_->SetIsBiteMode(true);
	player_->GravityReset();
	operate_ = true;
	mulInit_ = true;
	waiting_ = true;
	int flag = 1;
	world_->sendMessage(EventMessage::OPERATE_FLAG, (void*)flag);
}

//背景色変更
void CreditScene::ChangeBackGroundColor()
{
	bgColorNum_++;
	bgColorNum_ = MathHelper::Clamp(bgColorNum_, 0, 8);
	TweenManager::GetInstance().Add(Linear, &bgColor_, BgColor[bgColorNum_], 3.0f);
}
