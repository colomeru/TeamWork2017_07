#pragma once
#include "../Clothes.h"

//�S�[��
class GoalClothes: public Clothes, public std::enable_shared_from_this<GoalClothes>
{
public:
	//�R���X�g���N�^
	GoalClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos);
	//�f�X�g���N�^
	~GoalClothes();
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor* other, CollisionParameter colpara);
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);

	bool GetGoalFlag() {
		return isHit_;
	}

private:
};