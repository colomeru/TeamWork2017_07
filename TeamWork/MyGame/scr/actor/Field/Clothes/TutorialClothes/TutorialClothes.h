#pragma once
#include "../Clothes.h"
#include "../BasePointManager\BasePointManager.h"

class TutorialClothes : public Clothes, public std::enable_shared_from_this<TutorialClothes>
{
public:
	//�R���X�g���N�^
	TutorialClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, SPRITE_ID spriteId, bool is_Pin = false);
	//�f�X�g���N�^
	virtual ~TutorialClothes() override;
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�ؒf��Ԃɂ�铖���蔻��̃|�C���g�̐ݒ�
	virtual void SetLocalPoints() override;

private:
	//�����蔻��͈̔͂�`��
	void DrawRange() const;
private:
	BasePointManager pointManager_;
};