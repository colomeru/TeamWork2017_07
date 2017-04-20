#pragma once
#include "../Actor.h"
#include "../../math/Vector2.h"

class Player3 : public Actor, public std::enable_shared_from_this<Player3>
{
public:
	//コンストラクタ
	Player3(IWorld* world);
	//デストラクタ
	~Player3();
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor& other, CollisionParameter colpara);
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);

	//振り子運動
	void Pendulum(Vector2 fulcrum, float length);

private:
	//回転角度
	//衝突しているか
	bool isHit_;
	float jumpVec;

	//振り子関連
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
};