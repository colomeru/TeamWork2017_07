#pragma once
#include "../Actor.h"
#include"../player/Player.h"

class Player_Head : public Actor, public std::enable_shared_from_this<Player_Head>
{
public:
	//コンストラクタ
	Player_Head(IWorld* world,Player* targetP,Vector2 pos,int myNumber);
	//デストラクタ
	~Player_Head();
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor& other, CollisionParameter colpara)override;
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);

	bool getIsHit()const{
		return isHit_;
	}
	bool getIsBitePoint()const{
		return isBitePoint_;
	}
private:
	//衝突しているか
	bool isHit_;
	//衝突位置にいて、かつ噛み付いていないか
	bool isBitePoint_;
	Player* player_;

	Vector2 stopPos_;

	int myNumber_;

	bool isHitOnce;
};