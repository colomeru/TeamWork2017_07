#pragma once
#include "../Actor.h"
#include"../player/Player.h"

class Player_Sword : public Actor, public std::enable_shared_from_this<Player_Sword>
{
public:
	//�R���X�g���N�^
	Player_Sword(IWorld* world, Player* targetP, Vector2 pos);
	//�f�X�g���N�^
	~Player_Sword();
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;
	//�Փˎ��̃A�N�V����
	virtual void OnCollide(Actor& other, CollisionParameter colpara)override;
	//���b�Z�[�W�擾
	virtual void OnMessage(EventMessage message, void* param);
	void SetSwordVel(const Vector2& vel);
	virtual bool CamMoveUpdate();
	virtual void CamMoveUp()override;
	virtual void CamMoveDown() override;

	virtual void LaneChangeFall() override;

	Vector2 GetSwordStartPos() const;
	Vector2 GetSwordEndPos() const;
	void SetUseSword(bool useSword);
	bool GetUseSword()const;

private:
	Player* player_;

	Vector2 swordStartPos_;
	Vector2 swordEndPos_;

	bool useSword_;

};