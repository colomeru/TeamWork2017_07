#include "World.h"
#include "../actor/Actor.h"
#include"../math/Vector3.h"

// コンストラクタ
World::World():targetAct_(nullptr)
{
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
}

// 更新
void World::Update()
{
	if (targetAct_ != nullptr) {
		inv(targetMat_);
		//targetMat_ = Matrix::CreateTranslation(Vector3(targetAct_->GetPosition().x, targetAct_->GetPosition().y, 0));
		targetMat_ = Matrix::CreateTranslation(Vector3(targetAct_->GetPosition().x, 0, 0));
		//*Matrix::CreateRotationZ(targetAct_->GetAngle());
	}

	actors_.Update();
	
	// 受動更新
	if (!manualStackActor_.empty())
		manualStackActor_.top()->OnUpdate();

	// カメラ更新
	//if (!stackCamera_.empty())
	//	stackCamera_.top()->OnUpdate();
}

// 描画
void World::Draw() const
{
	DrawFormatString(0,600,GetColor(255,255,255),"%f:%f", inv_.Translation().x, inv_.Translation().y);
	actors_.Draw();
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

	//最大最小値の指定(マジックナンバー0,999999は後で消そう)
	float clampPosX = MathHelper::Clamp((int)playerMat.Translation().x, 0, 999999);
	float clampPosY = MathHelper::Clamp((int)playerMat.Translation().y, 0, 999999);

	//if (scrool.scroolJudge.x == 0)
	//	clampPosX = playerScreenPos_.x;
	//if (scrool.scroolJudge.y == 0)
	//	clampPosY = playerScreenPos_.y;
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
		pos.x,//*scrool.scroolJudge.x,
		pos.y,//*scrool.scroolJudge.y,
		0.0f));

	//1フレーム後の座標
	mCurPos = Vector2(inv_.Translation().x, inv_.Translation().y);
	//移動量を計算
	mVelo = mPrePos - mCurPos;
	mVelo = Vector2(mVelo.x/**scrool.scroolJudge.x*/, mVelo.y /** scrool.scroolJudge.y*/);

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
	//mVelo = mPrePos - mCurPos;
	return inv_;
}
