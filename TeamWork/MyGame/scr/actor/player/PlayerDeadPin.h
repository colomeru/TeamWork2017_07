#pragma once
#include "../Actor.h"

class PlayerDeadPin : public Actor, public std::enable_shared_from_this<PlayerDeadPin>
{
public:
	//コンストラクタ
	PlayerDeadPin(IWorld* world, const Vector2& position);
	//デストラクタ
	~PlayerDeadPin();
	virtual void FastUpdate()override {
		laneNum_ = world_->GetKeepDatas().playerLane_;
		isUpdate_ = true;
		isDraw_ = true;
	}
	//更新
	virtual void Update() override;
	virtual void LateUpdate()override {
		isDraw_ = true;

	}
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
	const Vector2 addgravity{0.f,0.1f};
};