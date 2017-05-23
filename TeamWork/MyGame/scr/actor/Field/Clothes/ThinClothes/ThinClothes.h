#pragma once
#include "../Clothes.h"

//�������n�̕�
class ThinClothes : public Clothes, public std::enable_shared_from_this<ThinClothes>
{
public:
	//�R���X�g���N�^
	ThinClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, bool is_Pin = false);
	//�f�X�g���N�^
	~ThinClothes();
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;

private:

};