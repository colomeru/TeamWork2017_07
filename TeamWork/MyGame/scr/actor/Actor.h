#pragma once
#include "ActorParameter.h"
#include "ActorPtr.h"
#include "../collision/CollisionFunction.h"
#include "../collision/CollisionParameter.h"
#include "../game/EventMessage.h"
#include "../game/ID.h"
#include "../math/Quaternion.h"
#include "../world/IWorld.h"
#include <functional>
#include <map>
#include"../math/Vector3.h"
#include"../world/World.h"
#include"../Def.h"

static const float alphaSetter[2] = { 1.f,0.5f };
//�X�V�y�є���̐����͈́A0=�X�V�͈͂̍��[,1=�X�V�͈͂̉E�[,2=����͈͂̍��[,3=����͈͂̉E�[
static const int cutSize[4] = { 600,2200 ,400,800};
class Actor
{
public:
	// �R���X�g���N�^
	Actor(IWorld* world, Actor* parent = nullptr);
	// ���z�f�X�g���N�^
	virtual ~Actor();
	// �X�V
	virtual void Update() = 0;
	// �`��
	void BaseDraw()const {
		if (!isDraw_)return;
		else
		{
			int drawLane = laneNum_ - world_->GetKeepDatas().playerLane_;
			drawLane = MathHelper::Abs(drawLane);
			Draw();

		}
	}
	virtual void Draw() const = 0;
	// ���S���Ă��邩�H
	virtual bool isDead() const { return parameter_.isDead; }
	// ���S����
	void Dead() { parameter_.isDead = true; };

	// �Փ˔���
	void Collide(COL_ID id, Actor& other);
	// �p�����[�^�̎擾
	ActorParameter GetParameter() const;
	// �|�[�Y�̎擾
	Matrix GetPose() const;
	// �|�[�Y�̐ݒ�
	virtual void SetPose(const Matrix& mat);
	// �󓮍X�V
	virtual void OnUpdate();

	void UpdateList();
	virtual void FastUpdate();
	Vector2 GetDrawAddPos()const;
	void FastComUpdate();
	virtual void LateUpdate();
	void CommonUpdate();
	void LateComUpdate();
	//���[���ړ�������̃A�b�v�f�[�g�Avirtual�����APlayer�ȊO��override���Ȃ��悤�ɂ��鎖
	virtual bool CamMoveUpdate();
	virtual void CamMoveOnlyUpdate();
	virtual void StartOnlyUpdate();
	virtual void StartOnlyLateUpdate();
	bool StartModeUpdate();
	virtual void CamMoveUp();
	virtual void CamMoveDown();
	Vector2 GetVelocity()const;
	Vector2 GetDrawPosVect(const Vector2& pos)const;
	//lanenum�����R�Ɏw�肵��drawpos���Z�o
	Vector2 GetFreeActorDrawPos(const Vector2& pos, int laneNum) const;

	// �����擾
	Actor* GetActor() const;
	// �e�擾
	Actor* GetParent() const;

	Vector2 GetPosition() const;
	Vector2 GetDrawPos()const;
	Vector2 GetPrevPosition() const;
	float GetAngle() const;
	int GetLaneNum() const;
	bool GetIsUpdate()const;
	bool GetIsDraw()const;
	void SetLaneNum(int laneNum);
	// ���b�Z�[�W����
	void handleMessage(EventMessage message, void* param);
	void DrawUpdate();
protected:
	// �����蔻�菈��
	virtual void OnCollide(Actor& other, CollisionParameter colpara);
	// ���b�Z�[�W����
	virtual void OnMessage(EventMessage message, void* param);
	//�����鎞�̃��[���ύX
	virtual void LaneChangeFall();
private:
	/* �R�s�[�֎~ */
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

protected:
	//�X�V�A����A�`����s�����ǂ���
	bool isUpdate_;
	bool isCheckCol_;
	//���g���`�悷��摜��ID
	SPRITE_ID spriteId_;
	// ���[���h
	IWorld*				world_;
	// �R���W�����t�@���N�V����
	CollisionFunction	colFunc_;
	// �e
	Actor*				parent_;

	// �p�����[�^
	ActorParameter	parameter_;
	// �ʒu
	Vector2			position_;
	//�O�t���[���̈ʒu
	Vector2			prevPosition_;

	//�e�A�N�^�[�̕`��ʒu���v�Z�����l
	Vector2			drawPos_;
	//���[���ύX����drawpos�ɉ��Z����l
	Vector2			drawAddPos_;
	bool isDraw_;
	// �ړ���
	Vector2			velocity_;
	//���g���������Ă��郌�[���̔ԍ�
	int				laneNum_;
	//���g�̊p�x
	float			angle_;
	
	// �t�@���N�V�����}�b�v
	std::map<COL_ID, std::function<CollisionParameter(const Actor&, const Actor&)>> colFuncMap_;
};