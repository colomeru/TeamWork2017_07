#include "CreditPlayer.h"
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
#include"PlayerNeckDraw.h"
#include"PlayerDeadPin.h"
#include"PlayerDeadHead.h"
#include"../../sound/sound.h"
#include"../../cheat/CheatData.h"
#include "../../scene/Credit2Scene.h"

static const float headShotPower = 0.3f;
static const float defMaxChainLength = 16.f;
static const float defJumpShotPower = 1.5f;
//リソースの横サイズ
const int resWidth = 15;
const int correctionHeight = 30;
const float correctionWidth = 1.8f;
//リソースの縦サイズ
const float oneLength = 30.f;

//コンストラクタ
CreditPlayer::CreditPlayer(IWorld * world, int maxLaneSize, int startLane)
	:Actor(world), isUseKey_(true), clearAddRot_(0.0f),
	isHit_(false), fulcrum_(0.0f, 0.0f), rot_(135.f), rot_spd_(-3.0f), length_(300.0f), gravity_(0.5f), currentHead_(0),
	headChangeTime_(0), pGrav_(defPGravPow), maxChainLength_(defMaxChainLength), playerMode_(MODE_FALL), isNextPushKey_(true),
	pendulumVect_(Vector2::Zero), slipCount_(defSlipCount), jumpShotPower_(defJumpShotPower), isSlipped_(false), chainLock_(false),/* isCanChangeLane_(false),*/
	otherClothesID_(CLOTHES_ID::FLUFFY_CLOTHES), friction(0.998f), spdLimit(2.75f), isCanNextHeadRot(true), chainLockCoolTime_(defChainLockCoolTime_), chainAddLength_(0),
	chainAddLengthMath_(0), maxLaneSize_(maxLaneSize), isPlayerFallLane_(false), changeType_(LaneChangeType::LaneChange_Normal), slipResistTime_(defResistTime), headPosAddVect_(Vector2::Zero)

{

	addscorelist_[0] = 300;
	addscorelist_[1] = 200;
	addscorelist_[2] = 100;

	//laneNum_ = startLane;
	laneNum_ = 1;

	spriteId_ = SPRITE_ID::PBODY_SPRITE;

	parameter_.ID = ACTOR_ID::PLAYER_ACTOR;
	parameter_.radius = Sprite::GetInstance().GetSize(spriteId_).x / 2;
	parameter_.size = Sprite::GetInstance().GetSize(spriteId_);
	parameter_.HP = 10;
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3::Zero);

	position_ = Vector2(0, 0);

	prevPosition_ = position_;

	slipCountMult_[CLOTHES_ID::BASE_CLOTHES]
		= slipCountMult_[CLOTHES_ID::GOAL_CLOTHES]
		= slipCountMult_[CLOTHES_ID::GUM_CLOTHES]
		= slipCountMult_[CLOTHES_ID::TEST_CLOTHES]
		= 1.f;
	slipCountMult_[CLOTHES_ID::FLUFFY_CLOTHES] = 0.f;
	slipCountMult_[CLOTHES_ID::HANGER] = 1.5f;
	slipCountMult_[CLOTHES_ID::THIN_CLOTHES] = 2.f;

	auto player = static_cast<Player*>(const_cast<Actor*>(this->GetActor()));
	pHeads_.resize(8);
	for (int i = 0; i < 8; i++)
	{
		//8方向のうち、各頭に対応した位置を作成
		Vector3 tgtRot = Vector3(pHDist.x, pHDist.y)*Matrix::CreateRotationZ((i + headAngleSetter) * 45);
		Vector2 cgToV2 = position_ + Vector2(tgtRot.x, tgtRot.y);
		pHeadPoses_.push_back(cgToV2);
		pHeadLength_.push_back(defHeadLength);
		pHeadDead_.push_back(false);

		pHeads_[i] = (std::make_shared<Player_Head>(world, player, pHeadPoses_[i], i));
		//world_->Add(ACTOR_ID::PLAYER_HEAD_ACTOR, pHeads_[i]);

		SetMyHeadLaneNum(i);
	}
	pSword_ = std::make_shared<Player_Sword>(world, player, position_);
	//world_->Add(ACTOR_ID::PLAYER_SWORD_ACTOR, pSword_);

	updateFunctionMap_[MODE_FALL] = std::bind(&CreditPlayer::FallUpdate, this);
	updateFunctionMap_[MODE_SHOOT] = std::bind(&CreditPlayer::ShootUpdate, this);
	updateFunctionMap_[MODE_SHOOT_END] = std::bind(&CreditPlayer::ShootEndUpdate, this);
	updateFunctionMap_[MODE_BITE] = std::bind(&CreditPlayer::BiteUpdate, this);
	updateFunctionMap_[MODE_SLIP] = std::bind(&CreditPlayer::SlipUpdate, this);
	updateFunctionMap_[MODE_RESIST] = std::bind(&CreditPlayer::ResistUpdate, this);
	updateFunctionMap_[MODE_CLEAR] = std::bind(&CreditPlayer::ClearUpdate, this);
	updateFunctionMap_[MODE_PLAYERDEAD] = std::bind(&CreditPlayer::DeadUpdate, this);

	worldSetMyDatas();
	StartPlayerSet();

	isClearShoot_ = false;

	StartPendulum();
	DeformationDraw();

}

//デストラクタ
CreditPlayer::~CreditPlayer()
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
	Sound::GetInstance().StopSE(SE_ID::HEAD_SHOOT_SE);
}

//更新
void CreditPlayer::Update()
{
	return;
	chainLockCoolTime_--;
	chainLockCoolTime_ = MathHelper::Clamp(chainLockCoolTime_, 0, defChainLockCoolTime_);
	//レーン変更のクールタイムを設定

	if (!GetIsClearShoot())pendulumVect_ -= pendulumVect_*0.05f;

	length_ = Vector2::Distance(pHeads_[currentHead_]->GetPosition(), position_);

	velocity_ = Vector2::Zero;
	isHit_ = false;

	//重力
	velocity_.y += pGrav_*pGrav_*defGravAddPow;

	//移動したかをリセットしてから入力関数に入る
	isNextLaneBite_ = false;
	//入力による動作をまとめた関数
	PlayerInputControl();
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

	laneNum_ = 1;

}

//最初の更新
void CreditPlayer::FastUpdate() {
	if (!world_->GetIsCamChangeMode()) {
		int nexLane = world_->GetKeepDatas().nextLane_;
		//UpdateLaneNum(nexLane, changeType_);
		world_->GetCanChangedKeepDatas().SetPlayerNextLane(0);
	}

}

//描画
void CreditPlayer::Draw() const
{
	return;
	auto is = Matrix::CreateRotationZ(angle_);
	auto pos = drawPos_;
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
	float aHeadAngle = (360 / pHeads_.size());
	float angle = currentHead_*aHeadAngle + aHeadAngle*(headChangeTime_ / defHeadChangeTime);
	Sprite::GetInstance().Draw(spriteId_, GetDrawPosVect(position_), crcOrigin, parameter_.spriteAlpha_, Vector2::One, angle);

	if (!pHeadDead_[currentHead_]) {

		for (int i = drawPoints.size() - 1; i > 0; i--) {
			auto p = drawPoints[i];
			Vector2 p0 = GetDrawPosVect(p.p0);
			Vector2 p1 = GetDrawPosVect(p.p1);
			Vector2 p2 = GetDrawPosVect(p.p2);
			Vector2 p3 = GetDrawPosVect(p.p3);
			DrawRectModiGraph(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, 0, 0, 41.0f, 76.0f * correctionLens[i], Sprite::GetInstance().GetHandle(SPRITE_ID::OROCHI_NECK_SPRITE), 1);
		}
	}
	for (int i = 0; i < pHeads_.size(); i++) {
		if (i == currentHead_)continue;
		if (pHeadDead_[i])continue;

		PlayerNeckDraw().Draw(GetDrawPosVect(pHeadPoses_[i]), pHeads_[i]->GetDrawPos());
	}

	if (BuildMode != 1)return;
	DrawFormatString(50, 100, GetColor(255, 255, 255), "%d", playerMode_);
	int count = 0;
	for (auto srmt : pHeadDead_)
	{
		if (srmt)DrawFormatString(300, 300 + (30 * count), GetColor(255, 255, 255), "true");
		else DrawFormatString(300, 300 + (30 * count), GetColor(255, 255, 255), "false");
		count++;
	}

	DrawFormatString(600, 750, GetColor(255, 0, 0), "%f", mRot.front());


}

//受動更新
void CreditPlayer::OnUpdate()
{
}

//衝突時のアクション
void CreditPlayer::OnCollide(Actor& other, CollisionParameter colpara)
{
	isHit_ = true;
}

//
void CreditPlayer::OnMessage(EventMessage message, void * param)
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

//
bool CreditPlayer::CamMoveUpdate() {
	if (world_->GetKeepDatas().nextLane_ < 0) {
		CamMoveUp();
	}
	else {
		CamMoveDown();
	}

	return true;
}

//
void CreditPlayer::CamMoveUp() {
}

//
void CreditPlayer::CamMoveDown() {
	//	if (changeType_ == LaneChangeType::LaneChange_Normal) {
	//		return;
	//	}
	//	drawPos_ = GetDrawPosVect(position_);
		//LaneChangeFall();
}

//
void CreditPlayer::LaneChangeFall() {

	float laneLerpNum = world_->GetKeepDatas().changeLaneLerpPos_;
	laneLerpNum = min(1.f, laneLerpNum);
	int targetNum = world_->GetKeepDatas().playerLane_ - laneNum_ + 2;
	drawAddPos_.y = MathHelper::Lerp(defDrawLineChangePosY[targetNum], defDrawLineChangePosY[targetNum - 1], laneLerpNum) - defDrawLineChangePosY[targetNum];

	if (changeType_ == LaneChangeType::LaneChange_Fall) {
		//drawAddPos_.y = MathHelper::Lerp(defDrawLineChangePosY[targetNum], defDrawLineChangePosY[targetNum - 1], laneLerpNum) - defDrawLineChangePosY[targetNum];
		drawAddPos_.y = drawAddPos_.y * fallAddPosMult;
	}
}

//
bool CreditPlayer::isLaneChangeFall() const {
	return changeType_ == LaneChangeType::LaneChange_Fall;
}

//
void CreditPlayer::Pendulum(Vector2 fulcrum, float length)
{
	//振り子
	//支点Posを設定
	float fx = fulcrum.x;
	float fy = fulcrum.y;
	//Gravityを設定
	float g = gravity_;
	//変更前のpositionを保存
	Vector2 curdefPos = position_;
	//それぞれの首の移動先座標を格納可能に
	std::array<Vector2, 8> outPos;
	//自身の半径を設定
	float r = parameter_.radius;
	float r1 = rot_;
	float r2 = 0.0f;
	bool rotDirection = true;

	//それぞれの首の長さを設定
	std::vector<float> neckLen = pHeadLength_;
	//現在の重りの位置
	auto px = fx + MathHelper::Cos(rot_) * length;
	auto py = fy + MathHelper::Sin(rot_) * length;

	//重力移動量を反映した重りの位置
	auto vx = px - fx;
	auto vy = py - fy;
	auto t = -(vy * g) / (vx * vx + vy * vy);
	auto gx = px + t * vx;
	auto gy = py + g + t * vy;

	//２つの重りの位置の確度差
	auto rDiff = MathHelper::ATan(gy - fy, gx - fx);

	//角度差を角速度に加算
	auto sub = rDiff - rot_;
	sub -= std::floor(sub / 360.0f) * 360.0f;
	if (sub < -180.0f) sub += 360.0f;
	if (sub > 180.0f) sub -= 360.0f;
	rot_spd_ += sub;

	//摩擦
	rot_spd_ *= friction;
	//角度に角速度を加算
	rot_ += rot_spd_;
	//rot_ = MathHelper::Clamp(rot_, -45.f, 225.f);
	//新しい重りの位置
	px = fx + MathHelper::Cos(rot_) * length;
	py = fy + MathHelper::Sin(rot_) * length;

	//重りの座標
	position_.x = px;
	position_.y = py;

	//角度調整
	float rot2 = rot_ - 90.0f;

	pHeadPoses_[currentHead_].x = position_.x + MathHelper::Cos(angle_ + rot2) * r;
	pHeadPoses_[currentHead_].y = position_.y + MathHelper::Sin(angle_ + rot2) * r;
	outPos[currentHead_].x = pHeadPoses_[currentHead_].x + MathHelper::Cos(angle_ + rot2) * (neckLen[currentHead_] - r);
	outPos[currentHead_].y = pHeadPoses_[currentHead_].y + MathHelper::Sin(angle_ + rot2) * (neckLen[currentHead_] - r);

	Vector2 pendulumAngleVec = fulcrum - position_;
	float angleDtData = Vector2::Dot(Vector2::Right, pendulumAngleVec.Normalize());

	angle_ = (angleDtData * 180) / 2;


	//1フレーム前のrotと比較
	r2 = rot_;
	if (r1 < r2) //右に回っていれば
	{
		rotDirection = true;
		r1 = rot_;
	}
	else if (r1 > r2) //左に回っていれば
	{
		rotDirection = false;
		r1 = rot_;
	}

	//circleの当たり判定
	if (
		(rot_spd_ < 0 && !rotDirection && (Keyboard::GetInstance().KeyStateDown(KEYCODE::D) || GamePad::GetInstance().Stick().x>0.01f)) ||
		(rot_spd_ > 0 && rotDirection && (Keyboard::GetInstance().KeyStateDown(KEYCODE::A) || GamePad::GetInstance().Stick().x < -0.01f)))
	{
		friction = 1.015f; //摩擦を減らす

	}
	else
	{
		friction = 0.985f; //摩擦を戻す
	}

	if (pHeads_[currentHead_]->GetPosition().y > position_.y) {
		friction = 0.945f; //摩擦を戻す
	}
	//スピード制限
	//
	spdLimit = sqrt(2 * g * (MathHelper::Sin(90.0f) * pHDist.Length())) / ((pHDist.Length()) * 0.02f);
	//spdLimit = sqrt(3.f) / ((pHDist.Length()) * 0.05f);
	rot_spd_ = MathHelper::Clamp(rot_spd_, -spdLimit, spdLimit);

	//頂点で正規化
	if (rot_ > 270.0f) rot_ = -90.0f;
	if (rot_ < -90) rot_ = 270.0f;


	pendulumVect_ = (position_ - curdefPos);
	pendulumVect_.x = pendulumVect_.x*jumpShotPower_;
	pendulumVect_.y = pendulumVect_.y*jumpShotPower_;

}

//
void CreditPlayer::StartPendulum() {
	Vector2 lngPs = pHeads_[currentHead_]->GetPosition() - position_;
	MultipleInit(lngPs.Length(), pHeads_[currentHead_]->GetPosition(), MathAngle(position_ - pHeadPoses_[currentHead_], Vector2::Down), parameter_.radius);
	Multiple();
}

//
Vector2 CreditPlayer::GetHeadPos(int headNum) const {
	return pHeadPoses_[headNum];
}

//Headの長さを実際のゲームに反映される値に変換して返す
float CreditPlayer::GetHeadLengthChangeToPosMult(int headNum) const {
	return pHeadLength_[headNum] * HeadShootMult;
}

//現在使用しているHeadの座標を返す
Vector2 CreditPlayer::GetCurrentPHeadPosition() const {
	return pHeads_[currentHead_]->GetPosition();
}

//
float CreditPlayer::GetPlayerSwordAngle() const {
	return MathFormedAngle(pSword_->GetSwordEndPos() - position_);
}

//振り子の支点位置を移動する
void CreditPlayer::SetMultipleFulcrumPos(const Vector2 & pos) {
	fPos_.front() = pos;
}
bool CreditPlayer::GetIsSwordActive() const {
	return pSword_->GetUseSword();
}

//クリア後に次ステージに飛ぶ状態かを調べる
bool CreditPlayer::GetIsClearBite() const
{
	return (mRot.front() >= -45.0f&&playerMode_ == MODE_CLEAR);
}

//
bool CreditPlayer::GetIsClearShoot() const
{
	return (mRot.front() < -45.0f&&playerMode_ == MODE_CLEAR);
}

//
Actor * CreditPlayer::GetCurrentHead() const {
	return pHeads_[currentHead_].get();
}

//
void CreditPlayer::deadLine()
{
	if (playerMode_ == MODE_CLEAR)return;
	//死亡条件を満たしたらDeadする(SetModeは同モードなら更新を行わない)
	if (isPlayerDead())SetMode(MODE_PLAYERDEAD);
	if (laneNum_ >= 2) {
		world_->FreeCameraPosY(false);

	}
	else if (position_.y >= 500 && playerMode_ != MODE_BITE) {
		//SetNextLane(1, LaneChangeType::LaneChange_Fall);

	}
	else if (position_.y >= 300 && playerMode_ != MODE_BITE) {
		world_->FreeCameraPosY(true);
		world_->GetCanChangedKeepDatas().isFallCamMode_ = true;
		world_->GetCanChangedKeepDatas().fallAddPos_ = 100.f;

	}

}

void CreditPlayer::GravityReset()
{
	pGrav_ = defPGravPow;
}

//
void CreditPlayer::MultipleInit(float len, const Vector2& fPos, float rot, float radius)
{
	mRot.clear();
	mRot_spd.clear();
	fPos_.clear();
	multiplePos.clear();
	drawPoints.clear();

	int s = len / oneLength;
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

//
void CreditPlayer::Multiple()
{
	Vector2 curdefPos = position_;

	//現在の重りの位置
	for (int i = 0; i < fPos_.size(); i++)
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


		//anyPos2[i] = fPos[i];


		//mRot_spd[i] = MathHelper::Clamp(mRot_spd[i],-3.f,3.f);
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
			if ((mRot_spd[0] < 0 && (Keyboard::GetInstance().KeyStateDown(KEYCODE::D) || GamePad::GetInstance().Stick().x>0.01f)) ||
				(mRot_spd[0] > 0 && (Keyboard::GetInstance().KeyStateDown(KEYCODE::A) || GamePad::GetInstance().Stick().x < -0.01f)))
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
void CreditPlayer::UpdateMultiplePos() {
	SetMultiplePos((position_ - prevPosition_));
}

//
void CreditPlayer::SetMultiplePos(const Vector2 & addpos) {
	for (int i = 0; i < multiplePos.size(); i++) {
		multiplePos[i] += addpos;
		if (i > 0) fPos_[i] = multiplePos[i - 1];
	}
	fPos_.front() = pHeads_[currentHead_]->GetPosition() + addpos;
}


//多重振り子を強制的に移動
void CreditPlayer::AddMultiplePos(const Vector2 & addPos) {
	for (int i = 0; i < multiplePos.size(); i++) {
		multiplePos[i] += (addPos);
		if (i > 0) fPos_[i] = multiplePos[i - 1];
	}
}

//
void CreditPlayer::SetNeckNonMult() {
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
	for (int i = 0; i < defHeadLength - 1; i++) {
		auto px = fPos_[i].x + MathHelper::Cos(mRot[i]) * 100;
		auto py = fPos_[i].y + MathHelper::Sin(mRot[i]) * 100;

		mRot.push_back(135.f);
		mRot_spd.push_back(0.0f);
		fPos_.push_back(Vector2(px, py));
		multiplePos.push_back(Vector2(0.0f, 0.0f));
	}

}

//
void CreditPlayer::DeformationDraw()
{
	drawPoints.clear();

	DrawPos p;
	for (int i = 0; i < fPos_.size(); i++) {
		Vector2 v = fPos_[i] - multiplePos[i];
		Vector2 n = Vector2(-v.y, v.x).Normalize();
		if (i == 0) {
			p.p0 = fPos_[i] + n * resWidth * correctionWidth * 1.2f;
			p.p1 = fPos_[i] - n * resWidth * correctionWidth * 1.2f;
			if (v.Length() > 0) {
				p.p2 = multiplePos[i] - n * resWidth * correctionWidth * 1.2f;
				p.p3 = multiplePos[i] + n * resWidth * correctionWidth * 1.2f;
				p.p2 -= v.Normalize() * correctionHeight * correctionWidth;
				p.p3 -= v.Normalize() * correctionHeight * correctionWidth;
				p.p0 += v.Normalize() * correctionHeight * correctionWidth;
				p.p1 += v.Normalize() * correctionHeight * correctionWidth;
				drawPoints.push_back(p);
			}
		}
		else {
			p.p0 = fPos_[i] + n * resWidth;
			p.p1 = fPos_[i] - n * resWidth;
			if (v.Length() > 0) {
				p.p2 = p.p1 - v.Normalize() * 15.f * correctionLens[i];
				p.p3 = p.p0 - v.Normalize() * 15.f * correctionLens[i];
				p.p0 += v.Normalize() * correctionHeight;
				p.p1 += v.Normalize() * correctionHeight;
				p.p2 -= v.Normalize() * correctionHeight;
				p.p3 -= v.Normalize() * correctionHeight;
				drawPoints.push_back(p);
			}
		}
	}
}

//
void CreditPlayer::SetDrawNeck(const Vector2 & bodyPoint, const Vector2 & headPoint)
{
	SetDrawPoint(bodyPoint, headPoint);
	SetDrawNeckParts(bodyPoint, headPoint);


}

//
void CreditPlayer::SetDrawPoint(const Vector2 & bodyPoint, const Vector2 & headPoint)
{
	//MultとfPosの数がここで合わなくなる
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

	DrawPos p;
	for (int i = 0; i < s; i++) {
		fPos_.push_back(headPoint + (vel*i));
		multiplePos.push_back(headPoint + (vel*(i + 1)));
		mRot.push_back(0);
		mRot_spd.push_back(0);
	}


}

//
void CreditPlayer::SetDrawNeckParts(const Vector2 & bodyPoint, const Vector2 & headPoint)
{
	drawPoints.clear();
	DrawPos p;

	Vector2 dir(bodyPoint - headPoint);

	////頭から体へのベクトル(正規化)
	for (int i = 0; i < fPos_.size(); i++) {
		if ((fPos_[i] - position_).Length() <= parameter_.radius) {
			if (drawPoints.empty()) {
				p = MathDrawPoint(fPos_[i], Vector2::Zero, resWidth, oneLength);
			}
			else p = drawPoints.back();
			drawPoints.push_back(p);
			continue;
		}
		p = MathDrawPoint(fPos_[i], dir, resWidth, oneLength);
		drawPoints.push_back(p);
	}
}

//
Vector2 CreditPlayer::GetCurrentHeadLength() const {
	Vector2 vel = pHeads_[currentHead_]->GetPosition() - position_;
	Vector2 msx = pHeadLength_[currentHead_] * Vector2(32.f, 32.f).Length();
	float velLe = vel.Length();
	float msxLe = msx.Length();
	float lep = MathHelper::Abs(velLe - msxLe);
	return msx;
}

//
void CreditPlayer::HeadPosUpdate()
{
	headChangeTime_ -= 0.016f*sign(headChangeTime_);

	rotTimer = 0;
	if (MathHelper::Abs(headChangeTime_) <= 0.01f)headChangeTime_ = 0;
	else if (MathHelper::Abs(headChangeTime_) > 0)rotTimer = headChangeTime_ * 5;//MathHelper::Abs(defHeadChangeTime/1.f);

	rotTimer += clearAddRot_;
	for (int i = 0; i < pHeadPoses_.size(); i++) {
		Vector3 tgtRot = Vector3(pHDist.x, pHDist.y)*Matrix::CreateRotationZ(((i + headAngleSetter - currentHead_) * 45)/*+angle_*/ + ((rotTimer)* 45));
		Vector2 cgToV2 = position_ + Vector2(tgtRot.x, tgtRot.y);
		pHeadPoses_[i] = cgToV2;
	}
}

//
void CreditPlayer::SwordPosUpdate() {
	if (!GetIsBiteMode() && !world_->GetIsCamChangeMode()) {
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
void CreditPlayer::changeHead() {
	//回転した時点でSlip状態を直す
	isSlipped_ = false;
	//pHeadLength_[currentHead_] = defHeadLength*HeadShootMult;
	currentHead_++;
	if (currentHead_ >= (int)pHeads_.size())currentHead_ = 0;
	headChangeTime_ = defHeadChangeTime;
}

//使用する頭を左隣の物に変更
void CreditPlayer::backChangeHead() {
	//pHeadLength_[currentHead_] = defHeadLength*HeadShootMult;
	currentHead_--;
	if (currentHead_ < 0)currentHead_ = pHeads_.size() - 1;
	headChangeTime_ = -defHeadChangeTime;
}

//
void CreditPlayer::StartPlayerSet() {
	SetMode(MODE_BITE);// playerMode_ = MODE_BITE;
	pHeadLength_[currentHead_] = 5.f;
	pHeads_[currentHead_]->StartPlayerHeadBite();

	StartPendulum();

	world_->FreeCameraPosY(false);
}

//
int CreditPlayer::GetCurHead() const {
	return currentHead_;
}

//
bool CreditPlayer::GetPHeadDead(int pHeadNum) const {
	return pHeadDead_[pHeadNum];
}

//
void CreditPlayer::CurHeadBite(const Vector2 & target) {

	if (playerMode_ != MODE_SHOOT_END)return;

	CreateBiteEffect();
	//スコア加算
	world_->sendMessage(EventMessage::ADD_SCORE, (int*)addscorelist_[laneNum_]);

	SetMode(MODE_BITE);//playerMode_ = MODE_BITE;
	pGrav_ = defPGravPow;
	//角度を求める
	rot_ = 135;
	rot_spd_ = -3.0f;

	StartPendulum();
}

//
void CreditPlayer::ResurrectHead() {
	for (int i = currentHead_; i < pHeads_.size() + currentHead_; i++) {
		int trgNum = i;
		if (trgNum >= pHeads_.size()) {
			trgNum = trgNum - pHeads_.size();
		}
		if (!pHeadDead_[trgNum])continue;
		pHeadDead_[trgNum] = false;
		break;
	}
}

//
void CreditPlayer::SetOtherClothesID_(CLOTHES_ID cId) {
	otherClothesID_ = cId;
}

//噛み付ける状態かを返す(レジスト含む)
bool CreditPlayer::GetIsShootMode() const {
	return playerMode_ == MODE_SHOOT;
}

//
bool CreditPlayer::GetIsBiteMode() const {
	return playerMode_ == MODE_BITE || playerMode_ == MODE_RESIST;
}

//
bool CreditPlayer::GetIsResistMode() const {
	return playerMode_ == MODE_RESIST;
}

//
bool CreditPlayer::GetIsClearMode() const {
	return playerMode_ == MODE_CLEAR;
}

//
int CreditPlayer::GetPHeadLiveCount() const {
	int result = 0;
	for (auto i : pHeadDead_) {
		if (!i)result++;
	}
	return result;
}


//Headのレーンを本体のレーンに合わせる
//噛み付き状態にするかをセット、
void CreditPlayer::SetIsBiteMode(bool ismode) {
	int setMode = (ismode) ? MODE_BITE : MODE_SHOOT;
	playerMode_ = setMode;
}

//
void CreditPlayer::SetMode(int pMode) {
	if (!isUseKey_)return;
	if (pMode == playerMode_)return;

	playerMode_ = pMode;
	//if (pMode == MODE_SLIP)
	headPosAddVect_ = Vector2::Normalize(pHeads_[currentHead_]->GetPosition() - position_)*Vector2(32.f, 32.f).Length();

	switch (playerMode_)
	{
	case MODE_FALL: {

		Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
		Sound::GetInstance().StopSE(SE_ID::HEAD_SHOOT_SE);
		break;
	}
	case MODE_SHOOT: {

		Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
		Sound::GetInstance().PlaySE(SE_ID::HEAD_SHOOT_SE, DX_PLAYTYPE_LOOP);
		break;
	}
	case MODE_SHOOT_END: {

		Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
		Sound::GetInstance().StopSE(SE_ID::HEAD_SHOOT_SE);
		break;
	}
	case MODE_BITE: {
		world_->FreeCameraPosY(false);
		Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
		Sound::GetInstance().StopSE(SE_ID::HEAD_SHOOT_SE);
		Sound::GetInstance().PlaySE(SE_ID::BITE_SE);
		break;
	}
	case MODE_SLIP: {

		Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
		Sound::GetInstance().StopSE(SE_ID::HEAD_SHOOT_SE);
		break;
	}
	case MODE_RESIST: {

		Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
		Sound::GetInstance().StopSE(SE_ID::HEAD_SHOOT_SE);
		break;
	}
	case MODE_CLEAR: {

		Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
		Sound::GetInstance().StopSE(SE_ID::HEAD_SHOOT_SE);
		break;
	}
	case MODE_PLAYERDEAD: {

		world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerDeadHead>(world_, position_));

		for (int i = 0; i < 8; i++) {
			world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerDeadPin>(world_, position_));
		}
		Sound::GetInstance().StopSE(SE_ID::FATIGUE_SE);
		break;
	}

	default:
		break;
	}
}

//シュート終了の瞬間かどうかを取る
bool CreditPlayer::GetIsShootModeEnd() const {
	return playerMode_ == MODE_SHOOT_END;
}

//
float CreditPlayer::GetSlipCount() const {
	return slipCount_;
}

//
bool CreditPlayer::GetIsSlipped() const {
	return playerMode_ == MODE_SLIP;
}

//
void CreditPlayer::SetMyHeadLaneNum(int targetNum) {
	pHeads_[targetNum]->SetLaneNum(laneNum_);
}

//
void CreditPlayer::SetAllHeadLaneNum() {
	for (auto& pHs : pHeads_)
	{
		pHs->SetLaneNum(laneNum_);
	}
}

//
void CreditPlayer::worldSetMyDatas() {
	//共有データに、自身の現レーン位置を保存
	world_->GetCanChangedKeepDatas().SetPlayerLane(laneNum_);
	world_->GetCanChangedKeepDatas().SetPlayerPos(position_);
}

//
void CreditPlayer::SetNextLane(int addNum, LaneChangeType changeType) {
	//if (laneNum_ + addNum > (maxLaneSize_ - 1) || laneNum_ + addNum < 0)return;

	//changeType_ = changeType;
	//if (changeType_ == LaneChangeType::LaneChange_Fall) {
	//	world_->GetChangeInv().Translation(world_->GetChangeInv().Translation() + Vector3(0.f, 500.f, 0.f));
	//	position_.y -= 500.f;
	//	pHeadPoses_[currentHead_] = multiplePos[0];
	//	UpdateLaneNum(addNum, LaneChangeType::LaneChange_Fall);
	//	pHeads_[currentHead_]->addPos(Vector2(0, -500.f));
	//	HeadPosUpdate();
	//	world_->UpdateDrawPos();
	//	UpdateMultiplePos();
	//	DeformationDraw();
	//	return;
	//}


	//world_->ChangeCamMoveMode(addNum);

	//world_->sendMessage(EventMessage::START_LANE_CHANGE);
}

//
void CreditPlayer::setCurPHeadSPos(const Vector2 & sPos) {
	//pHeads_[currentHead_]->setPHeadStopPos(sPos);
	SetMultiplePos(sPos - stopPos_);
	stopPos_ = sPos;
	//Vector2 lngPs = pHeads_[currentHead_]->GetPosition() - position_;
	//MultipleInit(lngPs.Length(), stopPos_, MathAngle(position_ - pHeadPoses_[currentHead_], Vector2::Down));

}

//
void CreditPlayer::setMaxLaneSize(int size) {
	maxLaneSize_ = size;
}

//
void CreditPlayer::curPHeadSlip(bool isSlip) {
	pHeads_[currentHead_]->setIsBiteSlipWind(isSlip);
}

//プレイヤーが死んでるか
bool CreditPlayer::isPlayerDead() const {
	//if (laneNum_ == (maxLaneSize_ - 1) && position_.y >= WINDOW_HEIGHT - 200)return true;
	////if (position_.y >= WINDOW_HEIGHT)return true;

	//for (auto pHD : pHeadDead_) {
	//	if (!pHD)return false;
	//}
	//return true;

	return false;
}

//
void CreditPlayer::PHeadChanger(int rot) {
	world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerMetamorEffect>(world_, pHeads_[currentHead_]->GetPosition(), pHeads_[currentHead_].get()));
	PHeadLengthReset();
	(sign(rot) == 1) ? backChangeHead() : changeHead();

	Vector2 addVec = position_ - pHeadPoses_[currentHead_];
	addVec = addVec.Normalize()*32.f;
	world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerMetamorEffect>(world_, pHeads_[currentHead_]->GetPosition(), pHeads_[currentHead_].get(), 0.3f, addVec));

	Sound::GetInstance().PlaySE(SE_ID::CHANGE_HEAD_SE);
}

//
void CreditPlayer::SetStopPos(Vector2 target) {
	stopPos_ = target;
}

//
Vector2 CreditPlayer::GetStopPos() const {
	return stopPos_;
}

//
float CreditPlayer::GetRot() const {
	return mRot.front();
}

//
Vector2 CreditPlayer::GetHeadPosAddVect() const {
	return headPosAddVect_;
}

//
Vector2 CreditPlayer::GetSlipHeadPoint() const {
	return fPos_.front();
}

//
void CreditPlayer::PlayerInputControl()
{
	if (BuildMode != 1)return;

	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::UP)) {
		velocity_.y -= 20.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::DOWN)) {
		velocity_.y += 20.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::RIGHT)) {
		velocity_.x += 20.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::LEFT)) {
		velocity_.x -= 20.0f;
	}

}

//
void CreditPlayer::PHeadLengthReset() {
	//長さの補間をリセットする
	chainAddLength_ = 0.f;
	chainAddLengthMath_ = 0.f;

	//チェーンのロックをリセットする
	chainLock_ = false;
	for (auto& pHL : pHeadLength_) {
		pHL = 2.f;
	}
}

void CreditPlayer::AllResurrectHead()
{
	for (int i = 0; i < pHeads_.size(); i++) {
		ResurrectHead();
	}
}


//
void CreditPlayer::CurPHeadLengPlus(float addPow) {

	//floatの誤差と、addPowによるLengthのズレを補正するための値、首の長さの値に補正が発生した場合は、この補正値をそこに加算する事で、長さの違和感を解決する
	float fSaveAddNum = 0.2f;
	if (pHeadLength_[currentHead_] > 16.f + fSaveAddNum) {
		pHeadLength_[currentHead_] = 16.f + fSaveAddNum + chainAddLength_ - chainAddLengthMath_;
		//長さの上昇に対する補間値
		chainAddLengthMath_ -= 0.4f;
		chainAddLengthMath_ = max(chainAddLengthMath_, 0.f);

		for (int i = currentHead_; i > -(int)pHeads_.size() + currentHead_; i--) {
			int trgNum = i;
			if (trgNum < 0) {
				trgNum = trgNum + pHeads_.size();
			}

			if (pHeadDead_[trgNum])continue;
			if (trgNum == currentHead_)continue;

			if (pHeadLength_[trgNum] <= 0.1f) {
				if (chainLockCoolTime_ > 0) {
					break;
				}
				chainLockCoolTime_ = defChainLockCoolTime_;
				chainAddLength_ += 2.f;
				chainAddLengthMath_ += 2.f;
				pHeadDead_[trgNum] = true;
			}
		}

		return;
	}
	//現在のHead以外の長さを伸ばした分だけマイナスする
	pHeadLength_[currentHead_] += addPow;
	//左隣がターゲット
	int targetNum = currentHead_ - 1;


	float LengthKeepNum = 1.01f;
	if (targetNum < 0) {
		targetNum = targetNum + (int)pHeads_.size();
	}
	for (;;) {
		if (LengthKeepNum < 0.f)break;

		if (pHeadLength_[targetNum] < LengthKeepNum)
		{
			targetNum--;

			if (targetNum < 0) {
				targetNum = targetNum + (int)pHeads_.size();
			}

			if (targetNum == currentHead_) {
				LengthKeepNum -= 1.f;
				targetNum--;

				if (targetNum < 0) {
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

//
void CreditPlayer::UpdateLaneNum(int updateNum, LaneChangeType changeType) {
	//if (updateNum == 0)return;
	//if (laneNum_ + updateNum > (maxLaneSize_ - 1) || laneNum_ + updateNum < 0)return;

	////次のレーンに対応したベクトルを作成し、重力の加算をリセットする
	//Vector2 nextVel_;
	////上がるとき
	//if (updateNum < 0) {
	//	nextVel_ = Vector2(0, -15.f);
	//	pGrav_ = 0.f;
	//	//position_.y += defDrawLinePosY[2]- defDrawLinePosY[1];
	//}
	////降りる時
	//else if (updateNum > 0) {
	//	if (changeType == LaneChangeType::LaneChange_Fall) {

	//	}
	//	else {
	//		nextVel_ = pendulumVect_ / 2;
	//		//position_.y += defDrawLinePosY[0] - defDrawLinePosY[1];
	//		pGrav_ *= 0.3f;
	//		//laneNum_ += updateNum;
	//	}
	//}

	//laneNum_ += updateNum;
	////レーン最大範囲を超えたらVectの補正を行わない

	//laneNum_ = MathHelper::Clamp(laneNum_, 0, (maxLaneSize_ - 1));

	////velocity_ = nextVel_;
	//pendulumVect_ = nextVel_;

	//if (GetIsBiteMode())playerMode_ = MODE_SLIP;
	////頭の長さをリセット
	////PHeadChanger();
	////PHeadLengthReset();

	//worldSetMyDatas();
}

//
void CreditPlayer::CreateBiteEffect()
{
	Vector2 vel = pHeads_[currentHead_]->GetPosition() - position_;
	vel = vel.Normalize();
	world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerBiteEffect>(world_, pHeads_[currentHead_]->GetPosition() + vel * 30));

}

//
void CreditPlayer::FallUpdate()
{
	pGrav_ += defPGravPow;
	if (isUseKey_) {
		if ((GamePad::GetInstance().Stick().x < -0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::A))) &&
			isCanNextHeadRot) {
			SetMode(MODE_FALL);//playerMode_ = MODE_FALL;
							   //キーを押し直したかの判断
			PHeadChanger();
			isCanNextHeadRot = false;
		}
		if ((GamePad::GetInstance().Stick().x > 0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::D))) &&
			isCanNextHeadRot) {
			SetMode(MODE_FALL);//playerMode_ = MODE_FALL;
							   //キーを押し直したかの判断
			PHeadChanger(1);
			isCanNextHeadRot = false;
		}
		if ((MathHelper::Abs(GamePad::GetInstance().Stick().x) < 0.3f &&
			(Keyboard::GetInstance().KeyStateUp(KEYCODE::D) &&
				Keyboard::GetInstance().KeyStateUp(KEYCODE::A)))) {
			isCanNextHeadRot = true;
		}

		if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
			SetMode(MODE_SHOOT);//playerMode_ = MODE_SHOOT;
			isNextPushKey_ = false;
		}
	}
	position_ += velocity_ + pendulumVect_;

	slipCount_ = defSlipCount;

	SetDrawNeck(position_, pHeads_[currentHead_]->GetPosition());
}

//
void CreditPlayer::ShootUpdate()
{
	pGrav_ += defPGravPow;
	if (isUseKey_) {
		if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyStateDown(KEYCODE::M)) {
			CurPHeadLengPlus(headShotPower);
		}
		else if ((GamePad::GetInstance().ButtonTriggerUp(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerUp(KEYCODE::M))) {
			pHeads_[currentHead_]->SetBiteSprite();
			SetMode(MODE_SHOOT_END);//playerMode_ = MODE_SHOOT_END;
		}
		else {
			pHeads_[currentHead_]->SetBiteSprite();
			SetMode(MODE_FALL);//playerMode_ = MODE_FALL;
		}

		if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
			SetMode(MODE_SHOOT);//playerMode_ = MODE_SHOOT;
			isNextPushKey_ = false;
		}
	}
	position_ += velocity_ + pendulumVect_;

	slipCount_ = defSlipCount;

	SetDrawNeck(position_, pHeads_[currentHead_]->GetPosition());
}

//
void CreditPlayer::ShootEndUpdate()
{
	CreateBiteEffect();
	SetMode(MODE_FALL);// playerMode_ = MODE_FALL;
	pGrav_ += defPGravPow;
	if (isUseKey_) {
		if ((GamePad::GetInstance().Stick().x < -0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::A))) && isCanNextHeadRot) {
			PHeadChanger();
			isCanNextHeadRot = false;
		}
		if ((GamePad::GetInstance().Stick().x > 0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::D))) && isCanNextHeadRot) {
			//キーを押し直したかの判断
			PHeadChanger(1);
			isCanNextHeadRot = false;
		}
		if ((MathHelper::Abs(GamePad::GetInstance().Stick().x) < 0.3f && (Keyboard::GetInstance().KeyStateUp(KEYCODE::D) && Keyboard::GetInstance().KeyStateUp(KEYCODE::A)))) {
			isCanNextHeadRot = true;
		}

		if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyStateDown(KEYCODE::M)) {
		}
		else if ((GamePad::GetInstance().ButtonTriggerUp(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerUp(KEYCODE::M))) {
		}

		if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
			SetMode(MODE_SHOOT);// playerMode_ = MODE_SHOOT;
			isNextPushKey_ = false;
		}
	}
	position_ += velocity_ + pendulumVect_;

	slipCount_ = defSlipCount;

	SetDrawNeck(position_, pHeads_[currentHead_]->GetPosition());
}

//
void CreditPlayer::BiteUpdate()
{
	Multiple();
	if (isUseKey_) {

		if (GamePad::GetInstance().Stick().y > 0.5f || Keyboard::GetInstance().KeyStateDown(KEYCODE::W)) {
			if (!pSword_->GetUseSword()) {
				world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<GetSwordEffect>(world_, pSword_->GetPosition(), pSword_.get()));
				pSword_->SetUseSword(true);
				Sound::GetInstance().PlaySE(SE_ID::CREATE_SWORD_SE);
			}
		}
		if ((GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM1) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::S))) {
			if (GamePad::GetInstance().Stick().y > 0.5f || Keyboard::GetInstance().KeyStateDown(KEYCODE::W)) {
				//SetNextLane(1);
			}
			else if (mRot.front() < 0.f || mRot.front() > 180.f) {
				//SetNextLane(-1);
			}
		}

		if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
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
			rot_spd_ += (spdLimit);
			for (auto& spd : mRot_spd) {
				spd += (spdLimit);
			}
		}
	}
	slipCount_ -= 0.016f*slipCountMult_[otherClothesID_];
	if (slipCount_ <= 0.f) {
		SetMode(MODE_SLIP);
		//首を殺して
		pHeadDead_[currentHead_] = true;
		//スティックをロックする
		isCanNextHeadRot = false;
		isSlipped_ = true;
		//スリップモードに移行すると同時に、その時点のベクトルをHeadに格納する
		pHeads_[currentHead_]->SetPosAddVect(pHeads_[currentHead_]->GetPosition() - position_);
		//ここやばいかも？
		PHeadChanger();
	}
	slipCount_ = max(slipCount_, 0.f);

	DeformationDraw();

}

//
void CreditPlayer::SlipUpdate()
{
	pGrav_ += defPGravPow;

	if (isUseKey_) {
		if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
			//Headを交代する
			PHeadChanger();
			//}
			SetMode(MODE_SHOOT);//playerMode_ = MODE_SHOOT;
			isNextPushKey_ = false;

		}
		if ((GamePad::GetInstance().Stick().x < -0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::A))) && isCanNextHeadRot) {
			SetMode(MODE_FALL);//playerMode_ = MODE_FALL;
			PHeadChanger();
			isCanNextHeadRot = false;
		}
		if ((GamePad::GetInstance().Stick().x > 0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::D))) && isCanNextHeadRot) {
			SetMode(MODE_FALL);//playerMode_ = MODE_FALL;
							   //キーを押し直したかの判断
			PHeadChanger(1);
			isCanNextHeadRot = false;
		}
	}

	position_ += velocity_ + pendulumVect_;

	slipCount_ = defSlipCount;

	UpdateMultiplePos();
	DeformationDraw();

}

//
void CreditPlayer::ResistUpdate()
{
	slipResistTime_ -= 0.016f;

	if (slipResistTime_ <= 0.f) {
		slipResistTime_ = defResistTime;
		SetMode(MODE_FALL);//playerMode_ = MODE_FALL;
		return;
	}

	BiteUpdate();
}

//
void CreditPlayer::ClearUpdate()
{
	for (auto& i : mRot_spd) {
		i *= 1.02f;
		i = MathHelper::Clamp(i, -60.f, 60.f);
	}
	if ((pHeads_[currentHead_]->GetDrawPos().y >= WINDOW_HEIGHT) || isClearShoot_) {
		if (!isClearShoot_) {
			isClearShoot_ = true;
		}
		PHeadChanger();
		pendulumVect_.y *= 0.98f;
		pendulumVect_.y = max(50.f, pendulumVect_.y);

		position_ -= pendulumVect_;
		UpdateMultiplePos();
		DeformationDraw();
	}
	else {
		Multiple();
		DeformationDraw();
		pendulumVect_ *= 3.f;
		Vector2 shiftPos_;
		shiftPos_.x = 0.f;
		shiftPos_.y = 100.f;

		pendulumVect_ = shiftPos_;
	}
	if (drawPos_.y <= -300) {
		world_->sendMessage(EventMessage::PLAY_NEXT_STAGE);
	}
}

//
void CreditPlayer::DeadUpdate()
{
	position_ += velocity_ + pendulumVect_;
}


