#pragma once
#include "../../../Actor.h"

//�������镞
class DropClothes : public Actor
{
public:
	// �R���X�g���N�^
	DropClothes(IWorld* world, Vector2 pos, int laneNum, SPRITE_ID spriteID, int drawFrame);
	//�f�X�g���N�^
	virtual ~DropClothes() override;
	// �X�V
	virtual void Update() override;
	// �`��
	virtual void Draw() const override;

private:
	//�`��̃R�}�ԍ�
	int drawFrame_;
	//������ۂ̉�]����
	float turn_;
};

