#include "Player.h"
#include "../../input/Keyboard.h"
#include "../../conv/DXConverter.h"
#include "../../graphic/Model.h"
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

static const float headShotPower = 0.3f;
static const float defMaxChainLength = 16.f;
static const float defJumpShotPower = 1.5f;
//リソースの横サイズ
const int resWidth = 15;
const int correctionHeight = 30;
const float correctionWidth = 1.8f;
//リソースの縦サイズ
const float oneLength = 30.f;

Player::Player(IWorld * world,int maxLaneSize, int startLane)
	:Actor(world), isUseKey_(true), clearAddRot_(0.0f), isTutorialText_(false),
	currentHead_(0),
	headChangeTime_(0), pGrav_(defPGravPow), maxChainLength_(defMaxChainLength), playerMode_(MODE_FALL), isNextPushKey_(true),
	pendulumVect_(Vector2::Zero), slipCount_(defSlipCount), jumpShotPower_(defJumpShotPower), isSlipped_(false), chainLock_(false),/* isCanChangeLane_(false),*/
	otherClothesID_(CLOTHES_ID::FLUFFY_CLOTHES), friction(0.998f), isCanNextHeadRot(true), chainLockCoolTime_(defChainLockCoolTime_), chainAddLength_(0),
	chainAddLengthMath_(0), maxLaneSize_(maxLaneSize), isPlayerFallLane_(false), changeType_(LaneChangeType::LaneChange_Normal),slipResistTime_(defResistTime), headPosAddVect_(Vector2::Zero)

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

	position_ = Vector2(0, 200);
	prevPosition_ = position_;

	slipCountMult_[CLOTHES_ID::BASE_CLOTHES]
		= slipCountMult_[CLOTHES_ID::GOAL_CLOTHES]
		= slipCountMult_[CLOTHES_ID::GUM_CLOTHES]
		= slipCountMult_[CLOTHES_ID::TEST_CLOTHES]
		= 1.f;
	slipCountMult_[CLOTHES_ID::FLUFFY_CLOTHES] = 0.f;
	slipCountMult_[CLOTHES_ID::HANGER] = 1.5f;
	slipCountMult_[CLOTHES_ID::THIN_CLOTHES] = 2.f;

	pHeads_.resize(8);
	for (int i = 0; i < 8; i++)
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

	updateFunctionMap_[MODE_FALL] = std::bind(&Player::FallUpdate, this);
	updateFunctionMap_[MODE_SHOOT] = std::bind(&Player::ShootUpdate, this);
	updateFunctionMap_[MODE_SHOOT_END] = std::bind(&Player::ShootEndUpdate, this);
	updateFunctionMap_[MODE_BITE] = std::bind(&Player::BiteUpdate, this);
	updateFunctionMap_[MODE_SLIP] = std::bind(&Player::SlipUpdate, this);
	updateFunctionMap_[MODE_RESIST] = std::bind(&Player::ResistUpdate, this);
	updateFunctionMap_[MODE_CLEAR] = std::bind(&Player::ClearUpdate, this);
	updateFunctionMap_[MODE_PLAYERDEAD] = std::bind(&Player::DeadUpdate, this);

	worldSetMyDatas();
	StartPlayerSet();

	isClearShoot_ = false;

	StartPendulum();
	DeformationDraw();
}

Player::~Player()
{
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

	//for (int i = 0; i < neckDrawPoints.size();i++) {
	//	neckDrawPoints[i]=MathNeckPiecePoint(fPos_[i], multiplePos[i], resWidth);
	//}
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

		//for (int i = drawPoints.size() - 1; i > 0; i--) {
		//	auto p = drawPoints[i];
		//	Vector2 p0 = GetDrawPosVect(p.p0);
		//	Vector2 p1 = GetDrawPosVect(p.p1);
		//	Vector2 p2 = GetDrawPosVect(p.p2);
		//	Vector2 p3 = GetDrawPosVect(p.p3);
		//	DrawRectModiGraphF(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, 0, 0, 41, (int)(76.0f * correctionLens[i]), Sprite::GetInstance().GetHandle(SPRITE_ID::OROCHI_NECK_SPRITE), 1);
		//}
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

		PlayerNeckDraw().Draw(GetDrawPosVect(pHeadPoses_[i]), pHeads_[i]->GetDrawPos());
	}

	
	DebugDraw::DebugDrawFormatString(50, 100, GetColor(255, 255, 255), "%d", playerMode_);
	//int count = 0;
	//for (auto srmt : pHeadDead_)
	//{
	//	if (srmt)DebugDraw::DebugDrawFormatString(300, 300 + (30 * count), GetColor(255, 255, 255), "true");
	//	else DebugDraw::DebugDrawFormatString(300, 300 + (30 * count), GetColor(255, 255, 255), "false");
	//	count++;
	//}

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
		isUseKey_ = false;
		break;
	}

	default:
		break;
	}

}
void Player::StartPendulum() {
	Vector2 lngPs = pHeads_[currentHead_]->GetPosition() - position_;
	MultipleInit(lngPs.Length(), pHeads_[currentHead_]->GetPosition(), MathAngle(position_-pHeadPoses_[currentHead_],Vector2::Down),parameter_.radius);
	Multiple();
}

//現在使用しているHeadの座標を返す

Vector2 Player::GetCurrentPHeadPosition() const {
	return pHeads_[currentHead_]->GetPosition();
}
float Player::GetPlayerSwordAngle() const {
	return MathFormedAngle(pSword_->GetSwordEndPos() - position_);
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
void Player::MultipleInit(float len, const Vector2& fPos, float rot, float radius)
{
	PlayerNeckPendulumSupport::Init(pHeads_[currentHead_]->GetPosition(), pHeadPoses_[currentHead_], fPos_, multiplePos, mRot, mRot_spd, neckDrawPoints);
	return;
	
	mRot.clear();
	mRot_spd.clear();
	fPos_.clear();
	multiplePos.clear();
	drawPoints.clear();

	int s = (int)(len / oneLength);
	correctionLens.clear();
	std::vector<float> data(s + 1, 0.0f);
	correctionLens = data;
	correctionLens.front() = 1.0f;
	len -= oneLength;
	len -= radius;
	int i;
	for (i = 1; i < len / oneLength; i++) {
		correctionLens[i] = 1.0f;

	}
	float h = (MathHelper::Mod(len - oneLength, oneLength) + correctionHeight * 2.0f) / (oneLength + correctionHeight * 2.0f);

	correctionLens[i] = h;
	int fNum = s;

	fPos_.push_back(fPos);
	mRot.push_back(rot);
	mRot_spd.push_back(0.0f);
	multiplePos.push_back(Vector2(0.0f, 0.0f));
	for (int i = 0; i < fNum; i++) {
		auto px = fPos_[i].x + MathHelper::Cos(mRot[i]) * oneLength;
		auto py = fPos_[i].y + MathHelper::Sin(mRot[i]) * oneLength;

		mRot.push_back(rot);
		mRot_spd.push_back(0.0f);
		fPos_.push_back(Vector2(px, py));
		multiplePos.push_back(Vector2(0.0f, 0.0f));
	}
}
void Player::Multiple()
{
	if (GetUseKey()) {
		if ((mRot_spd[0] < 0 && (Keyboard::GetInstance().KeyStateDown(KEYCODE::D) || GamePad::GetInstance().Stick().x>0.01f)) ||
			(mRot_spd[0] > 0 && (Keyboard::GetInstance().KeyStateDown(KEYCODE::A) || GamePad::GetInstance().Stick().x < -0.01f)))
		{
			for (auto& r : mRot_spd) {
				r += sign(r)*1.f;
				r = MathHelper::Clamp(r, -60.f, 60.f);
			}
		}
	}
	pendulumVect_=PlayerNeckPendulumSupport::Pendulum(fPos_, multiplePos, mRot, mRot_spd, neckDrawPoints);
	pendulumVect_ *= jumpShotPower_;
	Vector2 positionSurp = position_ - pHeadPoses_[currentHead_];

	position_ = multiplePos.back()+ positionSurp;

	return;

	for (auto& i : mRot_spd) {
		i = MathHelper::Clamp(i, -80.f, 80.f);
	}

	Vector2 curdefPos = position_;

	//現在の重りの位置
	for (int i = 0; i < (int)fPos_.size(); i++)
	{
		auto px = fPos_[i].x + MathHelper::Cos(mRot[i]) * (oneLength);
		auto py = fPos_[i].y + MathHelper::Sin(mRot[i]) * (oneLength);

		//重力移動量を反映した重りの位置
		auto vx = px - fPos_[i].x;
		auto vy = py - fPos_[i].y;
		auto t = -(vy * gravity_) / (vx * vx + vy * vy);
		auto gx = px + t * vx;
		auto gy = py + gravity_ + t * vy;

		//２つの重りの位置の確度差
		auto rDiff = MathHelper::ATan(gy - fPos_[i].y, gx - fPos_[i].x);

		//角度差を角速度に加算
		auto sub = rDiff - mRot[i];
		sub -= std::floor(sub / 360.0f) * 360.0f;
		if (sub < -180.0f) sub += 360.0f;
		if (sub > 180.0f) sub -= 360.0f;

		if (i == 0)mRot_spd[i] = mRot_spd[i] + sub;
		else mRot_spd[i] = (mRot_spd[i - 1] + sub) * 0.8f;

		//角度に角速度を加算
		if (i == 0) mRot[i] += mRot_spd[i] * 0.1f;
		else mRot[i] = mRot[0] + mRot_spd[i] * 0.1f;

		//新しい重りの位置
		px = fPos_[i].x + MathHelper::Cos(mRot[i] /*+ rot2*/) * (oneLength);
		py = fPos_[i].y + MathHelper::Sin(mRot[i] /*+ rot2*/) * (oneLength);

		//重りの座標
		multiplePos[i].x = px;
		multiplePos[i].y = py;

		if (i > 0) fPos_[i] = multiplePos[i - 1];

		//頂点で正規化
		if (mRot[i] > 270.0f) mRot[i] -= 360.0f;
		if (mRot[i] < -90) mRot[i] += 360.0f;



		if (isUseKey_) {
			if ((mRot_spd[0] < 0&& (Keyboard::GetInstance().KeyStateDown(KEYCODE::D) || GamePad::GetInstance().Stick().x>0.01f)) ||
				(mRot_spd[0] > 0&& (Keyboard::GetInstance().KeyStateDown(KEYCODE::A) || GamePad::GetInstance().Stick().x < -0.01f)))
			{
				friction = 1.015f; //摩擦を減らす
			}
			else
			{
				friction = 0.985f; //摩擦を戻す
			}
			//摩擦
			if (multiplePos[i].y < pHeads_[currentHead_]->GetPosition().y) {
				friction = 0.9f; //上の振り子の重りより高く上がったら摩擦を増やす
				world_->FreeCameraPosY(false);
			}
		}
		else {
			if (mRot.front() > 225.0f) {
				friction = 0.985f; //摩擦を戻す
			}
		}

		mRot_spd[i] *= friction;
	}

	pHeadPoses_[currentHead_] = multiplePos[0];
	position_ = multiplePos.back();
	pendulumVect_ = (position_ - curdefPos);
	pendulumVect_.x = pendulumVect_.x*jumpShotPower_;
	pendulumVect_.y = pendulumVect_.y*jumpShotPower_;

}

//多重振り子に移動量を加算
void Player::UpdateMultiplePos() {
	SetMultiplePos((position_ - prevPosition_));
}
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
void Player::SetNeckNonMult() {
	mRot.clear();
	mRot_spd.clear();
	fPos_.clear();
	multiplePos.clear();
	correctionLens.clear();
	drawPoints.clear();

	for (int i = 0; i < defHeadLength; i++) {
		correctionLens.push_back(1.0f);
	}

	fPos_.push_back(pHeads_[currentHead_]->GetPosition());
	mRot.push_back(135.f);
	mRot_spd.push_back(0.0f);
	multiplePos.push_back(Vector2(0.0f, 0.0f));
	for (int i = 0; i < defHeadLength-1; i++) {
		auto px = fPos_[i].x + MathHelper::Cos(mRot[i]) * 100;
		auto py = fPos_[i].y + MathHelper::Sin(mRot[i]) * 100;

		mRot.push_back(135.f);
		mRot_spd.push_back(0.0f);
		fPos_.push_back(Vector2(px, py));
		multiplePos.push_back(Vector2(0.0f, 0.0f));
	}

}
void Player::DeformationDraw()
{
	return;
	drawPoints.clear();

	DrawPos p;
	for (int i = 0; i < (int)fPos_.size(); i++) {
		Vector2 v = fPos_[i] - multiplePos[i];
		Vector2 n = Vector2(-v.y, v.x).Normalize();
		if (i == 0) {
			p.p0 = fPos_[i] + n * (float)resWidth * correctionWidth * 1.2f;
			p.p1 = fPos_[i] - n * (float)resWidth * correctionWidth * 1.2f;
			if (v.Length() > 0) {
				p.p2 = multiplePos[i] - n * (float)resWidth * correctionWidth * 1.2f;
				p.p3 = multiplePos[i] + n * (float)resWidth * correctionWidth * 1.2f;
				p.p2 -= v.Normalize() * (float)correctionHeight * correctionWidth;
				p.p3 -= v.Normalize() * (float)correctionHeight * correctionWidth;
				p.p0 += v.Normalize() * (float)correctionHeight * correctionWidth;
				p.p1 += v.Normalize() * (float)correctionHeight * correctionWidth;
				drawPoints.push_back(p);
			}
		}
		else {
			p.p0 = fPos_[i] + n * (float)resWidth;
			p.p1 = fPos_[i] - n * (float)resWidth;
			if (v.Length() > 0) {
				p.p2 = p.p1 - v.Normalize() * 15.f * correctionLens[i];
				p.p3 = p.p0 - v.Normalize() * 15.f * correctionLens[i];
				p.p0 += v.Normalize() * (float)correctionHeight;
				p.p1 += v.Normalize() * (float)correctionHeight;
				p.p2 -= v.Normalize() * (float)correctionHeight;
				p.p3 -= v.Normalize() * (float)correctionHeight;
				drawPoints.push_back(p);
			}
		}
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

void Player::StartPlayerSet() {
	SetMode(MODE_BITE,false);
	pHeadLength_[currentHead_]=5.f;
	pHeads_[currentHead_]->StartPlayerHeadBite();

	StartPendulum();

	world_->FreeCameraPosY(false);
}

int Player::GetCurHead() const {
	return currentHead_;
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


//Headのレーンを本体のレーンに合わせる

void Player::SetMode(Player_Mode pMode,bool isPlaySE) {
	if (!isUseKey_)return;
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
		}
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
	if (laneNum_ + addNum > (maxLaneSize_ - 1) || laneNum_ + addNum<0)return;

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
		DeformationDraw();

		return;
	}


	world_->ChangeCamMoveMode(addNum);

	world_->sendMessage(EventMessage::START_LANE_CHANGE);
}

void Player::curPHeadSlip(bool isSlip) {
	pHeads_[currentHead_]->setIsBiteSlipWind(isSlip);
}

void Player::PHeadChanger(int rot) {
	if(!pHeadDead_[currentHead_]) world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerMetamorEffect>(world_, pHeads_[currentHead_]->GetPosition(), pHeads_[currentHead_]));
	PHeadLengthReset();
	(sign(rot) == 1) ? backChangeHead() : changeHead();
	
	world_->sendMessage(EventMessage::CHANGE_HEAD);

	Vector2 addVec = position_- pHeadPoses_[currentHead_];
	addVec=addVec.Normalize()*32.f;
	if (!pHeadDead_[currentHead_]) world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerMetamorEffect>(world_, pHeads_[currentHead_]->GetPosition(), pHeads_[currentHead_],0.3f, addVec));

	Sound::GetInstance().PlaySE(SE_ID::CHANGE_HEAD_SE);
}

void Player::SetStopPos(Vector2 target) {
	stopPos_ = target;
}


void Player::CurPHeadLengPlus(float addPow) {

	if (pHeadDead_[currentHead_])return;

	//floatの誤差と、addPowによるLengthのズレを補正するための値、首の長さの値に補正が発生した場合は、この補正値をそこに加算する事で、長さの違和感を解決する
	float fSaveAddNum = 0.2f;
	if (pHeadLength_[currentHead_] > 16.f+ fSaveAddNum) {
		pHeadLength_[currentHead_] = 16.f+ fSaveAddNum + chainAddLength_- chainAddLengthMath_;
		//長さの上昇に対する補間値
		chainAddLengthMath_ -= 0.4f;
		chainAddLengthMath_ = max(chainAddLengthMath_, 0.f);

		for (int i = currentHead_; i > -(int)pHeads_.size() + currentHead_; i--) {
			int trgNum = i;
			if (trgNum<0) {
				trgNum = trgNum + pHeads_.size();
			}

			if (pHeadDead_[trgNum])continue;
			if (trgNum == currentHead_)continue;

			if (pHeadLength_[trgNum] <= 0.1f) {
				if (chainLockCoolTime_>0) {
					break;
				}
				chainLockCoolTime_ = defChainLockCoolTime_;
				chainAddLength_ += 2.f;
				chainAddLengthMath_ += 2.f;
				Vector2 toPos = pHeadPoses_[trgNum] - position_;
				world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerFallPin>(world_, pHeads_[trgNum]->GetPosition(), toPos));
				pHeadDead_[trgNum] = true;
			}
			else {
			}
		}

		return;
	}
	//現在のHead以外の長さを伸ばした分だけマイナスする
	pHeadLength_[currentHead_] += addPow;
	//左隣がターゲット
	int targetNum = currentHead_ - 1;


	float LengthKeepNum = 1.01f;
	if (targetNum<0) {
		targetNum = targetNum + (int)pHeads_.size();
	}
	for (;;) {
		if (LengthKeepNum < 0.f)break;

		if (pHeadLength_[targetNum] < LengthKeepNum)
		{
			targetNum--;

			if (targetNum<0) {
				targetNum = targetNum + (int)pHeads_.size();
			}

			if (targetNum == currentHead_) {
				LengthKeepNum -= 1.f;
				targetNum--;

				if (targetNum<0) {
					targetNum = targetNum + (int)pHeads_.size();
				}
			}

			continue;
		}
		pHeadLength_[targetNum] -= addPow;
		if (pHeadLength_[targetNum] < 0)pHeadLength_[targetNum] = 0;


		break;
	}
}

void Player::CreateBiteEffect()
{
	if (pHeadDead_[currentHead_])return;
	Vector2 vel = pHeads_[currentHead_]->GetPosition() - position_;
	vel = vel.Normalize();
	world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerBiteEffect>(world_, pHeads_[currentHead_]->GetPosition() + vel * 30));

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

	for (int i = 0; i < 8; i++) {
		world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerDeadPin>(world_, position_));
	}
	Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
	Sound::GetInstance().StopSE(SE_ID::HEAD_SHOOT_SE);
}

void Player::FallUpdate()
{
	pGrav_ += defPGravPow;
	if (isUseKey_) {
		if ((GamePad::GetInstance().Stick().x < -0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::A))) &&
			isCanNextHeadRot) {
			SetMode(MODE_FALL);
			//キーを押し直したかの判断
			PHeadChanger();
			world_->sendMessage(EventMessage::CHANGE_HEAD_KEY);
			isCanNextHeadRot = false;
		}
		if ((GamePad::GetInstance().Stick().x > 0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::D))) &&
			isCanNextHeadRot) {
			SetMode(MODE_FALL);
			//キーを押し直したかの判断
			PHeadChanger(1);
			world_->sendMessage(EventMessage::CHANGE_HEAD_KEY);
			isCanNextHeadRot = false;
		}
		if ((MathHelper::Abs(GamePad::GetInstance().Stick().x) < 0.3f &&
			(Keyboard::GetInstance().KeyStateUp(KEYCODE::D) &&
				Keyboard::GetInstance().KeyStateUp(KEYCODE::A)))) {
			isCanNextHeadRot = true;
		}

		if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM6) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
			world_->sendMessage(EventMessage::NECK_SHOOT);
			SetMode(MODE_SHOOT);
			isNextPushKey_ = false;
		}
		if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::N) || GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2)) {
			pHeads_[currentHead_]->SetBiteSprite();
			SetMode(MODE_SHOOT_END);
		}
	}
		position_ += velocity_ + pendulumVect_;

		slipCount_ = defSlipCount;

		SetDrawNeck(position_,pHeads_[currentHead_]->GetPosition());

}

void Player::ShootUpdate()
{
	pGrav_ += defPGravPow;
	if (isUseKey_) {
		if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::N)|| GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM2)) {
			pHeads_[currentHead_]->SetBiteSprite();
			SetMode(MODE_SHOOT_END);
		}
		else if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM6) || Keyboard::GetInstance().KeyStateDown(KEYCODE::M)) {
			CurPHeadLengPlus(headShotPower);
		}
		else {
			SetMode(MODE_FALL);
		}

		if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM6) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
			SetMode(MODE_SHOOT);
			isNextPushKey_ = false;
		}
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
	if (isUseKey_) {
		if ((GamePad::GetInstance().Stick().x < -0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::A))) && isCanNextHeadRot) {
			PHeadChanger();
			world_->sendMessage(EventMessage::CHANGE_HEAD_KEY);
			isCanNextHeadRot = false;
		}
		if ((GamePad::GetInstance().Stick().x > 0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::D))) && isCanNextHeadRot) {
			//キーを押し直したかの判断
			PHeadChanger(1);
			world_->sendMessage(EventMessage::CHANGE_HEAD_KEY);
			isCanNextHeadRot = false;
		}
		if ((MathHelper::Abs(GamePad::GetInstance().Stick().x) < 0.3f && (Keyboard::GetInstance().KeyStateUp(KEYCODE::D) && Keyboard::GetInstance().KeyStateUp(KEYCODE::A)))) {
			isCanNextHeadRot = true;
		}


		if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM6) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
			SetMode(MODE_SHOOT);
			isNextPushKey_ = false;
		}
	}
		position_ += velocity_ + pendulumVect_;

		slipCount_ = defSlipCount;

		SetDrawNeck(position_, pHeads_[currentHead_]->GetPosition());
}

void Player::BiteUpdate()
{
	Multiple();
	if (isUseKey_) {

		if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM1) || Keyboard::GetInstance().KeyStateDown(KEYCODE::W)) {
			if (!pSword_->GetUseSword()) {
				world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<GetSwordEffect>(world_, pSword_->GetPosition(), pSword_.get()));
				world_->sendMessage(EventMessage::USE_SWORD);
				pSword_->SetUseSword(true);
				Sound::GetInstance().PlaySE(SE_ID::CREATE_SWORD_SE);
			}
		}
		if ((GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM5) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::S))) {
			if (GamePad::GetInstance().Stick().y > 0.5f || Keyboard::GetInstance().KeyStateDown(KEYCODE::W)) {
				SetNextLane(1);
			}
			else if (mRot.front() < 0.f || mRot.front() > 180.f) {
				SetNextLane(-1);
			}
		}

		if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::N)) {
			SetMode(MODE_FALL);
			//Headを交代する
			PHeadChanger();
			isNextPushKey_ = false;
			isCanNextHeadRot = false;
		}
	}
		//下へのベクトルと現在のプレイヤーの位置ベクトルのなす角を取る
		Vector2 ptoPHVec = pHeads_[currentHead_]->GetPosition() - position_;
		float ptoDownCos = Vector2::Dot(ptoPHVec, Vector2::Down) / (ptoPHVec.Length()*Vector2::Down.Length());
		float ptoDownAngle = MathHelper::ACos(ptoDownCos);
		//取った角度が10以下なら此方に入れる

		if (isUseKey_) {
			if (otherClothesID_ == CLOTHES_ID::FLUFFY_CLOTHES && (MathHelper::Abs(GamePad::GetInstance().Stick().x > 0.01f) ||
				Keyboard::GetInstance().KeyStateDown(KEYCODE::D) ||
				Keyboard::GetInstance().KeyStateDown(KEYCODE::A)) &&
				MathHelper::Abs(mRot_spd[0]) <= 0.01f&&
				ptoDownAngle <= 10.f&&
				pHeads_[currentHead_]->GetPosition().y < position_.y) {
				for (auto& spd : mRot_spd) {
					spd += (spdLimit)*2;
				}
			}

			slipCount_ -= 0.016f*slipCountMult_[otherClothesID_];
			if (slipCount_ <= 0.f) {
				SetMode(MODE_SLIP);
				//首を殺して
				Vector2 toPos = pHeadPoses_[currentHead_]- position_;
				world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerFallPin>(world_,pHeads_[currentHead_]->GetPosition(),toPos));
				pHeadDead_[currentHead_] = true;
				//スティックをロックする
				isCanNextHeadRot = false;
				isSlipped_ = true;
				//スリップモードに移行すると同時に、その時点のベクトルをHeadに格納する
				pHeads_[currentHead_]->SetPosAddVect(pHeads_[currentHead_]->GetPosition() - position_);
				PHeadChanger();
			}
			slipCount_ = max(slipCount_, 0.f);
		}
		DeformationDraw();

}

void Player::SlipUpdate()
{
	pGrav_ += defPGravPow;

	//if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyStateDown(KEYCODE::M)) {
	//}
	//else{
	//	playerMode_ = MODE_FALL;
	//}
	if (isUseKey_) {
		if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
			//if (GetIsSlipped()) {
				//MODE_SLIP;
				//Headを交代する
			PHeadChanger();
			//}
			SetMode(MODE_SHOOT);//playerMode_ = MODE_SHOOT;
			isNextPushKey_ = false;

		}
		if ((GamePad::GetInstance().Stick().x < -0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::A))) && isCanNextHeadRot) {
			SetMode(MODE_FALL);//playerMode_ = MODE_FALL;
			PHeadChanger();
			world_->sendMessage(EventMessage::CHANGE_HEAD_KEY);
			isCanNextHeadRot = false;
		}
		if ((GamePad::GetInstance().Stick().x > 0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::D))) && isCanNextHeadRot) {
			SetMode(MODE_FALL);//playerMode_ = MODE_FALL;
			//キーを押し直したかの判断
			PHeadChanger(1);
			world_->sendMessage(EventMessage::CHANGE_HEAD_KEY);
			isCanNextHeadRot = false;
		}
		if ((MathHelper::Abs(GamePad::GetInstance().Stick().x) < 0.3f && (Keyboard::GetInstance().KeyStateUp(KEYCODE::D) && Keyboard::GetInstance().KeyStateUp(KEYCODE::A)))) {
			isCanNextHeadRot = true;
		}

	}

		position_ += velocity_ + pendulumVect_;

		slipCount_ = defSlipCount;

		UpdateMultiplePos();
		DeformationDraw();
		
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
	if ((pHeads_[currentHead_]->GetDrawPos().y>= WINDOW_HEIGHT+99.f)||isClearShoot_) {
		clearShootTimer_.Action();
		pendulumVect_.y *= 0.98f;
		pendulumVect_.y = max(50.f,pendulumVect_.y);

		position_ -= pendulumVect_;
		UpdateMultiplePos();
		DeformationDraw();
	}
	else {
		Multiple();
		DeformationDraw();
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
	DeformationDraw();
}
