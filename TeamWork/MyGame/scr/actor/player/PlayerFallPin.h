#pragma once
#include "../Actor.h"

class PlayerFallPin : public Actor, public std::enable_shared_from_this<PlayerFallPin>
{
public:
	//コンストラクタ
	PlayerFallPin(IWorld* world, const Vector2& position,const Vector2& toPos);
	//デストラクタ
	~PlayerFallPin();
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor& other, CollisionParameter colpara);
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);

private:
	int sinCount_;
	Vector2 direction_;
	Vector2 gravity_;
	const Vector2 addgravity{ 0.f,0.1f };
};