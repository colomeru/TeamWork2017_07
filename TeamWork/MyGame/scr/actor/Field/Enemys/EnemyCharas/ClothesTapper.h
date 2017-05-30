#pragma once
#include"../Enemys.h"
#include "../MyGame/scr/actor/player/Player.h"

class ClothesTapper : public Enemys, public std::enable_shared_from_this<ClothesTapper>
{
public:
	//コンストラクタ
	ClothesTapper(IWorld* world, int laneNum, Vector2 pos);
	//デストラクタ
	~ClothesTapper();
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
	void SetNextTapPos(const Vector2& pos=Vector2::Zero);

private:
	Player* player_;
	Actor* player_Head_;
	Vector2 basePos_;
	Vector2 targetPos_;
	float timeCount_;
};