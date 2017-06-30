#pragma once
#include "../Actor.h"
#include<vector>
#include"../../math/MathHelper.h"
#include"../../math/MyFuncionList.h"
#include"../../Def.h"
#include"../../graphic/DrawPos.h"
#include "../../scene/Credit2Scene.h"

class Player_Head;
class Player_Sword;

//const Vector2 pHDist = Vector2(32, 32);
//const int headAngleSetter = -2;
//const float defHeadChangeTime = 0.2f;
//const float defHeadLength = 2.f;
//const float defPGravPow = 0.05f;
//const float defGravAddPow = 0.2f;
//const float HeadShootMult = 0.5f;
//const float defSlipCount = 8.f;
//const int defLaneChangeCoolTime_ = 60;
//const int defChainLockCoolTime_ = 10;
////落ちた後のposition加算値
//const float fallAddPosMult = 3.4f;
////落ちるまでの最大時間
//const float defResistTime = 0.5f;
//レーンの本数

//enum {
//	MODE_FALL = 0,
//	MODE_SHOOT = 1,
//	MODE_SHOOT_END = 2,
//	MODE_BITE = 3,
//	MODE_SLIP = 4,
//	MODE_RESIST = 5,
//	MODE_CLEAR = 6,
//	MODE_PLAYERDEAD = 7
//};


class CreditPlayer : public Actor, public std::enable_shared_from_this<CreditPlayer>
{
private:
	enum LaneChangeType {
		LaneChange_Normal,
		LaneChange_Fall
	};
public:
	//コンストラクタ
	CreditPlayer(IWorld* world, int maxLaneSize = 3, int startLane = 1);
	//デストラクタ
	~CreditPlayer();
	//更新
	virtual void Update() override;
	//最初の更新
	virtual void FastUpdate() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor&, CollisionParameter colpara) override;
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);
	//
	virtual bool CamMoveUpdate();
	//
	virtual void CamMoveUp()override;
	//
	virtual void CamMoveDown() override;
	//
	virtual void LaneChangeFall() override;
	//
	bool isLaneChangeFall() const;
	//振り子運動
	void Pendulum(Vector2 fulcrum, float length);
	//スタート時の振り子
	void StartPendulum();
	//頭の座標を取得
	Vector2 GetHeadPos(int headNum)const;
	//Headの長さを実際のゲームに反映される値に変換して返す
	float GetHeadLengthChangeToPosMult(int headNum) const;
	//現在のHeadの首の長さを返す
	Vector2 GetCurrentHeadLength()const;
	//
	void HeadPosUpdate();
	//
	void SwordPosUpdate();
	//使用する頭を右隣の物に変更
	void changeHead();
	//使用する頭を左隣の物に変更
	void backChangeHead();
	//プレイヤーの開始時の状態設定
	void StartPlayerSet();
	//現在の頭の番号を取得
	int GetCurHead()const;
	//頭死亡
	bool GetPHeadDead(int pHeadNum)const;
	//頭が
	void CurHeadBite(const Vector2& target);
	//
	void ResurrectHead();
	//
	void SetOtherClothesID_(CLOTHES_ID cId);
	//噛み付ける状態かを返す(レジスト含む)
	bool GetIsShootMode()const;
	//
	bool GetIsBiteMode()const;
	//
	bool GetIsResistMode()const;
	//
	bool GetIsClearMode()const;
	//
	int GetPHeadLiveCount()const;
	//噛み付き状態にするかをセット、
	void SetIsBiteMode(bool ismode);
	//
	void SetMode(int pMode);
	//シュート終了の瞬間かどうかを取る
	bool GetIsShootModeEnd()const;
	//
	float GetSlipCount()const;
	//
	bool GetIsSlipped()const;
	//Headのレーンを本体のレーンに合わせる
	void SetMyHeadLaneNum(int targetNum);
	//
	void SetAllHeadLaneNum();
	//worldの共有データに自分の情報を代入する
	void worldSetMyDatas();
	//
	void SetNextLane(int addNum, LaneChangeType changeType = LaneChangeType::LaneChange_Normal);
	//
	void setCurPHeadSPos(const Vector2& sPos);
	//
	void setMaxLaneSize(int size);
	//現在の頭に対して滑るかどうかをセットする
	void curPHeadSlip(bool isSlip);
	//プレイヤーが死んでるか
	bool isPlayerDead()const;
	//
	void PHeadChanger(int rot = 0);
	//
	void SetStopPos(Vector2 target);
	//
	Vector2 GetStopPos()const;
	//
	float GetRot()const;
	//
	Vector2 GetHeadPosAddVect() const;
	//
	Vector2 GetSlipHeadPoint()const;
	//現在使用しているHeadの座標を返す
	Vector2 GetCurrentPHeadPosition()const;
	//
	float GetPlayerSwordAngle()const;
	//振り子の支点位置を移動する
	void SetMultipleFulcrumPos(const Vector2& pos);
	//
	bool GetIsSwordActive()const;
	//クリア後に次ステージに飛ぶ状態かを調べる
	bool GetIsClearBite()const;
	//
	bool GetIsClearShoot()const;
	//
	Actor* GetCurrentHead() const;
	//
	void deadLine();
	//重力リセット
	void GravityReset();
	//1で左隣の、未入力で右隣のHeadに回転し、長さをリセットする
	void PHeadLengthReset();
	//死亡した頭を全て回復
	void AllResurrectHead();
	//
	void SetBiteSprite();
	//
	void MultipleInit(float Length, const Vector2& fPos, float rot, float radius);
	//
	void DeformationDraw();
	//
	void SetPosition(Vector2 position);
	//
	Vector2 GetPlayerPosition();


private:
	//
	void Multiple();
	//多重振り子に移動量を加算
	void UpdateMultiplePos();
	void SetMultiplePos(const Vector2& addpos);
	//多重振り子を強制的に移動
	void AddMultiplePos(const Vector2& addPos);
	//
	void SetNeckNonMult();
	//首の描画に必要な一連の動作を行う
	void SetDrawNeck(const Vector2& bodyPoint, const Vector2& headPoint);
	//首の描画位置を設定
	void SetDrawPoint(const Vector2& bodyPoint, const Vector2& headPoint);
	//首の各描画位置を設定する
	void SetDrawNeckParts(const Vector2& bodyPoint, const Vector2& headPoint);
	//入力による動作をまとめる
	void PlayerInputControl();
	//チェーンの長さを加算する
	void CurPHeadLengPlus(float addPow);
	//
	void UpdateLaneNum(int updateNum, LaneChangeType changeType = LaneChangeType::LaneChange_Normal);
	//
	void CreateBiteEffect();


	//プレイヤーの状態に応じた更新
private:
	//
	void FallUpdate();
	//
	void ShootUpdate();
	//
	void ShootEndUpdate();
	//
	void BiteUpdate();
	//
	void SlipUpdate();
	//
	void ResistUpdate();
	//
	void ClearUpdate();
	//
	void DeadUpdate();


private:
	using PHeadPtr = std::shared_ptr<Player_Head>;
	using PSwordPtr = std::shared_ptr<Player_Sword>;
	//ステージの最大レーン数
	int maxLaneSize_;

	int addscorelist_[3];

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

	//多重振り子
	std::vector<Vector2> fPos_;
	std::vector<Vector2> multiplePos;
	std::vector<float> mRot;
	std::vector<float> mRot_spd;
	std::vector<float> correctionLens;
	std::vector<DrawPos> drawPoints;



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
	float clearAddRot_;

	float maxChainLength_;

	float slipCount_;

	bool isSlipped_;

	bool isPlayerFallLane_;

	Vector2 headPosAddVect_;

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

	bool isClearShoot_;

	bool isUseKey_;
	CLOTHES_ID otherClothesID_;
	LaneChangeType changeType_;

	//滑る時間の倍数(服毎)
	std::map<CLOTHES_ID, float> slipCountMult_;

	std::map<int, std::function<void()>> updateFunctionMap_;

	bool operatre_;

};

