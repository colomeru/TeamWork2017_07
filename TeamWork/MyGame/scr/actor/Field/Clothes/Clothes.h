#pragma once
#include "../../Actor.h"
#include "../MyGame/scr/game/ID.h"

class Clothes : public Actor
{
public:
	//�R���X�g���N�^
	Clothes(IWorld* world, CLOTHES_ID clothes);
	//���z�f�X�g���N�^
	virtual ~Clothes() = default;
	//�X�V
	virtual void Update() = 0;
	//�`��
	virtual void Draw() const = 0;

	//ID�̎擾
	CLOTHES_ID GetClothesID() const {
		return clothes_ID;
	}

protected:
	//�Փ˂��Ă��邩
	bool isHit_;
	//��ID
	CLOTHES_ID clothes_ID;
};