#pragma once
#include "../../../Actor.h"

//服に付いた糞
class ClothesFeces : public Actor, public std::enable_shared_from_this<Actor>
{
public:
	//コンストラクタ
	ClothesFeces(IWorld* world, int laneNum, Vector2 pos, Actor* clothes);
	//デストラクタ
	virtual ~ClothesFeces() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;

private:
	Vector2 pos_;
	Vector2 fulcrum_;
};