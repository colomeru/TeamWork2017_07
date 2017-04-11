#include "CreditScene.h"
#include"../../ResourceLoader/ResourceLoader.h"
#include"../../Game/Time.h"

CreditScene::CreditScene(SceneDataKeeper* keeper) :
	id(0), sinCount(0),isPlay_(false),timeCount(3) {
	isEnd_ = false;
	keeper_ = keeper;
	name_ = "Credit";


	//int listNum = 0;
	//listBase.push_back(changeTextList);
	//std::vector<std::string> list1;
	//list1.push_back("タイトル");
	//listBase[0] = list1;
	//lastPoses[0] = Vector2(200, 200);
	//setPoses[0] = Vector2(0, titleY);
	//textPosList.push_back(setPoses[0]);
	//changeTextList.clear();

	//listBase.push_back(changeTextList);
	//std::vector<std::string> list2;
	//list2.push_back("SPACEボタンを押してください");
	//listBase[1] = list2;
	//textPos = Vector2(200, 600);
	//lastPoses[1] = textPos;
	//setPoses[1] = Vector2(0, spaceY);
	//textPosList.push_back(setPoses[1]);
	//changeTextList.clear();

}

CreditScene::~CreditScene() {}

void CreditScene::start() {
	isEnd_ = false;

	isPlay_ = false;
	// 描画先画面を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);
	timeCount = 3;

	PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_MENU), DX_PLAYTYPE_LOOP);

	creditPosY = 1080;
}

void CreditScene::update() {
	sinCount += FlashTempo;
	sinCount = sinCount % 360;
	sinCount = min(max(sinCount, 0), 360);

	creditPosY-=2;

	if (creditPosY<-3300)
	{
		isEnd_ = true;
	}

	//timeCount -= static_cast<int>(Time::GetInstance().deltaTime());

	//if (!isPlay_) {
	//	isPlay_ = true;
	//	PlayMovie("./resources/file/pv.avi", 1, DX_MOVIEPLAYTYPE_BCANCEL);
	//}

	//if (timeCount > 0)return;
	if (InputMgr::GetInstance().IsPushButton()) {
		isEnd_=true;
	}
	//if (GetMovieStateToGraph())
	{
		//isEnd_ = true;
	}

	{
		//textPosList[1].y += 10;
		//textPosList[1].y = min(max(textPosList[1].y, setPoses[1].y), lastPoses[1].y);
	}

}

void CreditScene::draw() const {
	DrawGraph(0, creditPosY, ResourceLoader::GetInstance().getTextureID(TextureID::CREDIT_TEX), TRUE);
}

void CreditScene::end() {
	StopSoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_MENU));
}

bool CreditScene::isEnd() const {
	return isEnd_;
}

Scene CreditScene::next() const {
	return Scene::MainMenu;
}
