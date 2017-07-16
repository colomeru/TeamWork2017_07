#pragma once
#include "../Clothes.h"
#include <map>

class NotShakeClothes : public Clothes, public std::enable_shared_from_this<NotShakeClothes>
{
public:
	//�R���X�g���N�^
	NotShakeClothes(
		IWorld* world, 
		int laneNum, 
		Vector2 pos, 
		float weight, 
		std::pair<CLOTHES_ID, SPRITE_ID> ids,
		std::map<CuttingState, std::vector<Vector3>> localPoints,
		bool is_Pin = false);
	//�f�X�g���N�^
	virtual ~NotShakeClothes() override;
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
};