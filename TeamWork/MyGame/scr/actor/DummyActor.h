#pragma once
#include "Actor.h"

class DummyActor : public Actor
{
public:
	//コンストラクタ
	explicit DummyActor(IWorld* world);
	//デストラクタ
	~DummyActor();
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
};