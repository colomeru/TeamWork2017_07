#pragma once
#include "../Clothes.h"
#include "../MyGame/scr/actor/player/Player.h"

//ゴール
class GoalClothes: public Clothes, public std::enable_shared_from_this<GoalClothes>
{
public:
	//コンストラクタ
	GoalClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos);
	//デストラクタ
	~GoalClothes();
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param) override;

private:
	Player* player_;
	Player_Head* player_Head_;
};