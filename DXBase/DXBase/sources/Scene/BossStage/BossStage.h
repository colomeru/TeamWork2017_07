#ifndef BOSS_STAGE_H_
#define BOSS_STAGE_H_

#include "../Base/IScene.h"
#include <memory>
#include "../../Math/Math.h"
#include"../GamePlay/PauseScreen.h"
#include"../GamePlay/MoveScreen.h"
#include "../../Animation/Base/Animation2D.h"
#include "../../Actor/Door/Door.h"
#include "../../Animation/Base/Animation2D.h"
#include "../../Actor/Item/ItemAnm.h"
#include "../../Actor/Item/StarEffectManager.h"

class World;
class BackGraundManager;
class BaseBoss;
class StarEffect;

class BossStage : public IScene {
public:
	BossStage(SceneDataKeeper* keeper);
	~BossStage();
	virtual void start() override;
	virtual void update() override;
	virtual void draw() const override;
	virtual void end() override;
	virtual bool isEnd() const override;
	virtual Scene next() const override;

	void BossChildUpdate();
private:
	using WorldPtr = std::shared_ptr<World>;
	// ワールド
	WorldPtr	world_;

	BackGraundManager* backManager;
	BaseBoss* boss_;				// ボス
	Door* door_;

	Scene nextScene_;

	float deltaTime_;

	bool isStopped_;

	float mIvemtTime;

	PauseScreen pause_;
	MoveScreen move_;

	ActorPtr player;

	Animation2D hatenaAnm_;
	// スターマネージャー
	StarEffectManager starEffectManager_;

	ItemAnm anmer_;
	std::vector<float> rotateRange_;

	int maxStarCount_;
	int myStarCount_;

	//子分関係
	float bossChildTimer_;
	std::vector<ActorPtr> repairs_;
	std::vector<ActorPtr> blocks_;

	bool seBokoFlag_;
	bool seBreakFlag_;
};

#endif
