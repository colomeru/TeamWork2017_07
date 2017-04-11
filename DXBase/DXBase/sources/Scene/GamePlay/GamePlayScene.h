#ifndef _GAME_PLAY_SCENE_H_
#define _GAME_PLAY_SCENE_H_

#include "../Base/IScene.h"
#include <memory>
#include "../../Math/Math.h"
#include"PauseScreen.h"
#include"MoveScreen.h"
#include "../../ResourceLoader/ResourceLoader.h"
#include"ClearScreen.h"

class World;
class BackGraundManager;
class GamePlayScene : public IScene{
public:
	GamePlayScene(SceneDataKeeper* keeper);
	~GamePlayScene();
	virtual void start() override;
	virtual void update() override;
	virtual void draw() const override;
	virtual void end() override;
	virtual bool isEnd() const override;
	virtual Scene next() const override;
private:
	void updateCombo();
	void drawCombo() const;
private:
	using WorldPtr = std::shared_ptr<World>;
	// ワールド
	WorldPtr	world_;

	BackGraundManager* backManager;

	Scene nextScene_;

	float deltaTime_;

	bool isStopped_;

	bool isClearStage_;

	PauseScreen pause_;
	MoveScreen move_;
	ClearScreen clear_;
	
	//ステージ紹介のα
	float stageAlpha_;
	float stageTime_;
	bool stageFlag_;
	std::vector<TextureID> stageTexs_;
	int stageNum_;
	std::vector<TextureID> numberTexes_;

	int previousScoreKeeper_;
	int currentScoreKeeper_;
};

#endif