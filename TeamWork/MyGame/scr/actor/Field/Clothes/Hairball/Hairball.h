#pragma once
#include "../Clothes.h"
#include "../MyGame/scr/actor/player/Player.h"

class Hairball : public Clothes, public std::enable_shared_from_this<Hairball>
{
public:
	//コンストラクタ
	Hairball(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos);
	//デストラクタ
	~Hairball();
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);

private:
	Player* player_;
	Actor* player_Head_;
};