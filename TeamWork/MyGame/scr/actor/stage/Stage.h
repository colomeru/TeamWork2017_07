#pragma once

#include "../Actor.h"

class Stage : public Actor
{
public:
	//コンストラクタ
	Stage(IWorld* world);
	//デストラクタ
	~Stage();
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//
	virtual void OnCollide(Actor* other, CollisionParameter colpara);
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);

};