#pragma once
#include "../Actor.h"

class Player2 : public Actor, public std::enable_shared_from_this<Player2>
{
public:
	//コンストラクタ
	Player2(IWorld* world);
	//デストラクタ
	~Player2();
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

private:
	//回転角度
	//衝突しているか
	bool isHit_;
	float jumpVec;
};