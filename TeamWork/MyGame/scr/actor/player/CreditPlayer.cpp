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
#include "../../actor/player/PlayerFallPin.h"
#include "PlayerNeck\PlayerNeckPendulumSupport.h"
#include "../../debugdata/DebugDraw.h"

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
	:Player(world)

{
	addscorelist_[0] = 300;
	addscorelist_[1] = 200;
	addscorelist_[2] = 100;

	laneNum_ = 1;

	spriteId_ = SPRITE_ID::PBODY_SPRITE;

	parameter_.ID = ACTOR_ID::PLAYER_ACTOR;
	parameter_.radius = Sprite::GetInstance().GetSize(spriteId_).x / 2;
	parameter_.size = Sprite::GetInstance().GetSize(spriteId_);
	parameter_.HP = 10;

	position_ = Vector2(WINDOW_WIDTH / 2.0f - 412.0f, 0);
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(position_));

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
		pHeadPoses_[i] = cgToV2;
		pHeadLength_[i] = defHeadLength;

		pHeads_[i]->SetPose(Matrix::CreateTranslation(Vector3(pHeadPoses_[i])));

		SetMyHeadLaneNum(i);
	}
	pSword_ = std::make_shared<Player_Sword>(world, player, position_);
	world_->Add(ACTOR_ID::PLAYER_SWORD_ACTOR, pSword_);

	updateFunctionMap_[MODE_FALL] = std::bind(&CreditPlayer::FallUpdate, this);
	updateFunctionMap_[MODE_SHOOT] = std::bind(&CreditPlayer::ShootUpdate, this);
	updateFunctionMap_[MODE_BITE] = std::bind(&CreditPlayer::BiteUpdate, this);

	worldSetMyDatas();
	StartPlayerSet();

	isClearShoot_ = false;

	StartPendulum();
	DeformationDraw();
	auto a = 1;
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

	chainLockCoolTime_--;
	chainLockCoolTime_ = MathHelper::Clamp(chainLockCoolTime_, 0, defChainLockCoolTime_);
	//レーン変更のクールタイムを設定

	if (!GetIsClearShoot())pendulumVect_ -= pendulumVect_*0.05f;

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

	laneNum_ = 1;
}

//最初の更新
void CreditPlayer::FastUpdate() {
	if (!world_->GetIsCamChangeMode()) {
		int nexLane = world_->GetKeepDatas().nextLane_;
		world_->GetCanChangedKeepDatas().SetPlayerNextLane(0);
	}

}

//描画
void CreditPlayer::Draw() const
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
	Sprite::GetInstance().Draw(spriteId_, GetDrawPosVect(position_), crcOrigin, parameter_.spriteAlpha_, Vector2::One, angle);

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

	for (int i = 0; i < (int)pHeads_.size(); i++) {
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

//受動更新
void CreditPlayer::OnUpdate()
{
}

//衝突時のアクション
void CreditPlayer::OnCollide(Actor& other, CollisionParameter colpara)
{
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
	case EventMessage::OPERATE_FLAG: {
		if ((int)param > 0) operatre_ = true;
		else operatre_ = false;
	}
	default:
		break;
	}
}

//
void CreditPlayer::CamMoveUp() {
}

//
void CreditPlayer::CamMoveDown() {
}

//
void CreditPlayer::LaneChangeFall() {
}

//
void CreditPlayer::GravityReset()
{
	pGrav_ = defPGravPow;
}

//
void CreditPlayer::MultipleInit(float len, const Vector2& fPos, float rot, float radius)
{
	auto px = GetCurrentPHeadPosition().x + MathHelper::Cos(rot) * (len);
	auto py = GetCurrentPHeadPosition().y + MathHelper::Sin(rot) * (len);
	PlayerNeckPendulumSupport::Init(GetCurrentPHeadPosition(), Vector2(px, py), fPos_, multiplePos, mRot, mRot_spd, neckDrawPoints);
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

//
void CreditPlayer::SetPosition(Vector2 position)
{
	position_ = position;
}

//プレイヤーの座標を返す
Vector2 CreditPlayer::GetPlayerPosition()
{
	return position_;
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
void CreditPlayer::SetBiteSprite()
{
	pHeads_[currentHead_]->SetBiteSprite();
}

//
void CreditPlayer::FallUpdate()
{
	pGrav_ += defPGravPow;
	if (isUseKey_) {
		if (!operatre_) return;

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

		if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM6) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
			SetMode(MODE_SHOOT);//playerMode_ = MODE_SHOOT;
			isNextPushKey_ = false;
		}
		if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::N) || GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2)) {
			pHeads_[currentHead_]->SetBiteSprite();
			SetMode(MODE_SHOOT_END);//playerMode_ = MODE_SHOOT_END;


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
		if (!operatre_) return;

		if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::N) || GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM2)) {
			pHeads_[currentHead_]->SetBiteSprite();
			SetMode(MODE_SHOOT_END);//playerMode_ = MODE_SHOOT_END;
		}
		else if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM6) || Keyboard::GetInstance().KeyStateDown(KEYCODE::M)) {
			CurPHeadLengPlus(headShotPower);
		}
		else {
			SetMode(MODE_FALL);//playerMode_ = MODE_FALL;
		}

		if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM6) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
			SetMode(MODE_SHOOT);//playerMode_ = MODE_SHOOT;
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
	if (operatre_) {

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

				//if (GamePad::GetInstance().Stick().y > 0.5f || Keyboard::GetInstance().KeyStateDown(KEYCODE::W)) {
				//	SetNextLane(1);
				//}
				//else if (mRot.front() < 0.f || mRot.front() > 180.f) {
				//	SetNextLane(-1);
				//}
			}

			if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::N)) {
				SetMode(MODE_FALL);
				//Headを交代する
				PHeadChanger();
				isNextPushKey_ = false;
				isCanNextHeadRot = false;
			}
		}
	}
	//下へのベクトルと現在のプレイヤーの位置ベクトルのなす角を取る
	Vector2 ptoPHVec = pHeads_[currentHead_]->GetPosition() - position_;
	float ptoDownCos = Vector2::Dot(ptoPHVec, Vector2::Down) / (ptoPHVec.Length()*Vector2::Down.Length());
	float ptoDownAngle = MathHelper::ACos(ptoDownCos);
	//取った角度が10以下なら此方に入れる

	if (operatre_) {
		if (isUseKey_) {
			if (otherClothesID_ == CLOTHES_ID::FLUFFY_CLOTHES && (MathHelper::Abs(GamePad::GetInstance().Stick().x > 0.01f) ||
				Keyboard::GetInstance().KeyStateDown(KEYCODE::D) ||
				Keyboard::GetInstance().KeyStateDown(KEYCODE::A)) &&
				MathHelper::Abs(mRot_spd[0]) <= 0.01f&&
				ptoDownAngle <= 10.f&&
				pHeads_[currentHead_]->GetPosition().y < position_.y) {
				for (auto& spd : mRot_spd) {
					spd += (spdLimit)* 2;
				}
			}

			slipCount_ -= 0.016f*slipCountMult_[otherClothesID_];
			if (slipCount_ <= 0.f) {
				SetMode(MODE_SLIP);
				//首を殺して
				Vector2 toPos = pHeadPoses_[currentHead_] - position_;
				world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerFallPin>(world_, pHeads_[currentHead_]->GetPosition(), toPos));
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
		}
	}
	DeformationDraw();

}
