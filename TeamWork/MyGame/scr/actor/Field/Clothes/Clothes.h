#pragma once
#include "../../Actor.h"
#include "../MyGame/scr/game/ID.h"

class Clothes : public Actor
{
public:
	//コンストラクタ
	Clothes(IWorld* world, CLOTHES_ID clothes, int laneNum);
	//仮想デストラクタ
	virtual ~Clothes() = default;
	//更新
	virtual void Update() = 0;
	//描画
	virtual void Draw() const = 0;

	//IDの取得
	CLOTHES_ID GetClothesID() const {
		return clothes_ID;
	}
	//風を受けているかの取得
	bool GetIsWind() const {
		return isWind_;
	}

	// 当たり判定処理
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	// メッセージ処理
	virtual void OnMessage(EventMessage message, void* param) override;

	//振り子運動(風用)
	void Pendulum(Vector2 fulcrum, float length);
	//風による服揺らし
	void ShakesClothes();

	//コピー禁止
	Clothes(const Clothes& other) = delete;
	Clothes& operator = (const Clothes& other) = delete;

protected:
	//衝突しているか
	bool isHit_;
	//服ID
	CLOTHES_ID clothes_ID;

	//振り子関連(服用)
	//振り子フラグ
	bool isPendulum_;
	//摩擦が増加か減衰か
	bool isFriction_;
	//風を受けているか
	bool isWind_;
	//支点座標
	Vector2 fulcrum_;
	//角度
	float rot_;
	//角速度
	float rot_spd_;
	//紐の長さ
	float length_;
	//重力加速度
	float gravity_;
	//振り子前の固定位置
	Vector2 basePosition_;
	//摩擦
	float friction_;
	//振り子カウント
	int count_;
};