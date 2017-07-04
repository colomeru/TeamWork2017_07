#pragma once
#include "../Clothes.h"

class StartClothes : public Clothes, public std::enable_shared_from_this<StartClothes>
{
public:
	//�R���X�g���N�^
	StartClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, bool is_Pin = false);
	//�f�X�g���N�^
	virtual ~StartClothes() override;
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�����蔻��̃��[�J�����W�̐ݒ�
	virtual void SetLocalPoints() override;

private:
	Actor* pActor;
	bool isStart_;
};