#pragma once
#include "../../Actor.h"
#include "../MyGame/scr/game/ID.h"
#include "../../player/Player_Head.h"
#include <array>

class Enemys : public Actor
{
public:
	//�R���X�g���N�^
	Enemys(IWorld* world, int laneNum, const Vector2& pos);
	//���z�f�X�g���N�^
	virtual ~Enemys() = default;
	//�X�V
	virtual void Update() = 0;
	//�`��
	virtual void Draw() const = 0;

	// �����蔻�菈��
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	// ���b�Z�[�W����
	virtual void OnMessage(EventMessage message, void* param) override;



public:
	//�R�s�[�֎~
	Enemys(const Enemys& other) = delete;
	Enemys& operator = (const Enemys& other) = delete;

protected:
	//�Փ˂��Ă��邩
	bool isHit_;

};