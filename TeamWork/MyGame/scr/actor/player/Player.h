#pragma once
#include "../Actor.h"
#include<vector>
#include"../../math/MathHelper.h"
#include"../../math/MyFuncionList.h"
#include"../../Def.h"

class Player_Head;

static const Vector2 pHDist = Vector2(32, 32);
static const int headAngleSetter = -2;
static const float defHeadChangeTime = 0.2f;
static const float defHeadLength = 2.f;
static const float defPGravPow = 0.05f;
static const float defGravAddPow = 0.2f;
static const float HeadShootMult = 0.5f;
static const float defSlipCount = 8.f;
static const int defLaneChangeCoolTime_ = 60;
static const int defChainLockCoolTime_ = 10;
//落ちた後のposition加算値
static const float fallAddPosMult = 3.4f;
//落ちるまでの最大時間
static const float defResistTime = 0.5f;
//レーンの本数

enum {
	MODE_FALL = 0,
	MODE_SHOOT = 1,
	MODE_SHOOT_END = 2,
	MODE_BITE = 3,
	MODE_SLIP = 4,
	MODE_RESIST = 5,
};


class Player : public Actor, public std::enable_shared_from_this<Player>
{
private:
	enum LaneChangeType {
		LaneChange_Normal,
		LaneChange_Fall
	};
public:
	//コンストラクタ
	Player(IWorld* world,int maxLaneSize=3,int startLane=1);
	//デストラクタ
	~Player();
	//更新
	virtual void Update() override;
	virtual void FastUpdate() override {
		if (!world_->GetIsCamChangeMode()) {
			int nexLane = world_->GetKeepDatas().nextLane_;
			UpdateLaneNum(nexLane, changeType_);
			world_->GetCanChangedKeepDatas().SetPlayerNextLane(0);
		}

	}
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor&, CollisionParameter colpara) override;
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);
	virtual void LaneChangeFall() override {

		float laneLerpNum = world_->GetKeepDatas().changeLaneLerpPos_;
		laneLerpNum = min(1.f, laneLerpNum);
		int targetNum = world_->GetKeepDatas().playerLane_ - laneNum_ + 2;
		drawAddPos_.y = MathHelper::Lerp(defDrawLineChangePosY[targetNum], defDrawLineChangePosY[targetNum - 1], laneLerpNum) - defDrawLineChangePosY[targetNum];

		if (changeType_ == LaneChangeType::LaneChange_Fall) {
			//drawAddPos_.y = MathHelper::Lerp(defDrawLineChangePosY[targetNum], defDrawLineChangePosY[targetNum - 1], laneLerpNum) - defDrawLineChangePosY[targetNum];
			drawAddPos_.y = drawAddPos_.y * fallAddPosMult;
		}
	}
	bool isLaneChangeFall() const{
		return changeType_ == LaneChangeType::LaneChange_Fall;
	}
	//振り子運動
	void Pendulum(Vector2 fulcrum, float length);
	Vector2 GetHeadPos(int headNum)const {
		return pHeadPoses_[headNum];
	}
	//Headの長さを実際のゲームに反映される値に変換して返す
	float GetHeadLengthChangeToPosMult(int headNum) const {
		return pHeadLength_[headNum] * HeadShootMult;
	}
	void HeadPosUpdate();
	//使用する頭を右隣の物に変更
	void changeHead() {
		//回転した時点でSlip状態を直す
		isSlipped_ = false;
		//pHeadLength_[currentHead_] = defHeadLength*HeadShootMult;
		currentHead_++;
		if (currentHead_ >= (int)pHeads_.size())currentHead_ = 0;
		headChangeTime_ = defHeadChangeTime;
	}
	//使用する頭を左隣の物に変更
	void backChangeHead() {
		//pHeadLength_[currentHead_] = defHeadLength*HeadShootMult;
		currentHead_--;
		if (currentHead_ < 0)currentHead_ = pHeads_.size() - 1;
		headChangeTime_ = -defHeadChangeTime;
	}
	//プレイヤーの開始時の状態設定
	void StartPlayerSet();
	int GetCurHead()const;
	bool GetPHeadDead(int pHeadNum)const {
		return pHeadDead_[pHeadNum];
	}
	void CurHeadBite(const Vector2& target) {
		playerMode_ = MODE_BITE;
		pGrav_ = defPGravPow;
		//角度を求める
		//rot_ = MathHelper::ACos(Vector2::Dot(Vector2::Right, tpos)) *180 / MathHelper::Pi;
		rot_ = 135;
		rot_spd_ = -3.0f;
	}
	void ResurrectHead() {
		for (int i = currentHead_; i < pHeads_.size() + currentHead_; i++) {
			int trgNum = i;
			if (trgNum>=pHeads_.size()) {
				trgNum = trgNum - pHeads_.size();
			}
			if (!pHeadDead_[trgNum])continue;
			pHeadDead_[trgNum] = false;
			break;
		}
	}
	void SetOtherClothesID_(CLOTHES_ID cId) {
		otherClothesID_ = cId;
	}
	//噛み付ける状態かを返す(レジスト含む)
	bool GetIsBiteMode()const {
		return playerMode_==MODE_BITE||playerMode_==MODE_RESIST;
	}
	//噛み付き状態にするかをセット、
	void SetIsBiteMode(bool ismode) {
		int setMode = (ismode) ? MODE_BITE : MODE_SHOOT;
		playerMode_ = setMode;
	}
	void SetMode(int pMode) {
		playerMode_ = pMode;
	}
	//シュート終了の瞬間かどうかを取る
	bool GetIsShootModeEnd()const {
		return playerMode_==2;
	}
	float GetSlipCount()const {
		return slipCount_;
	}
	bool GetIsSlipped()const {
		return playerMode_==MODE_SLIP;
	}
	//Headのレーンを本体のレーンに合わせる
	void SetMyHeadLaneNum(int targetNum);
	void SetAllHeadLaneNum();

	//worldの共有データに自分の情報を代入する
	void worldSetMyDatas();

	//void SetIsCanChangeLane(bool isCanChange) {
	//	if (laneChangeCoolTime_ > 0)return;
	//	laneChangeCoolTime_ = defLaneChangeCoolTime_;
	//	isCanChangeLane_ = isCanChange;
	//}
	void SetNextLane(int addNum,LaneChangeType changeType=LaneChangeType::LaneChange_Normal) {
		if (laneNum_ + addNum > (maxLaneSize_-1) || laneNum_ + addNum<0)return;
		
		changeType_ = changeType;
		world_->ChangeCamMoveMode(addNum);
	}
	void setCurPHeadSPos(const Vector2& sPos);
	void setMaxLaneSize(int size) {
		maxLaneSize_ = size;
	}
	//現在の頭に対して滑るかどうかをセットする
	void curPHeadSlip(bool isSlip);
	//プレイヤーが死んでるか
	bool isPlayerDead()const {
		if(laneNum_==(maxLaneSize_-1)&&position_.y >= WINDOW_HEIGHT)return true;
		//if (position_.y >= WINDOW_HEIGHT)return true;

		for (auto pHD : pHeadDead_) {
			if (!pHD)return false;
		}
		return true;
	}
	void PHeadChanger(int rot = 0) {
		PHeadLengthReset();
		(sign(rot) == 1) ? backChangeHead() : changeHead();
	}
private:
	//入力による動作をまとめる
	void PlayerInputControl();
	//1で左隣の、未入力で右隣のHeadに回転し、長さをリセットする

	void PHeadLengthReset() {
		//長さの補間をリセットする
		chainAddLength_ = 0.f;
		chainAddLengthMath_ =0.f;

		//チェーンのロックをリセットする
		chainLock_ = false;
		for (auto& pHL : pHeadLength_) {
			pHL = 2.f;
		}
	}
	//チェーンの長さを加算する
	void CurPHeadLengPlus(float addPow);

	void UpdateLaneNum(int updateNum, LaneChangeType changeType = LaneChangeType::LaneChange_Normal) {
		if (updateNum == 0)return;
		if (laneNum_+updateNum > (maxLaneSize_-1) || laneNum_ + updateNum<0)return;
		
		//次のレーンに対応したベクトルを作成し、重力の加算をリセットする
		Vector2 nextVel_;
		if (updateNum < 0) {
			nextVel_ = Vector2(0, -35.f);
			pGrav_ = 0.f;
			position_.y += defDrawLinePosY[2]- defDrawLinePosY[1];
		}
		else if (updateNum > 0) {
			nextVel_ = pendulumVect_/2;
			pGrav_ *= 0.3f;
			if (changeType == LaneChangeType::LaneChange_Fall) position_.y += (defDrawLinePosY[0] - defDrawLinePosY[1])*fallAddPosMult;
			else position_.y += defDrawLinePosY[0] - defDrawLinePosY[1];
		}
		laneNum_ += updateNum;
		//レーン最大範囲を超えたらVectの補正を行わない

		laneNum_ = MathHelper::Clamp(laneNum_, 0, (maxLaneSize_ - 1));

		//velocity_ = nextVel_;
		pendulumVect_ = nextVel_;

		playerMode_ = MODE_FALL;
		//頭の長さをリセット
		PHeadChanger();

		worldSetMyDatas();
	}
//プレイヤーの状態に応じた更新
private:
	void FallUpdate();
	void ShootUpdate();
	void ShootEndUpdate();
	void BiteUpdate();
	void SlipUpdate();
	void ResistUpdate();
private:
	using PHeadPtr = std::shared_ptr<Player_Head>;
	//ステージの最大レーン数
	int maxLaneSize_;

	float spdLimit;

	//衝突しているか
	bool isHit_;

	//振り子関連
	//支点座標
	Vector2 fulcrum_;
	//角度
	float rot_;
	//角速度
	float rot_spd_;
	//紐の長さ
	float length_;
	//重力加速度
	float gravity_;
	//振り子移動の摩擦
	float friction;
	//振り子移動によるベクトルを作り出す
	Vector2 pendulumVect_;

	std::vector<PHeadPtr> pHeads_;
	std::vector<Vector2> pHeadPoses_;
	//各Headのチェーンの長さ
	std::vector<float> pHeadLength_;
	//首が外れた時に追加で伸びるチェーンの長さ
	float chainAddLength_;
	//首を追加で伸ばす際の補間値
	float chainAddLengthMath_;
	std::vector<bool> pHeadDead_;

	int currentHead_;

	float headChangeTime_;

	float pGrav_;

	float rotTimer;

	float maxChainLength_;

	float slipCount_;

	bool isSlipped_;

	bool isPlayerFallLane_;

	//滑り落ちるまでの時間
	float slipResistTime_;


	//0=滞空 1=発射時 2=発射終了 3=噛み付き 4=滑り落ち
	int playerMode_;
	//キーロック
	bool isNextPushKey_;
	float jumpShotPower_;
	bool chainLock_;

	//レーン移動したフレームで噛む対象があるかを調べる
	bool isNextLaneBite_;

	int laneAddNum_;
	//bool isCanChangeLane_;

	int chainLockCoolTime_;
	
	//Head回転をロックする(スティックを0に戻す事でリセット)
	bool isCanNextHeadRot;

	CLOTHES_ID otherClothesID_;
	LaneChangeType changeType_;
	//滑る時間の倍数(服毎)
	std::map<CLOTHES_ID, float> slipCountMult_;

	std::map<int, std::function<void()>> updateFunctionMap_;
};