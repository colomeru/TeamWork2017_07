#pragma once
#include "../../Base/ActorUI.h"
#include <vector>
struct GaugeState
{
	int Hp=100; //そのゲージのHP
	Vector3 Color; //そのゲージの色
	int GaugeNum; //ゲージ番号
};

class BossGaugeUI : public ActorUI {
public:
	BossGaugeUI(IWorld* world, const Vector2& position);
	~BossGaugeUI();
	virtual void onUpdate(float deltaTime) override;
	virtual void onDraw() const override;

public:
	void SetHp(int hp) {
		mResHp = hp;
	}
private:
	Vector2 mPosition;     //座標
	Vector2 textureSize;   //テクスチャサイズ
	float mHp;               //HP
	float mResHp;            //補間用HP
	float mGaugeHp;          //描写用HP
	float mGaugeNum;         //動かすゲージの番号
	float mTexDivision;    //テクスチャとHPを割ったやつ
	std::vector<GaugeState> gauges;  //ゲージベクター

	//補間関係
	float velo;

};