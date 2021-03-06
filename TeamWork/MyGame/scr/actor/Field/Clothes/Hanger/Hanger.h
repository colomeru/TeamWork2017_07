#pragma once
#include "../Clothes.h"
#include "../MyGame/scr/actor/player/Player.h"

//ハンガー
class Hanger : public Clothes, public std::enable_shared_from_this<Hanger>
{
public:
	//コンストラクタ
	Hanger(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos);
	//デストラクタ
	virtual ~Hanger() override;
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
	bool isStop_;		//ハンガーの機能が生きているかどうか
};