#pragma once

#include "../Base/IScene.h"
#include "../../Actor/Base/ActorPtr.h"
#include <memory>
#include <vector>
class World;
class BackGraundManager;
class BonusStage : public IScene {
public:
	BonusStage(SceneDataKeeper* keeper);
	~BonusStage();
	virtual void start() override;
	virtual void update() override;
	virtual void draw() const override;
	virtual void end() override;
	virtual bool isEnd() const override;
	virtual Scene next() const override;

private:
	using WorldPtr = std::shared_ptr<World>;
	// ワールド
	WorldPtr	world_;

	std::vector<int>points_;

	BackGraundManager* backManager;
	Scene nextScene_;
	Vector2 creditSize_;
	Vector2 creditPos_;

	Vector2 kiriTexSize_;
	int point_;
	int drawPoint_;
	int pointCount_;
	float pointRandomTime_;
	float pointDrawTime_;

	float resultAlpha_;

	bool isResult_;

	bool pointDrawFlag_;

	bool scoreEndFlag_;
	bool rankFlag_;
	bool thankFlag_;
	//かける倍数
	int scaleNum;


	//壊れるブロック
	std::vector<ActorPtr>breakBlocks_;
};

