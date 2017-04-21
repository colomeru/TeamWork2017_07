#pragma once
#include "IWorld.h"
#include "WorldActor.h"
#include"../math/MathHelper.h"
#include"../math/Vector3.h"
#include <stack>

//World内で、アクター全員が取得出来るデータ
struct KeepDatas {
	//playerの現在のレーン
	int playerLane_;

	KeepDatas(int lane=0):playerLane_(lane){}

	//playerの現レーンを変更する
	void SetPlayerLane(int pLane) {
		playerLane_ = pLane;
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
	void Draw() const;
	// クリア
	void Clear();
	// イベントリスナーの追加
	void AddEventMessageListener(std::function<void(EventMessage, void*)> listener);

	// アクターの取得
	WorldActor GetWorldActor() const;

	/* ワールドインターフェース */
	// 追加
	virtual void Add(ACTOR_ID id, ActorPtr actor);
	virtual void SetTarget(Actor* tgt){
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
	virtual KeepDatas& GetKeepDatas() override {
		return keepDatas_;
	}

	virtual KeepDatas GetCanChangedKeepDatas() const override {
		return keepDatas_;
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