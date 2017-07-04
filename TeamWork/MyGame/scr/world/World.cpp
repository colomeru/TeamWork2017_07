#include "World.h"
#include "../actor/Actor.h"
#include"../math/Vector3.h"
#include"../Def.h"

// �R���X�g���N�^
World::World() :targetAct_(nullptr), keepDatas_(), isChangeCam_(false), addNum_(0),inv_(), isChangeFrame_(false), camShootSpd_(0.f), isLockedCamY_(true), unlockYPos_(0.0f),maxSize_(999999)
{
	updateFunctionMap_[false] = std::bind(&WorldActor::Update, &actors_);
	updateFunctionMap_[true] = std::bind(&WorldActor::ChangeLaneUpdate, &actors_);

}

// �f�X�g���N�^
World::~World()
{
	Clear();
}

// ������
void World::Initialize()
{
	Clear();
	keepDatas_ = KeepDatas();
	isChangeCam_ = false;
	addNum_ = 0;
	isChangeFrame_ = false;
	isLockedCamY_ = true;
	unlockYPos_ = 0.0f;
}

// �X�V
void World::Update()
{
	if (targetAct_ != nullptr&&!isChangeCam_) {
		inv(targetMat_);

		if (isLockedCamY_) {
			if (isFreeCamY_)targetMat_ = Matrix::CreateTranslation(Vector3(targetAct_->GetPosition(), 0));
			else targetMat_ = Matrix::CreateTranslation(Vector3(targetAct_->GetPosition().x, 0, 0));
		}
		else {
			unlockYPos_ -= 3.0f;
			unlockYPos_ = max(unlockYPos_, -WINDOW_HEIGHT*1.5f);
			keepDatas_.camPosY_ = unlockYPos_;
			targetMat_ = Matrix::CreateTranslation(Vector3(targetAct_->GetPosition().x, unlockYPos_, 0));
		}

	}

	isChangeFrame_ = false;
	if (isChangeCam_) {
		if (addNum_ > 0) camShootSpd_ -= 0.1f;
		else camShootSpd_ += 0.1f;
		camShootSpd_ = max(camShootSpd_, 0.1f);
		keepDatas_.SetChangeLaneLerpPos_(keepDatas_.changeLaneLerpPos_ + 0.04f*camShootSpd_);
	}

	if (keepDatas_.changeLaneLerpPos_ >= 1.0f) {
		isChangeCam_ = false;
		isChangeFrame_ = true;
		keepDatas_.SetChangeLaneLerpPos_(0.0f);
	}
	if (keepDatas_.isFallCamMode_) {
		keepDatas_.fallAddPos_ -= 10.f;
		if (keepDatas_.fallAddPos_ <= 0.1f) {
			keepDatas_.isFallCamMode_=false;
		}
	}
	updateFunctionMap_[isChangeCam_]();
	// �󓮍X�V
	if (!manualStackActor_.empty()&&!isChangeCam_)
		manualStackActor_.top()->OnUpdate();

}

// �`��
void World::Draw(const int laneCount, const int playerLane) const
{
	if(!isChangeFrame_)actors_.Draw(laneCount, playerLane);

	if (BuildMode != 1)return;
	DrawFormatString(0,600,GetColor(255,255,255),"%f", keepDatas_.changeLaneLerpPos_);
}

// �N���A
void World::Clear()
{
	// �X�^�b�N����
	while (!manualStackActor_.empty())
		manualStackActor_.pop();
	while (!stackCamera_.empty())
		stackCamera_.pop();

	// �A�N�^�[����
	actors_.Clear();
}

// �C�x���g���X�i�[�̒ǉ�
void World::AddEventMessageListener(std::function<void(EventMessage, void*)> listener)
{
	actors_.AddEventMessageListener(listener);
}

// �A�N�^�[�̎擾
WorldActor World::GetWorldActor() const
{
	return actors_;
}

// �ǉ�
void World::Add(ACTOR_ID id, ActorPtr actor)
{
	actors_.Add(id, actor);
}

// 
bool World::IsEnd() const
{
	return false;
}

// �Փ˔���
void World::SetCollideSelect(ActorPtr thisActor, ACTOR_ID otherID, COL_ID colID)
{
	actors_.SetCollideSelect(thisActor, otherID, colID);
}

// ���b�Z�[�W�̑��M
void World::sendMessage(EventMessage message, void * param)
{
	actors_.HandleMessage(message, param);
}

// �w��O���[�v�̏���
void World::EachActor(ACTOR_ID id, std::function<void(Actor&)> func)
{
	actors_.EachActor(id, func);
}

// �w��O���[�v�̏���
void World::EachActor(ACTOR_ID id, std::function<void(const Actor&)> func) const
{
	actors_.EachActor(id, func);
}

int World::Count(ACTOR_ID id) const
{
	return actors_.Count(id);
}

// ����A�N�^�[�Z�b�g
void World::PushStackActor(ActorPtr actor)
{
	manualStackActor_.push(actor);
}

// ����A�N�^�[�|�b�v
void World::PopStackActor()
{
	if (!manualStackActor_.empty())
		manualStackActor_.pop();
}

void World::inv(const Matrix & mat)
{
	//1�t���[���O�̍��W
	mPrePos = Vector2(inv_.Translation().x, inv_.Translation().y);
	//�X�N���[���X�g�b�v����
	Matrix playerMat;
	playerMat = mat;

	//�ő�ŏ��l�̎w��(0,999999�͌�ŏ�����)
	float clampPosX = MathHelper::Clamp((int)playerMat.Translation().x, -100, maxSize_);
	float clampPosY = MathHelper::Clamp((int)playerMat.Translation().y, -10000, 999999);

	playerMat.Translation(Vector3(clampPosX, clampPosY, 0.0f));

	//�s���ׂ��ʒu��ݒ�(matrix��)
	resInv_ = Matrix::Invert(playerMat) *
		Matrix::CreateTranslation(Vector3(playerScreenPos_.x, playerScreenPos_.y));
	//�s���ׂ��ʒu��ݒ�
	Vector2 resPos = Vector2(resInv_.Translation().x, resInv_.Translation().y);
	Vector2 pos = Vector2(inv_.Translation().x, inv_.Translation().y);

	Spring(pos, resPos, velo, 0.2f);
	//�␳���ꂽ�ړ��}�g���b�N�X���
	inv_ = Matrix::CreateTranslation(Vector3(
		pos.x,
		pos.y,
		0.0f));

	//1�t���[����̍��W
	mCurPos = Vector2(inv_.Translation().x, inv_.Translation().y);
	//�ړ��ʂ��v�Z
	mVelo = mPrePos - mCurPos;
	mVelo = Vector2(mVelo.x, mVelo.y);

}
Matrix World::InitializeInv(Vector2 position)
{
	//1�t���[���O�̍��W
	mPrePos = Vector2(inv_.Translation().x, inv_.Translation().y);
	Matrix playerMat;
	playerMat.Translation(Vector3(position.x, position.y, 0.0f));

	inv_ = Matrix::Invert(playerMat) *
		Matrix::CreateTranslation(Vector3(playerScreenPos_.x, playerScreenPos_.y));
	resInv_ = Matrix::Invert(playerMat) *
		Matrix::CreateTranslation(Vector3(playerScreenPos_.x, playerScreenPos_.y));

	//1�t���[����̍��W
	mCurPos = Vector2(inv_.Translation().x, inv_.Translation().y);
	//�ړ��ʂ��v�Z
	OutputDebugString(std::to_string(position.x).c_str());
	OutputDebugString(":");
	OutputDebugString(std::to_string(position.y).c_str());
	OutputDebugString("\n");
	return inv_;
}

void World::StartModeUpdate()
{
	inv(targetMat_);
	targetMat_ = Matrix::CreateTranslation(Vector3(targetAct_->GetPosition().x, 0, 0));
	actors_.StartModeUpdate();
	isLockedCamY_ = true;
}
