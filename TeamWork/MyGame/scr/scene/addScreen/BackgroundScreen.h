#pragma once
#include <vector>
#include"../Scene.h"
#include"../../math/Vector2.h"
#include"screenSupport\BackgroundCharacters.h"


class World;
class BackgroundScreen
{
public:
	// コンストラクタ
	BackgroundScreen() {}
	BackgroundScreen(World* world);

	void Init();
	void Update();
	// 描画
	void Draw() const;
	void End();
	void addBGCharacters();
private:
	World* world_;

	int timeCount_;

	std::vector<BackgroundCharacters*> characters_;

	std::vector<Vector2> charactersPosition_;
	std::vector<bool> isCharactersPositionUsed_;
};