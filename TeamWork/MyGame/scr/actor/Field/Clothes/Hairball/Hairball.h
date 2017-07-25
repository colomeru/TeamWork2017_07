#pragma once
#include "../Clothes.h"
#include "../MyGame/scr/actor/player/Player_Head.h"

class Hairball : public Clothes, public std::enable_shared_from_this<Hairball>
{
public:
	//コンストラクタ
	Hairball(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos);
	//デストラクタ
	virtual ~Hairball() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//衝突時のアクション
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param) override;

private:
	Player_Head* player_Head_;
};