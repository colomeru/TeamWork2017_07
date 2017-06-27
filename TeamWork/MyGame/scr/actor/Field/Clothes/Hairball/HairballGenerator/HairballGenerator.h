#pragma once
#include "../MyGame/scr/actor/Actor.h"
#include "../MyGame/scr/actor/player/Player.h"

class HairballGenerator : public Actor
{
public:
	// コンストラクタ
	HairballGenerator(IWorld* world, int lane_num, Vector2 pos, int frequencyHairball, int hairballCnt);
	// デストラクタ
	virtual ~HairballGenerator() override;
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
	//毛玉生成
	void GenerateHairball();

private:
	Player* player_;

	//毛玉生成する
	int generate_Count_;
	//毛玉が飛んでくる確率
	int frequencyHairball_;
	//毛玉の生成頻度
	int defGenerate_Count_;
	bool is_Generate_;

};