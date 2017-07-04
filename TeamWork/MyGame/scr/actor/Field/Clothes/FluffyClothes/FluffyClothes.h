#pragma once
#include "../Clothes.h"
#include "FluffyPointManager\FluffyPointManager.h"

//�ӂ�ӂ�ȕ�
class FluffyClothes : public Clothes, public std::enable_shared_from_this<FluffyClothes>
{
public:
	//�R���X�g���N�^
	FluffyClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, SPRITE_ID spriteId, bool is_Pin = false);
	//�f�X�g���N�^
	virtual ~FluffyClothes() override;
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�ؒf��Ԃɂ�铖���蔻��̃|�C���g�̐ݒ�
	virtual void SetLocalPoints() override;
	//���̓����蔻��̐ݒ�
	virtual void SetPointsUpdate() override;

private:
	//�ӂ�ӂ핞�̃��[�J���ȓ����蔻��̈ʒu
	std::map<CuttingState, std::vector<Vector3>> fluffyLocalPoints_;
	//�����蔻��}�l�[�W���[
	FluffyPointManager pointManager_;
};