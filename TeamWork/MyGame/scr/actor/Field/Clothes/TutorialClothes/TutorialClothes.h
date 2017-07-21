#pragma once
#include "../Clothes.h"

class TutorialClothes : public Clothes, public std::enable_shared_from_this<TutorialClothes>
{
public:
	//�R���X�g���N�^
	TutorialClothes(
		IWorld* world,
		int laneNum, 
		Vector2 pos,
		float weight,
		std::pair<CLOTHES_ID, SPRITE_ID> ids,
		const CLPoints& localPoints,
		bool is_Pin = false);
	//�f�X�g���N�^
	virtual ~TutorialClothes() override;
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;

private:
	//�����蔻��͈̔͂�`��
	void DrawRange() const;
};