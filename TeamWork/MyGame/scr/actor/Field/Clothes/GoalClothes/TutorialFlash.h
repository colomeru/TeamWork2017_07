#pragma once
#include "../MyGame/scr/actor/Actor.h"

//�`���[�g���A���p�̌�
class TutorialFlash : public Actor, public std::enable_shared_from_this<Actor>
{
public:
	//�R���X�g���N�^
	TutorialFlash(IWorld* world, int laneNum, Vector2 pos, Actor* clothes);
	//�f�X�g���N�^
	virtual ~TutorialFlash() override;
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;

private:
};