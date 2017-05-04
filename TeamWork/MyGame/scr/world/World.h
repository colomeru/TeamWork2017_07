#pragma once
#include "IWorld.h"
#include "WorldActor.h"
#include"../math/MathHelper.h"
#include"../math/Vector3.h"
#include"../math/Vector2.h"
#include <stack>

static const float defDrawLinePosY[3] = { 0,400,1000 };
static const float defDrawLineChangePosY[5] = { -400,0,400,1000,1500 };

//World���ŁA�A�N�^�[�S�����擾�o����f�[�^
struct KeepDatas {
	//player�̌��݂̃��[��
	int playerLane_;
	//���̃��[���̕�����������(-1�`1)
	int nextLane_;
	Vector2 playerPos_;
	float changeLaneLerpPos_;

	KeepDatas(int lane = 0, Vector2 pos = Vector2::Zero, int nextLane = 0) :playerLane_(lane), playerPos_(pos), nextLane_(nextLane), changeLaneLerpPos_(0.f){}

	//player�̌����[�����X�V����
	void SetPlayerLane(const int& pLane) {
		playerLane_ = pLane;
	}
	//player�̌����[����ύX����
	void SetPlayerPos(const Vector2& pPos) {
		playerPos_ = pPos;
	}
	//�v���C���[�����ɍs�����[�����w�肷��
	void SetPlayerNextLane(const int& pNLane) {
		nextLane_ = pNLane;
	}
	void SetChangeLaneLerpPos_(float lNum) {
		changeLaneLerpPos_ = lNum;
	}
};

static const Vector2 playerScreenPos_ = Vector2(300, 0);
class World : public IWorld
{
public:
	// �R���X�g���N�^
	World();
	// �f�X�g���N�^
	~World();
	// ������
	void Initialize();
	// �X�V
	void Update();
	// �`��
	void Draw(const int laneCount = 0, const int playerLane = 0) const;
	// �N���A
	void Clear();
	// �C�x���g���X�i�[�̒ǉ�
	void AddEventMessageListener(std::function<void(EventMessage, void*)> listener);

	// �A�N�^�[�̎擾
	WorldActor GetWorldActor() const;

	/* ���[���h�C���^�[�t�F�[�X */
	// �ǉ�
	virtual void Add(ACTOR_ID id, ActorPtr actor);
	virtual void SetTarget(Actor* tgt) {
		targetAct_ = tgt;
	}
	// �I����Ă��邩�H
	virtual bool IsEnd() const;
	// �Փ˔���ݒ�
	virtual void SetCollideSelect(ActorPtr thisActor, ACTOR_ID otherID, COL_ID colID);
	// ���b�Z�[�W�̑��M
	virtual void sendMessage(EventMessage message, void* param = nullptr) override;
	// �w��O���[�v�̏���
	virtual void EachActor(ACTOR_ID id, std::function<void(Actor&)> func) override;
	// �w��O���[�v�̏���
	virtual void EachActor(ACTOR_ID id, std::function<void(const Actor&)> func) const override;
	// �w��O���[�v�̃A�N�^�[�����擾
	virtual int Count(ACTOR_ID id) const override;

	// ����A�N�^�[�Z�b�g
	virtual void PushStackActor(ActorPtr actor);
	// ����A�N�^�[�|�b�v
	virtual void PopStackActor();

	virtual void inv(const Matrix& mat) override;
	virtual Matrix InitializeInv(Vector2 position) override;

	virtual Matrix GetInv()override {
		return inv_;
	}
	virtual void SetScroolPos(const Vector2& pos) override {
		targetMat_.Translation(Vector3(pos.x, pos.y, 0));
	}
	//���L�f�[�^���X�V����A�ύX���s��Ȃ��l�̈����́A����KeepDatas�̒l��n����
	virtual void SetKeepDatas(KeepDatas data) override {
		keepDatas_ = data;
	}
	//���L�f�[�^���擾����
	virtual KeepDatas GetKeepDatas()const override {
		return keepDatas_;
	}

	virtual KeepDatas& GetCanChangedKeepDatas() override {
		return keepDatas_;
	}
	virtual void ChangeCamMoveMode(int addNum) override{
		isChangeCam_ = true;
		addNum_ = addNum;
	}
	virtual bool GetIsCamChangeMode()const override {
		return isChangeCam_;
	}
	virtual bool isChangeFrame()const {
		return isChangeFrame_;
	}

private:
	void Spring(Vector2 & pos, Vector2 & resPos, Vector2 & velo, float stiffness = 0.1f, float friction = 0.5f, float mass = 2.0f) const
	{
		// �o�l�̐L�ы���v�Z
		Vector2 stretch = (pos - resPos);
		// �o�l�̗͂��v�Z
		Vector2 force = -stiffness * stretch;
		// �����x��ǉ�
		Vector2 acceleration = force / mass;
		// �ړ����x���v�Z
		velo = friction * (velo + acceleration);

		pos = pos + velo;
	}

private:
	//�J�����X�V�p��Update�ֈڍs����
	bool isChangeCam_;
	int addNum_;
	bool isChangeFrame_;
	std::map<bool, std::function<void()>> updateFunctionMap_;

	Matrix inv_;
	Matrix resInv_;
	Matrix targetMat_;
	Actor* targetAct_;
	Vector2 mPrePos;
	Vector2 mCurPos;
	// �A�N�^�[
	WorldActor	actors_;

	Vector2 mVelo;
	Vector2 velo;

	KeepDatas keepDatas_;

	// �󓮍X�V�A�N�^�[�p�X�^�b�N
	std::stack<ActorPtr>	manualStackActor_;
	// �J�����p�X�^�b�N
	std::stack<ActorPtr>	stackCamera_;
};