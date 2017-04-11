#ifndef WORLD_H_
#define WORLD_H_

#include "IWorld.h"
#include "ScrollJudge.h"
#include "../Actor/Base/ActorManager.h"
#include "../Scene/Base/SceneDataKeeper.h"

enum class EventMessage;

class World : public IWorld {
public:
	// コンストラクタ
	World(SceneDataKeeper* keeper);
	// 更新
	void update(float deltaTime);
	// 描画
	void draw() const;
	// メッセージ処理
	void handleMessage(EventMessage message, void* param);
	// フィールドの追加
	void addField(const FieldPtr& field);
	// カメラの追加
	void addCamera(const ActorPtr& camera);
	// ライトの追加
	void addLight(const ActorPtr& light);

	// アクターの追加
	virtual void addActor(ActorGroup group, const ActorPtr& actor) override;
	//UIアクターの追加
	virtual void addUIActor(const ActorUIPtr& actor) override;
	// アクターの検索
	virtual ActorPtr findActor(const std::string& name) override;
	//アクターの検索(list返し)
	virtual std::forward_list<ActorPtr>findActors(const ActorGroup group) override;
	// フィールドの取得
	virtual FieldPtr getField() const override;
	virtual unsigned int fieldHandle() const override;

	// メッセージの送信
	virtual void sendMessage(EventMessage message, void* param = nullptr)  override;
	// イベントリスナーの追加
	void addEventMessageListener(std::function<void(EventMessage, void*)> listener);
	//クリア条件
	virtual bool is_clear()override;
	virtual void clear(bool clear)override;
	// ボスの口に侵入したかを返します
	bool isEntered() override;
	// ボスの口から脱出したかを返します
	bool isLetOuted() override;
	// ボスの口の侵入脱出を設定します
	void setEntry(const bool isEntry, const bool isLetOut) override;
	// タイマが止まっているかを返します
	bool isStopTime() override;
	// タイマを0にするかを設定します
	void setIsStopTime(const bool isTime) override;
	// デルタタイムを取得します(元のタイムクラスから取得)
	float getDeltaTime() override;
	// 中ボスのステージ内に入ったかを設定します
	void setIsMBossStage(bool isStage) override;
	// 中ボスのステージ内に入ったかを返します
	bool isMBossStage() override;
	virtual void SetScroolJudge(const Vector2& scroolJudge, const Vector2& scroolMinPos, const Vector2& scroolMaxPos,bool flag=false) override;
	virtual Vector2 GetScreenPlayerPos() override
	{
		return playerScreenPos_;
	}
	virtual ScroolJudge GetScroolJudge()override;
	virtual void PlayerNotMove(bool flag) override {
		mNoPlayerMove = flag;
	}
	virtual bool GetPlayerNotMove() override {
		return mNoPlayerMove;
	}
	virtual Matrix GetInv()override {
		return inv_;
		//Matrix mat;
		//mat.Translation(Vector3(mPlayerPos.x, mPlayerPos.y,0.0f));
		//return Matrix::Invert(mat) *
		//	Matrix::CreateTranslation(Vector3(PLAYER_SCREEN_POSITION.x, PLAYER_SCREEN_POSITION.y));
	}
	virtual void SetPlayerPos(Vector2 pos) override
	{
		//配列の初期化
		InitializeInv(pos);
		mPlayerPos = pos;
	}
	virtual Vector2 GetPlayerPos() override
	{
		return mPlayerPos;
	}
	virtual void CollisitionOffOn(bool col) {
		colOffOn_ = col;
	}
	virtual bool GetCollisitionOffOn() {
		return colOffOn_;
	}

	// IWorld を介して継承されました
	virtual void inv(const Matrix& mat) override;

	virtual Vector2 GetInvVelo() override {
		return mVelo;
	}
	//スクリーン上のプレイヤーの表示位置
	virtual void SetScreenPlayerPos(const Vector2& screenPos) override {
		playerScreenPos_ = screenPos;
	}

	virtual Matrix InitializeInv(Vector2 position) override;
	////座標を指定するとそこにスクロールする（毎フレーム呼ぶこと）
	virtual void SetScroolPos(const Vector2& pos) override;
	virtual void Spring(Vector2 & pos, Vector2 & resPos, Vector2 & velo, float stiffness = 0.1f, float friction = 0.5f, float mass = 2.0f) const override;

	// コピー禁止
	World(const World& other) = delete;
	World& operator = (const World& other) = delete;

	virtual void addCount(int i = 1)override {
		itemCount_ += i;
		getCount_ += i;
		itemCount_ = max(itemCount_, 0);
		getCount_ = max(getCount_, 0);
	}
	virtual void setCount(int i)override {
		itemCount_ = i;
	}
	virtual void minusCount()override {
		itemCount_ -= getCount_;
	}
	virtual void resetCount()override {
		itemCount_ = 0;
		getCount_ = 0;
	}
	virtual void resetGetCount()override {
		getCount_ = 0;
	}
	virtual int getCount()override {
		return itemCount_;
	}
	virtual int getGetCount()override {
		return getCount_;
	}
	virtual SceneDataKeeper* GetKeeper()override {
		return keeper_;
	}
	virtual std::string getCurrentSceneName() override{
		return currentSceneName_;
	}

private:
	// アクターマネージャー
	ActorManager	actors_;
	// ライト
	ActorPtr		light_;
	// カメラ
	ActorPtr		camera_;
	// フィールド
	FieldPtr		field_;
	// イベントリスナー
	std::function<void(EventMessage, void*)> listener_;

	bool is_clear_;
	// 口に出入りしたか
	bool isEntered_, isLetOuted_;
	// 時間を止めるか
	bool isStopTime_;
	// 中ボスのステージ内か
	bool isMBossStage_;
	// 現在の時間
	float deltaTime_;

	ScroolJudge scrool_;
	
	Vector2 mPlayerPos;
	
	bool mNoPlayerMove;
	bool colOffOn_;

	Vector2 playerScreenPos_;

	int itemCount_;
	int getCount_;

public:

	Matrix inv_;
	Matrix resInv_;
	//プレイヤーのマトリクス
	Matrix playerMat_;

	//補正された速度
	Vector2 mVelo;

	//補正用速度
	Vector2 velo;
	//1フレーム前
	Vector2 mPrePos;
	//1フレーム後
	Vector2 mCurPos;
	
	SceneDataKeeper* keeper_;

	std::string currentSceneName_;
};

#endif

