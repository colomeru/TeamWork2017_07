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
	~Hanger();
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;

private:

};