#include "BaseBoss.h"
#include "../../../../ResourceLoader/ResourceLoader.h"
#include "../../../Base/ActorGroup.h"
#include "../../../Body/CollisionBase.h"
#include "../FloorSearchPoint.h"
#include "BossManager.h"
#include "BossEntry.h"
#include "MiniBoss.h"
#include "MiniBoss/FlyingMiniBoss.h"
#include "bossAttack/importBossAttack.h"
#include "Effect/ImportEffects.h"
#include "../../../../World/IWorld.h"
#include "../../../../Math/Math.h"
// ボスの体力表示
#include "../../../UIActor/BossGaugeUI/BossGaugeUI.h"
#include "../../../../Define.h"
// デバッグ
#include "../../../../Input/InputMgr.h"
#include "MiniBoss/WingAttackMiniBoss.h"

// ボスクラス(ベース予定)
BaseBoss::BaseBoss(
	IWorld * world,
	const Vector2 & position,
	const float bodyScale) :
	Actor(world, "Boss", position,
		CollisionBase(const_cast<Vector2&>(position), bodyScale)),
	dp_(10),
	hp_(200),
	attackCount_(0),
	currentACount_(attackCount_),
	flinchCount_(0),
	piyoriCount_(5),
	bokoCreateCount_(0),
	miniBossCreateCount_(0),
	//alpha_(255),
	starCount_(0),
	allStarCount_(0),
	stateTimer_(0.0f),
	timer_(0.0f),
	deltaTimer_(0.0f),
	liftMoveTiemr_(0.0f),
	angle_(0.0f),
	effectCreateTimer_(0.0f),
	mbTimer_(4.0f),
	liftCount_(0.0f),
	bgmVolume_(1.0f),
	isGround_(false),
	isBottomHit_(false),
	isAttackHit_(false),
	isSceneEnd_(false),
	isBattle_(false),
	isEffectCreate_(true),
	isPlayerCollide_(true),
	isBossDead_(false),
	playerPastPosition_(Vector2::Zero),
	direction_(Vector2::One),
	player_(nullptr),
	state_(State::BattleIdel),
	attackState_(AttackState::JumpAttack),
	animeNum_(WAIT_NUMBER),
	animation_(EnemyAnimation2D()),
	wspObj_(nullptr),
	entryObj_(nullptr),
	mbManager_(MiniBossManager()),
	bossGaugeUI_(nullptr),
	bossManager_(BossManager(world, position)),
	movePos_(Vector2::Zero),
	moveSpeed_(0.0f),
	mt_(std::mt19937())
{
	// コンテナの初期化
	initContainer();
	// ボスマネージャーに攻撃を追加
	bossManager_.addAttack(std::make_shared<ThreeJumpAttack>(world_, position));
	bossManager_.addAttack(std::make_shared<WallAttack>(world_, position));	
	// アニメーションの追加
	addAnimation();
	animation_.changeAnimation(
		static_cast<int>(WAIT_NUMBER));
	// オブジェクトの追加
	createObject();
	// 初期seedの確定
	// 乱数の取得
	std::random_device random;
	// メルセンヌツイスター法 後で調べる
	std::mt19937 mt(random());
	// 初期Seed値を渡す
	mt_ = mt;
	//// 衝突判定を一回無くす
	//body_.enabled(false);
}

BaseBoss::~BaseBoss()
{
	asContainer_.clear();
	asAnimations_.clear();
	lockHps_.clear();
	stars_.clear();
}

void BaseBoss::onUpdate(float deltaTime)
{
	deltaTimer_ = deltaTime * 60.0f;
	// 補間タイマ(最大値１)の更新
	setTimer(deltaTime);
	// 体力の更新
	bossGaugeUI_->SetHp(hp_);
	// アニメーションの更新
	animation_.update(deltaTime);
	// 所持しているオブジェクトの位置更新
	entryObj_->setBossPosition(position_ + Vector2::Left * 50);
	wspObj_->setPosition(position_);
	// 状態の更新
	updateState(deltaTime);
	// SE
	poseStopSE();
	poseRestartSE();
	if (attackCount_ == 1 && !isBossDead_) {
		mbTimer_ = max(mbTimer_ - deltaTime, 0.0f);
		// ミニボスの生成
		createMiniBoss();
	}
	// 接地(仮)
	bossManager_.setIsGround(isGround_);
	bossManager_.setIsBottom(isBottomHit_);
	// bool系
	isGround_ = false;
	isBottomHit_ = false;
}

void BaseBoss::onEnd()
{
	bossManager_.attackRefresh();
	auto se = ResourceLoader::GetInstance().getSoundID(SoundID::SE_BOSS_POKO);
	// SEが再生中なら、止める
	if (CheckSoundMem(se) == 1)
		StopSoundMem(se);
}

void BaseBoss::onDraw() const
{
	auto vec3Pos = Vector3(position_.x, position_.y, 0.0f);
	vec3Pos = vec3Pos * inv_;
	// アニメーションの描画
	auto pos = Vector2(vec3Pos.x, vec3Pos.y);
	// αブレンドの設定
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
	animation_.draw(
		pos - Vector2::Up * 10,
		Vector2::One * (body_.GetCircle().getRadius()) + Vector2::Up * 20,
		body_.GetCircle().getRadius() / (128 / 2), angle_);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

void BaseBoss::onCollide(Actor & actor)
{
	auto actorName = actor.getName();
	//床関連のオブジェクトに当たっているなら
	auto getFloorName = strstr(actorName.c_str(), "Floor");
	auto getBlockName = strstr(actorName.c_str(), "Block");
	// バトル待機状態なら、触れた壁を壊す
	if (state_ == State::BattleIdel && 
		(getFloorName != NULL)) {
		actor.dead();
		auto se =
			ResourceLoader::GetInstance().getSoundID(SoundID::SE_BOSS_CHAKUCHI);
		PlaySoundMem(se, DX_PLAYTYPE_BACK);
		return;
	}
	// 運ばれる状態なら、ドアを壊す
	else if (state_ == State::LiftMove && actorName == "Door") {
		actor.dead();
		auto se =
			ResourceLoader::GetInstance().getSoundID(SoundID::SE_BOSS_CHAKUCHI);
		if (CheckSoundMem(se) == 0)
			PlaySoundMem(se, DX_PLAYTYPE_BACK);
		return;
	}
	// マップのブロックに当たったら、補間処理を行う
	if (getFloorName != NULL || getBlockName != NULL || actorName == "Door") {
		// 位置の補間
		if (state_ == State::BattleIdel) return;
		groundClamp(actor);
		bossManager_.setFloorName(actorName.c_str());
		return;
	}
	// プレイヤーの攻撃に当たらない場合は返す
	//if (!isAttackHit_ || !isPlayerCollide_) return;
	// if (damageTimer_ > 0)
	if (!isAttackHit_ || state_ == State::Flinch || isBossDead_) return;
	// プレイヤーの攻撃範囲に当たった場合の処理
	if (actorName == "PlayerAttackCollider") {
		// ダメージ処理
		auto addDamage = 0.0f;
		// ゼロ除算防止
		if (allStarCount_ != 0)
			addDamage = starCount_ / (float)allStarCount_;
		addDamage = min(addDamage, 1.0f);

		//// //仮
		//auto addDamage = 1.0f;
		//// // 仮終わり

		auto d = (int)(3 + 27 * addDamage);
		// 二段階目なら、ダメージ半減
		if (attackCount_ == 1)
			d /= 2;
		isPlayerCollide_ = false;
		damage(d, actor.getPosition(), 2.0f);
		// ボスマネージャーに設定
		bossManager_.setCollideObj(actor);
		return;
	}
}

void BaseBoss::onMessage(EventMessage event, void *){}

// シーンを終了させるかを返します
bool BaseBoss::isSceneEnd()
{
	return isSceneEnd_;
}

// 目的地に移動します
void BaseBoss::movePosition(float deltaTime)
{
	// 目的地と自分の方向を出す
	auto distance = movePos_ - position_;
	// 目的地と速度の差が速度以下なら、位置を変える
	if (distance.Length() < moveSpeed_) {
		position_ = movePos_;
	}
	else
		position_ += Vector2::Normalize(distance)
		* moveSpeed_ * (deltaTime * 60.0f);
}

// 戦闘を開始するかを返します
void BaseBoss::setIsBattle(bool isBattle)
{
	isBattle_ = isBattle;
}

// 星の獲得数を設定します
void BaseBoss::setStarCount(const int count, const int all)
{
	starCount_ = count;
	allStarCount_ = all;
}

// 目的の位置を設定します
void BaseBoss::setMovePosition(const Vector2 & position, const float speed)
{
	movePos_ = position;
	moveSpeed_ = speed;
}

bool BaseBoss::isMovePosition()
{
	if (movePos_.x == position_.x &&
		movePos_.y == position_.y)
		return true;
	return false;
}

void BaseBoss::updateState(float deltaTime)
{
	if (InputMgr::GetInstance().IsKeyDown(KeyCode::G)) {
		attackCount_ = 1;
	}

	player_ = world_->findActor("PlayerBody1");
	// 体力が0以下になったら死亡
	if (hp_ <= 0 && !isBossDead_) {
		name_ = "DeadBoss";
		isAttackHit_ = false;
		// エフェクトの削除
		auto effect = world_->findActor("EntrySignEffect");
		if (effect != nullptr)
			effect->dead();
		PlaySoundMem(
			ResourceLoader::GetInstance().getSoundID(SoundID::SE_BOSS_DEAD),
			DX_PLAYTYPE_BACK);
		changeState(State::Dead, DEATH_NUMBER);
		isBossDead_ = true;
	}
	// 状態の更新
	switch (state_)
	{
	case State::BattleIdel: battleIdel(deltaTime); break;
	case State::Idel: idel(deltaTime); break;
	case State::Attack: attack(deltaTime); break;
	case State::Damage: damage(deltaTime); break;
	case State::Flinch: flinch(deltaTime); break;
	case State::Piyori: piyori(deltaTime); break;
	case State::Boko: boko(deltaTime); break;
	case State::Dead: deadMove(deltaTime); break;
	case State::LiftIdel: liftIdel(deltaTime); break;
	case State::LiftMove: liftMove(deltaTime); break;
	}

	stateTimer_ += deltaTime;
}

void BaseBoss::changeState(State state, int num)
{
	// アニメーションの変更
	if (animeNum_ != num) {
		animation_.setIsLoop(true);
		animation_.changeAnimation(static_cast<int>(num));
	}
	animeNum_ = num;
	alpha_ = 255;
	// 同じ状態なら返す
	if (state_ == state) return;
	state_ = state;
	stateTimer_ = 0.0f;
	angle_ = 0.0f;
}

void BaseBoss::changeAttackState(AttackState aState, int num)
{
	// 攻撃状態に強制遷移する
	changeState(State::Attack, num);
	// 画像の方向を合わせる
	animation_.changeDirType(direction_.x);
	attackState_ = aState;
	bossManager_.prevPosition();
}

// 戦闘待機状態
void BaseBoss::battleIdel(float deltaTime)
{
	body_.enabled(true);
	// 目的地が設定されていないなら返す
	if (movePos_.x == Vector2::Zero.x &&
		movePos_.y == Vector2::Zero.y) return;
	// 目的地に移動
	movePosition(deltaTime);
	setBMStatus();
	// バトル開始でないなら返す
	if (!isBattle_) return;
	// ジャンプ攻撃状態に遷移
	changeAttackState(AttackState::JumpAttack, JUMP_UP_NUMBER);
	isAttackHit_ = true;
	//body_.enabled(true);
}

void BaseBoss::idel(float deltaTime)
{
	// プレイヤーが取得できていれば、エネミーマネージャーに位置などを入れる
	setBMStatus();
	// アルファ値の設定
	texAlpha(deltaTime);
	// 画像の方向を合わせる
	animation_.changeDirType(direction_.x);
	// 次に攻撃する行動を決定する
	if (!isACountDecision_) {
		// 攻撃行動のカウントで行動を決める
		currentACount_ = attackCount_;
		// 例外が出たために補間	
		if (attackCount_ != 0)
			currentACount_ = getRandomInt(0, 1);
		bossManager_.changeAttackNumber(currentACount_);
		isACountDecision_ = true;
	}
	// 一定時間経過で攻撃状態に遷移
	if (stateTimer_ >= 5.0f) {
		// 攻撃行動のカウントで行動を決める
		changeAttackState(
			asContainer_[currentACount_],
			asAnimations_[currentACount_]);
		isACountDecision_ = false;
		return;
	}
	// 重力
	if (!isGround_ && bossManager_.isUseGravity())
		position_.y += 9.8f * (deltaTime * 60.0f);
}

void BaseBoss::attack(float deltaTime)
{
	bossManager_.attackMove(deltaTime);
	position_ = bossManager_.getMovePosition();
	// アニメーションの変更
	animation_.changeAnimation(
		static_cast<int>(bossManager_.getAnimaNum()));
	animation_.setIsLoop(bossManager_.isAnimeLoop());
	// 攻撃状態の選択
	switch (attackState_) {
	case AttackState::JumpAttack: jumpAttack(deltaTime); break;
	case AttackState::WallAttack: wallAttack(deltaTime); break;
	}
	// アルファ値の設定
	texAlpha(deltaTime);
	// 攻撃が当たるかの判定を入れる
	isAttackHit_ = bossManager_.IsAttackHit();
}

// ダメージ状態
void BaseBoss::damage(float deltaTime)
{
	//name_ = "DamageBoss";
	animation_.setIsLoop(false);
	if (stateTimer_ > 0.5f) {
		changeState(State::Idel, WAIT_NUMBER);
		isPlayerCollide_ = true;
		isAttackHit_ = false;
		name_ = "Boss";
	}
}

// 怯み状態
void BaseBoss::flinch(float deltaTime)
{
	name_ = "FlinchBoss";
	flinchCount_ = 0;
	piyoriMove(deltaTime);
}

// ぴより状態
void BaseBoss::piyori(float deltaTime)
{
	name_ = "PiyoriBoss";
	piyoriCount_ = 5;
	piyoriMove(deltaTime);
}

void BaseBoss::boko(float deltaTime)
{
	name_ = "BokoBoss";
	// 煙の生成
	if (isEffectCreate_ && (int)effectCreateTimer_ % 50 <= 24 && bokoCreateCount_ < 2) {
		// エフェクトの追加
		auto addPos = Vector2::One * -40.0f;
		world_->addActor(ActorGroup::Effect,
			std::make_shared<BokoEffect>(world_, position_ + addPos, (int)(255 * 0.7f)));
		// 羽エフェクトの生成
		world_->addActor(ActorGroup::Effect,
			std::make_shared<ScatterWingEffect>(world_, position_ - Vector2::Up * 150.0f));
		bokoCreateCount_++;
		isEffectCreate_ = false;
	}
	else if ((int)effectCreateTimer_ % 50 > 24) {
		isEffectCreate_ = true;
	}
	effectCreateTimer_ += deltaTimer_;
	if (stateTimer_ < 2.0f) return;
	direction_.x = -1.0f;
	animation_.changeDirType(direction_.x);
	// 出た瞬間にSE再生
	if(world_->isEntered())
		PlaySoundMem(
			ResourceLoader::GetInstance().getSoundID(SoundID::SE_BOSS_DAMAGE),
			DX_PLAYTYPE_BACK);
	world_->setEntry(false, true);
	entryObj_->setIsEntry(false);
	if (stateTimer_ >= 3.0f) {
		name_ = "Boss";
		bokoCreateCount_ = 0;
		effectCreateTimer_ = 0.0f;
		isEffectCreate_ = true;
		isPlayerCollide_ = true;
		isAttackHit_ = false;
		changeState(State::Idel, WAIT_NUMBER);
	}
}

void BaseBoss::deadMove(float deltaTime)
{
	animation_.setIsLoop(false);
	// 重力
	position_.y += 9.8f * deltaTimer_;
	if (stateTimer_ < 3.0f) return;
	// BGMの音量の調整
	auto bgm = ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_5);
	if (CheckSoundMem(bgm) == 1) {
		bgmVolume_ -= deltaTime / 4.0f;
		bgmVolume_ = (float)max(bgmVolume_, 0.0f);
		ChangeVolumeSoundMem((int)(255 * bgmVolume_), bgm);
		// 音量が0になったら止める
		if (bgmVolume_ == 0.0f) {
			StopSoundMem(bgm);
			ChangeVolumeSoundMem(255, bgm);
		}
	}
	// ミニボスの生成
	if (isEffectCreate_ && miniBossCreateCount_ < 30 && 
		(int)effectCreateTimer_ % 10 <= 5) {
		auto miniBoss = std::make_shared<MiniBoss>(
			world_, Vector2(position_.x, 1020), (float)getRandomInt(40, 80) / 100);
		world_->addActor(ActorGroup::EnemyBullet, miniBoss);
		mbManager_.addMiniBoss(miniBoss.get());
		if (miniBossCreateCount_ % 2 == 0)
			PlaySoundMem(
				ResourceLoader::GetInstance().getSoundID(SoundID::SE_MINIBOSS_CRY),
				DX_PLAYTYPE_BACK);
		miniBossCreateCount_++;
		isEffectCreate_ = false;
	}
	else if ((int)effectCreateTimer_ % 10 > 5) {
		isEffectCreate_ = true;
	}
	effectCreateTimer_ += deltaTimer_;

	if (miniBossCreateCount_ >= 30) {
		mbManager_.backToSanity();
		changeState(State::LiftIdel, DEATH_NUMBER);
	}
}

// ぴより行動
void BaseBoss::piyoriMove(float deltaTime)
{
	isPlayerCollide_ = false;
	entryObj_->setIsEntry(true);
	auto dir = direction_;
	dir.y = 1.0f;
	entryObj_->setDirection(dir);
	animation_.setIsLoop(true);
	// 星の生成
	if (isEffectCreate_ && (int)effectCreateTimer_ % 50 <= 24 && stars_.size() < 6) {
		auto star = std::make_shared<PiyoriEffect>(
			world_, position_);
		world_->addActor(ActorGroup::Effect, star);
		stars_.push_back(star);
		isEffectCreate_ = false;
	}
	else if ((int)effectCreateTimer_ % 50 > 24) {
		isEffectCreate_ = true;
	}
	effectCreateTimer_ += deltaTimer_;
	// ぴよりエフェクトの位置
	for (auto i = stars_.begin(); i != stars_.end(); i++) {
		auto addPos = Vector2::Up * -150;
		i->get()->position_ = position_ + addPos;
	}
	// 重力
	if (!isGround_ && bossManager_.isUseGravity()) {
		position_.y += 9.8f * deltaTimer_;// * 60.0f);
	}
	// 口の印の更新
	auto posEffect = world_->findActor("EntrySignEffect");
	auto addPos = Vector2::Up * -225;
	if (posEffect != nullptr) {
		posEffect->position_ = entryObj_->getPosition() + addPos;
	}
	// 体内に入っていたら、ハートに入ったことを知らせる
	if (entryObj_->isEntered()) {
		// プレイヤーが出てきたら、待機状態にする
		stateTimer_ = 5.0f;
		// スターの削除
		for (auto i = stars_.begin(); i != stars_.end(); i++) {
			auto star = *i;
			star->dead();
		}
		// エフェクトの削除
		auto effect = world_->findActor("EntrySignEffect");
		if (effect != nullptr)
			effect->dead();
		stars_.clear();
		effectCreateTimer_ = 0.0f;
		entryObj_->letOut();
		isEffectCreate_ = true;
		// 状態によって行動を変える
		if (state_ == State::Flinch) {
			// カウントを減らす
			piyoriCount_ = max(piyoriCount_--, 0);
			// ダメージ処理
			auto d = 30;
			damage(d, entryObj_->getPosition(), 0.8f);
			return;
		}
		else if (state_ == State::Piyori) {
			if (attackCount_ < asContainer_.size() - 1)
				attackCount_++;
			// ぼこり状態に変更
			changeState(State::Boko, DAMAGE_BOKO_NUMBER);
			PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_BOSS_POKO), 
				DX_PLAYTYPE_BACK, false);
			isAttackHit_ = false;
			world_->setEntry(true, false);
			return;
		}
	}
	// 一定時間経過で待機状態に遷移
	if (stateTimer_ < 5.0f) return;
	changeState(State::Idel, WAIT_NUMBER);
	name_ = "Boss";
	isAttackHit_ = true;
	isPlayerCollide_ = true;
	entryObj_->setIsEntry(false);
	isEffectCreate_ = true;
	// エフェクトの削除
	auto effect = world_->findActor("EntrySignEffect");
	if (effect != nullptr)
		effect->dead();
	// スターの削除
	for (auto i = stars_.begin(); i != stars_.end(); i++) {
		auto star = *i;
		star->dead();
	}
	stars_.clear();
	effectCreateTimer_ = 0.0f;
}

// 持ち上げ待機状態
void BaseBoss::liftIdel(float deltaTime)
{
	if (!mbManager_.isMiniBossAllBP()) return;
	mbManager_.bossLift();
	if (liftCount_ < 40.0f) {
		auto power = 5.0f * (deltaTime * 60.0f);
		liftCount_ += power;
		position_.y -= power;
	}
	if (mbManager_.isLiftEnd()) {
		changeState(State::LiftMove, DEATH_NUMBER);
		PlaySoundMem(
			ResourceLoader::GetInstance().getSoundID(SoundID::SE_MINIBOSS_CRY),
			DX_PLAYTYPE_BACK);
	}
}

// 持ち上げ移動状態
void BaseBoss::liftMove(float deltaTime)
{
	auto speed = 20.0f;
	position_.x += -speed * (deltaTime * 60.0f);
	mbManager_.LiftMove(speed);
	// 死亡から一定時間経過なら、シーンを終了させる
	if (position_.x < 0.0f) {
		liftMoveTiemr_ += deltaTime;
		if (liftMoveTiemr_ >= 1.0f)
			isSceneEnd_ = true;
	}
}

void BaseBoss::jumpAttack(float deltaTime)
{
	// ジャンプ攻撃
	bossManager_.setPlayerPosition(player_->getPosition());
	animation_.changeDirType(bossManager_.getAttackDirection().x);
	// ジャンプ攻撃が終わったら、待機状態にする
	if (bossManager_.isAttackEnd()) {
		animation_.setIsLoop(true);
		changeState(State::Idel, WAIT_NUMBER);
		// 攻撃が当たるかの判定を入れる
		isAttackHit_ = true;
		bossManager_.attackRefresh();
	}
}

void BaseBoss::wallAttack(float deltaTime)
{
	angle_ = static_cast<float>(bossManager_.getAnimeAngle());
	animation_.changeDirType(Vector2::Left.x);
	// 攻撃動作中なら、壁捜索オブジェクトなどの判定をONにする
	if (bossManager_.isAttackStart()) {
		setBMStatus();
		bossManager_.getPlayerNormalizeDirection();
	}
	// ジャンプ攻撃が終わったら、待機状態にする
	if (bossManager_.isAttackEnd()) {
		flinchCount_++;
		if (wspObj_->isGround())
			wspObj_->setDirection(Vector2(1.0f, -1.0f));
		// ボスマネージャ側のひるみ回数以上なら、ひるみ状態に遷移	
		if (bossManager_.getFlinchCount() > flinchCount_) {
			changeState(State::Idel, WAIT_NUMBER);
			// 攻撃が当たるかの判定を入れる
			isAttackHit_ = true;
			bossManager_.attackRefresh();
			return;
		}
		else if (bossManager_.getFlinchCount() <= flinchCount_) {
			changeState(State::Flinch, PIYO_NUMBER);
			flinchCount_ = 0;
			isAttackHit_ = false;
			// 怯み状態の設定
			setPiyori();
			// 方向の変更
			auto dir = Vector2::Left;
			if(bossManager_.getAttackDirection().x >= 0)
			dir.x = 1.0f;
			direction_.x = dir.x;
			animation_.changeDirType(dir.x);
			bossManager_.attackRefresh();
			return;
		}
	}
}

void BaseBoss::specialAttack(float deltaTime)
{
	// 口にプレイヤー同士をつなぐ線が当たり、
	// ひるむ時間に達するまでに
	// 口から離れたら、もう一度攻撃を行う
	if (bossManager_.isFlinch()) {
		changeState(State::Flinch, PIYO_NUMBER);
		bossManager_.attackRefresh();
		isAttackHit_ = false;
		// 怯み状態の設定
		setPiyori();
		return;
	}

	name_ = "suikomiBoss";
	// isBlock() == true => 塞がれている
	bossManager_.setIsAttackMove(!entryObj_->isBlock());
	bossManager_.attackMove(deltaTime);
	position_ = bossManager_.getMovePosition();
	// 壁に当たっている場合
	bossManager_.setIsWallHit(wspObj_->isGround());
	wspObj_->setDirection(bossManager_.getWallMoveDirection());
	// アニメーションの変更
	animation_.changeAnimation(
		static_cast<int>(bossManager_.getAnimaNum()));
	animation_.setIsLoop(bossManager_.isAnimeLoop());
	animation_.changeDirType(bossManager_.getAttackDirection().x);
	// アニメーションの逆再生を変更
	animation_.setIsReverse(bossManager_.isAnimeReverse());

	if (bossManager_.isAttackEnd()) {
		name_ = "Boss";
		changeState(State::Idel, WAIT_NUMBER);
		// 攻撃が当たるかの判定を入れる
		isAttackHit_ = true;
		bossManager_.attackRefresh();
	}
}

void BaseBoss::setTimer(float deltaTime)
{
	timer_ = deltaTime * 60.0f;
}

// ボスマネージャーのステータスの設定
void BaseBoss::setBMStatus()
{
	if (player_ != nullptr) {
		bossManager_.setPlayerPosition(player_->getPosition());
		bossManager_.setPosition(position_);
		// 方向を入れる
		auto direction = bossManager_.getPlayerDirection();
		direction_ = direction;
		// 壁に当たっている場合
		bossManager_.setIsWallHit(wspObj_->isGround());
		wspObj_->setDirection(bossManager_.getWallMoveDirection());
		direction.y = 1.0f;
		entryObj_->setDirection(direction);
	}
}

// 指定した値のダメージ量を加算します
void BaseBoss::damage(const int damage, const Vector2& position, const float scale)
{
	// 体力がロックよりも小さくなったら補間する
	hp_ = max(hp_ - damage, lockHps_[attackCount_]);
	// カウントを減らす
	if (attackCount_ == 0)
		piyoriCount_--;
	if (hp_ > lockHps_[attackCount_])
		piyoriCount_ = 1;
	// 待機中に攻撃を受けたら次の行動までに攻撃を受けないようにする
	if (state_ == State::Idel)
		isAttackHit_ = false;
	// エフェクトの生成
	world_->addActor(ActorGroup::Effect,
		std::make_shared<AttackEffect>(world_, position, scale));
	// 羽エフェクトの生成
	world_->addActor(ActorGroup::Effect,
		std::make_shared<ScatterWingEffect>(world_, position));
	// 耐久値が0になったら、ぴよる
	if (piyoriCount_ <= 0) {
		if (hp_ > lockHps_[attackCount_]) return;
		changeState(State::Piyori, PIYO_NUMBER);
		// 衝突するかどうかの bool を全てtrueにする
		piyoriCount_ = 5;
		setBMStatus();
		// 画像の方向を合わせる
		animation_.setIsLoop(true);
		animation_.changeDirType(direction_.x);
		bossManager_.attackRefresh();
		// 怯み状態の設定
		setPiyori();
		return;
	}
	else {
		if (state_ == State::Idel || state_ == State::Flinch) {
			setBMStatus();
			// 画像の方向を合わせる
			animation_.changeDirType(direction_.x);
			changeState(State::Damage, DAMAGE_NUMBER);
			PlaySoundMem(
				ResourceLoader::GetInstance().getSoundID(SoundID::SE_BOSS_DAMAGE),
				DX_PLAYTYPE_BACK);
			isAttackHit_ = false;
		}
	}
}

//地面の位置に補正します
void BaseBoss::groundClamp(Actor& actor)
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
			// 接地
			isGround_ = true;
			isHit = true;
		}
		// 下に補間
		if (bottom > -actor.getBody().GetBox().getHeight() / 3) {
			position_.y = bottomRight.y + body_.GetCircle().getRadius();
			// ブロックの下側に当たった
			isBottomHit_ = true;
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
	bossManager_.setPosition(position_);
	bossManager_.prevPosition();
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
			// 接地
			isGround_ = true;
		}
		// 下に補間
		if (bottom > -actor.getBody().GetBox().getHeight() / 2.0f) {
			position_.y = bottomRight.y + body_.GetCircle().getRadius();
			// ブロックの下側に当たった
			isBottomHit_ = true;
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
	bossManager_.setPosition(position_);
	bossManager_.prevPosition();
}

// テクスチャの追加を行います
void BaseBoss::addAnimation()
{
	// 待機
	animation_.addAnimation(
		static_cast<int>(WAIT_NUMBER),
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_WAIT_TEX));
	animation_.addAnimation(
		static_cast<int>(WAIT_TURN_NUMBER),
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_WAIT_TURN_TEX));
	// ジャンプ攻撃
	animation_.addAnimation(
		static_cast<int>(JUMP_UP_NUMBER),
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_JUMP_UP_TEX));
	animation_.addAnimation(
		static_cast<int>(JUMP_DOWN_NUMBER),
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_JUMP_DOWN_TEX));
	// 壁移動攻撃
	animation_.addAnimation(
		static_cast<int>(WALLATTACK_DASH_NUMBER),
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_WALLATTACK_DASH_TEX));
	animation_.addAnimation(
		static_cast<int>(WALLATTACK_DASHJUMP_NUMBER),
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_WALLATTACK_DASHJUMP_TEX));
	animation_.addAnimation(
		static_cast<int>(WALLATTACK_DASHJUMP_STOP_NUMBER),
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_WALLATTACK_DASHJUMP_STOP_TEX));
	animation_.addAnimation(
		static_cast<int>(BREATH_DYSFUNCTION_NUMBER),
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_BREATH_DYSFUNCTION_TEX));
	// 怯み
	animation_.addAnimation(
		static_cast<int>(PIYO_NUMBER),
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_PIYO_TEX));
	// ダメージ
	animation_.addAnimation(
		static_cast<int>(DAMAGE_NUMBER),
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_DAMAGE_TEX));
	animation_.addAnimation(
		static_cast<int>(DAMAGE_BOKO_NUMBER),
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_DAMAGE_BOKO_TEX));
	// 死亡
	animation_.addAnimation(
		static_cast<int>(DEATH_NUMBER),
		ResourceLoader::GetInstance().getAnimationIDs(
			AnimationID::BOSS_DEATH_TEX));
}

// ぴより状態の設定を行います
void BaseBoss::setPiyori()
{
	// 攻撃が当たるかの判定を入れる
	isAttackHit_ = false;
	// エフェクトの生成
	auto effect = std::make_shared<EntrySignEffect>(
		world_, entryObj_->getPosition() - Vector2::Up * 150.0f);
	world_->addActor(ActorGroup::Effect, effect);
}

// ボスのアルファ値を設定します
void BaseBoss::texAlpha(float deltaTime)
{
	if (!isAttackHit_) {
		if ((int)(stateTimer_ * 10) % 2 < 1) alpha_ -= (int)(deltaTime * 750);
		else alpha_ += deltaTime * 750;
		alpha_ = MathHelper::Clamp((float)alpha_, 100, 255);
	}
	else alpha_ = 255;
}

void BaseBoss::poseStopSE()
{
	// 停止(ポーズ)していなかったら返す
	if (deltaTimer_ > 0.0f) return;
	if (playSEHandles_.size() != 0) return;
	for (auto i = seHandles_.begin(); i != seHandles_.end(); i++) {
		if (CheckSoundMem(*i) == 1) {
			StopSoundMem(*i);
			playSEHandles_.push_back(*i);
		}
	}
}

// ポーズ解除時にSEを再度再生します
void BaseBoss::poseRestartSE()
{
	// サイズが0ならば返す
	if (playSEHandles_.size() == 0) return;
	if (deltaTimer_ == 0.0f) return;
	for (auto i = playSEHandles_.begin(); i != playSEHandles_.end(); i++) {
		// 格納したSEを再度再生する
		if (CheckSoundMem(*i) == 0)
			PlaySoundMem(*i, DX_PLAYTYPE_BACK, false);
	}
	// 中身をクリアする
	playSEHandles_.clear();
}

// 浮遊ミニボスの生成を行います
void BaseBoss::createMiniBoss()
{
	if (mbTimer_ > 0.0f) return;
	mbTimer_ = getRandomInt(40, 100) / 10.0f;
	// 方向
	int chipsize = static_cast<int>(CHIPSIZE);
	auto pos = Vector2::Zero;
	auto count = getRandomInt(0, 2);
	if (count < 2) {
		// 左辺・右辺に生成
		if (count == 0) pos.x = chipsize * 18.0f;
		else if (count == 1) pos.x = chipsize * 2.0f;
		pos.y = (float)getRandomInt(chipsize * 2, chipsize * 7);
	}
	else if (count == 2) {
		// 上辺に生成
		pos.y = chipsize * 2.0f;
		pos.x = (float)getRandomInt(chipsize * 2, chipsize * 18);
	}
	// ミニボス(浮遊)の生成
	auto miniBoss = std::make_shared<FlyingMiniBoss>(world_, pos);
	world_->addActor(ActorGroup::Enemy, miniBoss);
	PlaySoundMem(
		ResourceLoader::GetInstance().getSoundID(SoundID::SE_MINIBOSS_CRY),
		DX_PLAYTYPE_BACK);
}

// ランダムの値を取得します
int BaseBoss::getRandomInt(const int min, const int max)
{
	// 範囲の指定(int型)
	std::uniform_int_distribution<> value(min, max);
	return value(mt_);
}

// コンテナの初期化
void BaseBoss::initContainer()
{
	asContainer_.clear();
	asAnimations_.clear();
	lockHps_.clear();
	seHandles_.clear();
	playSEHandles_.clear();
	stars_.clear();
	// 攻撃状態をコンテナに追加(攻撃順に追加する)
	asContainer_.push_back(AttackState::JumpAttack);
	asContainer_.push_back(AttackState::WallAttack);
	// 攻撃アニメーションコンテナ
	asAnimations_.push_back(JUMP_UP_NUMBER);
	asAnimations_.push_back(WALLATTACK_DASH_NUMBER);
	// 体力ロックの追加
	lockHps_.push_back(100);
	lockHps_.push_back(0);
	// SEハンドルの追加
	seHandles_.push_back(ResourceLoader::GetInstance().getSoundID(SoundID::SE_BOSS_CHAKUCHI));
	seHandles_.push_back(ResourceLoader::GetInstance().getSoundID(SoundID::SE_BOSS_DAMAGE));
	seHandles_.push_back(ResourceLoader::GetInstance().getSoundID(SoundID::SE_BOSS_JUMP));
	seHandles_.push_back(ResourceLoader::GetInstance().getSoundID(SoundID::SE_BOSS_WALLATTACK));
	seHandles_.push_back(ResourceLoader::GetInstance().getSoundID(SoundID::SE_BOSS_POKO));
	seHandles_.push_back(ResourceLoader::GetInstance().getSoundID(SoundID::SE_BOSS_DEAD));
	seHandles_.push_back(ResourceLoader::GetInstance().getSoundID(SoundID::SE_MINIBOSS_CRY));
}

// オブジェクトの生成
void BaseBoss::createObject()
{
	auto radius = body_.GetCircle().getRadius();
	// 床捜索オブジェクト
	auto wspObj = std::make_shared<FloorSearchPoint>(
		world_, position_, Vector2::One * radius, 10.0f);
	world_->addActor(ActorGroup::Enemy, wspObj);
	wspObj_ = &*wspObj;
	// ボス入口オブジェクト
	/*auto entryObj = std::make_shared<BossEntry>(
		world_, position_ + Vector2::Left * 50,
		Vector2(bodyScale / 1.75f, -bodyScale / 1.25f),
		bodyScale / 4.0f);*/
	auto entryObj = std::make_shared<BossEntry>(
		world_, position_ + Vector2::Left * 50,
		Vector2(radius / 1.75f, -radius / 1.25f), radius / 4.0f);
	world_->addActor(ActorGroup::Enemy, entryObj);
	entryObj_ = &*entryObj;
	// ボスの体力ゲージ
	auto bossUI = std::make_shared<BossGaugeUI>(world_, Vector2(64, -256));
	world_->addUIActor(bossUI);
	bossGaugeUI_ = bossUI.get();
	bossGaugeUI_->SetHp(hp_);
	// デバッグ
	/*auto wmBoss = std::make_shared<WingAttackMiniBoss>(world_, Vector2(1000, 600));
	world_->addActor(ActorGroup::Enemy, wmBoss);*/
}
