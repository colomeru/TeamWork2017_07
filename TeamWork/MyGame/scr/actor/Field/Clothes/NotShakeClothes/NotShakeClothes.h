#pragma once
#include "../Clothes.h"
#include <map>

class NotShakeClothes : public Clothes, public std::enable_shared_from_this<NotShakeClothes>
{
public:
	//�R���X�g���N�^
	NotShakeClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, SPRITE_ID spriteId, bool is_Pin = false);
	//�f�X�g���N�^
	virtual ~NotShakeClothes() override;
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�ؒf��Ԃɂ�铖���蔻��̃|�C���g�̐ݒ�
	virtual void SetLocalPoints() override;
private:
	void SetPoints01();
	void SetPoints02();
};