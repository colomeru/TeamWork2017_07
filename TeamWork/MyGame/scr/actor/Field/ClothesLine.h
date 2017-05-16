#pragma once
#include "../Actor.h"

static const int STAGE_TIP_SIZE = 128;

class ClothesLine : public Actor, public std::enable_shared_from_this<Actor>
{
public:
	//�R���X�g���N�^
	ClothesLine(IWorld* world, int laneNum,int blockSize, Vector2 pos);
	//�f�X�g���N�^
	~ClothesLine();
	//�X�V
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void StartOnlyUpdate()override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor* other, CollisionParameter colpara);
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);

private:
	int blockSize_;
};