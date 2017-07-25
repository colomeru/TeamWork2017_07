#include "Player.h"
#include "../../input/Keyboard.h"
#include "../../conv/DXConverter.h"
#include "../../graphic/Sprite.h"
#include"../../math/MathHelper.h"
#include"Player_Head.h"
#include"../../math/Matrix.h"
#include"../../math/MyFuncionList.h"
#include"../../input/GamePad.h"
#include"Player_Sword.h"
#include"../Effects/PlayerEffect/PlayerMetamorEffect.h"
#include"../Effects/PlayerEffect/PlayerBiteEffect.h"
#include"../Effects/PlayerEffect/GetSwordEffect.h"
#include"../Effects/PlayerEffect/ResurrectEffect.h"
#include"PlayerNeckDraw.h"
#include"PlayerDeadPin.h"
#include"PlayerDeadHead.h"
#include"../../sound/sound.h"
#include"../../cheat/CheatData.h"
#include"PlayerFallPin.h"
#include"PlayerNeck/PlayerNeckPendulumSupport.h"
#include"../../debugdata/DebugDraw.h"
#include"../../input/InputChecker.h"
#include"../../tween/TweenManager.h"
#include"../../time/Time.h"

static const float headShotPower = 0.3f;
static const float defMaxChainLength = 16.f;
static const float defJumpShotPower = 1.5f;
//リソースの横サイズ
const int resWidth = 15;
const int correctionHeight = 30;
const float correctionWidth = 1.8f;
//リソースの縦サイズ
const float oneLength = 30.f;

const int headCount = 8;

const int frontHead = -2;
const int backHead = -4;

Player::Player(IWorld * world,int maxLaneSize, int startLane,const Vector2& position)
	:Actor(world), isUseKey_(true), isTutorialText_(false),
	currentHead_(0),
	headChangeTime_(0), pGrav_(defPGravPow), playerMode_(MODE_FALL),
	pendulumVect_(Vector2::Zero), slipCount_(defSlipCount), jumpShotPower_(defJumpShotPower),
	otherClothesID_(CLOTHES_ID::FLUFFY_CLOTHES), friction(0.998f), chainLockCoolTime_(defChainLockCoolTime_), chainAddLength_(0),
	chainAddLengthMath_(0), maxLaneSize_(maxLaneSize), changeType_(LaneChangeType::LaneChange_Normal),slipResistTime_(defResistTime), headPosAddVect_(Vector2::Zero),
	headAngleSetter(frontHead), addLengthLocker_(false)
{
	addscorelist_[0] = 300;
	addscorelist_[1] = 200;
	addscorelist_[2] = 100;

	laneNum_ = startLane;

	spriteId_ = SPRITE_ID::PBODY_SPRITE;

	parameter_.ID = ACTOR_ID::PLAYER_ACTOR;
	parameter_.radius = (float)Sprite::GetInstance().GetSize(spriteId_).x / 2.f;
	parameter_.size = Sprite::GetInstance().GetSize(spriteId_);
	parameter_.HP = 10;
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3::Zero);

	position_ = position;
	prevPosition_ = position_;

	slipCountMult_[CLOTHES_ID::BASE_CLOTHES]
		= slipCountMult_[CLOTHES_ID::GOAL_CLOTHES]
		= slipCountMult_[CLOTHES_ID::GUM_CLOTHES]
		= slipCountMult_[CLOTHES_ID::TEST_CLOTHES]
		= 1.f;
	slipCountMult_[CLOTHES_ID::FLUFFY_CLOTHES] = 0.f;
	slipCountMult_[CLOTHES_ID::HANGER] = 1.5f;
	slipCountMult_[CLOTHES_ID::THIN_CLOTHES] = 2.f;

	pHeads_.resize(headCount);
	for (int i = 0; i < headCount; i++)
	{
		//8方向のうち、各頭に対応した位置を作成
		Vector3 tgtRot = Vector3(pHDist.x, pHDist.y)*Matrix::CreateRotationZ(((float)i + (float)headAngleSetter) * 45.f);
		Vector2 cgToV2 = position_ + Vector2(tgtRot.x, tgtRot.y);
		pHeadPoses_.push_back(cgToV2);
		pHeadLength_.push_back(defHeadLength);
		pHeadDead_.push_back(false);

		pHeads_[i] = (std::make_shared<Player_Head>(world, this, pHeadPoses_[i], i));
		world_->Add(ACTOR_ID::PLAYER_HEAD_ACTOR, pHeads_[i]);

		SetMyHeadLaneNum(i);
	}
	pSword_ = std::make_shared<Player_Sword>(world,this,position_);
	world_->Add(ACTOR_ID::PLAYER_SWORD_ACTOR, pSword_);

	//プレイヤーの状態毎の更新を格納
	updateFunctionMap_[MODE_FALL] = std::bind(&Player::FallUpdate, this);
	updateFunctionMap_[MODE_SHOOT] = std::bind(&Player::ShootUpdate, this);
	updateFunctionMap_[MODE_SHOOT_END] = std::bind(&Player::ShootEndUpdate, this);
	updateFunctionMap_[MODE_BITE] = std::bind(&Player::BiteUpdate, this);
	updateFunctionMap_[MODE_SLIP] = std::bind(&Player::SlipUpdate, this);
	updateFunctionMap_[MODE_RESIST] = std::bind(&Player::ResistUpdate, this);
	updateFunctionMap_[MODE_CLEAR] = std::bind(&Player::ClearUpdate, this);
	updateFunctionMap_[MODE_PLAYERDEAD] = std::bind(&Player::DeadUpdate, this);
	updateFunctionMap_[MODE_BACK_SHOOT] = std::bind(&Player::BackShootUpdate, this);

	worldSetMyDatas();
	StartPlayerSet();

	isClearShoot_ = false;

	StartPendulum();
}

Player::~Player()
{
	for (auto& phl : pHeadLength_) {
		TweenManager::GetInstance().Cancel(&phl);
	}

	slipCountMult_.clear();
	updateFunctionMap_.clear();
	pHeadDead_.clear();
	pHeads_.clear();
	pHeadPoses_.clear();
	pHeadLength_.clear();
	fPos_.clear();
	multiplePos.clear();
	mRot.clear();
	mRot_spd.clear();
	correctionLens.clear();
	drawPoints.clear();
	neckDrawPoints.clear();
	Sound::GetInstance().StopSE(SE_ID::HEAD_SHOOT_SE);
}

void Player::Update()
{
	if (isTutorialText_)return;

	chainLockCoolTime_--;
	chainLockCoolTime_ = MathHelper::Clamp(chainLockCoolTime_, 0, defChainLockCoolTime_);
	//レーン変更のクールタイムを設定

	if(!GetIsClearShoot())pendulumVect_ -= pendulumVect_*0.05f;
	
	velocity_ = Vector2::Zero;

	//重力
	velocity_.y += pGrav_*pGrav_*defGravAddPow;

	//移動したかをリセットしてから入力関数に入る
	isNextLaneBite_ = false;
	//自分の状態に応じた更新
	updateFunctionMap_[playerMode_]();

	//Headの表示レーンを本体に合わせる
	SetAllHeadLaneNum();

	//行列にangleをかける
	parameter_.mat += Matrix::CreateRotationZ(angle_);
	parameter_.mat.NormalizeRotationMatrix();

	HeadPosUpdate();
	SwordPosUpdate();

	worldSetMyDatas();

}

//事前更新

void Player::FastUpdate() {
	if (!world_->GetIsCamChangeMode()) {
		int nexLane = world_->GetKeepDatas().nextLane_;
		UpdateLaneNum(nexLane, changeType_);
		world_->GetCanChangedKeepDatas().SetPlayerNextLane(0);
	}

}

void Player::Draw() const
{
	auto is = Matrix::CreateRotationZ(angle_);
	auto pos = GetDrawPosVect(position_);
	auto sizeVec = Vector3((parameter_.size.x / 2), (parameter_.size.y / 2));

	auto box1 = Vector3(-sizeVec.x, -sizeVec.y)*is;
	auto box2 = Vector3(+sizeVec.x, -sizeVec.y)*is;
	auto box3 = Vector3(-sizeVec.x, +sizeVec.y)*is;
	auto box4 = Vector3(+sizeVec.x, +sizeVec.y)*is;

	//左上,右上,左下,右下
	auto pos1 = Vector3(pos.x + box1.x, pos.y + box1.y);
	auto pos2 = Vector3(pos.x + box2.x, pos.y + box2.y);
	auto pos3 = Vector3(pos.x + box3.x, pos.y + box3.y);
	auto pos4 = Vector3(pos.x + box4.x, pos.y + box4.y);
	Vector2 crcOrigin = Sprite::GetInstance().GetSize(spriteId_) / 2;
	float aHeadAngle = (360.f / (float)pHeads_.size());
	float angle = currentHead_*aHeadAngle + aHeadAngle*(headChangeTime_ / defHeadChangeTime);
	Sprite::GetInstance().Draw(spriteId_, GetDrawPosVect(position_), crcOrigin, parameter_.spriteAlpha_, Vector2::One,angle);
	
	if (!pHeadDead_[currentHead_]) {

		for (int i = neckDrawPoints.size() - 1; i > 0; i--) {
			auto p = neckDrawPoints[i];
			Vector2 p0 = GetDrawPosVect(p.fulcrumLeft);
			Vector2 p1 = GetDrawPosVect(p.fulcrumRight);
			Vector2 p2 = GetDrawPosVect(p.tipPosLeft);
			Vector2 p3 = GetDrawPosVect(p.tipPosRight);
			DrawRectModiGraphF(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, 0, 0, 41, (int)(76.0f), Sprite::GetInstance().GetHandle(SPRITE_ID::OROCHI_NECK_SPRITE), 1);
		}
	}

	for (int i = 0; i < (int)pHeads_.size();i++) {
		if (i == currentHead_)continue;
		if (pHeadDead_[i])continue;

		PlayerNeckDraw().Draw(GetDrawPosVect(pHeadPoses_[i]), GetDrawPosVect(pHeads_[i]->GetPosition()));
	}

	
	DebugDraw::DebugDrawFormatString(50, 100, GetColor(255, 255, 255), "%d", playerMode_);

	DebugDraw::DebugDrawFormatString(600, 750, GetColor(255, 0, 0), "%f", mRot.front());
	
}

void Player::OnUpdate()
{
}

void Player::OnCollide(Actor& other, CollisionParameter colpara)
{
}

void Player::OnMessage(EventMessage message, void * param)
{
	switch (message) {
	case EventMessage::GOAL_FLAG: {
		SetMode(MODE_CLEAR);
		world_->UnLockCameraPosY();
		isUseKey_.SetUseKey(false);
		break;
	}

	default:
		break;
	}

}

//レーン移動時更新

bool Player::CamMoveUpdate() {
	if (world_->GetKeepDatas().nextLane_ < 0) {
		CamMoveUp();
	}
	else {
		CamMoveDown();
	}

	return true;
}

//下移動

void Player::CamMoveDown() {
	if (changeType_ == LaneChangeType::LaneChange_Normal) {
		return;
	}
	LaneChangeFall();
}
void Player::LaneChangeFall() {

	float laneLerpNum = world_->GetKeepDatas().changeLaneLerpPos_;
	laneLerpNum = min(1.f, laneLerpNum);
	int targetNum = world_->GetKeepDatas().playerLane_ - laneNum_ + 2;
	drawAddPos_.y = MathHelper::Lerp(defDrawLineChangePosY[targetNum], defDrawLineChangePosY[targetNum - 1], laneLerpNum) - defDrawLineChangePosY[targetNum];

	if (changeType_ == LaneChangeType::LaneChange_Fall) {
		drawAddPos_.y = drawAddPos_.y * fallAddPosMult;
	}
}

//落下によるレーン移動かを調べる

bool Player::isLaneChangeFall() const {
	return changeType_ == LaneChangeType::LaneChange_Fall;
}
void Player::StartPendulum() {
	Vector2 lngPs = pHeads_[currentHead_]->GetPosition() - position_;
	MultipleInit(lngPs.Length(), pHeads_[currentHead_]->GetPosition(), MathAngle(position_-pHeadPoses_[currentHead_],Vector2::Down),parameter_.radius);
	Multiple();
}

//首の根本の位置を調べる

Vector2 Player::GetHeadPos(int headNum) const {
	return pHeadPoses_[headNum];
}

//現在の首の根本の位置を調べる

Vector2 Player::GetHeadPos() const {
	return pHeadPoses_[currentHead_];
}

//Headの長さを実際のゲームに反映される値に変換して返す

float Player::GetHeadLengthChangeToPosMult(int headNum) const {
	return pHeadLength_[headNum] * HeadShootMult;
}

//現在使用しているHeadの座標を返す

Vector2 Player::GetCurrentPHeadPosition() const {
	return pHeads_[currentHead_]->GetPosition();
}
float Player::GetPlayerSwordAngle() const {
	return MathFormedAngle(pSword_->GetSwordEndPos() - position_);
}

//振り子の支点位置を移動する

void Player::SetMultipleFulcrumPos(const Vector2 & pos) {
	fPos_.front() = pos;
}
bool Player::GetIsSwordActive() const {
	return pSword_->GetUseSword();
}
//クリア後に次ステージに飛ぶ状態かを調べる
bool Player::GetIsClearBite() const
{
	return (mRot.front() >= -45.0f&&playerMode_ == MODE_CLEAR);
}
bool Player::GetIsClearShoot() const
{
	return (mRot.front() < -45.0f&&playerMode_ == MODE_CLEAR);
}
Actor* Player::GetCurrentHead() const {
	return pHeads_[currentHead_].get();
}
void Player::deadLine()
{
	if (playerMode_ == MODE_CLEAR)return;
	//死亡条件を満たしたらDeadする(SetModeは同モードなら更新を行わない)
	if (isPlayerDead())SetMode(MODE_PLAYERDEAD);
	if (laneNum_ >= 2) {
		world_->FreeCameraPosY(false);

		if (position_.y >= WINDOW_HEIGHT) {
		}

	}
	else if (position_.y >= 500 && playerMode_ != MODE_BITE) {
		SetNextLane(1, LaneChangeType::LaneChange_Fall);
		world_->FreeCameraPosY(false);

	}
	else if (position_.y >= 300 && playerMode_ != MODE_BITE) {
		world_->FreeCameraPosY(true);
		world_->GetCanChangedKeepDatas().isFallCamMode_ = true;
		world_->GetCanChangedKeepDatas().fallAddPos_ = 100.f;
	}

}

//キーを有効化するかどうかを決定する

void Player::SetUseKey(bool key) {
	isUseKey_.SetUseKey(key);
}
bool Player::GetUseKey() const {
	return isUseKey_.GetUseKey(); 
}
void Player::SetIsTutorialTextWriting(bool is) {
	isTutorialText_ = is;
}
bool Player::IsLookBack() const
{
	return headAngleSetter==backHead;
}
void Player::MultipleInit(float len, const Vector2& fPos, float rot, float radius)
{
	PlayerNeckPendulumSupport::Init(pHeads_[currentHead_]->GetPosition(), pHeadPoses_[currentHead_], fPos_, multiplePos, mRot, mRot_spd, neckDrawPoints);

	for (auto& r : mRot) {
		r =fmodf(r,360.f);
	}
}
void Player::Multiple()
{
	if (GetUseKey()) {
		if ((mRot_spd[0] < 0 && (isUseKey_.StickStateDown(InputChecker::Input_Stick::Right)) ||
			(mRot_spd[0] > 0 && (isUseKey_.StickStateDown(InputChecker::Input_Stick::Left)))))
		{
			bool isChange = (mRot.front() <= 0.f &&mRot_spd.front() >= -10.f&&mRot_spd.front() <= 0.f)||( mRot.front() >= 180.f &&mRot_spd.front() <= 10.f&&mRot_spd.front()>=0.f);

			for (auto& r : mRot_spd) {
				r += sign(r);
				if (isChange)r = -(float)sign(r);
				r = MathHelper::Clamp(r, -60.f, 60.f);
			}

			mRot.front() = MathHelper::Clamp(mRot.front(), -45.f, 225.f);

		}
	}
	pendulumVect_=PlayerNeckPendulumSupport::Pendulum(fPos_, multiplePos, mRot, mRot_spd, neckDrawPoints);
	pendulumVect_ *= jumpShotPower_;
	Vector2 positionSurp = position_ - pHeadPoses_[currentHead_];

	position_ = multiplePos.back()+ positionSurp;

}

//多重振り子に移動量を加算
void Player::UpdateMultiplePos() {
	SetMultiplePos((position_ - prevPosition_));
}
//軸と先端の位置を修正し、同時に描画位置を変更する
void Player::SetMultiplePos(const Vector2 & addpos) {
	for (int i = 0; i < (int)multiplePos.size(); i++) {
		multiplePos[i] += addpos;
		if (i > 0) fPos_[i] = multiplePos[i - 1];
	}
	fPos_.front() = pHeads_[currentHead_]->GetPosition() + addpos;
	for (auto& n : neckDrawPoints) {
		n.fulcrumLeft += addpos;
		n.fulcrumRight += addpos;
		n.tipPosLeft += addpos;
		n.tipPosRight += addpos;
	}
}

//多重振り子を強制的に移動

void Player::AddMultiplePos(const Vector2 & addPos) {
	for (int i = 0; i < (int)multiplePos.size(); i++) {
		multiplePos[i] += (addPos);
		if (i > 0) fPos_[i] = multiplePos[i - 1];
	}
}

void Player::SetDrawNeck(const Vector2 & bodyPoint, const Vector2 & headPoint)
{
	SetDrawPoint(bodyPoint,headPoint);
	SetDrawNeckParts(bodyPoint, headPoint);


}

void Player::SetDrawPoint(const Vector2 & bodyPoint, const Vector2 & headPoint)
{
	fPos_.clear();
	multiplePos.clear();
	correctionLens.clear();
	mRot.clear();
	mRot_spd.clear();

	Vector2 vel(bodyPoint - headPoint);
	int s = (int)round(vel.Length() / oneLength);
	
	for (int i = 0; i < s + 1; i++) {
		correctionLens.push_back(1.0f);
	}
	if (vel.Length() - oneLength > 0) {
		float h = MathHelper::Mod(vel.Length() - oneLength, oneLength) / oneLength;
		correctionLens.push_back(h);
	}

	vel = vel.Normalize()*oneLength;
	
	for (int i = 0; i < s; i++) {
		fPos_.push_back(headPoint+(vel*(float)i));
		multiplePos.push_back(headPoint + (vel*((float)i+1.f)));
		mRot.push_back(MathAngle(multiplePos.back() - fPos_.back()));
		mRot_spd.push_back(0);
	}


}

void Player::SetDrawNeckParts(const Vector2 & bodyPoint, const Vector2 & headPoint)
{
	neckDrawPoints.clear();
	NeckPiecePoint p;

	Vector2 dir(bodyPoint - headPoint);
	for (int i = 0; i < (float)fPos_.size(); i++) {
		if ((fPos_[i] - position_).Length() <= parameter_.radius) {
			if (neckDrawPoints.empty()) {
				p = MathNeckPiecePoint(fPos_[i], multiplePos[i], resWidth);
			}
			else p = neckDrawPoints.back();
			neckDrawPoints.push_back(p);
			continue;
		}
		p = MathNeckPiecePoint(fPos_[i], multiplePos[i], resWidth);
		neckDrawPoints.push_back(p);
	}

}

Vector2 Player::GetCurrentHeadLength() const {
	Vector2 vel = pHeads_[currentHead_]->GetPosition() - position_;
	Vector2 msx = pHeadLength_[currentHead_] * Vector2(32.f, 32.f).Length();
	float velLe = vel.Length();
	float msxLe = msx.Length();
	float lep = MathHelper::Abs(velLe - msxLe);
	return msx;
}

void Player::HeadPosUpdate()
{
	headChangeTime_ -= 0.016f*sign(headChangeTime_);

	rotTimer = 0;
	if (MathHelper::Abs(headChangeTime_) <= 0.01f)headChangeTime_ = 0;
	else if (MathHelper::Abs(headChangeTime_) > 0)rotTimer = headChangeTime_ * 5;//MathHelper::Abs(defHeadChangeTime/1.f);

	for (int i = 0; i < (int)pHeadPoses_.size(); i++) {
		Vector3 tgtRot = Vector3(pHDist.x, pHDist.y)*Matrix::CreateRotationZ(((i + headAngleSetter - currentHead_) * 45)/*+angle_*/ + ((rotTimer)* 45));
		Vector2 cgToV2 = position_ + Vector2(tgtRot.x, tgtRot.y);
		pHeadPoses_[i] = cgToV2;
	}

}

void Player::SwordPosUpdate() {
	if (!GetIsBiteMode()&&!world_->GetIsCamChangeMode()) {
		pSword_->SetUseSword(false);
		return;
	}
	Vector2 velUnPos = pHeads_[currentHead_]->GetPosition();
	Vector2 vel = position_ - velUnPos;
	vel = Vector2::Normalize(vel);

	pSword_->SetSwordVel(vel);

	pSword_->SetLaneNum(laneNum_);
}

//使用する頭を右隣の物に変更

void Player::changeHead() {
	//回転した時点でSlip状態を直す
	currentHead_++;
	if (currentHead_ >= (int)pHeads_.size())currentHead_ = 0;
	headChangeTime_ = defHeadChangeTime;
}

//使用する頭を左隣の物に変更

void Player::backChangeHead() {
	currentHead_--;
	if (currentHead_ < 0)currentHead_ = pHeads_.size() - 1;
	headChangeTime_ = -defHeadChangeTime;
}

void Player::StartPlayerSet() {
	SetMode(MODE_BITE,false);
	pHeadLength_[currentHead_]=5.f;
	pHeads_[currentHead_]->StartPlayerHeadBite();

	HeadPosUpdate();

	StartPendulum();

	world_->FreeCameraPosY(false);
}

int Player::GetCurHead() const {
	return currentHead_;
}

//指定IDの頭が死んでいるかを調べる

bool Player::GetPHeadDead(int pHeadNum) const {
	return pHeadDead_[pHeadNum];
}

void Player::CurHeadBite(const Vector2 & target) {

	if (playerMode_ != MODE_SHOOT_END)return;

	CreateBiteEffect();
	//スコア加算
	world_->sendMessage(EventMessage::ADD_SCORE, (int*)addscorelist_[laneNum_]);

	SetMode(MODE_BITE);
	pGrav_ = defPGravPow;

	StartPendulum();
}

bool Player::ResurrectHead() {
	for (int i = currentHead_; i < (int)pHeads_.size() + currentHead_; i++) {
		int trgNum = i;
		if (trgNum >= (int)pHeads_.size()) {
			trgNum = trgNum - pHeads_.size();
		}
		if (!pHeadDead_[trgNum])continue;
		world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<ResurrectEffect>(world_, position_,pHeads_[trgNum].get()));
		Sound::GetInstance().PlaySE(SE_ID::RESURRECT_SE);

		pHeadDead_[trgNum] = false;
		return true;
	}
	return false;
}

//掴んでいる服の種類を設定する

void Player::SetOtherClothesID_(CLOTHES_ID cId) {
	otherClothesID_ = cId;
}

//噛み付ける状態かを返す(レジスト含む)

bool Player::GetIsShootMode() const {
	return playerMode_ == MODE_SHOOT;
}

//噛み付き状態or踏ん張り状態かを調べる

bool Player::GetIsBiteMode() const {
	return playerMode_ == MODE_BITE || playerMode_ == MODE_RESIST;
}

//プレイヤーの状態をチェックする、引数のモードと一致していればtrue

bool Player::PlayerModeChecker(Player_Mode pMode) {
	return playerMode_ == pMode;
}

//踏ん張り状態かを調べる

bool Player::GetIsResistMode() const {
	return playerMode_ == MODE_RESIST;
}

//ステージクリア状態かを調べる

bool Player::GetIsClearMode() const {
	return playerMode_ == MODE_CLEAR;
}

//現在生きている頭の数を調べる

int Player::GetPHeadLiveCount() const {
	int result = 0;
	for (auto i : pHeadDead_) {
		if (!i)result++;
	}
	return result;
}

//噛み付き状態にするかをセット、

void Player::SetIsBiteMode(bool ismode) {
	int setMode = (ismode) ? MODE_BITE : MODE_SHOOT;
	playerMode_ = setMode;
}


//Headのレーンを本体のレーンに合わせる

void Player::SetMode(Player_Mode pMode,bool isPlaySE) {
	if (!isUseKey_.GetUseKey())return;
	if (pMode == playerMode_)return;

	playerMode_ = pMode;
		headPosAddVect_= Vector2::Normalize(pHeads_[currentHead_]->GetPosition() - position_)*Vector2(32.f,32.f).Length();

		switch (playerMode_)
		{
		case MODE_FALL: {
			ToFallMode(isPlaySE);
			break;
		}
		case MODE_SHOOT: {
			ToShootMode(isPlaySE);
			break;
		}
		case MODE_SHOOT_END: {
			ToShootEndMode(isPlaySE);
			break;
		}
		case MODE_BITE: {
			ToBiteMode(isPlaySE);
			break;
		}
		case MODE_SLIP: {
			ToSlipMode(isPlaySE);
			break;
		}
		case MODE_RESIST: {
ToResistMode(isPlaySE);
break;
		}
		case MODE_CLEAR: {
			ToClearMode(isPlaySE);
			break;
		}
		case MODE_PLAYERDEAD: {
			ToDeadMode(isPlaySE);
			break;
		}
		case MODE_BACK_SHOOT: {
			ToBackShootMode(isPlaySE);
			break;
		}
		}
}

//シュート終了の瞬間かどうかを取る

bool Player::GetIsShootModeEnd() const {
	return playerMode_ == MODE_SHOOT_END;
}

//滑り落ちるまでの時間を返す

float Player::GetSlipCount() const {
	return slipCount_;
}

//滑り落ち状態かを調べる

bool Player::GetIsSlipped() const {
	return playerMode_ == MODE_SLIP;
}

//編集可能インプットを取得する

PlayerInputChecker & Player::GetEditableUseKey() {
	return isUseKey_;
}

std::vector<float>& Player::GetEditableRot_Speed() {
	return mRot_spd;
}

void Player::SetMyHeadLaneNum(int targetNum) {
	pHeads_[targetNum]->SetLaneNum(laneNum_);
}

void Player::SetAllHeadLaneNum() {
	for (auto& pHs : pHeads_)
	{
		pHs->SetLaneNum(laneNum_);
	}
}

void Player::worldSetMyDatas() {
	//共有データに、自身の現レーン位置を保存
	world_->GetCanChangedKeepDatas().SetPlayerLane(laneNum_);
	world_->GetCanChangedKeepDatas().SetPlayerPos(position_);
}

void Player::SetNextLane(int addNum, LaneChangeType changeType) {
	if (laneNum_ + addNum > (maxLaneSize_ - 1) || laneNum_ + addNum < 0)return;

	changeType_ = changeType;
	if (changeType_ == LaneChangeType::LaneChange_Fall) {
		world_->GetChangeInv().Translation(world_->GetChangeInv().Translation() + Vector3(0.f, 500.f, 0.f));
		position_.y -= 500.f;
		pHeadPoses_[currentHead_] = multiplePos[0];
		UpdateLaneNum(addNum, LaneChangeType::LaneChange_Fall);
		pHeads_[currentHead_]->addPos(Vector2(0, -500.f));
		HeadPosUpdate();
		world_->UpdateDrawPos();
		UpdateMultiplePos();

		return;
	}


	world_->ChangeCamMoveMode(addNum);

	world_->sendMessage(EventMessage::START_LANE_CHANGE);
}

//支点固定座標を設定し、首の位置を補正する

void Player::setCurPHeadSPos(const Vector2 & sPos) {
	if (isTutorialText_) return;

	SetMultiplePos(sPos - stopPos_);
	stopPos_ = sPos;
}

//振り子により作り出された移動ベクトルを取得する

Vector2 Player::GetPendulumVect() const {
	return pendulumVect_;
}

//振り子により作り出された移動ベクトルを上書きする

void Player::SetPendulumVect(const Vector2 & pvect) {
	pendulumVect_ = pvect;
}

void Player::curPHeadSlip(bool isSlip) {
	pHeads_[currentHead_]->setIsBiteSlipWind(isSlip);
}

//プレイヤーが死んでるか

bool Player::isPlayerDead() const {
	if (laneNum_ == (maxLaneSize_ - 1) && position_.y >= WINDOW_HEIGHT - 200)return true;

	for (auto pHD : pHeadDead_) {
		if (!pHD)return false;
	}
	return true;
}

void Player::PHeadChanger(int rot) {
	if (!pHeadDead_[currentHead_]) {
		world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerMetamorEffect>(world_, pHeads_[currentHead_]->GetPosition(), pHeads_[currentHead_]));
		world_->sendMessage(EventMessage::CREATE_METAMOR_EFFECT);
	}
	PHeadLengthReset();
	(sign(rot) == 1) ? backChangeHead() : changeHead();

	world_->sendMessage(EventMessage::CHANGE_HEAD);

	CreateMetamorEffect();

	Sound::GetInstance().PlaySE(SE_ID::CHANGE_HEAD_SE);
}

void Player::SetStopPos(Vector2 target) {
	stopPos_ = target;
}

//支点固定座標を取得する

Vector2 Player::GetStopPos() const {
	return stopPos_;
}

//首の先端の角度を調べる

float Player::GetRot() const {
	return mRot.front();
}

//首の根本の角度を調べる

float Player::GetRotBack() const {
	return mRot.back();
}

//滑り落ち時のHeadのあるべき位置を取得する

Vector2 Player::GetSlipHeadPoint() const {
	return fPos_.front();
}


void Player::PHeadLengthReset() {
	world_->sendMessage(EventMessage::NECK_SHOOT_END);
	//長さの補間をリセットする
	chainAddLength_ = 0.f;
	chainAddLengthMath_ = 0.f;

	for (auto& pHL : pHeadLength_) {
		TweenManager::GetInstance().Add(EaseOutQuart, &pHL, 2.f, 0.5f);
		//pHL = 2.f;
	}
}

void Player::CurPHeadLengPlus(float addPow) {

	if (pHeadDead_[currentHead_])return;
	if (chainAddLengthMath_>0.f) {
		chainAddLengthMath_ -= Time::DeltaTime;
		if (chainAddLengthMath_ <= 0.f) {
			chainAddLengthMath_ = 0.f;
			addLengthLocker_ = false;
		}
		return;
	}
	pHeadLength_[currentHead_] += addPow;
	pHeadLength_[currentHead_] = MathHelper::Clamp(pHeadLength_[currentHead_], 0.f, 16.f + chainAddLength_);
	if(pHeadLength_[currentHead_] >=16.f+chainAddLength_&&addLengthLocker_) {
		chainAddLengthMath_ = 0.0001f;
	}
	if (addLengthLocker_)return;

	//headの左隣を基準とする
	int targetNum = (currentHead_+headCount-1)%headCount;

	//死んでたらもう1つ手前にする
DEAD_CHECK:
		if (pHeadDead_[targetNum]) {
			if (((targetNum + headCount - 1) % headCount) == currentHead_)goto DEAD_END;
			targetNum = (targetNum + headCount - 1) % headCount;
			goto DEAD_CHECK;
		}
DEAD_END:

		//全員の長さの最低値を調べて
		float LengthDefaultNum = 2.f;
		for (int i = 0; i < headCount; i++) {
			if (pHeadDead_[i])continue;
			if (i == currentHead_)continue;
			LengthDefaultNum = min(pHeadLength_[i], LengthDefaultNum);
		}
		//長さの最低値に応じて首の停止位置を決める
		float LengthKeepNum = 1.f;
		if (LengthDefaultNum <= 1.f)LengthKeepNum = -0.1f;

		//長さを減らして、規定値を超えたら死ぬ
		pHeadLength_[targetNum] -= addPow;
		pHeadLength_[targetNum] = max(pHeadLength_[targetNum],LengthKeepNum);
		if (pHeadLength_[targetNum] < 0.f) {
			Vector2 toPos = pHeadPoses_[targetNum] - position_;
			world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerFallPin>(world_, pHeads_[targetNum]->GetPosition(), toPos));
			pHeadDead_[targetNum] = true;
			
			chainAddLength_ += 2.f;

			addLengthLocker_ = true;
		}
	return;

	////floatの誤差と、addPowによるLengthのズレを補正するための値、首の長さの値に補正が発生した場合は、この補正値をそこに加算する事で、長さの違和感を解決する
	//float fSaveAddNum = 0.2f;
	//if (pHeadLength_[currentHead_] > 16.f+ fSaveAddNum) {
	//	pHeadLength_[currentHead_] = 16.f+ fSaveAddNum + chainAddLength_- chainAddLengthMath_;
	//	//長さの上昇に対する補間値
	//	chainAddLengthMath_ -= 0.4f;
	//	chainAddLengthMath_ = max(chainAddLengthMath_, 0.f);
	//
	//	for (int i = currentHead_; i > -(int)pHeads_.size() + currentHead_; i--) {
	//		int trgNum = i;
	//		if (trgNum<0) {
	//			trgNum = trgNum + pHeads_.size();
	//		}
	//
	//		if (pHeadDead_[trgNum])continue;
	//		if (trgNum == currentHead_)continue;
	//
	//		if (pHeadLength_[trgNum] <= 0.1f) {
	//			if (chainLockCoolTime_>0) {
	//				break;
	//			}
	//			chainLockCoolTime_ = defChainLockCoolTime_;
	//			chainAddLength_ += 2.f;
	//			chainAddLengthMath_ += 2.f;
	//			Vector2 toPos = pHeadPoses_[trgNum] - position_;
	//			world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerFallPin>(world_, pHeads_[trgNum]->GetPosition(), toPos));
	//			pHeadDead_[trgNum] = true;
	//		}
	//		else {
	//		}
	//	}
	//
	//	return;
	//}
	////現在のHead以外の長さを伸ばした分だけマイナスする
	//pHeadLength_[currentHead_] += addPow;
	////左隣がターゲット
	//int targetNum = currentHead_ - 1;
	//
	//
	//float LengthKeepNum = 1.01f;
	//if (targetNum<0) {
	//	targetNum = targetNum + (int)pHeads_.size();
	//}
	//
	//for (;;) {
	//	if (LengthKeepNum < 0.f)break;
	//
	//	if (pHeadLength_[targetNum] < LengthKeepNum)
	//	{
	//		targetNum--;
	//
	//		if (targetNum<0) {
	//			targetNum = targetNum + (int)pHeads_.size();
	//		}
	//
	//		if (targetNum == currentHead_) {
	//			LengthKeepNum -= 1.f;
	//			targetNum--;
	//
	//			if (targetNum<0) {
	//				targetNum = targetNum + (int)pHeads_.size();
	//			}
	//		}
	//
	//		continue;
	//	}
	//
	//	pHeadLength_[targetNum] -= addPow;
	//	pHeadLength_[targetNum] = max(pHeadLength_[targetNum], 0.f);
	//
	//
	//	break;
	//}
}

void Player::UpdateLaneNum(int updateNum, LaneChangeType changeType) {
	if (updateNum == 0)return;
	if (laneNum_ + updateNum > (maxLaneSize_ - 1) || laneNum_ + updateNum < 0)return;

	//次のレーンに対応したベクトルを作成し、重力の加算をリセットする
	Vector2 nextVel_;
	//上がるとき
	if (updateNum < 0) {
		nextVel_ = LaneChange_Up();
	}
	//降りる時
	else if (updateNum > 0) {
		nextVel_ = LaneChange_Down(changeType);
	}

	laneNum_ += updateNum;
	//レーン最大範囲を超えたらVectの補正を行わない

	laneNum_ = MathHelper::Clamp(laneNum_, 0, (maxLaneSize_ - 1));

	pendulumVect_ = nextVel_;

	if (GetIsBiteMode())playerMode_ = MODE_SLIP;

	world_->sendMessage(EventMessage::LANE_CHANGE_END);

	worldSetMyDatas();
}

Vector2 Player::LaneChange_Up()
{
	pGrav_ = 0.f;
	PHeadChanger();
	SetMode(MODE_FALL);
	world_->sendMessage(EventMessage::LANE_CHANGE_UP_END);

	return Vector2(0, -15.f);
}

Vector2 Player::LaneChange_Down(LaneChangeType changeType)
{
	Vector2 result = Vector2(0.0f, 0.0f);

	if (changeType == LaneChangeType::LaneChange_Fall) {
		result = pendulumVect_ / 3;
		pGrav_ *= 0.1f;

		world_->sendMessage(EventMessage::LANE_CHANGE_FALL);
	}
	world_->sendMessage(EventMessage::LANE_CHANGE_DOWN_END);

	return result;
}

void Player::CreateBiteEffect()
{
	if (pHeadDead_[currentHead_])return;
	Vector2 vel = pHeads_[currentHead_]->GetPosition() - position_;
	vel = vel.Normalize();
	world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerBiteEffect>(world_, pHeads_[currentHead_]->GetPosition() + vel * 30));

}

void Player::CreateMetamorEffect()
{
	Vector2 addVec = position_ - pHeadPoses_[currentHead_];
	addVec = addVec.Normalize()*32.f;
	if (!pHeadDead_[currentHead_]) {
		world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerMetamorEffect>(world_, pHeads_[currentHead_]->GetPosition(), pHeads_[currentHead_], 0.15f, addVec));
		world_->sendMessage(EventMessage::CREATE_METAMOR_EFFECT);
	}
}

void Player::ToFallMode(bool isPlaySE)
{
	Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
	Sound::GetInstance().StopSE(SE_ID::HEAD_SHOOT_SE);

}

void Player::ToShootMode(bool isPlaySE)
{
	Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
	if (pHeadDead_[currentHead_]) {
		Sound::GetInstance().PlaySE(SE_ID::BAD_SE);
	}
	else {
		Sound::GetInstance().PlaySE(SE_ID::HEAD_SHOOT_SE, DX_PLAYTYPE_LOOP);
	}
	if (headAngleSetter == backHead) {
		if (!pHeadDead_[currentHead_]) {
			world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerMetamorEffect>(world_, pHeads_[currentHead_]->GetPosition(), pHeads_[(currentHead_ + headCount - 2) % headCount]));
			world_->sendMessage(EventMessage::CREATE_METAMOR_EFFECT);
		}
		TweenManager::GetInstance().Add(EaseOutQuart, &pHeadLength_[currentHead_], 2.f);
		headAngleSetter = frontHead;
		currentHead_ = (currentHead_ + 2) % headCount;
		CreateMetamorEffect();
	}
	TweenManager::GetInstance().Cancel(&pHeadLength_[currentHead_]);
	addLengthLocker_ = true;

}

void Player::ToShootEndMode(bool isPlaySE)
{
	Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
	Sound::GetInstance().StopSE(SE_ID::HEAD_SHOOT_SE);

}

void Player::ToBiteMode(bool isPlaySE)
{
	world_->FreeCameraPosY(false);
	Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
	Sound::GetInstance().StopSE(SE_ID::HEAD_SHOOT_SE);
	if (isPlaySE)Sound::GetInstance().PlaySE(SE_ID::BITE_SE);

}

void Player::ToSlipMode(bool isPlaySE)
{
	Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
	Sound::GetInstance().StopSE(SE_ID::HEAD_SHOOT_SE);

}

void Player::ToResistMode(bool isPlaySE)
{
	Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
	Sound::GetInstance().StopSE(SE_ID::HEAD_SHOOT_SE);
}

void Player::ToClearMode(bool isPlaySE)
{
	Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
	Sound::GetInstance().StopSE(SE_ID::HEAD_SHOOT_SE);

	clearShootTimer_.Initialize();
	clearShootTimer_.Add([this] {
		PHeadChanger();
		isClearShoot_ = true;
		pGrav_ = 0.0f;
		pendulumVect_ = Vector2(0.f, 100.f);
	});
}

void Player::ToDeadMode(bool isPlaySE)
{
	world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerDeadHead>(world_, position_));

	for (int i = 0; i < headCount; i++) {
		world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerDeadPin>(world_, position_));
	}
	Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
	Sound::GetInstance().StopSE(SE_ID::HEAD_SHOOT_SE);
}

void Player::ToBackShootMode(bool isPlaySE)
{
	Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
	if (pHeadDead_[currentHead_]) {
		Sound::GetInstance().PlaySE(SE_ID::BAD_SE);
	}
	else {
		Sound::GetInstance().PlaySE(SE_ID::HEAD_SHOOT_SE, DX_PLAYTYPE_LOOP);
	}
	if (headAngleSetter == frontHead) {
		if (!pHeadDead_[currentHead_]) {
			world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerMetamorEffect>(world_, pHeads_[currentHead_]->GetPosition(), pHeads_[(currentHead_ + 2) % headCount]));
			world_->sendMessage(EventMessage::CREATE_METAMOR_EFFECT);
		}
		TweenManager::GetInstance().Add(EaseOutQuart, &pHeadLength_[currentHead_], 2.f);
		headAngleSetter = backHead;
		currentHead_ = (currentHead_+headCount-2)%headCount;
		CreateMetamorEffect();
	}
	TweenManager::GetInstance().Cancel(&pHeadLength_[currentHead_]);
	addLengthLocker_ = true;

}

void Player::FallUpdate()
{
	pGrav_ += defPGravPow;
		if (isUseKey_.StickTriggerDown(InputChecker::Input_Stick::Left)) {
			SetMode(MODE_FALL);
			//キーを押し直したかの判断
			PHeadChanger();
			world_->sendMessage(EventMessage::CHANGE_HEAD_KEY);
		}
		if (isUseKey_.StickTriggerDown(InputChecker::Input_Stick::Right)) {
			SetMode(MODE_FALL);
			//キーを押し直したかの判断
			PHeadChanger(1);
			world_->sendMessage(EventMessage::CHANGE_HEAD_KEY);
		}

		if (isUseKey_.KeyTriggerDown(InputChecker::Input_Key::B)) {
			world_->sendMessage(EventMessage::NECK_SHOOT);
			SetMode(MODE_SHOOT);
		}
		else if (isUseKey_.KeyTriggerDown(InputChecker::Input_Key::X)) {
			world_->sendMessage(EventMessage::NECK_BACK_SHOOT);
			SetMode(MODE_BACK_SHOOT);
		}
	
		position_ += velocity_ + pendulumVect_;

		slipCount_ = defSlipCount;

		SetDrawNeck(position_,pHeads_[currentHead_]->GetPosition());

}

void Player::ShootUpdate()
{
	pGrav_ += defPGravPow;
	if (isUseKey_.KeyStateUp(InputChecker::Input_Key::B)) {
		pHeads_[currentHead_]->SetBiteSprite();
		SetMode(MODE_SHOOT_END);
	}
	else if(isUseKey_.KeyStateDown(InputChecker::Input_Key::B)){
		CurPHeadLengPlus(headShotPower);
	}

		position_ += velocity_ + pendulumVect_;

		slipCount_ = defSlipCount;

		SetDrawNeck(position_, pHeads_[currentHead_]->GetPosition());
}

void Player::ShootEndUpdate()
{
	CreateBiteEffect();
	SetMode(MODE_FALL);
	pGrav_ += defPGravPow;
	if (isUseKey_.StickTriggerDown(InputChecker::Input_Stick::Left)) {
		PHeadChanger();
		world_->sendMessage(EventMessage::CHANGE_HEAD_KEY);
	}
	if (isUseKey_.StickTriggerDown(InputChecker::Input_Stick::Right)) {
		//キーを押し直したかの判断
		PHeadChanger(1);
		world_->sendMessage(EventMessage::CHANGE_HEAD_KEY);
	}
	if (isUseKey_.KeyTriggerDown(InputChecker::Input_Key::B)) {
		SetMode(MODE_SHOOT);
	}

	position_ += velocity_ + pendulumVect_;

	slipCount_ = defSlipCount;

	SetDrawNeck(position_, pHeads_[currentHead_]->GetPosition());
}

void Player::BiteUpdate()
{
	Multiple();
	if (isUseKey_.KeyStateDown(InputChecker::Input_Key::A)) {
		if (!pSword_->GetUseSword()) {
			world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<GetSwordEffect>(world_, pSword_->GetPosition(), pSword_.get()));
			world_->sendMessage(EventMessage::USE_SWORD);
			pSword_->SetUseSword(true);
			Sound::GetInstance().PlaySE(SE_ID::CREATE_SWORD_SE);
		}
	}

	if (isUseKey_.KeyTriggerDown(InputChecker::Input_Key::B) || isUseKey_.KeyTriggerDown(InputChecker::Input_Key::X)) {
		if ((mRot.front() < 0.f || mRot.front() > 180.f)&&laneNum_!=0) {
			SetNextLane(-1);
		}
		else {
			SetMode(MODE_FALL);
			//Headを交代する
			PHeadChanger();
		}
	}

	slipCount_ -= 0.016f*slipCountMult_[otherClothesID_];
	if (slipCount_ <= 0.f) {
		SetMode(MODE_SLIP);
		//首を殺して
		Vector2 toPos = pHeadPoses_[currentHead_] - position_;
		world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerFallPin>(world_, pHeads_[currentHead_]->GetPosition(), toPos));
		pHeadDead_[currentHead_] = true;
		//スリップモードに移行すると同時に、その時点のベクトルをHeadに格納する
		pHeads_[currentHead_]->SetPosAddVect(pHeads_[currentHead_]->GetPosition() - position_);
		PHeadChanger();
	}
	slipCount_ = max(slipCount_, 0.f);
}

void Player::SlipUpdate()
{
	pGrav_ += defPGravPow;


	if (isUseKey_.KeyTriggerDown(InputChecker::Input_Key::B)) {
		PHeadChanger();

		SetMode(MODE_SHOOT);
	}
	if (isUseKey_.KeyTriggerDown(InputChecker::Input_Key::X)) {
		PHeadChanger();

		SetMode(MODE_BACK_SHOOT);
	}
	if (isUseKey_.StickTriggerDown(InputChecker::Input_Stick::Left)) {
		SetMode(MODE_FALL);
		PHeadChanger();
		world_->sendMessage(EventMessage::CHANGE_HEAD_KEY);
	}
	if (isUseKey_.StickTriggerDown(InputChecker::Input_Stick::Right)) {
		SetMode(MODE_FALL);
		//キーを押し直したかの判断
		PHeadChanger(1);
		world_->sendMessage(EventMessage::CHANGE_HEAD_KEY);
	}


	position_ += velocity_ + pendulumVect_;

	slipCount_ = defSlipCount;
	UpdateMultiplePos();
		
}

void Player::ResistUpdate()
{
	slipResistTime_ -= 0.016f;

	if (slipResistTime_ <= 0.f) {
		slipResistTime_ = defResistTime;
		SetMode(MODE_FALL);
		return;
	}

	BiteUpdate();
}

void Player::ClearUpdate()
{
	for (auto& i : mRot_spd) {
		i*=1.02f;
		i = MathHelper::Clamp(i, -60.f, 60.f);
	}
	mRot.front() = MathHelper::Clamp(mRot.front(), -45.f, 225.f);

	if ((pHeads_[currentHead_]->GetDrawPos().y>= WINDOW_HEIGHT+99.f)||isClearShoot_) {
		clearShootTimer_.Action();
		pendulumVect_.y *= 0.98f;
		pendulumVect_.y = max(50.f,pendulumVect_.y);

		position_ -= pendulumVect_;
		UpdateMultiplePos();
	}
	else {
		Multiple();
	}
	if (drawPos_.y <= -300) {
		world_->sendMessage(EventMessage::PLAY_NEXT_STAGE);
	}
}

void Player::DeadUpdate()
{
	pGrav_ += defPGravPow;
	velocity_.y = pGrav_*pGrav_*defGravAddPow;
	position_ += velocity_ + pendulumVect_;

	UpdateMultiplePos();
}

void Player::BackShootUpdate()
{
	pGrav_ += defPGravPow;
	if (isUseKey_.KeyStateUp(InputChecker::Input_Key::X)) {
		pHeads_[currentHead_]->SetBiteSprite();
		SetMode(MODE_SHOOT_END);
	}
	else if (isUseKey_.KeyStateDown(InputChecker::Input_Key::X)) {
		CurPHeadLengPlus(headShotPower);
	}

	position_ += velocity_ + pendulumVect_;

	slipCount_ = defSlipCount;

	SetDrawNeck(position_, pHeads_[currentHead_]->GetPosition());

}
