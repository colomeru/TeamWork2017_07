#include "World.h"
#include "../actor/Actor.h"
#include"../math/Vector3.h"

// �R���X�g���N�^
World::World() :targetAct_(nullptr), keepDatas_(), isChangeCam_(false), addNum_(0),inv_(), isChangeFrame_(false), camShootSpd_(0.f)
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
}

// �X�V
void World::Update()
{
	if (targetAct_ != nullptr&&!isChangeCam_) {
		inv(targetMat_);
		//targetMat_ = Matrix::CreateTranslation(Vector3(targetAct_->GetPosition().x, targetAct_->GetPosition().y, 0));
		targetMat_ = Matrix::CreateTranslation(Vector3(targetAct_->GetPosition().x, 0, 0));
		//*Matrix::CreateRotationZ(targetAct_->GetAngle());
	}

	isChangeFrame_ = false;
	if (isChangeCam_) {
		keepDatas_.SetPlayerNextLane(addNum_);
		//camShootSpd_ += 0.1f;
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
	//actors_.Update();
	updateFunctionMap_[isChangeCam_]();
	// �󓮍X�V
	if (!manualStackActor_.empty())
		manualStackActor_.top()->OnUpdate();

	// �J�����X�V
	//if (!stackCamera_.empty())
	//	stackCamera_.top()->OnUpdate();
}

// �`��
void World::Draw(const int laneCount, const int playerLane) const
{
	//DrawFormatString(0,600,GetColor(255,255,255),"%f:%f", inv_.Translation().x, inv_.Translation().y);
	//actors_.Draw(laneCount, playerLane);
	if(!isChangeFrame_)actors_.Draw(laneCount, playerLane);
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
	float clampPosX = MathHelper::Clamp((int)playerMat.Translation().x, 0, 999999);
	float clampPosY = MathHelper::Clamp((int)playerMat.Translation().y, 0, 999999);

	//if (scrool.scroolJudge.x == 0)
	//	clampPosX = playerScreenPos_.x;
	//if (scrool.scroolJudge.y == 0)
	//	clampPosY = playerScreenPos_.y;
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
		pos.x,//*scrool.scroolJudge.x,
		pos.y,//*scrool.scroolJudge.y,
		0.0f));

	//1�t���[����̍��W
	mCurPos = Vector2(inv_.Translation().x, inv_.Translation().y);
	//�ړ��ʂ��v�Z
	mVelo = mPrePos - mCurPos;
	mVelo = Vector2(mVelo.x/**scrool.scroolJudge.x*/, mVelo.y /** scrool.scroolJudge.y*/);

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
	//mVelo = mPrePos - mCurPos;
	return inv_;
}

void World::StartModeUpdate()
{
	inv(targetMat_);
	targetMat_ = Matrix::CreateTranslation(Vector3(targetAct_->GetPosition().x, 0, 0));
	actors_.StartModeUpdate();
}
