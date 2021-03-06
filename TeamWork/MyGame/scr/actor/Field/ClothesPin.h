#pragma once
#include "../Actor.h"

class ClothesPin : public Actor, public std::enable_shared_from_this<Actor>
{
public:
	//コンストラクタ
	ClothesPin(IWorld* world, int laneNum, Vector2 pos, Actor* clothes, Vector2 fulcrum);
	//デストラクタ
	virtual ~ClothesPin() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor& other, CollisionParameter colpara)override;
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);

	void ClearThis() {
		parameter_.isDead = true;
	}
private:
	Vector2 pos_;
	Vector2 fulcrum_;
};