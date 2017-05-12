#pragma once
#include "../MyGame/scr/actor/Actor.h"

class HairballGenerator : public Actor
{
public:
	// コンストラクタ
	HairballGenerator(IWorld* world, int lane_num);
	// デストラクタ
	~HairballGenerator();
	// 更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	// メッセージ処理
	virtual void OnMessage(EventMessage message, void* param) override;

private:
	Actor* player_;

};