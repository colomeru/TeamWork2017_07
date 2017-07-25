#pragma once
#include "../../../actor/player/CreditPlayer.h"
#include "../../../actor/Field/Clothes/Clothes.h"
#include "../../../actor/Field/Enemys/EnemyCharas/CharacterAnmManager.h"

class CreditPostText : public Clothes/*,std::enable_shared_from_this<CreditText>*/ {
public:
	//�R���X�g���N�^
	CreditPostText(IWorld* world, CLOTHES_ID id, SPRITE_ID sprite, int laneNum, Vector2 position, Vector2 size, int frame);
	//�f�X�g���N�^
	~CreditPostText();
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�Փ˔���
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	// ���b�Z�[�W����
	virtual void OnMessage(EventMessage message, void* param) override;

private:
	CreditPlayer* cPlayer_;
	int frame_;	//�t���[����
	float f2;	//�O�̒l

	CharacterAnmManager anmManager_; //�A�j���[�V�����}�l�[�W���[
};
