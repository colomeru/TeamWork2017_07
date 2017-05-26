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

static const float headShotPower = 0.3f;
static const float defMaxChainLength = 16.f;
static const float defJumpShotPower = 1.5f;

Player::Player(IWorld * world,int maxLaneSize, int startLane)
	:Actor(world),
	isHit_(false), fulcrum_(500.0f, 200.0f), rot_(135.f), rot_spd_(-3.0f), length_(300.0f), gravity_(0.5f), currentHead_(0),
	headChangeTime_(0), pGrav_(defPGravPow), maxChainLength_(defMaxChainLength), playerMode_(MODE_FALL), isNextPushKey_(true),
	pendulumVect_(Vector2::Zero), slipCount_(defSlipCount), jumpShotPower_(defJumpShotPower), isSlipped_(false), chainLock_(false),/* isCanChangeLane_(false),*/
	otherClothesID_(CLOTHES_ID::FLUFFY_CLOTHES), friction(0.998f), spdLimit(2.75f), isCanNextHeadRot(true), chainLockCoolTime_(defChainLockCoolTime_), chainAddLength_(0),
	chainAddLengthMath_(0), maxLaneSize_(maxLaneSize), isPlayerFallLane_(false), changeType_(LaneChangeType::LaneChange_Normal),slipResistTime_(defResistTime), headPosAddVect_(Vector2::Zero)

{
	laneNum_ = startLane;

	spriteId_ = SPRITE_ID::PBODY_SPRITE;

	parameter_.ID = ACTOR_ID::PLAYER_ACTOR;
	parameter_.radius = Sprite::GetInstance().GetSize(spriteId_).x / 2;
	parameter_.size = Sprite::GetInstance().GetSize(spriteId_);
	parameter_.HP = 10;
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3::Zero);

	position_ = Vector2(0, 200);

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
		Vector3 tgtRot = Vector3(pHDist.x, pHDist.y)*Matrix::CreateRotationZ((i + headAngleSetter) * 45);
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

	worldSetMyDatas();
	StartPlayerSet();
}

Player::~Player()
{
}

void Player::Update()
{

	chainLockCoolTime_--;
	chainLockCoolTime_ = MathHelper::Clamp(chainLockCoolTime_, 0, defChainLockCoolTime_);
	//レーン変更のクールタイムを設定

	pendulumVect_ -= pendulumVect_*0.05f;

	length_ = Vector2::Distance(pHeads_[currentHead_]->GetPosition(), position_);
	//if(!pHeads_[currentHead_]->getIsHit())pGrav_+= defPGravPow;
	
	//if (!GetIsBiteMode())pGrav_ += defPGravPow;
	
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

	if (position_.y >= WINDOW_HEIGHT) {
		SetNextLane(1,LaneChangeType::LaneChange_Fall);
	}
	//Headの表示レーンを本体に合わせる
	SetAllHeadLaneNum();

	//自分,相手のID,Colの種類
	//world_->SetCollideSelect(shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::BOX_CLOTHES_COL);

	//行列にangleをかける
	parameter_.mat += Matrix::CreateRotationZ(angle_);
	parameter_.mat.NormalizeRotationMatrix();

	HeadPosUpdate();
	SwordPosUpdate();

	worldSetMyDatas();

}

void Player::Draw() const
{
	//auto pos_1 = DXConverter::GetInstance().ToVECTOR(parameter_.mat.Translation());
	//auto pos_2 = DXConverter::GetInstance().ToVECTOR(parameter_.mat.Translation() + Vector3(0, 10, 0));
	//DrawCapsule3D(pos_1, pos_2, 5.0f, 4, GetColor(255, 0, 0), GetColor(255, 0, 0), true);

	//Model::GetInstance().Draw(MODEL_ID::PLAYER_MODEL, parameter_.mat);

	auto is = Matrix::CreateRotationZ(angle_);
	auto pos = drawPos_;
	auto sizeVec = Vector3((parameter_.size.x / 2), (parameter_.size.y / 2));

	auto box1 = Vector3(-sizeVec.x, -sizeVec.y)*is;
	auto box2 = Vector3(+sizeVec.x, -sizeVec.y)*is;
	auto box3 = Vector3(-sizeVec.x, +sizeVec.y)*is;
	auto box4 = Vector3(+sizeVec.x, +sizeVec.y)*is;
	//auto seg = Vector3(+sizeVec.x, 0)*is;

	//左上,右上,左下,右下
	auto pos1 = Vector3(pos.x + box1.x, pos.y + box1.y);
	auto pos2 = Vector3(pos.x + box2.x, pos.y + box2.y);
	auto pos3 = Vector3(pos.x + box3.x, pos.y + box3.y);
	auto pos4 = Vector3(pos.x + box4.x, pos.y + box4.y);
	//Model::GetInstance().Draw(MODEL_ID::PLAYER_MODEL, parameter_.mat);
	//DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, GetColor(255, 255, 255));
	//DrawLine(pos1.x, pos1.y, pos3.x, pos3.y, GetColor(255, 255, 255));
	//DrawLine(pos2.x, pos2.y, pos4.x, pos4.y, GetColor(255, 255, 255));
	//DrawLine(pos3.x, pos3.y, pos4.x, pos4.y, GetColor(255, 255, 255));
	//DrawCircle(drawPos_.x, drawPos_.y, pHDist.Length(), GetColor(255, 255, 255));
	Vector2 crcOrigin = Sprite::GetInstance().GetSize(spriteId_) / 2;
	float aHeadAngle = (360 / pHeads_.size());
	float angle = currentHead_*aHeadAngle + aHeadAngle*(headChangeTime_ / defHeadChangeTime);
	Sprite::GetInstance().Draw(spriteId_, GetDrawPosVect(position_), crcOrigin, spriteAlpha_, Vector2::One,angle);
	//DrawLine(pos.x - seg.x, pos.y-seg.y, pos.x + seg.x, pos.y+seg.y, GetColor(255, 255, 255));
	//DrawFormatString(0, 60, GetColor(255, 255, 255), "position x:%f y:%f z:%f", pos.x, pos.y);
	//DrawFormatString(0, 80, GetColor(255, 255, 255), "angle %f", velocity_.y);
	//DrawFormatString(0, 100, GetColor(255, 255, 255), "%d", laneNum_);
	DrawFormatString(50, 100, GetColor(255, 255, 255), "%d", playerMode_);
	//DrawFormatString(400, 100, GetColor(255, 255, 255), "%f",angle_);
	//if (isShootMode_>=1)DrawFormatString(0, 700, GetColor(255, 255, 255), "true");
	//else DrawFormatString(0, 700, GetColor(255, 255, 255), "false");
	//DrawFormatString(0, 700, GetColor(255, 255, 255), "%f:%f", pHeadPoses_[currentHead_].x, pHeadPoses_[currentHead_].y);
	//DrawFormatString(400, 100, GetColor(255, 255, 255), "%f:%f",GamePad::GetInstance().Stick().x, GamePad::GetInstance().Stick().y);
	//int count = 0;
	//for (auto sgT : pHeadLength_) {
	//DrawFormatString(300, 300 + (30 * count),GetColor(255,255,255),"%f",sgT );
	////if (pHeadDead_[count])DrawFormatString(300, 300 + (30 * count), GetColor(255, 255, 255), "true");
	////else DrawFormatString(300, 300 + (30 * count), GetColor(255, 255, 255), "false");
	//count++;
	//}
	int count = 0;
	for (auto srmt : pHeadDead_)
	{
		if (srmt)DrawFormatString(300, 300 + (30 * count), GetColor(255, 255, 255), "true");
		else DrawFormatString(300, 300 + (30 * count), GetColor(255, 255, 255), "false");
		count++;
	}
}

void Player::OnUpdate()
{

}

void Player::OnCollide(Actor& other, CollisionParameter colpara)
{
	isHit_ = true;
}

void Player::OnMessage(EventMessage message, void * param)
{
}
void Player::Pendulum(Vector2 fulcrum, float length)
{
	//振り子
	//支点Posを設定
	float fx = fulcrum.x;
	float fy = fulcrum.y;
	//Gravityを設定
	float g = gravity_;
	//変更前のpositionを保存
	Vector2 curdefPos = position_;
	//摩擦を設定
	//float friction = 1.f;
	//それぞれの首の移動先座標を格納可能に
	std::array<Vector2, 8> outPos;
	//自身の半径を設定
	float r = parameter_.radius;
	float r1 = rot_;
	float r2 = 0.0f;
	bool rotDirection = true;

	//それぞれの首の長さを設定
	std::vector<float> neckLen = pHeadLength_;
	////支点を格納可能に
	//std::array<Vector2, 8> fulcrums;
	//現在の重りの位置
	auto px = fx + MathHelper::Cos(rot_) * length;
	auto py = fy + MathHelper::Sin(rot_) * length;
	//auto px = fx + MathHelper::Cos(rot) * length;
	//auto py = fy + MathHelper::Sin(rot) * length;

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

	//if (rot_spd_ < 1.5f)friction = 1.f;
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


	//inPos外周
	//outPosLength分のばしたposition
	//各Headの角度を代入

	pHeadPoses_[currentHead_].x = position_.x + MathHelper::Cos(angle_ + rot2) * r;
	pHeadPoses_[currentHead_].y = position_.y + MathHelper::Sin(angle_ + rot2) * r;
	outPos[currentHead_].x = pHeadPoses_[currentHead_].x + MathHelper::Cos(angle_ + rot2) * (neckLen[currentHead_] - r);
	outPos[currentHead_].y = pHeadPoses_[currentHead_].y + MathHelper::Sin(angle_ + rot2) * (neckLen[currentHead_] - r);
	//fulcrums[currentHead_] = outPos[currentHead_];
	//lineRot[i] = pHeads_[i]->GetAngle();
	//inPos[i].x = spherePos.x + MathHelper::Cos(lineRot[i] + rot2) * r;
	//inPos[i].y = spherePos.y + MathHelper::Sin(lineRot[i] + rot2) * r;
	//outPos[i].x = inPos[i].x + MathHelper::Cos(lineRot[i] + rot2) * (neckLen[i] - r);
	//outPos[i].y = inPos[i].y + MathHelper::Sin(lineRot[i] + rot2) * (neckLen[i] - r);
	//fulcrum[i] = outPos[i];

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
		(rot_spd_ > 0 && rotDirection && (Keyboard::GetInstance().KeyStateDown(KEYCODE::A) || GamePad::GetInstance().Stick().x<-0.01f)))
	{
		friction = 1.015f; //摩擦を減らす
		
	}
	else
	{
		friction = 0.985f; //摩擦を戻す
	}
	//Vector2::Up;

	//if (rot_ < 0.f) {

	//	friction = (1.f - ((0 - rot_) / 90.f))*1.4f;
	//	friction = MathHelper::Clamp(friction, 0.f, 1.f);
	//}
	//else if (rot_>180.f)
	//{
	//	friction = (1.f - ((rot_ - 180) / 90.f))*1.4f;
	//	friction = MathHelper::Clamp(friction, 0.f, 1.f);
	//}

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
	//if (pHeadDead_[currentHead_]) {
	//	Vector3 pHRt= Vector3(pHDist.x, pHDist.y)*Matrix::CreateRotationZ((360.f/(float)pHeads_.size())*6);

	//	velUnPos.x = pHRt.x;
	//	velUnPos.y = pHRt.y;
	//	velUnPos += position_;
	//}
	Vector2 vel = position_ - velUnPos;
	vel = Vector2::Normalize(vel);

	pSword_->SetSwordVel(vel);

	pSword_->SetLaneNum(laneNum_);
}

void Player::StartPlayerSet() {
	playerMode_=MODE_BITE;
	pHeadLength_[currentHead_]=5.f;
	pHeads_[currentHead_]->StartPlayerHeadBite();
}

int Player::GetCurHead() const {
	return currentHead_;
}


//Headのレーンを本体のレーンに合わせる

void Player::SetMode(int pMode) {
	playerMode_ = pMode;
	//if (pMode == MODE_SLIP)
		headPosAddVect_= Vector2::Normalize(pHeads_[currentHead_]->GetPosition() - position_)*Vector2(32.f,32.f).Length();
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
	//world_->SetKeepDatas(KeepDatas(laneNum_));
	world_->GetCanChangedKeepDatas().SetPlayerLane(laneNum_);
	world_->GetCanChangedKeepDatas().SetPlayerPos(position_);
}

void Player::setCurPHeadSPos(const Vector2 & sPos) {
	//pHeads_[currentHead_]->setPHeadStopPos(sPos);
	stopPos_ = sPos;
}

void Player::curPHeadSlip(bool isSlip) {
	pHeads_[currentHead_]->setIsBiteSlipWind(isSlip);
}

void Player::PlayerInputControl()
{
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

void Player::CurPHeadLengPlus(float addPow) {

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
				//chainLock_ = false;
				chainAddLength_ += 2.f;
				chainAddLengthMath_ += 2.f;
				pHeadDead_[trgNum] = true;
			}
			else {
				//pHeadDead_[i] = false;
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

void Player::FallUpdate()
{
	pGrav_ += defPGravPow;

	if ((GamePad::GetInstance().Stick().x<-0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::A)))&& 
		isCanNextHeadRot) {
		playerMode_ = MODE_FALL;
		//キーを押し直したかの判断
		PHeadChanger();
		isCanNextHeadRot = false;
	}
	if ((GamePad::GetInstance().Stick().x>0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::D)))&& 
		isCanNextHeadRot) {
		playerMode_ = MODE_FALL;
		//キーを押し直したかの判断
		PHeadChanger(1);
		isCanNextHeadRot = false;
	}
	if ((MathHelper::Abs(GamePad::GetInstance().Stick().x)<0.3f &&
		(Keyboard::GetInstance().KeyStateUp(KEYCODE::D) &&
		Keyboard::GetInstance().KeyStateUp(KEYCODE::A)))) {
		isCanNextHeadRot = true;
	}

	if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
			playerMode_ = MODE_SHOOT;
			isNextPushKey_ = false;
	}

		position_ += velocity_ + pendulumVect_;

		slipCount_ = defSlipCount;

}

void Player::ShootUpdate()
{
	pGrav_ += defPGravPow;

	if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyStateDown(KEYCODE::M)) {
		CurPHeadLengPlus(headShotPower);
	}
	else if ((GamePad::GetInstance().ButtonTriggerUp(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerUp(KEYCODE::M))) {
		playerMode_ = MODE_SHOOT_END;
	}
	else {
		playerMode_ = MODE_FALL;
	}

	if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
		playerMode_ = MODE_SHOOT;
		isNextPushKey_ = false;
	}

		position_ += velocity_ + pendulumVect_;

		slipCount_ = defSlipCount;
}

void Player::ShootEndUpdate()
{
	playerMode_ = MODE_FALL;
	pGrav_ += defPGravPow;
	if ((GamePad::GetInstance().Stick().x<-0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::A))) && isCanNextHeadRot) {
		PHeadChanger();
		isCanNextHeadRot = false;
	}
	if ((GamePad::GetInstance().Stick().x>0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::D)))&& isCanNextHeadRot) {
		//playerMode_ = MODE_FALL;
		//キーを押し直したかの判断
		PHeadChanger(1);
		isCanNextHeadRot = false;
	}
	if ((MathHelper::Abs(GamePad::GetInstance().Stick().x)<0.3f && (Keyboard::GetInstance().KeyStateUp(KEYCODE::D) && Keyboard::GetInstance().KeyStateUp(KEYCODE::A)))) {
		isCanNextHeadRot = true;
	}

	if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyStateDown(KEYCODE::M)) {
	}
	else if ((GamePad::GetInstance().ButtonTriggerUp(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerUp(KEYCODE::M))) {
	}
	else {
	//	playerMode_ = MODE_FALL;
	}

	if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
		playerMode_ = MODE_SHOOT;
		isNextPushKey_ = false;
	}

		position_ += velocity_ + pendulumVect_;

		slipCount_ = defSlipCount;
}

void Player::BiteUpdate()
{
	Pendulum(pHeads_[currentHead_]->GetPosition(), length_);
		if (GamePad::GetInstance().Stick().y > 0.5f || Keyboard::GetInstance().KeyStateDown(KEYCODE::W)) {
			pSword_->SetUseSword(true);
		}
		if ((GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM1) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::S))) {
			if (GamePad::GetInstance().Stick().y>0.5f || Keyboard::GetInstance().KeyStateDown(KEYCODE::W)) {
				//pSword_->SetUseSword(true);
				SetNextLane(1);
				//UpdateLaneNum(1);
			}
			else if (rot_<0.f || rot_>180.f) {
				SetNextLane(-1);
				//UpdateLaneNum(-1);
			}
		}

		if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
			//if (GetIsBiteMode()) {
				SetMode(MODE_FALL);
				//Headを交代する
				PHeadChanger();
				isNextPushKey_ = false;
				isCanNextHeadRot = false;
		//	}
		}

		//下へのベクトルと現在のプレイヤーの位置ベクトルのなす角を取る
		Vector2 ptoPHVec = pHeads_[currentHead_]->GetPosition() - position_;
		float ptoDownCos = Vector2::Dot(ptoPHVec, Vector2::Down) / (ptoPHVec.Length()*Vector2::Down.Length());
		float ptoDownAngle = MathHelper::ACos(ptoDownCos);
		//取った角度が10以下なら此方に入れる
		if (otherClothesID_ == CLOTHES_ID::FLUFFY_CLOTHES && (MathHelper::Abs(GamePad::GetInstance().Stick().x > 0.01f) ||
			Keyboard::GetInstance().KeyStateDown(KEYCODE::D) ||
			Keyboard::GetInstance().KeyStateDown(KEYCODE::A)) &&
			MathHelper::Abs(rot_spd_) <= 0.01f&&
			ptoDownAngle <= 10.f&&
			pHeads_[currentHead_]->GetPosition().y < position_.y) {
			rot_spd_ += (spdLimit);
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
			changeHead();
		}
		slipCount_ = max(slipCount_, 0.f);

}

void Player::SlipUpdate()
{
	pGrav_ += defPGravPow;

	//if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyStateDown(KEYCODE::M)) {
	//}
	//else{
	//	playerMode_ = MODE_FALL;
	//}

	if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
		//if (GetIsSlipped()) {
			//MODE_SLIP;
			//Headを交代する
			PHeadChanger();
		//}
		playerMode_ = MODE_SHOOT;
		isNextPushKey_ = false;
		
	}
	if ((GamePad::GetInstance().Stick().x<-0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::A))) && isCanNextHeadRot) {
		playerMode_ = MODE_FALL;
		PHeadChanger();
		isCanNextHeadRot = false;
	}
	if ((GamePad::GetInstance().Stick().x>0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::D))) && isCanNextHeadRot) {
		playerMode_ = MODE_FALL;
		//キーを押し直したかの判断
		PHeadChanger(1);
		isCanNextHeadRot = false;
	}

		position_ += velocity_ + pendulumVect_;

		slipCount_ = defSlipCount;

}

void Player::ResistUpdate()
{
	slipResistTime_ -= 0.016f;

	if (slipResistTime_ <= 0.f) {
		slipResistTime_ = defResistTime;
		playerMode_ = MODE_FALL;
		return;
	}

	BiteUpdate();
}
