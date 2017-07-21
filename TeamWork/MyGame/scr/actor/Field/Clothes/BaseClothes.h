#pragma once
#include "Clothes.h"

class BaseClothes : public Clothes, public std::enable_shared_from_this<BaseClothes>
{
public:
	//�R���X�g���N�^
	BaseClothes(
		IWorld* world, 
		int laneNum, 
		Vector2 pos, 
		float weight, 
		std::pair<CLOTHES_ID, SPRITE_ID> ids,
		const std::map<CuttingState, std::vector<Vector3>>& localPoints,
		bool is_Pin = false
		);

	//�f�X�g���N�^
	virtual ~BaseClothes() override;
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
};