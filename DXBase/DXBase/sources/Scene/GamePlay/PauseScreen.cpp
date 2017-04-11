#include "PauseScreen.h"
#include"../../Input/InputMgr.h"
#include"../../Renderer/FontManager.h"
#include"../../Define.h"
#include"../../ResourceLoader/ResourceLoader.h"

const static int FONT_SIZE = 64;

PauseScreen::PauseScreen()
{

	int listNum = 0;
	listBase.push_back(changeTextList);
	std::vector<std::string> list1;
	list1.push_back("Tボタンでゲームに戻る");
	list1.push_back("〇ボタンでタイトルへ");
	listBase[0] = list1;
	textPosList.push_back(Vector2(200, 800));
	changeTextList.clear();

	textIDs[1] = TextureID::PAUSE_CIRCLE_TEX;
	textIDs[2] = TextureID::PAUSE_START_TEX;

}

bool PauseScreen::update(Scene& next)
{

	if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_CIRCLE)) {
		next = Scene::MainMenu;
		return true;
	}
	return false;
}

void PauseScreen::draw() const
{
	DrawGraph(0, 0,ResourceLoader::GetInstance().getTextureID(TextureID::PAUSE_BACK_TEX), TRUE);

	int center, count, heightPoint;
	//int strLen, strWidth, center, count, heightPoint;
	count = 0;
	heightPoint = 0;
	int forcount = 0;
	center = static_cast<int>(SCREEN_SIZE.x) / 2;


	DrawGraph(center - 320, static_cast<int>(textPosList.at(count).y), ResourceLoader::GetInstance().getTextureID(textIDs.at(1)), TRUE);
	DrawGraph(center - 320, static_cast<int>(textPosList.at(count).y+160), ResourceLoader::GetInstance().getTextureID(textIDs.at(2)), TRUE);

	//for (auto lists : listBase) {
	//	for (auto my : lists) {
	//		if (count == 0)break;

	//		strLen = strlen(my.c_str());
	//		strWidth = GetDrawStringWidthToHandle(my.c_str(), strLen, FontManager::GetInstance().ChangeFont(FontName::GamePlayFont));

	//		//if (forcount == targetPoint && forcount != 0)SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(sin(sinCount*MathHelper::Pi / 180)) * 255));
	//		//DrawStringToHandle(center - (strWidth / 2), textPosList.at(count).y + ((FontManager::GetInstance().GetFontSize(FontName::GamePlayFont))*heightPoint), my.c_str(), GetColor(255, 255, 255), FontManager::GetInstance().ChangeFont(FontName::GamePlayFont));


	//		//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//		heightPoint++;
	//	}
	//	forcount++;
	//	count++;
	//	heightPoint = 0;
	//}


	//int strLen, strWidth, center, count, heightPoint;
	//count = 0;
	//heightPoint = 0;
	//for (auto lists : listBase) {
	//	for (auto my : lists) {
	//		strLen = strlen(my.c_str());
	//		strWidth = GetDrawStringWidthToHandle(my.c_str(), strLen, FontManager::GetInstance().ChangeFont(FontName::GamePlayFont));
	//		center = static_cast<int>(SCREEN_SIZE.x) / 2;
	//		//DrawStringToHandle(center - (strWidth / 2), static_cast<int>(textPosList.at(count).y) + ((FontManager::GetInstance().GetFontSize(FontName::GamePlayFont))*heightPoint), my.c_str(), GetColor(255, 255, 255), FontManager::GetInstance().ChangeFont(FontName::GamePlayFont));
	//		
	//		DrawGraph(center - 320, static_cast<int>(textPosList.at(count).y), ResourceLoader::GetInstance().getTextureID(textIDs.at(count)), TRUE);

	//		heightPoint++;
	//	}
	//	count++;
	//	heightPoint = 0;
	//}
}
