#pragma once
#include "../actor/ActorPtr.h"
#include <functional>
#include"../math/Matrix.h"
#include"../math/Vector2.h"

enum ACTOR_ID;
enum COL_ID;
enum class EventMessage;
struct KeepDatas;

// ワールドインターフェース
class IWorld
{
public:
	// 仮想デストラクタ
	virtual ~IWorld() = default;
	// 追加
	virtual void Add(ACTOR_ID id_, ActorPtr actor_) = 0;
	// 終了
	virtual bool IsEnd() const = 0;
	// 衝突判定の設定
	virtual void SetCollideSelect(ActorPtr thisActor, ACTOR_ID otherID, COL_ID colID) = 0;
	// メッセージの送信
	virtual void sendMessage(EventMessage message, void* param = nullptr) = 0;
	// 指定グループの巡回
	virtual void EachActor(ACTOR_ID id, std::function<void(Actor&)> func) = 0;
	// 指定グループの巡回
	virtual void EachActor(ACTOR_ID id, std::function<void(const Actor&)> func) const = 0;
	// 指定グループのアクター数を取得
	virtual int Count(ACTOR_ID id) const = 0;

	virtual void inv(const Matrix& mat) = 0;
	virtual Matrix InitializeInv(Vector2 position) = 0;
	virtual Matrix GetInv() = 0;
	virtual void SetScroolPos(const Vector2& pos) = 0;
	//スクロール対象の設定
	virtual void SetTarget(Actor* tgt) = 0;
	// 操作アクターセット
	virtual void PushStackActor(ActorPtr actor) = 0;
	// 操作アクターポップ
	virtual void PopStackActor() = 0;

	virtual void SetKeepDatas(KeepDatas data) = 0;
	virtual KeepDatas GetKeepDatas() const = 0;
	virtual KeepDatas& GetCanChangedKeepDatas() = 0;
	virtual void ChangeCamMoveMode(int addNum) = 0;
	virtual bool GetIsCamChangeMode()const = 0;
	virtual bool isChangeFrame()const = 0;
	virtual void SetIsChangeFrame(bool is) = 0;

	virtual void StartModeUpdate()=0;
	virtual void UnLockCameraPosY() = 0;
	virtual void FreeCameraPosY(bool is) = 0;
	virtual bool GetIsFreeCamY_()const=0;
	virtual void UpdateDrawPos() = 0;
	virtual Matrix& GetChangeInv() = 0;
};