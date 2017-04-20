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

	bool getIsHit()const{
		return isHit_;
	}
	bool getIsBitePoint()const{
		return isBitePoint_;
	}
private:
	//�Փ˂��Ă��邩
	bool isHit_;
	//�Փˈʒu�ɂ��āA�����ݕt���Ă��Ȃ���
	bool isBitePoint_;
	Player* player_;

	Vector2 stopPos_;

	int myNumber_;

	bool isHitOnce;
};