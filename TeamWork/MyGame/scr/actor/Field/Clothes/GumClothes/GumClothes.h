#pragma once
#include "../Clothes.h"
#include "../MyGame/scr/actor/player/Player_Head.h"

//ゴムの服
class GumClothes : public Clothes, public std::enable_shared_from_this<GumClothes>
{
public:
	//コンストラクタ
	GumClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, bool is_Pin = false);
	//デストラクタ
	~GumClothes();
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
	//Player* player_;
	//Player_Head* player_Head_;
};