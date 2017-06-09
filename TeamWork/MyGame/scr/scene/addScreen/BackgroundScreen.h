#pragma once
#include <vector>
#include"../Scene.h"
#include"../../math/Vector2.h"
#include"screenSupport\BackgroundCharacters.h"
#include"../../stageGenerator/Stage.h"

class World;
class BackgroundScreen
{
public:
	// コンストラクタ
	BackgroundScreen() {}
	BackgroundScreen(World* world);

	void Init(Stage currentStage);
	void Update();
	// 描画
	void Draw() const;
	void End();
	void addBGCharacters();
private:
	World* world_;

	Stage currentStage_;
	int timeCount_;

	//std::vector<BackgroundCharacters*> characters_;

	std::vector<Vector2> charactersPosition_;
	std::vector<bool> isCharactersPositionUsed_;
	std::map<int, Vector2> createWindTimeAndPos_;

	std::map<Stage,SPRITE_ID> BGList_;
};