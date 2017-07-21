#pragma once
#include "../../../Actor.h"

//���ɕt������
class ClothesFeces : public Actor, public std::enable_shared_from_this<Actor>
{
public:
	//�R���X�g���N�^
	ClothesFeces(IWorld* world, int laneNum, Vector2 pos, Actor* clothes);
	//�f�X�g���N�^
	virtual ~ClothesFeces() override;
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;

private:
	Vector2 pos_;
	Vector2 fulcrum_;
};