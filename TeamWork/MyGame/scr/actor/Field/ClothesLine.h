#pragma once
#include "../Actor.h"

static const int STAGE_TIP_SIZE = 128;

class ClothesLine : public Actor, public std::enable_shared_from_this<Actor>
{
public:
	//�R���X�g���N�^
	ClothesLine(IWorld* world, int laneNum,const Vector2& endPos, Vector2 pos);
	//�f�X�g���N�^
	virtual ~ClothesLine() override;
	//�X�V
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void StartOnlyLateUpdate()override;
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

	Vector2 endPos_;

	Vector2 addPos_;
};