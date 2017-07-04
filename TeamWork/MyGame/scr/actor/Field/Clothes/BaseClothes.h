#pragma once
#include "Clothes.h"
#include "BasePointManager\BasePointManager.h"

class BaseClothes : public Clothes, public std::enable_shared_from_this<BaseClothes>
{
public:
	//�R���X�g���N�^
	BaseClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, SPRITE_ID spriteId, bool is_Pin = false);
	//�f�X�g���N�^
	virtual ~BaseClothes() override;
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//���̓����蔻��̐ݒ�
	virtual void SetPointsUpdate() override;
	//�ؒf��Ԃɂ�铖���蔻��̃|�C���g�̐ݒ�
	virtual void SetLocalPoints() override;

private:
	std::map<CuttingState, std::vector<Vector3>> baseLocalPoints_;
	BasePointManager pointManager_;
};