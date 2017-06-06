#pragma once
#include "../MyGame/scr/actor/Actor.h"
#include "../MyGame/scr/actor/player/Player.h"

class HairballGenerator : public Actor
{
public:
	// �R���X�g���N�^
	HairballGenerator(IWorld* world, int lane_num, Vector2 pos, int hairballCnt);
	// �f�X�g���N�^
	~HairballGenerator();
	// �X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	// �󓮍X�V
	virtual void OnUpdate() override;
	// �����蔻�菈��
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	// ���b�Z�[�W����
	virtual void OnMessage(EventMessage message, void* param) override;
	
private:
	void GenerateHairball();

private:
	Player* player_;

	int generate_Count_;
	int defGenerate_Count_;
	bool is_Generate_;

};