#pragma once

#include "../actor/Actor.h"
#include "../math/Vector2.h"
class PlayerLine : public Actor, public std::enable_shared_from_this<PlayerLine>
{
public:
	//コンストラクタ
	PlayerLine(IWorld* world);
	//デストラクタ
	~PlayerLine();
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
	float angle_;
	//衝突しているか
	bool isHit_;
	
	Vector2 line1_;
	Vector2 head1_;
	
	Vector2 line2_;
	Vector2 head2_;

	Vector2 pos_;
};