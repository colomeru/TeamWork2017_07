#pragma once
#include "IWorld.h"
#include "WorldActor.h"
#include"../math/MathHelper.h"
#include"../math/Vector3.h"
#include"../math/Vector2.h"
#include <stack>

static const float defDrawLinePosY[3] = { 0,400,1000 };
static const float defDrawLineChangePosY[5] = { -400,0,400,1000,1500 };

//World内で、アクター全員が取得出来るデータ
struct KeepDatas {
	//playerの現在のレーン
	int playerLane_;
	//次のレーンの方向を代入する(-1～1)
	int nextLane_;
	Vector2 playerPos_;
	float changeLaneLerpPos_;

	KeepDatas(int lane = 0, Vector2 pos = Vector2::Zero, int nextLane = 0) :playerLane_(lane), playerPos_(pos), nextLane_(nextLane), changeLaneLerpPos_(0.f){}

	//playerの現レーンを更新する
	void SetPlayerLane(const int& pLane) {
		playerLane_ = pLane;
	}
	//playerの現レーンを変更する
	void SetPlayerPos(const Vector2& pPos) {
		playerPos_ = pPos;
	}
	//プレイヤーが次に行くレーンを指定する
	void SetPlayerNextLane(const int& pNLane) {
		nextLane_ = pNLane;
	}
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
	virtual void SetTarget(Actor* tgt) {
		targetAct_ = tgt;
	}
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

	virtual Matrix GetInv()override {
		return inv_;
	}
	virtual void SetScroolPos(const Vector2& pos) override {
		targetMat_.Translation(Vector3(pos.x, pos.y, 0));
	}
	//共有データを更新する、変更を行わない値の引数は、元のKeepDatasの値を渡す事
	virtual void SetKeepDatas(KeepDatas data) override {
		keepDatas_ = data;
	}
	//共有データを取得する
	virtual KeepDatas GetKeepDatas()const override {
		return keepDatas_;
	}

	virtual KeepDatas& GetCanChangedKeepDatas() override {
		return keepDatas_;
	}
	virtual void ChangeCamMoveMode(int addNum) override{
		isChangeCam_ = true;
		addNum_ = addNum;
	}
	virtual bool GetIsCamChangeMode()const override {
		return isChangeCam_;
	}
	virtual bool isChangeFrame()const {
		return isChangeFrame_;
	}

private:
	void Spring(Vector2 & pos, Vector2 & resPos, Vector2 & velo, float stiffness = 0.1f, float friction = 0.5f, float mass = 2.0f) const
	{
		// バネの伸び具合を計算
		Vector2 stretch = (pos - resPos);
		// バネの力を計算
		Vector2 force = -stiffness * stretch;
		// 加速度を追加
		Vector2 acceleration = force / mass;
		// 移動速度を計算
		velo = friction * (velo + acceleration);

		pos = pos + velo;
	}

private:
	//カメラ更新用のUpdateへ移行する
	bool isChangeCam_;
	int addNum_;
	bool isChangeFrame_;
	std::map<bool, std::function<void()>> updateFunctionMap_;

	Matrix inv_;
	Matrix resInv_;
	Matrix targetMat_;
	Actor* targetAct_;
	Vector2 mPrePos;
	Vector2 mCurPos;
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