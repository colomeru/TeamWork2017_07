#pragma once
#include "Clothes.h"

class TestClothes : public Clothes, public std::enable_shared_from_this<TestClothes>
{
public:
	//�R���X�g���N�^
	TestClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, bool is_Pin = false);
	//�f�X�g���N�^
	~TestClothes();
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);

private:
};