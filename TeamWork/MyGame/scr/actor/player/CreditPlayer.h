#pragma once
#include "../Actor.h"
#include <vector>
#include "../../math/MathHelper.h"
#include "../../math/MyFuncionList.h"
#include "../../Def.h"
#include "../../graphic/DrawPos.h"
#include "Player.h"

class Player_Head;
class Player_Sword;

class CreditPlayer : public Player
{
private:
	enum LaneChangeType {
		LaneChange_Normal,
		LaneChange_Fall
	};
public:
	//�R���X�g���N�^
	CreditPlayer(IWorld* world, int maxLaneSize = 3, int startLane = 1);
	//�f�X�g���N�^
	~CreditPlayer();
	//���b�Z�[�W����
	virtual void OnMessage(EventMessage message, void* param);
	//�d�̓��Z�b�g
	void GravityReset();
	//1�ō��ׂ́A�����͂ŉE�ׂ�Head�ɉ�]���A���������Z�b�g����
	void PHeadLengthReset();
	//���S��������S�ĉ�
	void AllResurrectHead();
	//���d�U��q�̏�����
	void MultipleInit(float Length, const Vector2& fPos, float rot, float radius);

private:
	//��Ԃ��Ƃ̍X�V
	void FallUpdate();	//������
	void ShootUpdate();	//�V���[�g��
	void BiteUpdate();	//���ݒ�

private:
	bool operatre_; //����\���H
};

