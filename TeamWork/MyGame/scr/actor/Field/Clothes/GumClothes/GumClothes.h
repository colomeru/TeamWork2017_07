#pragma once
#include "../Clothes.h"
#include "../MyGame/scr/actor/player/Player_Head.h"

//�S���̕�
class GumClothes : public Clothes, public std::enable_shared_from_this<GumClothes>
{
public:
	//�R���X�g���N�^
	GumClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, bool is_Pin = false);
	//�f�X�g���N�^
	~GumClothes();
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);

private:
	//Player* player_;
	//Player_Head* player_Head_;
};