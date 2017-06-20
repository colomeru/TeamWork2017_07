#pragma once
#include "../Clothes.h"
#include <map>

class NotSlashClothes : public Clothes, public std::enable_shared_from_this<NotSlashClothes>
{
public:
	//�R���X�g���N�^
	NotSlashClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, SPRITE_ID spriteId, bool is_Pin = false);
	//�f�X�g���N�^
	virtual ~NotSlashClothes() override;
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
	void SetPoints01();
	void SetPoints02();

private:
};