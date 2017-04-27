#include "Player.h"
#include "../../input/Keyboard.h"
#include "../../conv/DXConverter.h"
#include "../../graphic/Model.h"
#include"../../math/MathHelper.h"
#include"Player_Head.h"
#include"../../math/Matrix.h"
#include"../../math/MyFuncionList.h"
#include"../../input/GamePad.h"

static const float headShotPower = 0.3f;
static const float defMaxChainLength = 16.f;
static const float defJumpShotPower = 1.5f;

Player::Player(IWorld * world)
	:Actor(world),
	isHit_(false), fulcrum_(500.0f, 200.0f), rot_(135.f), rot_spd_(-3.0f), length_(300.0f), gravity_(0.5f), currentHead_(0),
	headChangeTime_(0), pGrav_(defPGravPow), maxChainLength_(defMaxChainLength), isBiteMode_(false), isShootMode_(0), isNextPushKey_(true),
	pendulumVect_(Vector2::Zero), slipCount_(defSlipCount), jumpShotPower_(defJumpShotPower), isSlipped_(false), chainLock_(false), nextLane_(999),/* isCanChangeLane_(false),*/ laneChangeCoolTime_(0),
	otherClothesID_(CLOTHES_ID::FLUFFY_CLOTHES), isReSetClothesType_(false),friction(0.998f), spdLimit(2.75f), isCanNextHeadRot(true)

{
	laneNum_ = 1;

	spriteId_ = SPRITE_ID::CIRCLE_SPRITE;

	parameter_.ID = ACTOR_ID::PLAYER_ACTOR;
	parameter_.radius = Sprite::GetInstance().GetSize(spriteId_).x / 2;
	parameter_.size = Sprite::GetInstance().GetSize(spriteId_);
	parameter_.HP = 10;
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3::Zero);

	position_ = Vector2(0, 200);

	slipCountMult_[CLOTHES_ID::BASE_CLOTHES] = 1.f;
	slipCountMult_[CLOTHES_ID::FLUFFY_CLOTHES] = 0.f;
	slipCountMult_[CLOTHES_ID::GOAL_CLOTHES] = 1.f;
	slipCountMult_[CLOTHES_ID::GUM_CLOTHES] = 1.f;
	slipCountMult_[CLOTHES_ID::HANGER] = 1.5f;
	slipCountMult_[CLOTHES_ID::TEST_CLOTHES] = 1.f;
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
	worldSetMyDatas();

	StartPlayerSet();
}

Player::~Player()
{
}

void Player::Update()
{
	//if (isCanChangeLane_) {
	//	UpdateLaneNum(laneAddNum_);
	//}
	//���[���ύX�̃N�[���^�C����ݒ�
	laneChangeCoolTime_--;

	pendulumVect_ -= pendulumVect_*0.05f;

	length_ = Vector2::Distance(pHeads_[currentHead_]->GetPosition(), position_);
	//if(!pHeads_[currentHead_]->getIsHit())pGrav_+= defPGravPow;
	if (!isBiteMode_)pGrav_ += defPGravPow;
	velocity_ = Vector2::Zero;
	isHit_ = false;
	auto pos = parameter_.mat.Translation();

	//�d��
	velocity_.y += pGrav_*pGrav_*defGravAddPow;

	//�ړ������������Z�b�g���Ă�����͊֐��ɓ���
	isNextLaneBite_ = false;
	//nextLane�����Z�b�g���Ă�����͊֐��ɓ���
	nextLane_ = 999;
	//���͂ɂ�铮����܂Ƃ߂��֐�
	PlayerInputControl();





	//����,�����ID,Col�̎��
	//world_->SetCollideSelect(shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::BOX_CLOTHES_COL);

	//�s���angle��������
	parameter_.mat += Matrix::CreateRotationZ(angle_);
	parameter_.mat.NormalizeRotationMatrix();

	//���ʂւ̈ړ��ʂ�ǉ�(DX���C�u����������Z�Ƀ}�C�i�X)
	//velocity_ += parameter_.mat.Backward() * speed;
	//velocity_ += parameter_.mat.Up() * jumpVec;
	//velocity��position�ɒǉ�
	pos += Vector3(velocity_.x, velocity_.y);

	//if (pos.y < 0)pos.y = 0;

	if (!isBiteMode_) {
		position_ += velocity_ + pendulumVect_;

		slipCount_ = defSlipCount;
	}
	parameter_.mat.Translation(pos);

	HeadPosUpdate();


	if (isBiteMode_) {
		slipCount_ -= 0.016f*slipCountMult_[otherClothesID_];
		if (slipCount_ <= 0.f) {
			isBiteMode_ = false;
			isSlipped_ = true;
			//�X���b�v���[�h�Ɉڍs����Ɠ����ɁA���̎��_�̃x�N�g����Head�Ɋi�[����
			pHeads_[currentHead_]->SetPosAddVect(pHeads_[currentHead_]->GetPosition() - position_);
		}
		slipCount_ = max(slipCount_, 0.f);
	}

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
	Sprite::GetInstance().Draw(spriteId_, drawPos_, crcOrigin, spriteAlpha_, Vector2::One);
	//DrawLine(pos.x - seg.x, pos.y-seg.y, pos.x + seg.x, pos.y+seg.y, GetColor(255, 255, 255));
	//DrawFormatString(0, 60, GetColor(255, 255, 255), "position x:%f y:%f z:%f", pos.x, pos.y);
	//DrawFormatString(0, 80, GetColor(255, 255, 255), "angle %f", velocity_.y);
	//DrawFormatString(0, 100, GetColor(255, 255, 255), "%d", laneNum_);
	DrawFormatString(50, 100, GetColor(255, 255, 255), "%f", rot_);
	//DrawFormatString(400, 100, GetColor(255, 255, 255), "%f",angle_);
	//if (isShootMode_>=1)DrawFormatString(0, 700, GetColor(255, 255, 255), "true");
	//else DrawFormatString(0, 700, GetColor(255, 255, 255), "false");
	//DrawFormatString(0, 700, GetColor(255, 255, 255), "%f:%f", pHeadPoses_[currentHead_].x, pHeadPoses_[currentHead_].y);
	DrawFormatString(400, 100, GetColor(255, 255, 255), "%f:%f",GamePad::GetInstance().Stick().x, GamePad::GetInstance().Stick().y);

	//int count = 0;
	//for (auto sgT : pHeadLength_) {
	//DrawFormatString(300, 300 + (30 * count),GetColor(255,255,255),"%f",sgT );
	//if (pHeadDead_[count])DrawFormatString(300, 300 + (30 * count), GetColor(255, 255, 255), "true");
	//else DrawFormatString(300, 300 + (30 * count), GetColor(255, 255, 255), "false");
	//count++;
	//}


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

	////circle�̓����蔻��
	//Vector2 absA = spherePos - arrowPos;
	//float distanceS = sqrt((absA.x * absA.x) + (absA.y * absA.y));
	//if (distanceS < 32.0f ||
	//	rot_spd < 0 && (Keyboard::GetInstance().KeyStateDown(KEYCODE::RIGHT) || GamePad::GetInstance().ButtonStateDown(PADBUTTON::RIGHT)) ||
	//	rot_spd > 0 && (Keyboard::GetInstance().KeyStateDown(KEYCODE::LEFT) || GamePad::GetInstance().ButtonStateDown(PADBUTTON::LEFT)))
	//{
	//	aAlpha = 0.5f; //circle�ɓ������Ă���Δ�����
	//	friction = 1.02f; //���C�����炷
	//}
	//else
	//{
	//	aAlpha = 1.0f; //circle�ɓ������Ă��Ȃ���Εs����
	//	friction = 0.98f;
	//}

	////�X�s�[�h����
	//if (rot_spd > 4.0f) rot_spd = 4.0f;
	//if (rot_spd < -4.0f) rot_spd = -4.0f;

	////�x�_���ړ�
	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::RSHIFT) ||
	//	GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2))
	//{
	//	vec += 1;
	//	if (vec > 7) vec = 0;
	//	length = neckLen[vec];
	//	fx = fulcrums[vec].x;
	//	fy = fulcrums[vec].y;
	//	lineRot[0] -= 45.0f;
	//	rot += 45.0f;
	//	turn = false;
	//}
	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::LSHIFT) ||
	//	GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM3))
	//{
	//	vec -= 1;
	//	if (vec < 0) vec = 7;
	//	length = neckLen[vec];
	//	fx = fulcrums[vec].x;
	//	fy = fulcrums[vec].y;
	//	lineRot[0] += 45.0f;
	//	rot -= 45.0f;
	//	turn = true;
	//}

	//for (int i = 1; i < 8; i++)
	//{
	//	lineRot[i] = lineRot[i - 1] + 45.0f;
	//}

	////������
	//if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM6))
	//{
	//	if (vec == 7)
	//	{
	//		if (neckLen[vec - 1] <= r + 20.0f) return;
	//		neckLen[0] += 5.0f;
	//		neckLen[vec - 1] -= 5.0f;
	//	}
	//	else
	//	{
	//		if ((vec != 0 && neckLen[vec - 1] <= r + 20.0f) || (vec == 0 && neckLen[7] <= r + 20.0f)) return;
	//		neckLen[vec + 1] += 5.0f;
	//		if (vec == 0) neckLen[7] -= 5.0f;
	//		else neckLen[vec - 1] -= 5.0f;
	//		//neckLen[vec - 1, 7] = MathHelper::Clamp(neckLen[vec - 1,7], 0.0f, 500.0f);
	//	}
	//}
	//else if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM5))
	//{
	//	if (vec == 0)
	//	{
	//		if (neckLen[vec + 1] <= r + 20.0f) return;
	//		neckLen[7] += 5.0f;
	//		neckLen[vec + 1] += 5.0f;
	//	}
	//	else
	//	{
	//		if ((vec != 7 && neckLen[vec + 1] <= r + 20.0f) || (vec == 7 && neckLen[0] <= r + 20.0f)) return;
	//		neckLen[vec - 1] += 5.0f;
	//		if (vec == 7) neckLen[0] -= 5.0f;
	//		else neckLen[vec + 1] -= 5.0f;
	//		//neckLen[vec + 1, 0] = MathHelper::Clamp(neckLen[vec + 1,0], 20.0f, 500.0f);
	//	}
	//}
}

//void Player::Pendulum(Vector2 fulcrum, float length)
//{
//	float friction = 0.998f;								//���C
//	//���݂̏d��̈ʒu
//	position_.x = fulcrum.x + MathHelper::Cos(rot_) * length;
//	position_.y = fulcrum.y + MathHelper::Sin(rot_) * length;
//	
//	Vector2 curdefPos = position_;
//
//	//�d�͈ړ��ʂ𔽉f�����d��̈ʒu
//	auto length_vec = position_ - fulcrum;
//	auto t = -(length_vec.y * gravity_) / (length_vec.x * length_vec.x + length_vec.y * length_vec.y);
//	auto move_weightX = position_.x + t * length_vec.x;
//	auto move_weightY = position_.y + gravity_ + t * length_vec.y;
//
//	//2�̏d��̈ʒu�̊p�x��
//	auto r = MathHelper::ATan(move_weightY - fulcrum.y, move_weightX - fulcrum.x);
//	//�p�x�����p���x�ɉ��Z
//	auto sub = r - rot_;
//	sub -= std::floor(sub / 360.0f) * 360.0f;
//	if (sub < -180.0f) sub += 360.0f;
//	if (sub > 180.0f) sub -= 360.0f;
//	rot_spd_ += sub;
//
//	//���C
//	rot_ *= friction;
//
//	//�p�x�Ɋp���x�����Z
//	rot_ += rot_spd_;
//
//	//�V�����d��̈ʒu
//	position_.x = fulcrum.x + MathHelper::Cos(rot_) * length;
//	position_.y = fulcrum.y + MathHelper::Sin(rot_) * length;
//
//	//�����܂ŐU��q
//	//
//	Vector2 pendulumAngleVec = fulcrum - position_;
//	
//	float angleDtData = Vector2::Dot(Vector2::Right, pendulumAngleVec.Normalize());
//
//	angle_ = (angleDtData * 180)/2;
//	
//	pendulumVect_ = (position_ - curdefPos);
//	pendulumVect_.x = pendulumVect_.x*jumpShotPower_;
//}

void Player::HeadPosUpdate()
{
	headChangeTime_ -= 0.016f*sign(headChangeTime_);

	rotTimer = 0;
	if (MathHelper::Abs(headChangeTime_) <= 0.01f)headChangeTime_ = 0;
	else if (MathHelper::Abs(headChangeTime_) > 0)rotTimer = headChangeTime_ * 5;//MathHelper::Abs(defHeadChangeTime/1.f);

	for (int i = 0; i < pHeadPoses_.size(); i++) {
		Vector3 tgtRot = Vector3(pHDist.x, pHDist.y)*Matrix::CreateRotationZ(((i + headAngleSetter - currentHead_) * 45)/*+angle_*/ + ((rotTimer)* 45));
		Vector2 cgToV2 = position_ + Vector2(tgtRot.x, tgtRot.y);
		pHeadPoses_[i] = cgToV2;
	}

}

void Player::StartPlayerSet() {
	isBiteMode_ = true;
	pHeadLength_[currentHead_]=5.f;
	pHeads_[currentHead_]->StartPlayerHeadBite();
}

int Player::GetCurHead() const {
	return currentHead_;
}


//Head�̃��[����{�̂̃��[���ɍ��킹��

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
	world_->GetCanChangedKeepDatas().SetPlayerNextLane(nextLane_);
}

void Player::PlayerInputControl()
{
	//if (pHeads_[currentHead_]->getIsHit()) {
	//if (isBiteMode_&&pHeads_[currentHead_]->getIsHit()) {
	if (isBiteMode_) {
		//Pendulum(pHeadPoses_[currentHead_]+(pHeads_[currentHead_]->GetPosition()- pHeadPoses_[currentHead_]), length_);
		Pendulum(pHeads_[currentHead_]->GetPosition(), length_);
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::UP)) {
		velocity_.y -= 20.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::DOWN)) {
		//velocity_.y += 20.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::RIGHT)) {
		velocity_.x += 20.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::LEFT)) {
		velocity_.x -= 20.0f;
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::W)) {
	}
	if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM1)&&isBiteMode_) {
		if (GamePad::GetInstance().Stick().y>0.1f) {
			UpdateLaneNum(1);
		}
		else if (rot_<0.f || rot_>180.f) {
			UpdateLaneNum(-1);
			//SetNextLane(-1);
		}
		//SetNextLane(1);
	}
	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::A)) {
	//	rot_spd_ += 1.f;
	//}
	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::D)) {
	//	rot_spd_-=1.f;
	//}
	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::Q)) {
	//	jumpShotPower_ -= 1.f;
	//}
	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::E)) {
	//	jumpShotPower_ += 1.f;
	//}

	SetAllHeadLaneNum();
	if (GamePad::GetInstance().Stick().x<-0.5f && !isBiteMode_&&isCanNextHeadRot) {
		isShootMode_ = 0;
		isBiteMode_ = false;
		//�L�[���������������̔��f
		PHeadLengthReset();
		changeHead();
		isCanNextHeadRot = false;
	}
	if (GamePad::GetInstance().Stick().x>0.5f && !isBiteMode_&&isCanNextHeadRot) {
		isShootMode_ = 0;
		isBiteMode_ = false;
		//�L�[���������������̔��f
		PHeadLengthReset();
		backChangeHead();
		isCanNextHeadRot = false;
	}
	if (MathHelper::Abs(GamePad::GetInstance().Stick().x)<0.5f && !isBiteMode_) {
		isCanNextHeadRot = true;
	}
	if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2)) {
		if (isSlipped_) {
			isBiteMode_ = false;
			//�ꏊ��߂��Ă���
			PHeadLengthReset();
			//Head����シ��
			changeHead();
			isNextPushKey_ = false;
		}
		isNextPushKey_ = true;
		if (isBiteMode_) {
			//if (rot_<0.f || rot_>180.f) {
			//	UpdateLaneNum(-1);
			//	//SetNextLane(-1);
			//	isNextPushKey_ = false;
			//}
			//else {
				isBiteMode_ = false;
				//�ꏊ��߂��Ă���
				PHeadLengthReset();
				//Head����シ��
				changeHead();
				isNextPushKey_ = false;
			//}
				isCanNextHeadRot = false;
		}
		if (!isBiteMode_&&isNextPushKey_) {
			isShootMode_ = 1;
			isNextPushKey_ = false;
		}
	}
	if (GamePad::GetInstance().ButtonStateDown(PADBUTTON::NUM2)) {
		if (!isBiteMode_&&isShootMode_ == 1)CurPHeadLengPlus(headShotPower);
	}
	else if (GamePad::GetInstance().ButtonTriggerUp(PADBUTTON::NUM2) && (isShootMode_ == 1)) {
		isShootMode_ = 2;
	}
	else if (GamePad::GetInstance().ButtonTriggerUp(PADBUTTON::NUM2) && (isShootMode_ == 3)) {
		isShootMode_ = 4;
	}
	else {
		isShootMode_ = 0;
	}
	//if (Keyboard::GetInstance().KeyStateDown(KEYCODE::M)) {
	//	if (!isBiteMode_)isShootMode_ = true;
	//	if (!isBiteMode_&&isNextPushKey_) {
	//		isShootMode_ = true;

	//		CurPHeadLengPlus(headShotPower);
	//	}
	//	isBiteMode_ = false;
	//}
	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
	//	if (pHeads_[currentHead_]->getIsHit()) {
	//		isShootMode_ = false;
	//		//�L�[���������������̔��f
	//		isNextPushKey_ = false;
	//		PHeadLengthReset();
	//		//�ꏊ��߂��Ă���Head����シ��
	//		changeHead();
	//	}
	//	else {
	//		isNextPushKey_ = true;
	//		isShootMode_ = false;
	//	}
	//}
	//if(!Keyboard::GetInstance().KeyStateDown(KEYCODE::M)){
	//	if (isShootMode_) {
	//		if (pHeads_[currentHead_]->getIsBitePoint()) {
	//			//isBiteMode_ = true;
	//		}
	//	}
	//	//isShootMode_ = false;
	//}

}

void Player::CurPHeadLengPlus(float addPow) {

	if (pHeadLength_[currentHead_] >= 16.f) {
		pHeadLength_[currentHead_] = 16.f;

		if (!chainLock_) {
			chainLock_ = true;
			isShootMode_ = 3;
			return;
		}

		int nextNum = currentHead_ + 1;
		if (nextNum>7) {
			nextNum = nextNum - 8;
		}
		pHeadDead_[nextNum] = true;
		return;
	}

	//���݂�Head�ȊO�̒�����L�΂����������}�C�i�X����
	pHeadLength_[currentHead_] += addPow;
	//�`�F�[���̒����̍ő�l�𒴂�����A�ő�l�ɕ␳����
	if (pHeadLength_[currentHead_] > 16.f)pHeadLength_[currentHead_] = 16.f;

	//���ׂ��^�[�Q�b�g
	int targetNum = currentHead_ - 1;


	float LengthKeepNum = 1.01f;
	if (targetNum<0) {
		targetNum = targetNum + 8;
	}
	for (;;) {
		if (LengthKeepNum < 0.f)break;

		if (pHeadLength_[targetNum] < LengthKeepNum)
		{
			targetNum--;

			if (targetNum<0) {
				targetNum = targetNum + 8;
			}

			if (targetNum == currentHead_) {
				LengthKeepNum -= 1.f;
				targetNum--;

				if (targetNum<0) {
					targetNum = targetNum + 8;
				}
			}

			continue;
		}
		pHeadLength_[targetNum] -= addPow;
		if (pHeadLength_[targetNum] < 0)pHeadLength_[targetNum] = 0;


		for (int i = 0; i < pHeads_.size(); i++) {
			if (pHeadDead_[i])continue;

			if (pHeadLength_[i] <= 0.1f) {
				if (!chainLock_) {
					chainLock_ = true;
					isShootMode_ = 3;
					break;
				}
				chainLock_ = false;
				pHeadDead_[i] = true;
			}
			else {
				//pHeadDead_[i] = false;
			}
		}
		break;
	}
}
