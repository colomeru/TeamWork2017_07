#pragma once
#include "../Actor.h"
#include"../player/Player.h"

class Player_Head : public Actor, public std::enable_shared_from_this<Player_Head>
{
public:
	//�R���X�g���N�^
	Player_Head(IWorld* world,Player* targetP,Vector2 pos,int myNumber);
	//�f�X�g���N�^
	~Player_Head();
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor& other, CollisionParameter colpara)override;
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);
	virtual bool CamMoveUpdate();
	virtual void CamMoveUp()override {}
	virtual void CamMoveDown() override;

	virtual void LaneChangeFall() override;
	void addPos(const Vector2& addpos);
	void UpdatePos();
	bool ResurrectHead();
	void StartPlayerHeadBite();

	bool getIsHit()const;
	bool getIsBitePoint()const;
	bool getIsCurrentHead()const;
	void SetPosAddVect(const Vector2& posAV);
	//�������藎���邩�ǂ������Z�b�g����
	void setIsBiteSlipWind(bool isSlip);
	Player* GetPlayerPointer() const;
	void CreateFatigueEffect();
	void SetBiteSprite();

private:
	float MathHeadRotation_Bite()const;
	float MathHeadRotation_Fall()const;

private:
	//�Փ˂��Ă��邩
	bool isHit_;
	//�Փˈʒu�ɂ��āA�����ݕt���Ă��Ȃ���
	bool isBitePoint_;
	bool isBiteSlipWind_;
	Player* player_;

	//Vector2 stopPos_;

	int myNumber_;

	float biteSpriteTimer_;

	bool isHitOnce;

	Vector2 posAddVect_;

	int fatigueCheckColor_;

	bool isAlreadyCreateSplash_;

};