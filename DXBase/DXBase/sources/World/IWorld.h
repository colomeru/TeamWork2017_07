#ifndef IWORLD_H_
#define IWORLD_H_

#include "../Actor/Base/ActorPtr.h"
#include "../Field/FieldPtr.h"
#include <string>
#include <forward_list>

enum class ActorGroup;
enum class EventMessage;

struct Vector2;
struct Matrix;
struct ScroolJudge;

class SceneDataKeeper;

// ワールド抽象インターフェース
class IWorld {
public:
	// 仮想デストラクタ
	virtual ~IWorld() {}
	// アクターの追加
	virtual void addActor(ActorGroup group, const ActorPtr& actor) = 0;
	//UIアクターの追加
	virtual void addUIActor(const ActorUIPtr& actor) = 0;
	// アクターの検索
	virtual ActorPtr findActor(const std::string& name) = 0;
	// フィールドの取得
	virtual FieldPtr getField() const = 0;
	virtual unsigned int fieldHandle() const = 0;
	//アクターの検索(list返し)
	virtual std::forward_list<ActorPtr> findActors(const ActorGroup group) = 0;
	virtual bool is_clear() = 0;
	virtual void clear(bool clear) = 0;
	// ボスの口に侵入したかを返します
	virtual bool isEntered() = 0;
	// ボスの口から脱出したかを返します
	virtual bool isLetOuted() = 0;
	// ボスの口の侵入脱出を設定します
	virtual void setEntry(const bool isEntry, const bool isLetOut) = 0;
	// タイマが止まっているかを返します
	virtual bool isStopTime() = 0;
	// タイマを0にするかを設定します
	virtual void setIsStopTime(const bool isTime) = 0;
	// デルタタイムを取得します(元のタイムクラスから取得)
	virtual float getDeltaTime() = 0;
	// 中ボスのステージ内に入ったかを設定します
	virtual void setIsMBossStage(bool isStage) = 0;
	// 中ボスのステージ内に入ったかを返します
	virtual bool isMBossStage() = 0;
	virtual void SetPlayerPos(Vector2 pos) = 0;
	// 縦スクロール横スクロールするかどうか(引数 軸ごとにスクロールする場合は1、しない場合は0)
	virtual void SetScroolJudge(const Vector2& scroolJudge, const Vector2& scroolMinPos, const Vector2& scroolMaxPos, bool flag = false) =0;
	virtual ScroolJudge GetScroolJudge()=0;
	// プレイヤーの操作不能にするかどうか
	virtual void PlayerNotMove(bool flag) = 0;
	// プレイヤーの操作不能にするかどうかを取得
	virtual bool GetPlayerNotMove() = 0;
	// 画面上のプレイヤーの座標
	virtual Vector2 GetScreenPlayerPos()=0;
	// データキーパーの取得
	virtual SceneDataKeeper* GetKeeper() = 0;
	// メッセージの送信
	virtual void sendMessage(EventMessage message, void* param = nullptr) = 0;
	//
	virtual Vector2 GetPlayerPos() = 0;
	virtual Matrix GetInv() = 0;

	//全あたり判定を切るかどうか true:on false:off
	virtual void CollisitionOffOn(bool col)=0;
	virtual bool GetCollisitionOffOn()=0;

	virtual void inv(const Matrix& mat) = 0;
	//

	//座標を指定するとそこにスクロールする（毎フレーム呼ぶこと）
	virtual void SetScroolPos(const Vector2& pos)=0;

	virtual Vector2 GetInvVelo() = 0;
	//
	virtual void SetScreenPlayerPos(const Vector2& screenPos) = 0;
	virtual Matrix InitializeInv(Vector2 position) = 0;

	virtual void Spring(Vector2& pos, Vector2& resPos, Vector2& velo, float stiffness = 0.1f, float friction = 0.5f, float mass = 2.0f)const = 0;

	virtual void addCount(int i = 1) = 0;
	virtual void setCount(int i) = 0;
	virtual void minusCount() = 0;
	virtual void resetCount() = 0;
	virtual void resetGetCount() = 0;
	virtual int getCount() = 0;
	virtual int getGetCount() = 0;
	virtual std::string getCurrentSceneName() = 0;

};

#endif
