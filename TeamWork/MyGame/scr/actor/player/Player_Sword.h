#pragma once
#include "../Actor.h"
#include"../player/Player.h"

class Player_Sword : public Actor, public std::enable_shared_from_this<Player_Sword>
{
public:
	//コンストラクタ
	Player_Sword(IWorld* world, Player* targetP, Vector2 pos);
	//デストラクタ
	~Player_Sword();
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor& other, CollisionParameter colpara)override;
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);
	void SetSwordVel(const Vector2& vel) {
		position_ = player_->GetPosition();
		swordEndPos_ = position_+(vel*(float)Sprite::GetInstance().GetSize(SPRITE_ID::SWORD_SPRITE).y);
		
		swordStartPos_= position_ + (swordEndPos_ - position_).Normalize()*128.f;
		
	}
	virtual bool CamMoveUpdate() {
		if (world_->GetKeepDatas().nextLane_ < 0) {
			CamMoveUp();
		}
		else {
			CamMoveDown();
		}
		return true;
	}
	virtual void CamMoveUp()override {
	}
	virtual void CamMoveDown() override {
		if (!player_->isLaneChangeFall()) {
			return;
		}
		LaneChangeFall();
		drawPos_ = GetDrawPosVect(position_);

	}

	virtual void LaneChangeFall() override {
		float laneLerpNum = world_->GetKeepDatas().changeLaneLerpPos_;
		laneLerpNum = min(1.f, laneLerpNum);
		int targetNum = world_->GetKeepDatas().playerLane_ - laneNum_ + 2;
		drawAddPos_.y = MathHelper::Lerp(defDrawLineChangePosY[targetNum], defDrawLineChangePosY[targetNum - 1], laneLerpNum) - defDrawLineChangePosY[targetNum];

		if (player_->isLaneChangeFall()) {
			drawAddPos_.y = drawAddPos_.y * fallAddPosMult;
		}
	}

	Vector2 GetSwordStartPos() const {
		return swordStartPos_;
	}
	Vector2 GetSwordEndPos() const {
		return swordEndPos_;
	}
	void SetUseSword(bool useSword) {
		useSword_ = useSword;
	}
	bool GetUseSword()const {
		return useSword_;
	}

private:
	Player* player_;

	Vector2 swordStartPos_;
	Vector2 swordEndPos_;

	bool useSword_;

	friend class PlayerModify;
};