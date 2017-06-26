#pragma once
#include "../Actor.h"

static const int STAGE_TIP_SIZE = 128;

class ClothesLine : public Actor, public std::enable_shared_from_this<Actor>
{
public:
	//コンストラクタ
	ClothesLine(IWorld* world, int laneNum,const Vector2& endPos, Vector2 pos);
	//デストラクタ
	virtual ~ClothesLine() override;
	//更新
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void StartOnlyLateUpdate()override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor* other, CollisionParameter colpara);
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);

private:
	int blockSize_;

	Vector2 endPos_;

	Vector2 addPos_;
};