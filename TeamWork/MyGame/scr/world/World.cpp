#include "World.h"
#include "../actor/Actor.h"
#include"../math/Vector3.h"
#include"../Def.h"

// コンストラクタ
World::World() :targetAct_(nullptr), keepDatas_(), isChangeCam_(false), addNum_(0),inv_(), isChangeFrame_(false), camShootSpd_(0.f), isLockedCamY_(true), unlockYPos_(0.0f),maxSize_(999999)
{
	updateFunctionMap_[false] = std::bind(&WorldActor::Update, &actors_);
	updateFunctionMap_[true] = std::bind(&WorldActor::ChangeLaneUpdate, &actors_);

}

// デストラクタ
World::~World()
{
	Clear();
}

// 初期化
void World::Initialize()
{
	Clear();
	keepDatas_ = KeepDatas();
	isChangeCam_ = false;
	addNum_ = 0;
	isChangeFrame_ = false;
	isLockedCamY_ = true;
	unlockYPos_ = 0.0f;
}

// 更新
void World::Update()
{
	if (targetAct_ != nullptr&&!isChangeCam_) {
		inv(targetMat_);

		if (isLockedCamY_) {
			if (isFreeCamY_)targetMat_ = Matrix::CreateTranslation(Vector3(targetAct_->GetPosition(), 0));
			else targetMat_ = Matrix::CreateTranslation(Vector3(targetAct_->GetPosition().x, 0, 0));
		}
		else {
			unlockYPos_ -= 3.0f;
			unlockYPos_ = max(unlockYPos_, -WINDOW_HEIGHT*1.5f);
			keepDatas_.camPosY_ = unlockYPos_;
			targetMat_ = Matrix::CreateTranslation(Vector3(targetAct_->GetPosition().x, unlockYPos_, 0));
		}

	}

	isChangeFrame_ = false;
	if (isChangeCam_) {
		if (addNum_ > 0) camShootSpd_ -= 0.1f;
		else camShootSpd_ += 0.1f;
		camShootSpd_ = max(camShootSpd_, 0.1f);
		keepDatas_.SetChangeLaneLerpPos_(keepDatas_.changeLaneLerpPos_ + 0.04f*camShootSpd_);
	}

	if (keepDatas_.changeLaneLerpPos_ >= 1.0f) {
		isChangeCam_ = false;
		isChangeFrame_ = true;
		keepDatas_.SetChangeLaneLerpPos_(0.0f);
	}
	if (keepDatas_.isFallCamMode_) {
		keepDatas_.fallAddPos_ -= 10.f;
		if (keepDatas_.fallAddPos_ <= 0.1f) {
			keepDatas_.isFallCamMode_=false;
		}
	}
	updateFunctionMap_[isChangeCam_]();
	// 受動更新
	if (!manualStackActor_.empty()&&!isChangeCam_)
		manualStackActor_.top()->OnUpdate();

}

// 描画
void World::Draw(const int laneCount, const int playerLane) const
{
	if(!isChangeFrame_)actors_.Draw(laneCount, playerLane);

	if (BuildMode != 1)return;
	DrawFormatString(0,600,GetColor(255,255,255),"%f", keepDatas_.changeLaneLerpPos_);
}

// クリア
void World::Clear()
{
	// スタック消去
	while (!manualStackActor_.empty())
		manualStackActor_.pop();
	while (!stackCamera_.empty())
		stackCamera_.pop();

	// アクター消去
	actors_.Clear();
}

// イベントリスナーの追加
void World::AddEventMessageListener(std::function<void(EventMessage, void*)> listener)
{
	actors_.AddEventMessageListener(listener);
}

// アクターの取得
WorldActor World::GetWorldActor() const
{
	return actors_;
}

// 追加
void World::Add(ACTOR_ID id, ActorPtr actor)
{
	actors_.Add(id, actor);
}

// 
bool World::IsEnd() const
{
	return false;
}

// 衝突判定
void World::SetCollideSelect(ActorPtr thisActor, ACTOR_ID otherID, COL_ID colID)
{
	actors_.SetCollideSelect(thisActor, otherID, colID);
}

// メッセージの送信
void World::sendMessage(EventMessage message, void * param)
{
	actors_.HandleMessage(message, param);
}

// 指定グループの巡回
void World::EachActor(ACTOR_ID id, std::function<void(Actor&)> func)
{
	actors_.EachActor(id, func);
}

// 指定グループの巡回
void World::EachActor(ACTOR_ID id, std::function<void(const Actor&)> func) const
{
	actors_.EachActor(id, func);
}

int World::Count(ACTOR_ID id) const
{
	return actors_.Count(id);
}

// 操作アクターセット
void World::PushStackActor(ActorPtr actor)
{
	manualStackActor_.push(actor);
}

// 操作アクターポップ
void World::PopStackActor()
{
	if (!manualStackActor_.empty())
		manualStackActor_.pop();
}

void World::inv(const Matrix & mat)
{
	//1フレーム前の座標
	mPrePos = Vector2(inv_.Translation().x, inv_.Translation().y);
	//スクロールストップ処理
	Matrix playerMat;
	playerMat = mat;

	//最大最小値の指定(0,999999は後で消そう)
	float clampPosX = MathHelper::Clamp((int)playerMat.Translation().x, -100, maxSize_);
	float clampPosY = MathHelper::Clamp((int)playerMat.Translation().y, -10000, 999999);

	playerMat.Translation(Vector3(clampPosX, clampPosY, 0.0f));

	//行くべき位置を設定(matrix版)
	resInv_ = Matrix::Invert(playerMat) *
		Matrix::CreateTranslation(Vector3(playerScreenPos_.x, playerScreenPos_.y));
	//行くべき位置を設定
	Vector2 resPos = Vector2(resInv_.Translation().x, resInv_.Translation().y);
	Vector2 pos = Vector2(inv_.Translation().x, inv_.Translation().y);

	Spring(pos, resPos, velo, 0.2f);
	//補正された移動マトリックス代入
	inv_ = Matrix::CreateTranslation(Vector3(
		pos.x,
		pos.y,
		0.0f));

	//1フレーム後の座標
	mCurPos = Vector2(inv_.Translation().x, inv_.Translation().y);
	//移動量を計算
	mVelo = mPrePos - mCurPos;
	mVelo = Vector2(mVelo.x, mVelo.y);

}
Matrix World::InitializeInv(Vector2 position)
{
	//1フレーム前の座標
	mPrePos = Vector2(inv_.Translation().x, inv_.Translation().y);
	Matrix playerMat;
	playerMat.Translation(Vector3(position.x, position.y, 0.0f));

	inv_ = Matrix::Invert(playerMat) *
		Matrix::CreateTranslation(Vector3(playerScreenPos_.x, playerScreenPos_.y));
	resInv_ = Matrix::Invert(playerMat) *
		Matrix::CreateTranslation(Vector3(playerScreenPos_.x, playerScreenPos_.y));

	//1フレーム後の座標
	mCurPos = Vector2(inv_.Translation().x, inv_.Translation().y);
	//移動量を計算
	OutputDebugString(std::to_string(position.x).c_str());
	OutputDebugString(":");
	OutputDebugString(std::to_string(position.y).c_str());
	OutputDebugString("\n");
	return inv_;
}

void World::StartModeUpdate()
{
	inv(targetMat_);
	targetMat_ = Matrix::CreateTranslation(Vector3(targetAct_->GetPosition().x, 0, 0));
	actors_.StartModeUpdate();
	isLockedCamY_ = true;
}
