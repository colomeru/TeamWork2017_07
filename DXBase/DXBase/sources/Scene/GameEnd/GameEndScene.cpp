#include "GameEndScene.h"

GameEndScene::GameEndScene(SceneDataKeeper* keeper) {
	isEnd_ = false;
	keeper_ = keeper;
	name_ = "End";


	int listNum = 0;
	listBase.push_back(changeTextList);
	std::vector<std::string> list1;
	list1.push_back("ThankYouForPlaying");
	listBase[0] = list1;
	lastPoses[0] = Vector2(200, 200);
	setPoses[0] = Vector2(0, 500);
	textPosList.push_back(setPoses[0]);
	changeTextList.clear();

}

GameEndScene::~GameEndScene() {}

void GameEndScene::start() {
	isEnd_ = false;

	for (int i = 0; i < static_cast<int>(textPosList.size()); i++) {
		textPosList[i] = setPoses[i];
		shotPos[i] = Vector2::Zero;

	}

	// ï`âÊêÊâÊñ Çó†âÊñ Ç…ÉZÉbÉg
	SetDrawScreen(DX_SCREEN_BACK);

}

void GameEndScene::update() {
	DxLib_End();
	exit(0);

}

void GameEndScene::draw() const {

	//int strLen, strWidth, center, count, heightPoint;
	//count = 0;
	//heightPoint = 0;
	//int forcount = 0;
	//for (auto lists : listBase) {
	//	for (auto my : lists) {
	//		strLen = strlen(my.c_str());
	//		strWidth = GetDrawStringWidthToHandle(my.c_str(), strLen, FontManager::GetInstance().ChangeFont(FontName::GameEndFont));
	//		center = SCREEN_SIZE.x / 2;

	//		DrawStringToHandle(center - (strWidth / 2), textPosList.at(count).y + ((FontManager::GetInstance().GetFontSize(FontName::GameEndFont))*heightPoint), my.c_str(), GetColor(255, 255, 255), FontManager::GetInstance().ChangeFont(FontName::GameEndFont));

	//		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//		heightPoint++;
	//	}
	//	forcount++;
	//	count++;
	//	heightPoint = 0;
	//}
	//if(isArrive.at(1))SetDrawBlendMode(DX_BLENDMODE_ALPHA, abs(sin(sinCount*MathHelper::Pi / 180)) * 255);
	//DrawGraph(textPos.x + 324, textPos.y-28, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_ALPHA_TEX), TRUE);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void GameEndScene::end() {}

bool GameEndScene::isEnd() const {
	return isEnd_;
}

Scene GameEndScene::next() const {
	return Scene::MainMenu;
}
