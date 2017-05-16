#pragma once
#include "Clothes.h"

class TestClothes : public Clothes, public std::enable_shared_from_this<TestClothes>
{
public:
	//コンストラクタ
	TestClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, bool is_Pin = false);
	//デストラクタ
	~TestClothes();
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);

private:
};