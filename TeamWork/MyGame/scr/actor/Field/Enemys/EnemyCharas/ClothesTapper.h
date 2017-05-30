#pragma once
#include"../Enemys.h"
#include "../MyGame/scr/actor/player/Player.h"

enum {

	MODE_MOVE = 0,
	MODE_TAP = 1,
	MODE_IDLE=2

};

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
	void PlayTap();
	void ToTapMode();
	void ToMoveMode();
	void ToIdleMode();

private:
	void MoveUpdate();
	void TapUpdate();
	void IdleUpdate();
private:
	Player* player_;
	Actor* player_Head_;
	Vector2 basePos_;
	Vector2 targetPos_;
	float timeCount_;
	
	int updateMode_;
	SPRITE_ID spriteID_;
	std::map<int, std::function<void()>> updateFunctionMap_;

};