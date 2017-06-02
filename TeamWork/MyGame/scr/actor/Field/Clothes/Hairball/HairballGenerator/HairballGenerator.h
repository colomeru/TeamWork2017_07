#pragma once
#include "../MyGame/scr/actor/Actor.h"
#include "../MyGame/scr/actor/player/Player.h"

class HairballGenerator : public Actor
{
public:
	// コンストラクタ
	HairballGenerator(IWorld* world, int lane_num, Vector2 pos, int hairballCnt);
	// デストラクタ
	~HairballGenerator();
	// 更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	// 受動更新
	virtual void OnUpdate() override;
	// 当たり判定処理
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	// メッセージ処理
	virtual void OnMessage(EventMessage message, void* param) override;
	
private:
	void GenerateHairball();

private:
	Player* player_;

	int generate_Count_;
	int defGenerate_Count_;
	bool is_Generate_;

};