#include "BossWingEffect.h"
#include "../../../../../ResourceLoader/ResourceLoader.h"

BossWingEffect::BossWingEffect(
	IWorld * world, 
	const Vector2 & position, 
	const float bodyScale) :
	Actor(world, "WingEffect", position, 
		CollisionBase(const_cast<Vector2&>(position), bodyScale)),
	rotaSpeed_(0.0f),
	degree_(0.0f),
	texDegree_(0.0f),
	stateTimer_(0.0f),
	bezierTimer_(0.0f),
	flyPower_(0.0f),
	//alpha_(255.0f),
	isTexTurn_(false),
	direction_(Vector2::One),
	color_(Vector3::Zero),
	state_(State::FlyOut),
	mt_(std::mt19937())
{
	// 乱数の取得
	std::random_device random;
	// メルセンヌツイスター法 後で調べる
	// 初期Seed値を渡す
	std::mt19937 mt(random());
	mt_ = mt;
	if (getRandomInt(0, 1) > 0)
		direction_.x = 1;
	else {
		direction_.x = -1;
		isTexTurn_ = true;
	}
	// 角度
	degree_ = (float)getRandomInt(20, 180 - 20);
	// 力
	flyPower_ = getRandomInt(200, 400) / 10.0f;
	// 回転速度
	rotaSpeed_ = getRandomInt(30, 40) / 10.0f;
	// 色
	color_ = Vector3::One * 255;
	color_.y = (float)getRandomInt(0, 255);
}

void BossWingEffect::onUpdate(float deltaTime)
{
	// 状態の更新
	switch (state_)
	{
	case State::FlyOut: flyOut(deltaTime); break;
	case State::Fall: fall(deltaTime); break;
	case State::Dead: deadMove(deltaTime); break;
	}
	// 透明処理
	alpha_ -= 255 * deltaTime;
	alpha_ = max(alpha_, 0.0f);
	if (alpha_ <= 0.0f)
		dead();

	stateTimer_ += deltaTime;
}

void BossWingEffect::onDraw() const
{
	auto vec3Pos = Vector3(position_.x, position_.y, 0.0f);
	vec3Pos = vec3Pos * inv_;
	auto pos = Vector2(vec3Pos.x, vec3Pos.y);
	// 画像の表示
	auto scale = 16.0f;
	// αブレンドの設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
	SetDrawBright((int)color_.x, (int)color_.y, (int)color_.z);
	DrawRotaGraph3(
		(int)pos.x, (int)pos.y,
		(int)body_.GetCircle().getRadius(), (int)body_.GetCircle().getRadius(),
		body_.GetCircle().getRadius() / scale,
		body_.GetCircle().getRadius() / scale, MathHelper::ToRadians(texDegree_),
		ResourceLoader::GetInstance().getTextureID(TextureID::BOSS_WING_TEX), 1, isTexTurn_);
	SetDrawBright(255, 255, 255);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

void BossWingEffect::onCollide(Actor & actor)
{
	auto actorName = actor.getName();
	auto getFloorName = strstr(actorName.c_str(), "Floor");
	//床関連のオブジェクトに当たっているなら死亡処理
	if (getFloorName != NULL || actorName == "Door") {
		// 位置の補間
		groundClamp(actor);
		//changeState(State::Dead);
		return;
	}
}

// 状態の変更
void BossWingEffect::changeState(State state)
{
	if (state_ == state)return;
	state_ = state;
}

// 排出状態
void BossWingEffect::flyOut(float deltaTime)
{
	position_ += flyPower_ * 
		Vector2(MathHelper::Cos(degree_), -MathHelper::Sin(degree_)) * 
		(deltaTime * 60.0f);
	// 力の減算
	flyPower_ -= deltaTime * 22.0f * 10.0f;
	flyPower_ = max(flyPower_, 0.0f);
	// 力が無くなったら、落下状態に遷移
	if (flyPower_ == 0.0f) 
		changeState(State::Fall);
}

// 落下状態
void BossWingEffect::fall(float deltaTime)
{
	//auto time = deltaTime / 5;
	auto time = deltaTime / 5 * (rotaSpeed_ / 4.0f);
	// 移動量
	auto velo = bezier(time, Vector2(2.0f, 1.0f));
	// 簡易摩擦力
	// rotaSpeed_ = 4.0f;
	auto f = bezier(time, Vector2(0.0f, 0.0f), Vector2(0.0f, 1.0f), Vector2(0.0f, 0.0f));
	texDegree_ -= direction_.x * (1 - velo.y) * (rotaSpeed_ * f.y) * (deltaTime * 60.0f);
	position_ += direction_ * velo *  4.0f * (deltaTime * 60.0f);
	// ベジェが終わったら、再度行う
	if (bezierTimer_ >= 1.0f) {
		bezierTimer_ = 0.0f;
		direction_.x *= -1.0f;
	}
}

// 死亡状態
void BossWingEffect::deadMove(float deltaTime)
{
	/*alpha_ -= 255 * (deltaTime / 2);
	alpha_ = max(alpha_, 0.0f);
	if (alpha_ <= 0.0f)
		dead();*/
	//dead();
}

// ベジェ曲線を取得します(始点・終点0)
Vector2 BossWingEffect::bezier(float deltaTime, const Vector2 & direPoint)
{
	/*auto startPoint = Vector2(0.0f, 0.0f);auto endPoint = Vector2(0.0f, 0.0f);*/
	return bezier(deltaTime, Vector2::Zero, direPoint, Vector2::Zero);
}

// ベジェ曲線を取得します(始点と終点の指定)
Vector2 BossWingEffect::bezier(
	float deltaTime, const Vector2 & startPoint, 
	const Vector2 & direPoint, const Vector2 & endPoint)
{
	bezierTimer_ += deltaTime;
	bezierTimer_ = min(bezierTimer_, 1.0f);

	auto x = (1.0f - bezierTimer_) * (1.0f - bezierTimer_) * startPoint.x +
		2 * (1.0f - bezierTimer_) * bezierTimer_ * direPoint.x +
		bezierTimer_ * bezierTimer_ * endPoint.x;

	auto y = (1.0f - bezierTimer_) * (1.0f - bezierTimer_) * startPoint.y +
		2 * (1.0f - bezierTimer_) * bezierTimer_ * direPoint.y +
		bezierTimer_ * bezierTimer_ * endPoint.y;

	return Vector2(x, y);
}

//地面の位置に補正します
void  BossWingEffect::groundClamp(Actor & actor)
{
	if (actor.body_.GetBox().getWidth() == 0) return;
	// 新円と正方形の衝突判定
	// 自分自身の1f前の中心位置を取得
	auto isHit = false;
	auto pos = body_.GetCircle().previousPosition_;
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
	// 過去の位置
	// Y方向に位置を補間する
	if (left < 0 &&
		right < 0) {
		// 上に補間
		if (top > -actor.getBody().GetBox().getHeight() / 3) {
			position_.y = topLeft.y - body_.GetCircle().getRadius();
			changeState(State::Dead);
			isHit = true;
			return;
		}
		// 下に補間
		if (bottom > -actor.getBody().GetBox().getHeight() / 3) {
			position_.y = bottomRight.y + body_.GetCircle().getRadius();
			isHit = true;
		}
	}
	// X方向に位置を補間する
	else if (top < 0 &&
		bottom < 0) {
		// 左に補間
		if (left > 0) {
			position_.x = bottomLeft.x - body_.GetCircle().getRadius();
			isHit = true;
		}
		// 右に補間
		if (right > 0) {
			position_.x = topRight.x + body_.GetCircle().getRadius();
			isHit = true;
		}
	}
	if (isHit) return;

	// 現在の判定
	pos = position_;
	// 外積を使って、縦の長さを計算する
	top = Vector2::Cross(
		(topLeft - topRight).Normalize(), (pos - topRight));
	bottom = Vector2::Cross(
		(bottomRight - bottomLeft).Normalize(), (pos - bottomLeft));
	right = Vector2::Cross(
		(topRight - bottomRight).Normalize(), (pos - bottomRight));
	left = Vector2::Cross(
		(bottomLeft - topLeft).Normalize(), (pos - topLeft));
	// Y方向に位置を補間する
	if (left < 0 &&
		right < 0) {
		// 上に補間
		if (top > -actor.getBody().GetBox().getHeight() / 2.0f) {
			position_.y = topLeft.y - body_.GetCircle().getRadius();
			changeState(State::Dead);
			return;
		}
		// 下に補間
		if (bottom > -actor.getBody().GetBox().getHeight() / 2.0f) {
			position_.y = bottomRight.y + body_.GetCircle().getRadius();
		}
	}
	// X方向に位置を補間する
	if (top < 0 &&
		bottom < 0) {
		// 左に補間
		if (left > -actor.getBody().GetBox().getWidth() / 2.0f)
			position_.x = bottomLeft.x - body_.GetCircle().getRadius();
		// 右に補間
		if (right > -actor.getBody().GetBox().getWidth() / 2.0f)
			position_.x = topRight.x + body_.GetCircle().getRadius();
	}
}

// ランダムの値を取得します
int BossWingEffect::getRandomInt(const int min, const int max)
{
	// 範囲の指定(int型)
	std::uniform_int_distribution<> value(min, max);
	return value(mt_);
}
