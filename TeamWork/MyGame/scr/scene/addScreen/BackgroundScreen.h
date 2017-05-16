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
private:
	void addBGCharacters();
private:
	World* world_;

	int timeCount_;

	std::vector<BackgroundCharacters*> characters_;
};