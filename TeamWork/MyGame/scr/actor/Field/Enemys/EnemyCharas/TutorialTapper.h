#pragma once
#include"../Enemys.h"
#include "../MyGame/scr/actor/player/Player.h"
#include"CharacterAnmManager.h"
#include"ClothesTapper.h"

class TutorialTapper : public Enemys, public std::enable_shared_from_this<TutorialTapper>
{
public:
	//�R���X�g���N�^
	TutorialTapper(IWorld* world, int laneNum, Vector2 pos);
	//�f�X�g���N�^
	~TutorialTapper();
	virtual void FastUpdate()override {
		isUpdate_ = true;
	}
	virtual bool CamMoveUpdate() override {
		if (0 > world_->GetKeepDatas().nextLane_) {
			CamMoveUp();
		}
		else {
			CamMoveDown();
		}
		return true;
	}
	virtual void LateUpdate() {
		laneNum_ = world_->GetKeepDatas().playerLane_;
	}
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

	Vector2 GetShiftPos()const {
		return shiftPos_;
	}
private:
	void SetNextTapPos(const Vector2& pos = Vector2::Zero);
	void PlayTap();
	void ToTapMode();
	void ToMoveMode();
	void ToIdleMode();
	void ToDeadMode();
private:
	void MoveUpdate();
	void TapUpdate();
	void IdleUpdate();
	void DeadUpdate();
private:
	Player* player_;
	Actor* player_Head_;
	Vector2 basePos_;
	Vector2 targetPos_;
	float timeCount_;

	Vector2 shiftPos_;

	CharacterAnmManager anmManager_;
	CharacterAnmManager anmManager2_;

	int updateMode_;
	SPRITE_ID spriteID_;
	std::map<int, std::function<void()>> updateFunctionMap_;

};