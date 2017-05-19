#pragma once
#include <vector>
#include"../Scene.h"
#include"../../math/Vector2.h"
#include"screenSupport\BackgroundCharacters.h"


class World;
class BackgroundScreen
{
public:
	// �R���X�g���N�^
	BackgroundScreen() {}
	BackgroundScreen(World* world);

	void Init();
	void Update();
	// �`��
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