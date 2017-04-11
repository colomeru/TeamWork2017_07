#include "TeamLogoScene.h"

static const Vector2 LogoSize = Vector2(600,839);

TeamLogoScene::TeamLogoScene(SceneDataKeeper* keeper)
{
	isEnd_ = false;
	keeper_ = keeper;
	name_ = "TeamLogo";

}

TeamLogoScene::~TeamLogoScene() {}

void TeamLogoScene::start() {
	isEnd_ = false;

	timeCount = 2;

	// ï`âÊêÊâÊñ Çó†âÊñ Ç…ÉZÉbÉg
	SetDrawScreen(DX_SCREEN_BACK);

}

void TeamLogoScene::update() {
	Time::GetInstance().update();

	timeCount -= Time::GetInstance().deltaTime();

	if (timeCount < 0) {
		isEnd_ = true;
	}
}


void TeamLogoScene::draw() const {

	DrawGraph(static_cast<int>(SCREEN_SIZE.x/2-(LogoSize.x/2)), static_cast<int>(SCREEN_SIZE.y/2 - (LogoSize.y / 2)), ResourceLoader::GetInstance().getTextureID(TextureID::TEAM_LOGO_TEX), TRUE);

}

void TeamLogoScene::end() {}

bool TeamLogoScene::isEnd() const {
	return isEnd_;
}

Scene TeamLogoScene::next() const {
	return Scene::MainMenu;
}
