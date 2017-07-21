#pragma once
#include "../Actor.h"
#include"../player/Player.h"

class Player_Sword : public Actor, public std::enable_shared_from_this<Player_Sword>
{
public:
	//コンストラクタ
	Player_Sword(IWorld* world, Player* targetP, Vector2 pos);
	//デストラクタ
	~Player_Sword();
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
	void SetSwordVel(const Vector2& vel);
	virtual bool CamMoveUpdate();
	virtual void CamMoveUp()override;
	virtual void CamMoveDown() override;

	virtual void LaneChangeFall() override;

	Vector2 GetSwordStartPos() const;
	Vector2 GetSwordEndPos() const;
	void SetUseSword(bool useSword);
	bool GetUseSword()const;

private:
	Player* player_;

	Vector2 swordStartPos_;
	Vector2 swordEndPos_;

	bool useSword_;

};