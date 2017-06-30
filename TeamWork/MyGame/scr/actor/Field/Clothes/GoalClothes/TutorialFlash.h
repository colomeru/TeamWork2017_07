#pragma once
#include "../MyGame/scr/actor/Actor.h"

//チュートリアル用の光
class TutorialFlash : public Actor, public std::enable_shared_from_this<Actor>
{
public:
	//コンストラクタ
	TutorialFlash(IWorld* world, int laneNum, Vector2 pos, Actor* clothes);
	//デストラクタ
	virtual ~TutorialFlash() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;

private:
};