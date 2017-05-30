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
//���\�[�X�̉��T�C�Y
const int resWidth = 15;
const int correctionHeight = 30;
const float correctionWidth = 1.8f;
//���\�[�X�̏c�T�C�Y
const float oneLength = 30.f;

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
		//8�����̂����A�e���ɑΉ������ʒu���쐬
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
	//���[���ύX�̃N�[���^�C����ݒ�

	pendulumVect_ -= pendulumVect_*0.05f;

	length_ = Vector2::Distance(pHeads_[currentHead_]->GetPosition(), position_);
	//if(!pHeads_[currentHead_]->getIsHit())pGrav_+= defPGravPow;
	
	//if (!GetIsBiteMode())pGrav_ += defPGravPow;
	
	velocity_ = Vector2::Zero;
	isHit_ = false;

	//�d��
	velocity_.y += pGrav_*pGrav_*defGravAddPow;

	//�ړ������������Z�b�g���Ă�����͊֐��ɓ���
	isNextLaneBite_ = false;
	//���͂ɂ�铮����܂Ƃ߂��֐�
	PlayerInputControl();
	//�����̏�Ԃɉ������X�V
	updateFunctionMap_[playerMode_]();

	if (position_.y >= WINDOW_HEIGHT) {
		SetNextLane(1,LaneChangeType::LaneChange_Fall);
	}
	//Head�̕\�����[����{�̂ɍ��킹��
	SetAllHeadLaneNum();

	//����,�����ID,Col�̎��
	//world_->SetCollideSelect(shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::BOX_CLOTHES_COL);

	//�s���angle��������
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

	//����,�E��,����,�E��
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
	//for (int i = 0; i < fPos_.size(); i++)
	//{
	//	Sprite::GetInstance().Draw(SPRITE_ID::HITO_SPRITE, multiplePos[i], Vector2(16, 32), Vector2::One, mRot[i] - 90.0f);
	//	//Sprite::GetInstance().Draw(SPRITE_ID::LANE_SPRITE, multiplePos[i], Vector2(64, 25), Vector2::One, mRot[i]);
	//	//DrawModiGraph(pos[i][0].x, pos[i][0].y, pos[i][1].x, pos[i][1].y, pos[i][2].x, pos[i][2].y, pos[i][3].x, pos[i][3].y, gHandle, true);

	//	DrawCircle(multiplePos[i].x, multiplePos[i].y, (int)parameter_.radius, GetColor(255, 255, 255), 0, 1);
	//	DrawLine(fPos_[i].x, fPos_[i].y, multiplePos[i].x, multiplePos[i].y, GetColor(239, 117, 188), 1); //�s���N
	//																									  //DrawBox(fPos[i].x - 20.0f, fPos[i].y, multiplePos[i].x + 20.0f, multiplePos[i].y, GetColor(239, 117, 188), 1); //�s���N
	//}
	if (!pHeadDead_[currentHead_]) {
		for (int i = drawPoints.size() - 1; i > 0; i--) {
			auto p = drawPoints[i];
			Vector2 p0 = GetDrawPosVect(p.p0);
			Vector2 p1 = GetDrawPosVect(p.p1);
			Vector2 p2 = GetDrawPosVect(p.p2);
			Vector2 p3 = GetDrawPosVect(p.p3);
			DrawRectModiGraph(
				p0.x, p0.y,
				p1.x, p1.y,
				p2.x, p2.y,
				p3.x, p3.y,
				0, 0, 41, 76 * correctionLens[i],
				Sprite::GetInstance().GetHandle(SPRITE_ID::OROCHI_NECK_SPRITE), 1);
		}
	}
	int count = 0;
	for (auto srmt : pHeadDead_)
	{
		if (srmt)DrawFormatString(300, 300 + (30 * count), GetColor(255, 255, 255), "true");
		else DrawFormatString(300, 300 + (30 * count), GetColor(255, 255, 255), "false");
		count++;
	}

	DrawFormatString(600, 750, GetColor(255, 255, 255), "%f:%f",multiplePos[0].x, multiplePos[0].y);
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
	//�U��q
	//�x�_Pos��ݒ�
	float fx = fulcrum.x;
	float fy = fulcrum.y;
	//Gravity��ݒ�
	float g = gravity_;
	//�ύX�O��position��ۑ�
	Vector2 curdefPos = position_;
	//���C��ݒ�
	//float friction = 1.f;
	//���ꂼ��̎�̈ړ�����W���i�[�\��
	std::array<Vector2, 8> outPos;
	//���g�̔��a��ݒ�
	float r = parameter_.radius;
	float r1 = rot_;
	float r2 = 0.0f;
	bool rotDirection = true;

	//���ꂼ��̎�̒�����ݒ�
	std::vector<float> neckLen = pHeadLength_;
	////�x�_���i�[�\��
	//std::array<Vector2, 8> fulcrums;
	//���݂̏d��̈ʒu
	auto px = fx + MathHelper::Cos(rot_) * length;
	auto py = fy + MathHelper::Sin(rot_) * length;
	//auto px = fx + MathHelper::Cos(rot) * length;
	//auto py = fy + MathHelper::Sin(rot) * length;

	//�d�͈ړ��ʂ𔽉f�����d��̈ʒu
	auto vx = px - fx;
	auto vy = py - fy;
	auto t = -(vy * g) / (vx * vx + vy * vy);
	auto gx = px + t * vx;
	auto gy = py + g + t * vy;

	//�Q�̏d��̈ʒu�̊m�x��
	auto rDiff = MathHelper::ATan(gy - fy, gx - fx);

	//�p�x�����p���x�ɉ��Z
	auto sub = rDiff - rot_;
	sub -= std::floor(sub / 360.0f) * 360.0f;
	if (sub < -180.0f) sub += 360.0f;
	if (sub > 180.0f) sub -= 360.0f;
	rot_spd_ += sub;

	//if (rot_spd_ < 1.5f)friction = 1.f;
	//���C
	rot_spd_ *= friction;
	//�p�x�Ɋp���x�����Z
	rot_ += rot_spd_;
	//rot_ = MathHelper::Clamp(rot_, -45.f, 225.f);
	//�V�����d��̈ʒu
	px = fx + MathHelper::Cos(rot_) * length;
	py = fy + MathHelper::Sin(rot_) * length;

	//�d��̍��W
	position_.x = px;
	position_.y = py;

	//�p�x����
	float rot2 = rot_ - 90.0f;


	//inPos�O��
	//outPosLength���̂΂���position
	//�eHead�̊p�x����

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


	//1�t���[���O��rot�Ɣ�r
	r2 = rot_;
	if (r1 < r2) //�E�ɉ���Ă����
	{
		rotDirection = true;
		r1 = rot_;
	}
	else if (r1 > r2) //���ɉ���Ă����
	{
		rotDirection = false;
		r1 = rot_;
	}

	//circle�̓����蔻��
	if (
		(rot_spd_ < 0 && !rotDirection && (Keyboard::GetInstance().KeyStateDown(KEYCODE::D) || GamePad::GetInstance().Stick().x>0.01f)) ||
		(rot_spd_ > 0 && rotDirection && (Keyboard::GetInstance().KeyStateDown(KEYCODE::A) || GamePad::GetInstance().Stick().x<-0.01f)))
	{
		friction = 1.015f; //���C�����炷
		
	}
	else
	{
		friction = 0.985f; //���C��߂�
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
		friction = 0.945f; //���C��߂�
	}
	//�X�s�[�h����
	//
	spdLimit = sqrt(2 * g * (MathHelper::Sin(90.0f) * pHDist.Length())) / ((pHDist.Length()) * 0.02f);
	//spdLimit = sqrt(3.f) / ((pHDist.Length()) * 0.05f);
	rot_spd_ = MathHelper::Clamp(rot_spd_, -spdLimit, spdLimit);

	//���_�Ő��K��
	if (rot_ > 270.0f) rot_ = -90.0f;
	if (rot_ < -90) rot_ = 270.0f;


	pendulumVect_ = (position_ - curdefPos);
	pendulumVect_.x = pendulumVect_.x*jumpShotPower_;
	pendulumVect_.y = pendulumVect_.y*jumpShotPower_;

}
void Player::StartPendulum() {
	Vector2 lngPs = pHeads_[currentHead_]->GetPosition() - position_;
	MultipleInit(lngPs.Length(), pHeads_[currentHead_]->GetPosition(), MathAngle(position_-pHeadPoses_[currentHead_],Vector2::Down));
	Multiple();
}

//���ݎg�p���Ă���Head�̍��W��Ԃ�

Vector2 Player::GetCurrentPHeadPosition() const {
	return pHeads_[currentHead_]->GetPosition();
}
void Player::MultipleInit(float len, const Vector2& fPos, float rot)
{
	mRot.clear();
	mRot_spd.clear();
	mLimit.clear();
	fPos_.clear();
	multiplePos.clear();
	correctionLens.clear();
	drawPoints.clear();

	int s = len / oneLength;

	for (int i = 0; i < s + 1; i++) {
		correctionLens.push_back(1.0f);
	}
	if (len - oneLength > 0) {
		float h = MathHelper::Mod(len - oneLength, oneLength) / oneLength;
		correctionLens.push_back(h);
	}


	fPos_.push_back(fPos);
	mRot.push_back(rot);
	mLimit.push_back(0.0f);
	mRot_spd.push_back(0.f);
	multiplePos.push_back(Vector2(0.0f, 0.0f));
	for (int i = 0; i < s; i++) {
		auto px = fPos_[i].x + MathHelper::Cos(mRot[i]) * (oneLength);
		auto py = fPos_[i].y + MathHelper::Sin(mRot[i]) * (oneLength);

		mRot.push_back(rot);
		mRot_spd.push_back(0.f);
		mLimit.push_back(0.0f);
		fPos_.push_back(Vector2(px, py));
		multiplePos.push_back(Vector2(0.0f, 0.0f));
	}
	//Multiple();
	//
}
void Player::Multiple()
{
	Vector2 curdefPos = position_;

	//���݂̏d��̈ʒu
	for (int i = 0; i < fPos_.size(); i++)
	{
		auto px = fPos_[i].x + MathHelper::Cos(mRot[i]) * (oneLength);
		auto py = fPos_[i].y + MathHelper::Sin(mRot[i]) * (oneLength);

		//�d�͈ړ��ʂ𔽉f�����d��̈ʒu
		auto vx = px - fPos_[i].x;
		auto vy = py - fPos_[i].y;
		auto t = -(vy * gravity_) / (vx * vx + vy * vy);
		auto gx = px + t * vx;
		auto gy = py + gravity_ + t * vy;

		//�Q�̏d��̈ʒu�̊m�x��
		auto rDiff = MathHelper::ATan(gy - fPos_[i].y, gx - fPos_[i].x);

		//�p�x�����p���x�ɉ��Z
		auto sub = rDiff - mRot[i];
		sub -= std::floor(sub / 360.0f) * 360.0f;
		if (sub < -180.0f) sub += 360.0f;
		if (sub > 180.0f) sub -= 360.0f;

		if (i == 0)mRot_spd[i] = mRot_spd[i] + sub;
		else mRot_spd[i] = (mRot_spd[i - 1] + sub) * 0.8f;


		//anyPos2[i] = fPos[i];


		//mRot_spd[i] = MathHelper::Clamp(mRot_spd[i],-3.f,3.f);
		//�p�x�Ɋp���x�����Z
		if (i == 0) mRot[i] += mRot_spd[i]*0.1f;
		else mRot[i] = mRot[0] + mRot_spd[i]*0.1f;

		//�V�����d��̈ʒu
		px = fPos_[i].x + MathHelper::Cos(mRot[i] /*+ rot2*/) * (oneLength);
		py = fPos_[i].y + MathHelper::Sin(mRot[i] /*+ rot2*/) * (oneLength);

		//�d��̍��W
		multiplePos[i].x = px;
		multiplePos[i].y = py;

		//if (fPos.size() - 1 > i) {
		//	fPos[i + 1].x = px;
		//	fPos[i + 1].y = py;
		//}
		//else {
		//}
		if (i > 0) fPos_[i] = multiplePos[i - 1];

		//���_�Ő��K��
		if (mRot[i] > 270.0f) mRot[i] -= 360.0f;
		if (mRot[i] < -90) mRot[i] += 360.0f;



	if ((mRot_spd[0] < 0 /*&& !rotDirection*/ && (Keyboard::GetInstance().KeyStateDown(KEYCODE::D) || GamePad::GetInstance().Stick().x>0.01f)) ||
			(mRot_spd[0] > 0 /*&& rotDirection*/ && (Keyboard::GetInstance().KeyStateDown(KEYCODE::A) || GamePad::GetInstance().Stick().x<-0.01f)))
		{
			friction = 1.015f; //���C�����炷
		}
		else
		{
			friction = 0.985f; //���C��߂�
		}
	//���C
	if (multiplePos[i].y < pHeads_[currentHead_]->GetPosition().y){ 
		friction = 0.9f; //��̐U��q�̏d���荂���オ�����疀�C�𑝂₷
}	//else if (i > 0 && (multiplePos[i].y < multiplePos[i - 1].y)) friction = friction;
	////if (mRot <45.0f || mRot > 135.0f) mFriction = 0.985; //��̐U��q���45�x�ȏ�傫���U�ꂽ�疀�C�𑝂₷
	//else friction = friction; //��L�ȊO�͏�̐U��q�̖��C����
							  //mFriction = friction;

		mRot_spd[i] *= friction;

		////�x�_���ړ�
		//if (Keyboard::GetInstance().KeyStateDown(KEYCODE::RSHIFT) ||
		//	GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM6))
		//{
		//	neckLengh += 1.0f;
		//}
		//if (Keyboard::GetInstance().KeyStateDown(KEYCODE::LSHIFT) ||
		//	GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM5))
		//{
		//	neckLengh -= 1.0f;
		//}
	}

	pHeadPoses_[currentHead_] = multiplePos[0];
	position_ = multiplePos.back();
	pendulumVect_ = (position_ - curdefPos);
	pendulumVect_.x = pendulumVect_.x*jumpShotPower_;
	pendulumVect_.y = pendulumVect_.y*jumpShotPower_;

}

//���d�U��q�Ɉړ��ʂ����Z

void Player::UpdateMultiplePos() {
	SetMultiplePos((position_ - prevPosition_));
	
	//for (int i = 0; i < multiplePos.size(); i++) {
	//	multiplePos[i] += (position_ - prevPosition_);
	//	if (i > 0) fPos_[i] = multiplePos[i - 1];
	//}
}
void Player::SetMultiplePos(const Vector2 & addpos) {
	for (int i = 0; i < multiplePos.size(); i++) {
		multiplePos[i] += addpos;
		if (i > 0) fPos_[i] = multiplePos[i - 1];
	}
	fPos_.front() = pHeads_[currentHead_]->GetPosition() + addpos;
}
void Player::SetNeckNonMult() {
	mRot.clear();
	mRot_spd.clear();
	mLimit.clear();
	fPos_.clear();
	multiplePos.clear();
	correctionLens.clear();
	drawPoints.clear();

	//int s = GetCurrentHeadLength().Length() / oneLength;

	for (int i = 0; i < defHeadLength; i++) {
		correctionLens.push_back(1.0f);
	}
	//if (len - oneLength > 0) {
	//	float h = MathHelper::Mod(len - oneLength, oneLength) / oneLength;
	//	correctionLens.push_back(h);
	//}


	fPos_.push_back(pHeads_[currentHead_]->GetPosition());
	mRot.push_back(135.f);
	mLimit.push_back(0.0f);
	mRot_spd.push_back(0.0f);
	multiplePos.push_back(Vector2(0.0f, 0.0f));
	for (int i = 0; i < defHeadLength-1; i++) {
		auto px = fPos_[i].x + MathHelper::Cos(mRot[i]) * 100;
		auto py = fPos_[i].y + MathHelper::Sin(mRot[i]) * 100;

		mRot.push_back(135.f);
		mRot_spd.push_back(0.0f);
		mLimit.push_back(0.0f);
		fPos_.push_back(Vector2(px, py));
		multiplePos.push_back(Vector2(0.0f, 0.0f));
	}

}
void Player::DeformationDraw()
{
	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::NUM1)) {
	//	posNum = 0;
	//}
	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::NUM2)) {
	//	posNum = 1;
	//}
	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::NUM3)) {
	//	posNum = 2;
	//}
	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::NUM4)) {
	//	posNum = 3;
	//}

	//if (Keyboard::GetInstance().KeyStateDown(KEYCODE::A)) {
	//	gPos1[posNum].x -= 5;
	//}
	//if (Keyboard::GetInstance().KeyStateDown(KEYCODE::D)) {
	//	gPos1[posNum].x += 5;
	//}
	//if (Keyboard::GetInstance().KeyStateDown(KEYCODE::W)) {
	//	gPos1[posNum].y -= 5;
	//}
	//if (Keyboard::GetInstance().KeyStateDown(KEYCODE::S)) {
	//	gPos1[posNum].y += 5;
	//}
	//if (Keyboard::GetInstance().KeyStateDown(KEYCODE::Z)) {
	//	gPos1[2].x -= 5;
	//	gPos1[3].x -= 5;
	//}
	//if (Keyboard::GetInstance().KeyStateDown(KEYCODE::X)) {
	//	gPos1[2].x += 5;
	//	gPos1[3].x += 5;
	//}

	//gPos2[0] = Vector2(gPos1[3].x, gPos1[3].y);
	//gPos2[1] = Vector2(gPos1[2].x, gPos1[2].y);

	//
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
		else if (fPos_.size() - 1 == i) {
			p.p0 = fPos_[i] + n * resWidth;
			p.p1 = fPos_[i] - n * resWidth;
			if (v.Length() > 0) {
				p.p2 = p.p1 - v.Normalize() * 76.0f * correctionLens.back();
				p.p3 = p.p0 - v.Normalize() * 76.0f * correctionLens.back();
				drawPoints.push_back(p);
			}
		}
		else {
			p.p0 = fPos_[i] + n * resWidth;
			p.p1 = fPos_[i] - n * resWidth;
			if (v.Length() > 0) {
				p.p2 = multiplePos[i] - n * resWidth;
				p.p3 = multiplePos[i] + n * resWidth;
				p.p2 -= v.Normalize() * correctionHeight;
				p.p3 -= v.Normalize() * correctionHeight;
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
	correctionLens.clear();

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
		fPos_.push_back(headPoint+(vel*i));
	}


}

void Player::SetDrawNeckParts(const Vector2 & bodyPoint, const Vector2 & headPoint)
{
	drawPoints.clear();
	DrawPos p;

	Vector2 dir(bodyPoint - headPoint);
	//dir = dir.Normalize()*oneLength;

	////������̂ւ̃x�N�g��(���K��)
	//Vector3 pHtoBVec = dir.Normalize();
	for (int i = 0; i < fPos_.size(); i++) {
		p = MathDrawPoint(fPos_[i], dir, resWidth, oneLength);
		drawPoints.push_back(p);

	//	//Vector2 v = fPos_[i] - multiplePos[i];
	//	Vector2 n = Vector2(-v.y, v.x).Normalize();
	//	if (i == 0) {
	//		p.p0 = fPos_[i] + n * resWidth * correctionWidth * 1.2f;
	//		p.p1 = fPos_[i] - n * resWidth * correctionWidth * 1.2f;
	//		if (v.Length() > 0) {
	//			p.p2 = headPoint - n * resWidth * correctionWidth * 1.2f;
	//			p.p3 = headPoint + n * resWidth * correctionWidth * 1.2f;
	//			p.p2 -= v.Normalize() * correctionHeight * correctionWidth;
	//			p.p3 -= v.Normalize() * correctionHeight * correctionWidth;
	//			p.p0 += v.Normalize() * correctionHeight * correctionWidth;
	//			p.p1 += v.Normalize() * correctionHeight * correctionWidth;
	//			drawPoints.push_back(p);
	//		}
	//	}
	//	else if (fPos_.size() - 1 == i) {
	//		p.p0 = fPos_[i] + n * resWidth;
	//		p.p1 = fPos_[i] - n * resWidth;
	//		if (v.Length() > 0) {
	//			p.p2 = p.p1 - v.Normalize() * 76.0f * correctionLens.back();
	//			p.p3 = p.p0 - v.Normalize() * 76.0f * correctionLens.back();
	//			drawPoints.push_back(p);
	//		}
	//	}
	//	else {
	//		p.p0 = fPos_[i] + n * resWidth;
	//		p.p1 = fPos_[i] - n * resWidth;
	//		if (v.Length() > 0) {
	//			p.p2 = fPos_[i-1] - n * resWidth;
	//			p.p3 = fPos_[i - 1] + n * resWidth;
	//			p.p2 -= v.Normalize() * correctionHeight;
	//			p.p3 -= v.Normalize() * correctionHeight;
	//			drawPoints.push_back(p);
	//		}
	//	}
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

	StartPendulum();
}

int Player::GetCurHead() const {
	return currentHead_;
}


//Head�̃��[����{�̂̃��[���ɍ��킹��

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
	//���L�f�[�^�ɁA���g�̌����[���ʒu��ۑ�
	//world_->SetKeepDatas(KeepDatas(laneNum_));
	world_->GetCanChangedKeepDatas().SetPlayerLane(laneNum_);
	world_->GetCanChangedKeepDatas().SetPlayerPos(position_);
}

void Player::setCurPHeadSPos(const Vector2 & sPos) {
	//pHeads_[currentHead_]->setPHeadStopPos(sPos);
	SetMultiplePos(sPos - stopPos_);
	stopPos_ = sPos;
	//Vector2 lngPs = pHeads_[currentHead_]->GetPosition() - position_;
	//MultipleInit(lngPs.Length(), stopPos_, MathAngle(position_ - pHeadPoses_[currentHead_], Vector2::Down));

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

	//float�̌덷�ƁAaddPow�ɂ��Length�̃Y����␳���邽�߂̒l�A��̒����̒l�ɕ␳�����������ꍇ�́A���̕␳�l�������ɉ��Z���鎖�ŁA�����̈�a������������
	float fSaveAddNum = 0.2f;
	if (pHeadLength_[currentHead_] > 16.f+ fSaveAddNum) {
		pHeadLength_[currentHead_] = 16.f+ fSaveAddNum + chainAddLength_- chainAddLengthMath_;
		//�����̏㏸�ɑ΂����Ԓl
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
	//���݂�Head�ȊO�̒�����L�΂����������}�C�i�X����
	pHeadLength_[currentHead_] += addPow;
	//���ׂ��^�[�Q�b�g
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
		//�L�[���������������̔��f
		PHeadChanger();
		isCanNextHeadRot = false;
	}
	if ((GamePad::GetInstance().Stick().x>0.3f || (Keyboard::GetInstance().KeyStateDown(KEYCODE::D)))&& 
		isCanNextHeadRot) {
		playerMode_ = MODE_FALL;
		//�L�[���������������̔��f
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

		//SetNeckNonMult();
		SetDrawNeck(position_,pHeads_[currentHead_]->GetPosition());
		//UpdateMultiplePos();

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

		SetDrawNeck(position_, pHeads_[currentHead_]->GetPosition());
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
		//�L�[���������������̔��f
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

		SetDrawNeck(position_, pHeads_[currentHead_]->GetPosition());
}

void Player::BiteUpdate()
{
	//Pendulum(pHeads_[currentHead_]->GetPosition(), length_);
	Multiple();
	if (GamePad::GetInstance().Stick().y > 0.5f || Keyboard::GetInstance().KeyStateDown(KEYCODE::W)) {
			pSword_->SetUseSword(true);
		}
		if ((GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM1) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::S))) {
			if (GamePad::GetInstance().Stick().y>0.5f || Keyboard::GetInstance().KeyStateDown(KEYCODE::W)) {
				//pSword_->SetUseSword(true);
				SetNextLane(1);
				//UpdateLaneNum(1);
			}
			// if (rot_<0.f || rot_>180.f) {
			else if (mRot.front()<0.f || mRot.front()>180.f) {
				SetNextLane(-1);
				//UpdateLaneNum(-1);
			}
		}

		if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
			//if (GetIsBiteMode()) {
				SetMode(MODE_FALL);
				//Head����シ��
				PHeadChanger();
				isNextPushKey_ = false;
				isCanNextHeadRot = false;
		//	}
		}

		//���ւ̃x�N�g���ƌ��݂̃v���C���[�̈ʒu�x�N�g���̂Ȃ��p�����
		Vector2 ptoPHVec = pHeads_[currentHead_]->GetPosition() - position_;
		float ptoDownCos = Vector2::Dot(ptoPHVec, Vector2::Down) / (ptoPHVec.Length()*Vector2::Down.Length());
		float ptoDownAngle = MathHelper::ACos(ptoDownCos);
		//������p�x��10�ȉ��Ȃ獟���ɓ����
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
			//����E����
			pHeadDead_[currentHead_] = true;
			//�X�e�B�b�N�����b�N����
			isCanNextHeadRot = false;
			isSlipped_ = true;
			//�X���b�v���[�h�Ɉڍs����Ɠ����ɁA���̎��_�̃x�N�g����Head�Ɋi�[����
			pHeads_[currentHead_]->SetPosAddVect(pHeads_[currentHead_]->GetPosition() - position_);
			changeHead();
		}
		slipCount_ = max(slipCount_, 0.f);
		
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

	if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
		//if (GetIsSlipped()) {
			//MODE_SLIP;
			//Head����シ��
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
		//�L�[���������������̔��f
		PHeadChanger(1);
		isCanNextHeadRot = false;
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
		playerMode_ = MODE_FALL;
		return;
	}

	BiteUpdate();
}
