#include "CreditText.h"
#include "../../../tween/TweenManager.h"
#include "../../../actor/player/Player_Head.h"
#include "../../../game/ID.h"
#include "../../../debugdata/DebugDraw.h"
#include "../../../graphic/Sprite.h"

//コンストラクタ
CreditText::CreditText(IWorld* world, CLOTHES_ID id, SPRITE_ID sprite, int laneNum, Vector2 position, Vector2 size, int frame) :
	Clothes(world, id, laneNum, 0.0f),f2(0.0f),cPlayer_(nullptr)
{
	player_ = nullptr;
	spriteId_ = sprite;
	frame_ = frame;
	laneNum_ = world_->GetKeepDatas().playerLane_;
	position_ = position;
	parameter_.size = size;
	parameter_.radius = 32.0f;
	//始点
	fulcrum_ = Vector2(position_.x, position_.y - parameter_.size.y / 2.0f);
	auto nameCount = Sprite::GetInstance().GetSliptFrameSize(sprite);
	//死亡座標
	auto toX = -400 - 370 * (nameCount - frame);
	TweenManager::GetInstance().Add(Linear, &position_, Vector2(toX, position_.y), 10.0f, [=]() {
		Dead();
		if (cPlayer_ != nullptr) {
			//プレイヤーをスタート位置に戻す
			world_->sendMessage(EventMessage::PLAYER_POS_RESET);
		}
	});
	//移動
	TweenManager::GetInstance().Add(Linear, &fulcrum_, Vector2(toX, fulcrum_.y), 10.0f);
	colFuncMap_[COL_ID::BOX_BOX_COL] = std::bind(&CollisionFunction::IsHit_OBB_OBB, colFunc_, std::placeholders::_1, std::placeholders::_2);
	parameter_.ID = ACTOR_ID::STAGE_ACTOR;
	localPoints_[CuttingState::Normal].push_back(Vector3(-parameter_.size.x / 2.0f, 0.0f, 0.0f));
	localPoints_[CuttingState::Normal].push_back(Vector3(-parameter_.size.x / 2.0f, parameter_.size.y, 0.0f));
	localPoints_[CuttingState::Normal].push_back(Vector3(parameter_.size.x / 2.0f, parameter_.size.y, 0.0f));
	localPoints_[CuttingState::Normal].push_back(Vector3(parameter_.size.x / 2.0f, 0.0f, 0.0f));
	SetPointsUpdate();
}

//デストラクタ
CreditText::~CreditText()
{
}

//更新
void CreditText::Update()
{
	//前フレームからの移動量を取得
	velocity_ = Vector2(position_.x - f2, 0.0f);
	f2 = position_.x;

	SetPointsUpdate();

	if (parent_ == nullptr || cPlayer_ == nullptr) return;
	if (!cPlayer_->GetIsBiteMode()) {
		parent_ = nullptr;
		cPlayer_ = nullptr;
		return;
	}

	//頭座標
	Vector2 pos = parent_->GetPosition() + velocity_;
	cPlayer_->setCurPHeadSPos(pos);
	parent_->SetPose(Matrix::CreateTranslation(Vector3(pos.x, pos.y, 0)));

}

//描画
void CreditText::Draw() const
{
	auto drawPos = GetDrawPosVect(position_);

	//服
	Sprite::GetInstance().SplitDraw(SPRITE_ID::BASE_CLOTHES_04_SPRITE, Vector2(drawPos.x, drawPos.y - 100.0f), 0, Vector2(100, 100), Vector2(2.0f, 2.0f));
	//名前
	Sprite::GetInstance().SplitDraw(spriteId_, drawPos, frame_, Vector2(parameter_.size.x / 2.0f, parameter_.size.y / 2.0f), Vector2::One);

	if (BuildMode != 1) return;
	if (!collisionPoints.empty()) {
		auto drawP1 = GetDrawPosVect(collisionPoints[0]);
		auto drawP2 = GetDrawPosVect(collisionPoints[1]);
		auto drawP3 = GetDrawPosVect(collisionPoints[2]);
		auto drawP4 = GetDrawPosVect(collisionPoints[3]);
		DebugDraw::DebugDrawCircle(drawP1.x, drawP1.y, parameter_.radius, GetColor(255, 255, 255));
		DebugDraw::DebugDrawCircle(drawP2.x, drawP2.y, parameter_.radius, GetColor(255, 255, 255));
		DebugDraw::DebugDrawCircle(drawP3.x, drawP3.y, parameter_.radius, GetColor(255, 255, 255));
		DebugDraw::DebugDrawCircle(drawP4.x, drawP4.y, parameter_.radius, GetColor(255, 255, 255));
		DebugDraw::DebugDrawLine(drawP1.x, drawP1.y, drawP2.x, drawP2.y, GetColor(255, 255, 255));
		DebugDraw::DebugDrawLine(drawP2.x, drawP2.y, drawP3.x, drawP3.y, GetColor(255, 255, 255));
		DebugDraw::DebugDrawLine(drawP3.x, drawP3.y, drawP4.x, drawP4.y, GetColor(255, 255, 255));
	}

}

//衝突判定
void CreditText::OnCollide(Actor & other, CollisionParameter colpara)
{
	//判定が重なっていたら一つだけをつかむ
	if (other.GetParameter().ID != ACTOR_ID::PLAYER_HEAD_ACTOR) return;
	auto temp = static_cast<CreditPlayer*>(other.GetParent());
	if (temp->GetIsBiteMode()) return;
	parent_ = &other;
	static_cast<Player_Head*>(const_cast<Actor*>(parent_))->setIsBiteSlipWind(false);
	cPlayer_ = temp;
	cPlayer_->CurHeadBite(other.GetPosition());
	cPlayer_->SetIsBiteMode(true);
}

//メッセージ処理
void CreditText::OnMessage(EventMessage message, void * param)
{
	switch (message)
	{
	case EventMessage::OPERATE_FLAG:
		if ((int)param < 1) {
			cPlayer_ = nullptr;
		}
		break;
	}
}
