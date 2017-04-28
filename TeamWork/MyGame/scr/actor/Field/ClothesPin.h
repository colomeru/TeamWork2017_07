#pragma once
#include "../Actor.h"

class ClothesPin : public Actor, public std::enable_shared_from_this<Actor>
{
public:
	//コンストラクタ
	ClothesPin(IWorld* world, int laneNum, Vector2 pos);
	//デストラクタ
	~ClothesPin();
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor* other, CollisionParameter colpara);
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);

	void ClearThis() {
		parameter_.isDead = true;
	}
private:
};