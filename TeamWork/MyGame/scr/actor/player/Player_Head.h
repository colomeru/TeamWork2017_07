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
	virtual bool CamMoveUpdate();
	virtual void CamMoveUp()override {}
	virtual void CamMoveDown() override;

	virtual void LaneChangeFall() override;
	void addPos(const Vector2& addpos);
	void UpdatePos();
	bool ResurrectHead();
	void StartPlayerHeadBite();

	bool getIsHit()const;
	bool getIsBitePoint()const;
	bool getIsCurrentHead()const;
	void SetPosAddVect(const Vector2& posAV);
	//頭が滑り落ちるかどうかをセットする
	void setIsBiteSlipWind(bool isSlip);
	Player* GetPlayerPointer() const;
	void CreateFatigueEffect();
	void SetBiteSprite();

private:
	float MathHeadRotation_Bite()const;
	float MathHeadRotation_Fall()const;

private:
	//衝突しているか
	bool isHit_;
	//衝突位置にいて、かつ噛み付いていないか
	bool isBitePoint_;
	bool isBiteSlipWind_;
	Player* player_;

	//Vector2 stopPos_;

	int myNumber_;

	float biteSpriteTimer_;

	bool isHitOnce;

	Vector2 posAddVect_;

	int fatigueCheckColor_;

	bool isAlreadyCreateSplash_;

};