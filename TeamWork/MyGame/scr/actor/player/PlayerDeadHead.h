#pragma once
#include "../Actor.h"

class PlayerDeadHead : public Actor, public std::enable_shared_from_this<PlayerDeadHead>
{
public:
	//コンストラクタ
	PlayerDeadHead(IWorld* world, const Vector2& position);
	//デストラクタ
	~PlayerDeadHead();
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
};