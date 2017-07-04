#pragma once
#include <vector>
#include <List>
#include"../Scene.h"
#include"../../math/Vector2.h"
#include"screenSupport\BackgroundCharacters.h"
#include"../../stageGenerator/Stage.h"

class World;

class BackgroundScreen
{
public:
	struct WindTimeAndPos {
		int windTime_;
		Vector2 windPos_;

		WindTimeAndPos(int time, Vector2 pos) :windTime_(time), windPos_(pos) {

		}

		bool operator <(const WindTimeAndPos &wtap)const {
			return windTime_ < wtap.windTime_;
		}
	};

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

	void DownCeil();
private:
	World* world_;

	Stage currentStage_;
	int timeCount_;
	Vector2 ceilPos;

	std::vector<Vector2> charactersPosition_;
	std::vector<bool> isCharactersPositionUsed_;
	std::list<WindTimeAndPos> createWindTimeAndPos_;
	
	std::map<Stage,SPRITE_ID> BGList_;
};