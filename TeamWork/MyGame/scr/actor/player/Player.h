#pragma once
#include "../Actor.h"
#include<vector>
#include"../../math/MathHelper.h"
#include"../../math/MyFuncionList.h"
#include"../../Def.h"
#include"../../graphic/DrawPos.h"
#include"PlayerNeck\NeckPiecePoint.h"
#include"../../method/MethodTimer.h"
#include"Input/PlayerInputChecker.h"

class Player_Head;
class Player_Sword;

static const Vector2 pHDist = Vector2(32, 32);
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

enum Player_Mode{
	MODE_FALL = 0,
	MODE_SHOOT = 1,
	MODE_SHOOT_END = 2,
	MODE_BITE = 3,
	MODE_SLIP = 4,
	MODE_RESIST = 5,
	MODE_CLEAR = 6,
	MODE_PLAYERDEAD = 7,
	MODE_BACK_SHOOT=8,
};


class Player : public Actor, public std::enable_shared_from_this<Player>
{

protected:
	enum LaneChangeType {
		LaneChange_Normal,
		LaneChange_Fall
	};
public:
	//コンストラクタ
	Player(IWorld* world,int maxLaneSize=3,int startLane=1,const Vector2& position=Vector2(0.f,200.f));
	//デストラクタ
	~Player();
	//更新
	virtual void Update() override;
	//事前更新
	virtual void FastUpdate() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor&, CollisionParameter colpara) override;
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);
	//レーン移動時更新
	virtual bool CamMoveUpdate() {
		if (world_->GetKeepDatas().nextLane_ < 0) {
			CamMoveUp();
		}
		else {
			CamMoveDown();
		}

		return true;
	}
	//上移動(無効化)
	virtual void CamMoveUp()override {
	}

	//下移動
	virtual void CamMoveDown() override{
		if (changeType_ == LaneChangeType::LaneChange_Normal) {
			return;
		}
		LaneChangeFall();
	}
	//落下
	virtual void LaneChangeFall() override;
	//落下によるレーン移動かを調べる
	bool isLaneChangeFall() const{
		return changeType_ == LaneChangeType::LaneChange_Fall;
	}
	//振り子開始
	void StartPendulum();
	//首の根本の位置を調べる
	Vector2 GetHeadPos(int headNum)const {
		return pHeadPoses_[headNum];
	}
	//現在の首の根本の位置を調べる
	Vector2 GetHeadPos()const {
		return pHeadPoses_[currentHead_];
	}
	//Headの長さを実際のゲームに反映される値に変換して返す
	float GetHeadLengthChangeToPosMult(int headNum) const {
		return pHeadLength_[headNum] * HeadShootMult;
	}
	//現在のHeadの首の長さを返す
	Vector2 GetCurrentHeadLength()const;
	//全ての頭の位置を更新する
	void HeadPosUpdate();
	//剣の位置を更新する
	void SwordPosUpdate();
	//使用する頭を右隣の物に変更
	void changeHead() {
		//回転した時点でSlip状態を直す
		currentHead_++;
		if (currentHead_ >= (int)pHeads_.size())currentHead_ = 0;
		headChangeTime_ = defHeadChangeTime;
	}
	//使用する頭を左隣の物に変更
	void backChangeHead() {
		currentHead_--;
		if (currentHead_ < 0)currentHead_ = pHeads_.size() - 1;
		headChangeTime_ = -defHeadChangeTime;
	}
	//プレイヤーの開始時の状態設定
	void StartPlayerSet();
	//現在使われている頭のIDを取得
	int GetCurHead()const;
	//指定IDの頭が死んでいるかを調べる
	bool GetPHeadDead(int pHeadNum)const {
		return pHeadDead_[pHeadNum];
	}
	//噛み付きを開始する
	void CurHeadBite(const Vector2& target);
	//頭を1つ蘇生する
	bool ResurrectHead();
	//掴んでいる服の種類を設定する
	void SetOtherClothesID_(CLOTHES_ID cId) {
		otherClothesID_ = cId;
	}
	//噛み付ける状態かを返す(レジスト含む)
	bool GetIsShootMode()const {
		return playerMode_ == MODE_SHOOT;
	}
	//噛み付き状態or踏ん張り状態かを調べる
	bool GetIsBiteMode()const {
		return playerMode_ == MODE_BITE || playerMode_ == MODE_RESIST;
	}
	//プレイヤーの状態をチェックする、引数のモードと一致していればtrue
	bool PlayerModeChecker(Player_Mode pMode) {
		return playerMode_ == pMode;
	}
	//踏ん張り状態かを調べる
	bool GetIsResistMode()const {
		return playerMode_ == MODE_RESIST;
	}
	//ステージクリア状態かを調べる
	bool GetIsClearMode()const {
		return playerMode_ == MODE_CLEAR;
	}
	//現在生きている頭の数を調べる
	int GetPHeadLiveCount()const {
		int result = 0;
		for (auto i : pHeadDead_) {
			if (!i)result++;
		}
		return result;
	}
	//噛み付き状態にするかをセット、
	void SetIsBiteMode(bool ismode) {
		int setMode = (ismode) ? MODE_BITE : MODE_SHOOT;
		playerMode_ = setMode;
	}
	//プレイヤーの状態を変更する
	void SetMode(Player_Mode pMode, bool isPlaySE=true);
	//シュート終了の瞬間かどうかを取る
	bool GetIsShootModeEnd()const {
		return playerMode_==MODE_SHOOT_END;
	}
	//滑り落ちるまでの時間を返す
	float GetSlipCount()const {
		return slipCount_;
	}
	//滑り落ち状態かを調べる
	bool GetIsSlipped()const {
		return playerMode_==MODE_SLIP;
	}
	//編集可能インプットを取得する
	PlayerInputChecker& GetEditableUseKey() {
		return isUseKey_;
	}
	std::vector<float>& GetEditableRot_Speed() {
		return mRot_spd;
	}
	//Headのレーンを本体のレーンに合わせる
	void SetMyHeadLaneNum(int targetNum);
	void SetAllHeadLaneNum();

	//worldの共有データに自分の情報を代入する
	void worldSetMyDatas();
	//次に移動するレーンを設定する
	void SetNextLane(int addNum, LaneChangeType changeType = LaneChangeType::LaneChange_Normal);
	//支点固定座標を設定し、首の位置を補正する
	void setCurPHeadSPos(const Vector2& sPos) {
		if (isTutorialText_) return;

		SetMultiplePos(sPos - stopPos_);
		stopPos_ = sPos;
	}
	//振り子により作り出された移動ベクトルを取得する
	Vector2 GetPendulumVect()const {
		return pendulumVect_;
	}
	//振り子により作り出された移動ベクトルを上書きする
	void SetPendulumVect(const Vector2& pvect) {
		pendulumVect_ = pvect;
	}
	//現在の頭に対して滑るかどうかをセットする
	void curPHeadSlip(bool isSlip);
	//プレイヤーが死んでるか
	bool isPlayerDead()const {
		if(laneNum_==(maxLaneSize_-1)&&position_.y >= WINDOW_HEIGHT-200)return true;

		for (auto pHD : pHeadDead_) {
			if (!pHD)return false;
		}
		return true;
	}
	//首の長さをリセットし、頭を回転させるまでの一連の動作を行う(1で右回転、デフォルトで左回転)
	void PHeadChanger(int rot = 0);
	//支点固定座標を設定する
	void SetStopPos(Vector2 target);
	//支点固定座標を取得する
	Vector2 GetStopPos()const {
		return stopPos_;
	}
	//首の先端の角度を調べる
	float GetRot()const {
		return mRot.front();
	}
	//首の根本の角度を調べる
	float GetRotBack()const {
		return mRot.back();
	}
	//滑り落ち時のHeadのあるべき位置を取得する
	Vector2 GetSlipHeadPoint()const{
		return fPos_.front();
	}
	//現在使用しているHeadの座標を返す
	Vector2 GetCurrentPHeadPosition()const;
	//
	float GetPlayerSwordAngle()const;
	//振り子の支点位置を移動する
	void SetMultipleFulcrumPos(const Vector2& pos) {
		fPos_.front() = pos;
	}
	//現在剣が有効かを調べる
	bool GetIsSwordActive()const;
	//クリア時の振り子状態かを調べる
	bool GetIsClearBite()const;
	//クリア時の発射状態かを調べる
	bool GetIsClearShoot()const;
	//現在の頭を調べる
	Actor* GetCurrentHead() const;
	//プレイヤーの位置に応じた状態遷移を行う(シーン側で有効、無効を決定する)
	void deadLine();
	//キーを有効化するかどうかを決定する
	void SetUseKey(bool key) {
		isUseKey_.SetUseKey(key);
	}
	bool GetUseKey()const { return isUseKey_.GetUseKey(); }
	void SetIsTutorialTextWriting(bool is) {
		isTutorialText_ = is;
	}
	bool IsLookBack()const;
protected:
	void MultipleInit(float Length, const Vector2& fPos, float rot, float radius);
	void Multiple();
	//多重振り子に移動量を加算
	void UpdateMultiplePos();
	void SetMultiplePos(const Vector2& addpos);
	//多重振り子を強制的に移動
	void AddMultiplePos(const Vector2& addPos) {
		for (int i = 0; i < (int)multiplePos.size(); i++) {
			multiplePos[i] += (addPos);
			if (i > 0) fPos_[i] = multiplePos[i - 1];
		}
	}
	//首の描画に必要な一連の動作を行う
	void SetDrawNeck(const Vector2& bodyPoint,const Vector2& headPoint);
	//首の描画位置を設定
	void SetDrawPoint(const Vector2& bodyPoint, const Vector2& headPoint);
	//首の各描画位置を設定する
	void SetDrawNeckParts(const Vector2& bodyPoint, const Vector2& headPoint);
	//1で左隣の、未入力で右隣のHeadに回転し、長さをリセットする

	void PHeadLengthReset();
	//チェーンの長さを加算する
	void CurPHeadLengPlus(float addPow);
	void UpdateLaneNum(int updateNum, LaneChangeType changeType = LaneChangeType::LaneChange_Normal);
	Vector2 LaneChange_Up();
	Vector2 LaneChange_Down(LaneChangeType changeType);

	//噛み付きエフェクトを生成
	void CreateBiteEffect();
	//首変更エフェクトを生成
	void CreateMetamorEffect();

//プレイヤーの状態に応じた更新
protected:
	void ToFallMode(bool isPlaySE);
	void ToShootMode(bool isPlaySE);
	void ToShootEndMode(bool isPlaySE);
	void ToBiteMode(bool isPlaySE);
	void ToSlipMode(bool isPlaySE);
	void ToResistMode(bool isPlaySE);
	void ToClearMode(bool isPlaySE);
	void ToDeadMode(bool isPlaySE);
	void ToBackShootMode(bool isPlaySE);

	void FallUpdate();
	void ShootUpdate();
	void ShootEndUpdate();
	void BiteUpdate();
	void SlipUpdate();
	void ResistUpdate();
	void ClearUpdate();
	void DeadUpdate();
	void BackShootUpdate();

protected:
	using PHeadPtr = std::shared_ptr<Player_Head>;
	using PSwordPtr = std::shared_ptr<Player_Sword>;
	//ステージの最大レーン数
	int maxLaneSize_;

	int addscorelist_[3];

	//振り子移動の摩擦
	float friction;
	//振り子移動によるベクトルを作り出す
	Vector2 pendulumVect_;

	//多重振り子
	std::vector<Vector2> fPos_;
	std::vector<Vector2> multiplePos;
	std::vector<float> mRot;
	std::vector<float> mRot_spd;
	std::vector<float> correctionLens;
	std::vector<DrawPos> drawPoints;
	//首描画テスト
	std::vector<NeckPiecePoint> neckDrawPoints;

	//Headが静止する位置を格納する
	Vector2 stopPos_;

	PSwordPtr pSword_;

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

	float slipCount_;

	Vector2 headPosAddVect_;

	//滑り落ちるまでの時間
	float slipResistTime_;

	//頭の位置を決定する,0で右
	float headAngleSetter;
	//0=滞空 1=発射時 2=発射終了 3=噛み付き 4=滑り落ち
	int playerMode_;
	float jumpShotPower_;

	//レーン移動したフレームで噛む対象があるかを調べる
	bool isNextLaneBite_;

	int laneAddNum_;

	int chainLockCoolTime_;
	
	bool isClearShoot_;
	//プレイヤー用のキー入力チェッカー
	PlayerInputChecker isUseKey_;
	bool isTutorialText_;
	//現在掴んでいる服
	CLOTHES_ID otherClothesID_;
	LaneChangeType changeType_;
	//滑る時間の倍数(服毎)
	std::map<CLOTHES_ID, float> slipCountMult_;
	//更新関数リスト
	std::map<int, std::function<void()>> updateFunctionMap_;

	//クリア時にプレイヤーが発射される瞬間の処理を行う
	MethodTimer clearShootTimer_;
	
	//首をロックする処理を行う
	bool addLengthLocker_;
protected:
	const float gravity_{0.5f};
	const float spdLimit{ 2.75f };

};

