#pragma once
#include "../Actor.h"
#include"../player/Player.h"

class Player_Head : public Actor, public std::enable_shared_from_this<Player_Head>
{
public:
	//コンストラクタ
	Player_Head(IWorld* world,Player* targetP,Vector2 pos,int myNumber);
	//デストラクタ
	~Player_Head();
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
		//drawPos_ = GetDrawPosVect(position_);

	}

	virtual void LaneChangeFall() override {
		float laneLerpNum = world_->GetKeepDatas().changeLaneLerpPos_;
		laneLerpNum = min(1.f, laneLerpNum);
		int targetNum = world_->GetKeepDatas().playerLane_ - laneNum_ + 2;
		drawAddPos_.y = MathHelper::Lerp(defDrawLineChangePosY[targetNum], defDrawLineChangePosY[targetNum - 1], laneLerpNum) - defDrawLineChangePosY[targetNum];

		if (player_->isLaneChangeFall()) {
			//drawAddPos_.y = MathHelper::Lerp(defDrawLineChangePosY[targetNum], defDrawLineChangePosY[targetNum - 1], laneLerpNum) - defDrawLineChangePosY[targetNum];
			drawAddPos_.y = drawAddPos_.y * fallAddPosMult;
		}
	}
	void addPos(const Vector2& addpos) {
		position_ += addpos;
	}
	void UpdatePos();
	//virtual void SetOtherClothesType(CLOTHES_ID type) override{
	//	//現在使用している頭の時のみ当たった服の種類をセット
	//	if (!getIsCurrentHead())return;
	//
	//	player_->SetOtherClothesID_(type);
	//}

	void StartPlayerHeadBite() {
		isHit_ = true;
		isBitePoint_ = false;

		auto basePos = player_->GetHeadPos(myNumber_);
		Vector2 vel = basePos - player_->GetPosition();


		Vector2 bPlusLngPos = vel*player_->GetHeadLengthChangeToPosMult(myNumber_);

		position_ = basePos + bPlusLngPos;

		//stopPos_ = position_;
		player_->SetStopPos(position_);
		player_->SetMode(MODE_BITE);

	}

	bool getIsHit()const{
		return isHit_;
	}
	bool getIsBitePoint()const{
		return isBitePoint_;
	}
	bool getIsCurrentHead()const {
		return player_->GetCurHead() == myNumber_;
	}
	void SetPosAddVect(const Vector2& posAV) {
		posAddVect_ = posAV;
	}
	//void setPHeadStopPos(const Vector2& sPos){
	//	stopPos_=sPos;
	//}
	//頭が滑り落ちるかどうかをセットする
	void setIsBiteSlipWind(bool isSlip) {
		isBiteSlipWind_ = isSlip;

	}
	Player* GetPlayerPointer() const{
		return player_;
	}
	void CreateFatigueEffect();
	void SetBiteSprite() {
		biteSpriteTimer_ = 0.15f;
	}
private:
	//衝突しているか
	bool isHit_;
	//衝突位置にいて、かつ噛み付いていないか
	bool isBitePoint_;
	bool isBiteSlipWind_;
	Player* player_;

	//Vector2 stopPos_;

	int myNumber_;

	float biteSpriteTimer_;

	bool isHitOnce;

	Vector2 posAddVect_;

	int fatigueCheckColor_;

	bool isAlreadyCreateSplash_;
};