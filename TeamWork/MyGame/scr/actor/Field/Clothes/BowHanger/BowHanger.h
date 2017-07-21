#pragma once
#include "../Clothes.h"

//ハンガー
class BowHanger : public Clothes, public std::enable_shared_from_this<BowHanger>
{
public:
	//コンストラクタ
	BowHanger(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos);
	//デストラクタ
	virtual ~BowHanger() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//衝突時のアクション
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param) override;

private:
	//ばねの計算
	void Spring(const Vector2& pos, float stiffnes, float friction, float mass);
	void Cancel();

private:
	bool isMove_;
	bool isPull_;
	Vector2 baseCenter_;
	Vector2 pHeadPos_;
	Vector2 codePos_[6];
	Vector2 codeCenterPos_;
};