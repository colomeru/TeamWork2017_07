#pragma once
#include "../Actor.h"

class GoalUI : public Actor
{
public:
	// コンストラクタ
	GoalUI(IWorld* world, Vector2 pos);
	// デストラクタ
	~GoalUI();
	// 更新
	virtual void Update() override;
	// 描画
	virtual void Draw() const override;
	// 受動更新
	virtual void OnUpdate() override;
	// 当たり判定処理
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	// メッセージ処理
	virtual void OnMessage(EventMessage message, void* param) override;

private:
	
};

