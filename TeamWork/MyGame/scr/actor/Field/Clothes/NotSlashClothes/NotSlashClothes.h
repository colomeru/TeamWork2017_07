#pragma once
#include "../Clothes.h"

class NotSlashClothes : public Clothes, public std::enable_shared_from_this<NotSlashClothes>
{
public:
	//�R���X�g���N�^
	NotSlashClothes(
		IWorld* world,
		int laneNum,
		Vector2 pos,
		float weight,
		std::pair<CLOTHES_ID, SPRITE_ID> ids,
		std::map<CuttingState, std::vector<Vector3>> localPoints,
		bool is_Pin = false);
	//�f�X�g���N�^
	virtual ~NotSlashClothes() override;
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;

};