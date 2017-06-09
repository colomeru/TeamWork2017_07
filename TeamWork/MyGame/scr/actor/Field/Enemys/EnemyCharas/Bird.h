#pragma once
#include"../Enemys.h"
#include "../MyGame/scr/actor/player/Player.h"
#include"CharacterAnmManager.h"

class Bird : public Enemys, public std::enable_shared_from_this<Bird>
{
public:
	//コンストラクタ
	Bird(IWorld* world, int laneNum, Vector2 pos);
	//デストラクタ
	~Bird();
	virtual void FastUpdate()override {
		isUpdate_ = true;
	}
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
	float timeCount_;
	int dropTimer_;
	Vector2 basePos_;

	CharacterAnmManager anmManager_;
};