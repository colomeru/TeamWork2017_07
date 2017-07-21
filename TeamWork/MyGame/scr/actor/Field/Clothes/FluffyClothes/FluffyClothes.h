#pragma once
#include "../Clothes.h"

//�ӂ�ӂ�ȕ�
class FluffyClothes : public Clothes, public std::enable_shared_from_this<FluffyClothes>
{
public:
	//�R���X�g���N�^
	FluffyClothes(
		IWorld* world,
		int laneNum, 
		Vector2 pos,
		float weight,
		std::pair<CLOTHES_ID, SPRITE_ID> ids, 
		const std::map<CuttingState, std::vector<Vector3>>& localPoints, 
		bool is_Pin = false);
	//�f�X�g���N�^
	virtual ~FluffyClothes() override;
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;

};