#pragma once
#include"../Enemys.h"
#include "../MyGame/scr/actor/player/Player.h"

enum {

	MODE_MOVE = 0,
	MODE_TAP = 1,
	MODE_IDLE=2

};

class ClothesTapper : public Enemys, public std::enable_shared_from_this<ClothesTapper>
{
public:
	//�R���X�g���N�^
	ClothesTapper(IWorld* world, int laneNum, Vector2 pos);
	//�f�X�g���N�^
	~ClothesTapper();
	virtual void FastUpdate()override {
		isUpdate_ = true;
	}

	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);

private:
	void SetNextTapPos(const Vector2& pos=Vector2::Zero);
	void PlayTap();
	void ToTapMode();
	void ToMoveMode();
	void ToIdleMode();

private:
	void MoveUpdate();
	void TapUpdate();
	void IdleUpdate();
private:
	Player* player_;
	Actor* player_Head_;
	Vector2 basePos_;
	Vector2 targetPos_;
	float timeCount_;
	
	int updateMode_;
	SPRITE_ID spriteID_;
	std::map<int, std::function<void()>> updateFunctionMap_;

};