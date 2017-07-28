#include "CreditPlayer.h"
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
#include"PlayerNeckDraw.h"
#include"PlayerDeadPin.h"
#include"PlayerDeadHead.h"
#include"../../sound/sound.h"
#include"../../cheat/CheatData.h"
#include "../../actor/player/PlayerFallPin.h"
#include "PlayerNeck\PlayerNeckPendulumSupport.h"
#include "../../debugdata/DebugDraw.h"
#include "../../input/InputChecker.h"

static const float headShotPower = 0.3f;
static const float defMaxChainLength = 16.f;
static const float defJumpShotPower = 1.5f;
//���\�[�X�̉��T�C�Y
const int resWidth = 15;
const int correctionHeight = 30;
const float correctionWidth = 1.8f;
//���\�[�X�̏c�T�C�Y
const float oneLength = 30.f;

//�R���X�g���N�^
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
		//8�����̂����A�e���ɑΉ������ʒu���쐬
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
	auto a = 1;
}

//�f�X�g���N�^
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

//���b�Z�[�W����
void CreditPlayer::OnMessage(EventMessage message, void * param)
{
	switch (message) {
	case EventMessage::GOAL_FLAG: {
		SetMode(MODE_CLEAR);
		world_->UnLockCameraPosY();
		isUseKey_.SetUseKey(false);
		break;
	}
	case EventMessage::OPERATE_FLAG: {
		if ((int)param > 0) operatre_ = true;
		//���A���͑�����󂯕t���Ȃ�
		else operatre_ = false;
	}
	default:
		break;
	}
}

//�d�̓��Z�b�g
void CreditPlayer::GravityReset()
{
	pGrav_ = defPGravPow;
}

//���d�U��q�̏�����
void CreditPlayer::MultipleInit(float len, const Vector2& fPos, float rot, float radius)
{
	auto px = GetCurrentPHeadPosition().x + MathHelper::Cos(rot + 90.0f) * (len);
	auto py = GetCurrentPHeadPosition().y + MathHelper::Sin(rot + 90.0f) * (len);
	PlayerNeckPendulumSupport::Init(GetCurrentPHeadPosition(), Vector2(px, py), fPos_, multiplePos, mRot, mRot_spd, neckDrawPoints);
}

//1�ō��ׂ́A�����͂ŉE�ׂ�Head�ɉ�]���A���������Z�b�g����
void CreditPlayer::PHeadLengthReset() {
	//�����̕�Ԃ����Z�b�g����
	chainAddLength_ = 0.f;
	chainAddLengthMath_ = 0.f;

	//�`�F�[���̃��b�N�����Z�b�g����
	for (auto& pHL : pHeadLength_) {
		pHL = 2.f;
	}
}

//���S��������S�ĉ�
void CreditPlayer::AllResurrectHead()
{
	for (int i = 0; i < pHeads_.size(); i++) {
		ResurrectHead();
	}
}

//������
void CreditPlayer::FallUpdate()
{
	pGrav_ += defPGravPow;
	if (!operatre_) return;
	if (isUseKey_.StickTriggerDown(InputChecker::Input_Stick::Left)) {
		SetMode(MODE_FALL);
		//�L�[���������������̔��f
		PHeadChanger();
		world_->sendMessage(EventMessage::CHANGE_HEAD_KEY);
	}
	if (isUseKey_.StickTriggerDown(InputChecker::Input_Stick::Right)) {
		SetMode(MODE_FALL);
		//�L�[���������������̔��f
		PHeadChanger(1);
		world_->sendMessage(EventMessage::CHANGE_HEAD_KEY);
	}

	if (isUseKey_.KeyTriggerDown(InputChecker::Input_Key::B)) {
		world_->sendMessage(EventMessage::NECK_SHOOT);
		SetMode(MODE_SHOOT);
	}
	else if (isUseKey_.KeyTriggerDown(InputChecker::Input_Key::X)) {
		world_->sendMessage(EventMessage::NECK_SHOOT);
		SetMode(MODE_BACK_SHOOT);
	}

	position_ += velocity_ + pendulumVect_;

	slipCount_ = defSlipCount;

	SetDrawNeck(position_, pHeads_[currentHead_]->GetPosition());
}

//�V���[�g��
void CreditPlayer::ShootUpdate()
{
	pGrav_ += defPGravPow;
	if (!operatre_) return;
	if (isUseKey_.KeyStateUp(InputChecker::Input_Key::B)) {
		pHeads_[currentHead_]->SetBiteSprite();
		SetMode(MODE_SHOOT_END);
	}
	else if (isUseKey_.KeyStateDown(InputChecker::Input_Key::B)) {
		CurPHeadLengPlus(headShotPower);
	}

	position_ += velocity_ + pendulumVect_;

	slipCount_ = defSlipCount;

	SetDrawNeck(position_, pHeads_[currentHead_]->GetPosition());
}

//���ݒ�
void CreditPlayer::BiteUpdate()
{
	Multiple();
	if (!operatre_) return;
	if (isUseKey_.KeyStateDown(InputChecker::Input_Key::A)) {
		if (!pSword_->GetUseSword()) {
			world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<GetSwordEffect>(world_, pSword_->GetPosition(), pSword_.get()));
			world_->sendMessage(EventMessage::USE_SWORD);
			pSword_->SetUseSword(true);
			Sound::GetInstance().PlaySE(SE_ID::CREATE_SWORD_SE);
		}
	}

	//if (GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2) || Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
	if (isUseKey_.KeyTriggerDown(InputChecker::Input_Key::B) || InputChecker::GetInstance().KeyTriggerDown(InputChecker::Input_Key::X)) {
		SetMode(MODE_FALL);
		PHeadChanger();
	}

	slipCount_ -= 0.016f*slipCountMult_[otherClothesID_];
	if (slipCount_ <= 0.f) {
		SetMode(MODE_SLIP);
		//����E����
		Vector2 toPos = pHeadPoses_[currentHead_] - position_;
		world_->Add(ACTOR_ID::EFFECT_ACTOR, std::make_shared<PlayerFallPin>(world_, pHeads_[currentHead_]->GetPosition(), toPos));
		pHeadDead_[currentHead_] = true;
		//�X���b�v���[�h�Ɉڍs����Ɠ����ɁA���̎��_�̃x�N�g����Head�Ɋi�[����
		pHeads_[currentHead_]->SetPosAddVect(pHeads_[currentHead_]->GetPosition() - position_);
		PHeadChanger();
	}
	slipCount_ = max(slipCount_, 0.f);
}
