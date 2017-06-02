#pragma once
#include "../Clothes.h"

//�ӂ�ӂ�ȕ�
class FluffyClothes : public Clothes, public std::enable_shared_from_this<FluffyClothes>
{
public:
	//�R���X�g���N�^
	FluffyClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, bool is_Pin = false);
	//�f�X�g���N�^
	~FluffyClothes();
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;

private:

};