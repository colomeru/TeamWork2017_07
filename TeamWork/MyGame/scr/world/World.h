#pragma once
#include "IWorld.h"
#include "WorldActor.h"
#include"../math/MathHelper.h"
#include"../math/Vector3.h"
#include"../math/Vector2.h"
#include"../stageGenerator/Stage.h"
#include <stack>

static const float defDrawLinePosY[3] = { 0,500,1000 };
static const float defDrawLineChangePosY[5] = { -400,0,500,1000,1500 };

class Actor;
//World内で、アクター全員が取得出来るデータ
struct KeepDatas {
	//playerの現在のレーン
	int playerLane_;
	int prevPlayerLane_;
	//次のレーンの方向を代入する(-1～1)
	int nextLane_;
	Vector2 playerPos_;
	Vector2 startPointPos_;
	float changeLaneLerpPos_;
	bool isFallCamMode_;
	float fallAddPos_;
	float camPosY_;
	Stage currentStage_;
	KeepDatas(int lane = 0, Vector2 pos = Vector2::Zero,Vector2 sPPos=Vector2::Zero, int nextLane = 0,bool isFallCamMode=false,float fallAddPos=0.f,Stage currentStage=Stage::Stage1) :playerLane_(lane), currentStage_(currentStage), prevPlayerLane_(lane), playerPos_(pos),startPointPos_(sPPos), nextLane_(nextLane), changeLaneLerpPos_(0.f), isFallCamMode_(isFallCamMode), fallAddPos_(fallAddPos), camPosY_(0.f){}

	//playerの現レーンを更新する
	void SetPlayerLane(const int& pLane) {
		if (playerLane_ == pLane)return;

		prevPlayerLane_ = playerLane_;
		playerLane_ = pLane;
	}
	//playerの現レーンを変更する
	void SetPlayerPos(const Vector2& pPos) {
		playerPos_ = pPos;
	}
	void SetStartPointPos(const Vector2& sPPos) {
		startPointPos_ = sPPos;
	}
	//プレイヤーが次に行くレーンを(+下-上)
	void SetPlayerNextLane(const int& pNLane) {
		nextLane_ = pNLane;
	}
	//レーン移動時の位置(割合0～1)
	void SetChangeLaneLerpPos_(float lNum) {
		changeLaneLerpPos_ = lNum;
	}
};

static const Vector2 playerScreenPos_ = Vector2(300, 0);
class World : public IWorld
{
public:
	// コンストラクタ
	World();
	// デストラクタ
	~World();
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw(const int laneCount = 0, const int playerLane = 0) const;
	// クリア
	void Clear();
	// イベントリスナーの追加
	void AddEventMessageListener(std::function<void(EventMessage, void*)> listener);

	// アクターの取得
	WorldActor GetWorldActor() const;

	/* ワールドインターフェース */
	// 追加
	virtual void Add(ACTOR_ID id, ActorPtr actor);
	virtual void SetTarget(Actor* tgt);
	// 終わっているか？
	virtual bool IsEnd() const;
	// 衝突判定設定
	virtual void SetCollideSelect(ActorPtr thisActor, ACTOR_ID otherID, COL_ID colID);
	// メッセージの送信
	virtual void sendMessage(EventMessage message, void* param = nullptr) override;
	// 指定グループの巡回
	virtual void EachActor(ACTOR_ID id, std::function<void(Actor&)> func) override;
	// 指定グループの巡回
	virtual void EachActor(ACTOR_ID id, std::function<void(const Actor&)> func) const override;
	// 指定グループのアクター数を取得
	virtual int Count(ACTOR_ID id) const override;

	// 操作アクターセット
	virtual void PushStackActor(ActorPtr actor);
	// 操作アクターポップ
	virtual void PopStackActor();

	virtual void inv(const Matrix& mat) override;
	virtual Matrix InitializeInv(Vector2 position) override;

	virtual Matrix GetInv()override;
	virtual Matrix& GetChangeInv()override;
	virtual void SetScrollPos(const Vector2& pos) override;
	//共有データを更新する、変更を行わない値の引数は、元のKeepDatasの値を渡す事
	virtual void SetKeepDatas(KeepDatas data) override;
	//共有データを取得する
	virtual KeepDatas GetKeepDatas()const override;

	virtual KeepDatas& GetCanChangedKeepDatas() override;
	virtual void ChangeCamMoveMode(int addNum) override;
	virtual bool GetIsCamChangeMode()const override;
	virtual bool isChangeFrame()const;
	virtual void SetIsChangeFrame(bool is);
	virtual void StartModeUpdate()override;

	virtual void UnLockCameraPosY()override;
	virtual void FreeCameraPosY(bool is);
	virtual bool GetIsFreeCamY_()const;
	virtual void UpdateDrawPos();
	virtual void SetMaxSize(int size) override;
private:
	void Spring(Vector2 & pos, Vector2 & resPos, Vector2 & velo, float stiffness = 0.1f, float friction = 0.5f, float mass = 2.0f) const;

private:
	int maxSize_;

	float camShootSpd_;
	//カメラ更新用のUpdateへ移行する
	bool isChangeCam_;
	int addNum_;
	bool isChangeFrame_;
	//クリア用
	bool isLockedCamY_;
	//ゲームプレイ用
	bool isFreeCamY_;
	std::map<bool, std::function<void()>> updateFunctionMap_;

	Matrix inv_;
	Matrix resInv_;
	Matrix targetMat_;
	Actor* targetAct_;
	Vector2 mPrePos;
	Vector2 mCurPos;

	float unlockYPos_;
	// アクター
	WorldActor	actors_;

	Vector2 mVelo;
	Vector2 velo;

	KeepDatas keepDatas_;

	// 受動更新アクター用スタック
	std::stack<ActorPtr>	manualStackActor_;
	// カメラ用スタック
	std::stack<ActorPtr>	stackCamera_;
};