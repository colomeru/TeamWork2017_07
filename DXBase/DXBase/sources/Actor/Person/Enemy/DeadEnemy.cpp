#include "DeadEnemy.h"
#include "EnemyHeaderImport.h"
#include "Bosses/Effect/EnemyDeadEffect.h"
#include "../../../Scene/Base/SceneDataKeeper.h"
#include "../../../World/IWorld.h"
#include "../../Item/Items.h"
#include "../../Base/ActorGroup.h"
#include "../../Body/CollisionBase.h"
#include "../../../ResourceLoader/ResourceLoader.h"

DeadEnemy::DeadEnemy(
	IWorld * world,
	const Vector2 & position,
	const float bodyScale,
	const Vector2 & direction,
	const AnimationID animaID) :
	Actor(world, "DeadEnemy", position, 
		CollisionBase(CollisionBase(
			Vector2(position.x + bodyScale / 2.0f, position.y + bodyScale / 2.0f),
			Vector2(position.x - bodyScale / 2.0f, position.y + bodyScale / 2.0f),
			Vector2(position.x + bodyScale / 2.0f, position.y - bodyScale / 2.0f),
			Vector2(position.x - bodyScale / 2.0f, position.y - bodyScale / 2.0f)
			))),
	stateTimer_(0.0f),
	playerSpeed_(0.0f),
	isGround_(false),
	holdLength_(0.0f),
	prevHoldDegree_(0.0f),
	throwDegree_(0.0f),
	playerPravPosition_(Vector2::Zero),
	holdPosition_(Vector2::Zero),
	prevPosition_(position),
	throwVector_(Vector2::Zero),
	color_(Vector3::One * 255.0f),
	isHold_(false),
	playerName_(""),
	otherName_(""),
	isBlockCollideBegin_(false),
	isBlockCollideEnter_(false),
	isBlockCollidePrevEnter_(false),
	isBlockCollideExit_(false),
	animation_(EnemyAnimation2D()),
	state_(State::Dead)
{
	auto id = 0;
	animation_.addAnimation(id,
		ResourceLoader::GetInstance().getAnimationIDs(animaID));
	animation_.changeAnimation(id);
	animation_.setIsLoop(false);
}

void DeadEnemy::onUpdate(float deltaTime)
{
	animation_.update(deltaTime);
	// 状態の更新
	updateState(deltaTime);
	// 衝突判定の更新
	updateCollide();
}

void DeadEnemy::onDraw() const
{
	auto vec3Pos = Vector3(position_.x, position_.y, 0.0f);
	vec3Pos = vec3Pos * inv_;
	// アニメーションの描画
	auto pos = Vector2(vec3Pos.x, vec3Pos.y);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
	SetDrawBright((int)color_.x, (int)color_.y, (int)color_.z);
	animation_.draw(
		pos, Vector2::One * (body_.GetBox().getWidth() * 2) + Vector2(0.0f, 40.0f),
		0.5f);
	SetDrawBright(255, 255, 255);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

void DeadEnemy::onCollide(Actor & actor)
{
	auto actorName = actor.getName();
	//auto getPlayerName = strstr(actorName.c_str(), "Player");
	// プレイヤー関連のオブジェクトに当たっているなら
	auto getFloorName = strstr(actorName.c_str(), "Floor");
	// マップのブロックに当たったら、処理を行う
	if (getFloorName != NULL) {
		groundClamp(actor);
		// 衝突中にtrueを入れる
		isBlockCollideEnter_ = true;
		// 過去の衝突中がfalseで、、過去と現在の衝突中が違うなら場合
		// 衝突直後になる
		if (!isBlockCollidePrevEnter_ &&
			isBlockCollidePrevEnter_ != isBlockCollideEnter_)
			isBlockCollideBegin_ = true;
		// つかみ状態で床に触れていたら、アイテムを排出する
		if (state_ == State::Hold && isBlockCollideBegin_ && playerSpeed_) {
			// アイテムの生成
			world_->addActor(ActorGroup::Item,
				std::make_shared<Items>(world_, position_));
		}
		return;
	}
	if (state_ == State::Throw || state_ == State::Delete) return;
	// プレイヤーのつかみ判定に衝突したら、つかみ状態に遷移
	if (actorName == "PlayerHoldCollider") {
		isHold_ = true;
		if (state_ == State::Hold) return;
		changeState(State::Hold);
		holdPosition_ = position_;
		prevPosition_ = position_;
		// プレイヤーの名前を設定します
		setPlayerName();
		return;
	}
}

void DeadEnemy::onMessage(EventMessage event, void *){}

// 状態の更新
void DeadEnemy::updateState(float deltaTime)
{
	switch (state_)
	{
	case State::Dead: deadMove(); break;
	case State::Hold: holdMove(deltaTime); break;
	case State::Throw: throwMove(deltaTime); break;
	case State::Delete: deleteMove(deltaTime); break;
	}
	// 重力
	if (!isGround_)
		position_.y += 9.8f * (deltaTime * 60.0f);

	stateTimer_ += deltaTime;
}

// 状態の変更
void DeadEnemy::changeState(State state)
{
	if (state_ == state) return;
	state_ = state;
	stateTimer_ = 0.0f;
}

// 死亡状態
void DeadEnemy::deadMove()
{
	if (stateTimer_ < 5.0f) return;
	changeState(State::Delete);
}

// 持ち上げ状態
void DeadEnemy::holdMove(float deltaTime)
{
	auto player = world_->findActor(playerName_);
	auto otherPlayer = world_->findActor(otherName_);
	// プレイヤーがいなければ死亡
	if (player == nullptr || otherPlayer == nullptr) {
		changeState(State::Delete);
		return;
	}
	// 角度を求める
	// つかんだプレイヤーともう片方のプレイヤーとのベクトルを求める
	auto playerVector = player->getPosition() - otherPlayer->getPosition();
	auto direction = Vector2(playerVector).Normalize();
	// 角度の計算
	auto degree = std::atan2(direction.y, direction.x) * 180.0f / MathHelper::Pi;
	// ホールドコライダに当たっていない場合は、投げ状態に遷移
	if (!isHold_) {
		changeState(State::Throw);
		throwVector_ = Vector2(prevPosition_ - position_);
		throwDegree_ = degree;
		return;
	}
	if (degree < 0) degree += 360.0f;
	// 床に触れているときは角度の修正
	if (isGround_) {
		if (degree > 180)
			degree = prevHoldDegree_;
		//degree = MathHelper::Clamp(degree, 0.0f, 180.0f);
	}
		
	auto degreePos = Vector2(
		MathHelper::Cos(degree),
		MathHelper::Sin(degree));
	// つかんでいる相手の位置 + 角度 * つかまれた時の長さ
	position_ = player->getPosition() + degreePos * holdLength_;
	// 星の生成
	auto length = Vector2(position_ - prevPosition_).Length();
	if (isBlockCollideBegin_ && length >= 20.0f) {
		auto star = std::make_shared<Items>(world_, position_);
		world_->addActor(ActorGroup::Item, star);
		starCount_++;
		if (starCount_ >= 5)
			changeState(State::Delete);
	}
	prevPosition_ = position_;
	prevHoldDegree_ = degree;
}

// 投げ状態
void DeadEnemy::throwMove(float deltaTime)
{
	auto timer = 0.0f;
	auto power = 15.0f;
	//position_.y += (power + stateTimer_) * (deltaTime * 60.0f);
	//position_ += throwVector_ * 2.0f;
	position_ += Vector2(
		MathHelper::Cos(throwDegree_), MathHelper::Sin(throwDegree_)) *
		(power + throwVector_.Length() / 10.0f);
	position_.y += stateTimer_ * 9.8f * (deltaTime * 60.0f);
	if (isGround_)
		changeState(State::Delete);
}

// 消滅状態
void DeadEnemy::deleteMove(float deltaTime)
{
	alpha_ = max(alpha_ - 255 * deltaTime , 0.0f);
	if (!isGround_)
		position_.y += 9.8f * (deltaTime * 60.0f);
	if (alpha_ > 0.0f) return;
	// 死亡エフェクトの追加
	world_->addActor(ActorGroup::Effect,
		std::make_shared<EnemyDeadEffect>(
			world_, position_ - Vector2::Up * 325.0f, 0));
	auto se =
		ResourceLoader::GetInstance().getSoundID(SoundID::SE_ENEMY_DEAD);
	PlaySoundMem(se, DX_PLAYTYPE_BACK);
	dead();
}

// 衝突関連の更新
void DeadEnemy::updateCollide()
{
	// bool系列
	// 接地をfalseにする
	isGround_ = false;
	isHold_ = false;
	// 最初に衝突直後と衝突後の判定をfalseにする
	isBlockCollideBegin_ = false;
	isBlockCollideExit_ = false;
	// 過去の衝突中がtureで、過去と現在の衝突中が違うなら場合
	// 衝突後になる
	if (isBlockCollidePrevEnter_ &&
		isBlockCollidePrevEnter_ != isBlockCollideEnter_) {
		// 衝突後の判定
		isBlockCollideBegin_ = false;
		isBlockCollideExit_ = true;
	}
	// 過去の衝突中に、現在の衝突中を入れる
	isBlockCollidePrevEnter_ = isBlockCollideEnter_;
	// ブロックに当たっていればtrueになるので、falseを入れる
	isBlockCollideEnter_ = false;
}

// 地面の位置に補正します
void DeadEnemy::groundClamp(Actor & actor)
{
	auto clampScale = 3.0f;
	if (actor.body_.GetBox().getWidth() == 0) return;
	// 正方形同士の計算
	// 自分自身の1f前の中心位置を取得
	auto pos = body_.GetBox().previousPosition_;
	// 相手側の四角形の4点を取得
	auto topLeft = actor.getBody().GetBox().component_.point[0];
	auto topRight = actor.getBody().GetBox().component_.point[1];
	auto bottomLeft = actor.getBody().GetBox().component_.point[2];
	auto bottomRight = actor.getBody().GetBox().component_.point[3];
	// 外積を使って、縦の長さを計算する
	auto top = Vector2::Cross(
		(topLeft - topRight).Normalize(), (pos - topRight));
	auto bottom = Vector2::Cross(
		(bottomRight - bottomLeft).Normalize(), (pos - bottomLeft));
	auto right = Vector2::Cross(
		(topRight - bottomRight).Normalize(), (pos - bottomRight));
	auto left = Vector2::Cross(
		(bottomLeft - topLeft).Normalize(), (pos - topLeft));
	// Y方向に位置を補間する
	if (left < body_.GetBox().getWidth() / clampScale &&
		right < body_.GetBox().getWidth() / clampScale) {
		// 上に補間
		if (top > 0) {
			position_.y = topLeft.y - body_.GetBox().getHeight() / 2.0f;
			// 接地
			isGround_ = true;
		}
		// 下に補間
		if (bottom > 0)
			position_.y = bottomRight.y + body_.GetBox().getHeight() / 2.0f;
	}
	// X方向に位置を補間する
	if (top < body_.GetBox().getHeight() / clampScale &&
		bottom < body_.GetBox().getHeight() / clampScale) {
		// 左に補間
		if (left > 0)
			position_.x = bottomLeft.x - body_.GetBox().getWidth() / 2.0f;
		// 右に補間
		if (right > 0)
			position_.x = topRight.x + body_.GetBox().getWidth() / 2.0f;
	}
}

// プレイヤーの名前を設定します
void DeadEnemy::setPlayerName()
{
	auto player1 = world_->findActor("PlayerBody1");
	auto player2 = world_->findActor("PlayerBody2");
	if (player1 == nullptr || player2 == nullptr) return;
	auto lenght1 = Vector2(position_ - player1->getPosition()).Length();
	auto lenght2 = Vector2(position_ - player2->getPosition()).Length();
	if (lenght1 < lenght2) {
		playerName_ = player1->getName();
		otherName_ = player2->getName();
		holdLength_ = Vector2(position_ - player1->getPosition()).Length();
		//holdPosition_ = player1->getPosition();
	}
	else {
		playerName_ = player2->getName();
		otherName_ = player1->getName();
		holdLength_ = Vector2(position_ - player2->getPosition()).Length();
		//holdPosition_ = player2->getPosition();
	}
	//// つかまれた位置との距離を計算
	//holdLength_ = Vector2(position_ - holdPosition_).Length();
}

// 色の変更を行います
void DeadEnemy::setColor(Vector3 color)
{
	color_ = color;
}

