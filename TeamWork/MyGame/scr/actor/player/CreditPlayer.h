#pragma once
#include "../Actor.h"
#include<vector>
#include"../../math/MathHelper.h"
#include"../../math/MyFuncionList.h"
#include"../../Def.h"
#include"../../graphic/DrawPos.h"
#include "../../scene/Credit2Scene.h"

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
	//�X�V
	virtual void Update() override;
	//�ŏ��̍X�V
	virtual void FastUpdate() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor&, CollisionParameter colpara) override;
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);
	//
	virtual void CamMoveUp()override;
	//
	virtual void CamMoveDown() override;
	//
	virtual void LaneChangeFall() override;
	//�d�̓��Z�b�g
	void GravityReset();
	//1�ō��ׂ́A�����͂ŉE�ׂ�Head�ɉ�]���A���������Z�b�g����
	void PHeadLengthReset();
	//���S��������S�ĉ�
	void AllResurrectHead();
	//
	void SetBiteSprite();
	//���d�U��q�̏�����
	void MultipleInit(float Length, const Vector2& fPos, float rot, float radius);
	//
	void SetPosition(Vector2 position);
	//�{�̂̍��W��Ԃ�
	Vector2 GetPlayerPosition();

private:
	void FallUpdate();
	//
	void ShootUpdate();
	//
	void BiteUpdate();

private:
	bool operatre_; //����\���H
};

