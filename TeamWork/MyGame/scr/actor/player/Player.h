#pragma once
#include "../Actor.h"
#include<vector>
#include"../../math/MathHelper.h"
class Player_Head;

static const Vector2 pHDist = Vector2(32, 32);
static const int headAngleSetter = -2;
static const float defHeadChangeTime = 0.2f;
static const float defHeadLength = 2.f;
static const float defPGravPow = 0.07f;
static const float defGravAddPow = 0.0f;
static const float HeadShootMult = 0.5f;
static const float defSlipCount = 1.f;
class Player : public Actor, public std::enable_shared_from_this<Player>
{
public:
	//コンストラクタ
	Player(IWorld* world);
	//デストラクタ
	~Player();
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor&, CollisionParameter colpara) override;
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);
	//振り子運動
	void Pendulum(Vector2 fulcrum, float length);
	Vector2 GetHeadPos(int headNum)const {
		return pHeadPoses_[headNum];
	}
	float GetHeadLength(int headNum) const {
		return pHeadLength_[headNum];
	}
	float GetHeadLengthChangeToPosMult(int headNum) const {
		return pHeadLength_[headNum] * HeadShootMult;
	}
	void HeadPosUpdate();
	void changeHead() {
		//回転した時点でSlip状態を直す
		isSlipped_ = false;
		//pHeadLength_[currentHead_] = defHeadLength*HeadShootMult;
		currentHead_++;
		if (currentHead_ >= 8)currentHead_ = 0;
		headChangeTime_ = defHeadChangeTime;
	}
	void backChangeHead() {
		//pHeadLength_[currentHead_] = defHeadLength*HeadShootMult;
		currentHead_--;
		if (currentHead_ < 0)currentHead_ = pHeads_.size() - 1;
		headChangeTime_ = -defHeadChangeTime;
	}

	int GetCurHead()const {
		return currentHead_;
	}
	
	void CurHeadBite(const Vector2& target) {
		pGrav_ = defPGravPow;
		Vector2 tpos = target - position_;
		//角度を求める
		//rot_ = MathHelper::ACos(Vector2::Dot(Vector2::Right, tpos)) *180 / MathHelper::Pi;
		rot_ = 135;
		rot_spd_ = -3.0f;
	}
	//噛み付ける状態かを返す
	bool GetIsBiteMode()const {
		return isBiteMode_;
	}
	float GetSlipCount()const {
		return slipCount_;
	}
	bool GetIsSlipped()const {
		return isSlipped_;
	}
	//Headのレーンを本体のレーンに合わせる
	void SetMyHeadLaneNum(int targetNum);
	void SetAllHeadLaneNum();

	//worldの共有データに自分の情報を代入する
	void worldSetMyDatas();
private:
	//入力による動作をまとめる
	void PlayerInputControl();

	void PHeadLengthReset() {
		for (auto& pHL : pHeadLength_) {
			pHL = 2.f;
		}
	}
	void CurPHeadLengPlus(float addPow);

	void UpdateLaneNum(int updateNum) {
		laneNum_+=updateNum;
		laneNum_ = MathHelper::Clamp(laneNum_, 0, 2);

		worldSetMyDatas();
	}
private:
	using PHeadPtr = std::shared_ptr<Player_Head>;
	
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
	//振り子移動によるベクトルを作り出す
	Vector2 pendulumVect_;

	std::vector<PHeadPtr> pHeads_;
	std::vector<Vector2> pHeadPoses_;
	//各Headのチェーンの長さ
	std::vector<float> pHeadLength_;

	int currentHead_;

	float headChangeTime_;

	float pGrav_;

	float rotTimer;

	float maxChainLength_;

	float slipCount_;
	bool isSlipped_;

	bool isBiteMode_;

	bool isShootMode_;
	bool isNextPushKey_;
	float jumpShotPower_;
};