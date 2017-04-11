#pragma once

#include "../Base/IScene.h"
#include "../../Animation/Base/Animation2D.h"
#include <memory>

class World;

class LoadingScene : public IScene {
public:
	LoadingScene(SceneDataKeeper* keeper);
	~LoadingScene();
	virtual void start() override;
	virtual void update() override;
	virtual void draw() const override;
	virtual void end() override;
	virtual bool isEnd() const override;
	virtual Scene next() const override;

private:
	void load_player_res();
	void load_enemy_res();
	void load_boss_res();
	void load_ui_res();
	void load_bgm_res();
	void load_se_res();
private:
	////bool isEnd_;
	Vector2 mPosition;
	float mAngle;
	Animation2D mLoadAnim;
	Animation2D mLoadPuyo;
	int now_;
	int allCount_;
};