#include "MainMenuScene.h"
#include"../../ResourceLoader/ResourceLoader.h"
#include "../../Actor/BackGraundManager/BackGraundManager.h"


static const float shotSpeed = 5;
static const int boundCount = 5;
static const int TitleMainFadeInSpeed = 15;
static const int SelectPointerFadeInSpeed = 15;
static const int DefAlphaSlideCount = 100;
static const float titleY = 10;
static const float spaceY = 400;
static const float boundPower = 30;
static const int MaxTexSize = 1920;
static const int SlideSpeed = 7;
static const float DefBackSpriteChangeTime = 10.f;
static const int TitleFadeInSpeed = 10;
static const int alphaspeed = 5;
static const int center = static_cast<int>(SCREEN_SIZE.x) / 2;

MainMenuScene::MainMenuScene(SceneDataKeeper* keeper) :
	id(0),sinCount(0),targetPoint(1) {
	isEnd_ = false;
	keeper_ = keeper;
	name_ = "MainMenu";

	alphadefSpeeds[0] = TitleMainFadeInSpeed;
	alphadefSpeeds[1] = SelectPointerFadeInSpeed;

	textIDs[0] = TextureID::TEXT_TITLE_TEX;
	textIDs[1] = TextureID::TEXT_GAMESTART_TEX;
	textIDs[2] = TextureID::TEXT_TUTORIAL_TEX;
	textIDs[3] = TextureID::TEXT_CREDIT_TEX;
	textIDs[4] = TextureID::TEXT_GAMEEND_TEX;
	textIDs[5] = TextureID::TEXT_MARU_TEX;
	textIDs[6] = TextureID::TUTO_SKIP_TEX;
	textIDs[7] = TextureID::TUTO_YES_TEX;
	textIDs[8] = TextureID::TUTO_NO_TEX;

	defposlist[0] = 0;
	defposlist[1] = 340;
	defposlist[2] = 420;
	defposlist[3] = 500;
	defposlist[4] = 620;
	defposlist[5] = 450;

	nextScene[1] = FirstGamePlay;
	nextScene[2] = Tutoreal;
	nextScene[3] = Credit;
	nextScene[4] = GameEnd;
	nextScene[5] = GamePlay;



	int listNum = 0;
	listBase.push_back(changeTextList);
	std::vector<std::string> list1;
	list1.push_back("タイトル");
	listBase[0] = list1;
	
	lastPoses[0] = Vector2(200, 200);
	setPoses[0] = Vector2(0, static_cast<float>(defposlist[0]));

	textPosList.push_back(Vector2(200,200));
	changeTextList.clear();


	listBase.push_back(changeTextList);
	std::vector<std::string> list2;
	list2.push_back("ゲームスタート");
	listBase[1] = list2;
	textPoses[1] = Vector2(500, 500);
	textPosList.push_back(textPoses[1]);
	changeTextList.clear();

	lastPoses[1] = Vector2(200,500);
	setPoses[1] = Vector2(0, static_cast<float>(defposlist[1]));

	listBase.push_back(changeTextList);
	std::vector<std::string> list3;
	list3.push_back("チュートリアル");
	listBase[2] = list3;
	textPoses[2] = Vector2(500, 600);
	textPosList.push_back(textPoses[2]);
	changeTextList.clear();

	lastPoses[2] = Vector2(200,600);
	setPoses[2] = Vector2(0, static_cast<float>(defposlist[2]));

	listBase.push_back(changeTextList);
	std::vector<std::string> list4;
	list4.push_back("クレジット");
	listBase[3] = list4;
	textPoses[3] = Vector2(500, 700);
	textPosList.push_back(textPoses[3]);
	changeTextList.clear();

	lastPoses[3] = Vector2(200, 700);
	setPoses[3] = Vector2(0, static_cast<float>(defposlist[3]));

	listBase.push_back(changeTextList);
	std::vector<std::string> list5;
	list5.push_back("ゲーム終了");
	listBase[4] = list5;
	textPoses[4] = Vector2(500, 800);
	textPosList.push_back(textPoses[4]);
	changeTextList.clear();

	lastPoses[4] = Vector2(200, 800);
	setPoses[4] = Vector2(0, static_cast<float>(defposlist[4]));



	currentTitleBackID.push_back(TextureID::TITLE_BACK_TEX);
	currentTitleBackID.push_back(TextureID::TITLE_BACK_TEX);

	changeTargetChecker.push_back(0);
	changeTargetChecker.push_back(0);
	
	baseTitleBackID[0] = TextureID::TITLE_BACK_TEX;
	baseTitleBackID[1] = TextureID::TITLE_BACK2_TEX;
	baseTitleBackID[2] = TextureID::TITLE_BACK_TEX;



	listBase.push_back(changeTextList);
	std::vector<std::string>  list6;
	list6.push_back("〇ボタンを押してください");
	listBase[5] = list6;
	textPoses[5] = Vector2(500, 600);
	lastPoses[5] = textPoses[5];
	setPoses[5] = Vector2(0, spaceY);
	textPosList.push_back(setPoses[5]);
	changeTextList.clear();

}

MainMenuScene::~MainMenuScene() {}

void MainMenuScene::start() {
	isEnd_ = false;
	isTitle_ = true;
	isSelectGamePlay_ = false;
	isDrawAlphaBack_ = false;
	oneFlag_ = true;
	backManager = new BackGraundManager();
	backNum_ = 0;

	alphaCou[0] = 0;
	alphaCou[1] = 0;

	titleBackAlpha_ = 0;
	alphaSlideCount_ = 0.0f;

	targetPoint = 1;

	sinCount = 0;
	//背景のID設定
	BackTitles id;
	id.push_back(TextureID::BACKSTAGE1_1_TEX);
	id.push_back(TextureID::BACKSTAGE1_2_TEX);
	id.push_back(TextureID::BACKSTAGE1_3_TEX);
	id.push_back(TextureID::BACKSTAGE1_4_TEX);
	id.push_back(TextureID::BACKSTAGE1_5_TEX);
	id.push_back(TextureID::BACKSTAGE1_6_1_TEX);
	id.push_back(TextureID::BACKSTAGE1_6_1_TEX);
	id.push_back(TextureID::BACKSTAGE1_7_TEX);
	id.push_back(TextureID::BACKSTAGE1_8_TEX);
	BackTitles id2;
	id2.push_back(TextureID::BACKSTAGE2_TOP1_TEX);
	id2.push_back(TextureID::BACKSTAGE2_1_TEX);
	id2.push_back(TextureID::BACKSTAGE2_2_TEX);
	id2.push_back(TextureID::BACKSTAGE2_3_TEX);
	id2.push_back(TextureID::BACKSTAGE2_4_TEX);
	id2.push_back(TextureID::BACKSTAGE2_TOP2_TEX);
	id2.push_back(TextureID::BACKSTAGE2_5_TEX);
	id2.push_back(TextureID::BACKSTAGE2_6_TEX);
	id2.push_back(TextureID::BACKSTAGE2_7_TEX);
	id2.push_back(TextureID::BACKSTAGE2_8_TEX);
	/*id2.push_back(TextureID::BACKSTAGE2_9_TEX);
	id2.push_back(TextureID::BACKSTAGE2_10_TEX);*/
	BackTitles id3;
	id3.push_back(TextureID::BACKSTAGE4_1_TEX);
	id3.push_back(TextureID::BACKSTAGE4_2_TEX);

	titleTexs.push_back(id);
	titleTexs.push_back(id2);
	titleTexs.push_back(id3);
	//backManagerの初期化
	for (int i = 0; i < (int)titleTexs[0].size(); i++) {
		backManager->SetBackGraund(titleTexs[0][i], titleTexs[0][i]);
	}



	for (int i = 0; i < static_cast<int>(textPosList.size()); i++) {
		textPosList[i] = setPoses[i];
		shotPos[i] = Vector2::Zero;
		isPoint[i] = false;
		isArrive[i] = false;
		isShotArrive[i] = false;
		boundCou[i] = boundCount;

	}
	for (int i = 0; i < 2; i++) {
		titleBackStageNum[i] = 1;
		//0が1枚目、1が2枚目で、1枚につきMaxTexSize移動フレーム分の時間をスライドの変更時間に追加する
		titleBackChangeTime[i] = DefBackSpriteChangeTime + ((MaxTexSize / SlideSpeed) / 60 * (i));

		currentTitleBackID[i] = TextureID::TITLE_BACK_TEX;

		changeBackChecker[i] = false;
	}

	// 描画先画面を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);
	//for (int i = 0; i < 3; i++) {
	//	textPosList[i + 1].y = textPoses[i + 1].y;
	//}

	PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_MENU), DX_PLAYTYPE_LOOP);
	//PlaySoundFile("./resources/file/game_menuBGM.mp3", DX_PLAYTYPE_LOOP);

	mButtyAnim = PlayerAnimation2D("PlayerBody1");
	mRettyAnim = PlayerAnimation2D("PlayerBody2");
	mButtyAnim.change(PlayerAnimID::SWIM);
	mRettyAnim.change(PlayerAnimID::SWIM);
	mRettyAnim.change_dir(PlayerAnimID::SWIM, ActionType::Left);

	mCursorPos = Vector2(SCREEN_SIZE.x/2-128, 500);

	//keeper_->ItemReset();
	//keeper_->resultJumpReset();
	//keeper_->resultDamageReset();
	keeper_->resetDatas();

	gamePlaySlimePos_ = Vector2(center, 500);
}

void MainMenuScene::update() {

	
	Vector2 alphasize= ResourceLoader::GetInstance().GetTextureSize(TextureID::TITLE_BACK_ALPHA_TEX);

	sinCount += FlashTempo;
	sinCount = sinCount % 360;
	sinCount = min(max(sinCount, 0), 360);

	alphaSlideCount_ += Time::GetInstance().deltaTime();

	//5秒に一回白くなる
	if (alphaSlideCount_ >= 10.0f) {
		titleBackAlpha_ += Time::GetInstance().deltaTime();
		//完全に白くなったら背景を交換
		if (oneFlag_&&titleBackAlpha_ >= 0.5f) {
			//背景番号を変更
			backNum_++;
			//現在の背景を全部消す
			backManager->AllDeleteBackGraund();
			//新しい背景をセット
			for (int i = 0; i < (int)titleTexs[backNum_].size(); i++) {
				backManager->SetBackGraund(titleTexs[backNum_][i], titleTexs[backNum_][i]);
			}
			//一回入ったら次が来るまで入らないようにする
			oneFlag_ = false;
		}
		if (titleBackAlpha_ >= 1.0f) {
			//一周したら戻す
			if (backNum_ == 2) backNum_ = -1;
			//戻す
			alphaSlideCount_ = 0.0f;
			titleBackAlpha_ = 0.0f;
			oneFlag_ = true;
		}
	}
	//背景Update
	backManager->Update(0, true);

	if (isEnd_)return;

	if (!isTitle_)
	{
		if (isSelectGamePlay_) {
			if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_UP)) {
				targetPoint=1;
				sinCount = 0;
				PlaySound("./resources/sounds/menuse/menu_cursor.mp3", DX_PLAYTYPE_BACK);
			
				gamePlaySlimePos_ = Vector2(center, 500);
			}
			if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_DOWN)) {
				targetPoint=5;
				sinCount = 0;
				PlaySound("./resources/sounds/menuse/menu_cursor.mp3", DX_PLAYTYPE_BACK);
			
				gamePlaySlimePos_ = Vector2(center, 700);
			}
			if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_CIRCLE))
			{
				PlaySound("./resources/sounds/menuse/menu_decision.mp3", DX_PLAYTYPE_BACK);
				keeper_->setSceneName("stage04");
				isEnd_ = true;
			}

			mButtyAnim.update(Time::GetInstance().deltaTime());
			mRettyAnim.update(Time::GetInstance().deltaTime());

			mCursorPos = gamePlaySlimePos_;
		}
		else {
			if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_UP)) {
				targetPoint--;
				sinCount = 0;
				PlaySound("./resources/sounds/menuse/menu_cursor.mp3", DX_PLAYTYPE_BACK);

			}
			if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_DOWN)) {
				targetPoint++;
				sinCount = 0;
				PlaySound("./resources/sounds/menuse/menu_cursor.mp3", DX_PLAYTYPE_BACK);
			}
			targetPoint = min(max(targetPoint, 1), 4);

			if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_CIRCLE))
			{
				PlaySound("./resources/sounds/menuse/menu_decision.mp3", DX_PLAYTYPE_BACK);
				if (targetPoint == 1) {
					isSelectGamePlay_ = true;
					keeper_->setSceneName("stage04");
				}
				else isEnd_ = true;
			}

			if (alphaCou[1] <= 255 && isArrive.at(0)) {
				alphaCou[1] += alphadefSpeeds[1];
				alphaCou[1] = min(max(alphaCou[1], 0), 255);
			}
			if (alphaCou[0] <= 255) {
				alphaCou[0] += alphadefSpeeds[0];
				alphaCou[0] = min(max(alphaCou[0], 0), 255);
			}
		}
	}
	else {
		if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_CIRCLE)) {
			isTitle_ = false;
			PlaySound("./resources/sounds/menuse/menu_decision.mp3", DX_PLAYTYPE_BACK);
		}
	}
	if (!isSelectGamePlay_) {
		for (int i = 0; i < 5; i++)
		{
			isPoint[i] ? moveText(i) : slideText(i);
		}
		isPoint[5] ? moveText(5) : slideText(5);
		mButtyAnim.update(Time::GetInstance().deltaTime());
		mRettyAnim.update(Time::GetInstance().deltaTime());

		mCursorPos.y = Vector2::Lerp(mCursorPos, textPoses.at(targetPoint), 0.5f).y;
	}

}
void MainMenuScene::slideText(int targettext)
{
	if (isArrive[targettext]) return;

	textPosList[targettext].y += 10;
	textPosList[targettext].y = min(max(textPosList[targettext].y, setPoses[targettext].y), lastPoses[targettext].y);

	if (textPosList[targettext].y == lastPoses[targettext].y) isPoint[targettext] = true;
}

void MainMenuScene::moveText(int targettext)
{
	if (isArrive[targettext]) return;


	if (!isShotArrive[targettext]) {
		boundCou[targettext]--;
		boundCou[targettext] = min(max(boundCou[targettext], 0), boundCount);

		//shotPos[targettext].y =(defposlist[targettext])-abs(((boundPower / 3)*boundCou[targettext]));
		shotPos[targettext].y = lastPoses[targettext].y - (lastPoses[targettext].y - defposlist[targettext]) + ((boundCount - boundCou[targettext]) * 50);
		isShotArrive[targettext] = true;
	}


	if (textPosList[targettext].y <= shotPos[targettext].y) {
		isShotArrive[targettext] = false;
		isPoint[targettext] = false;

	}
	else {
		textPosList[targettext].y -= (shotSpeed*(boundCou[targettext] + 1));
	}

	if (!isPoint[targettext] && boundCou[targettext] <= 0) {
		isArrive[targettext] = true;
		textPosList[targettext].y = lastPoses[targettext].y;
	}

}

void MainMenuScene::draw() const {

	//DrawGraph(MaxTexSize - slideSize, 0, ResourceLoader::GetInstance().getTextureID(currentTitleBackID[0]), TRUE);
	//DrawGraph(-slideSize, 0, ResourceLoader::GetInstance().getTextureID(currentTitleBackID[1]), TRUE);
	
	//背景Draw
	backManager->Draw(true);
	//α値を0から180で線形補間をしてそれをsinに持っていきその結果を0から255に線形補間する
	float alpha = MathHelper::Lerp(0.0f, 255.0f, MathHelper::Sin(MathHelper::Lerp(0, 180, titleBackAlpha_)));
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha);
	/*if (isDrawAlphaBack_)*/DrawGraph(0, 0, ResourceLoader::GetInstance().getTextureID(TextureID::TITLE_BACK_ALPHA_TEX), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int strLen, strWidth, count, heightPoint;
	count = 0;
	heightPoint = 0;
	int forcount = 0;

	if (isSelectGamePlay_) {
		float sizeList[2];
		if (targetPoint == 1) {
			sizeList[0] = 1.f;
			sizeList[1] = 0.7f;
		}
		if (targetPoint == 5) {
			sizeList[0] = 0.7f;
			sizeList[1] = 1.f;

		}
		DrawRotaGraph2(center, 0, (int)(ResourceLoader::GetInstance().GetTextureSize(textIDs.at(6)).x / 2), 0, 1, 0, ResourceLoader::GetInstance().getTextureID(textIDs.at(6)), TRUE);
		DrawRotaGraph2(center, 500, (int)(ResourceLoader::GetInstance().GetTextureSize(textIDs.at(7)).x / 2), (int)(ResourceLoader::GetInstance().GetTextureSize(textIDs.at(7)).y / 2), sizeList[0], 0, ResourceLoader::GetInstance().getTextureID(textIDs.at(7)), TRUE);
		DrawRotaGraph2(center, 700, (int)(ResourceLoader::GetInstance().GetTextureSize(textIDs.at(8)).x / 2), (int)(ResourceLoader::GetInstance().GetTextureSize(textIDs.at(8)).y / 2), sizeList[1], 0, ResourceLoader::GetInstance().getTextureID(textIDs.at(8)), TRUE);

		Vector2 setVect = ResourceLoader::GetInstance().GetTextureSize(AnimationID::PLAYER_RETTY_IDLE)/2;
		setVect.y -= 50;
		mButtyAnim.draw(mCursorPos + Vector2::Left * 420-setVect, Vector2::Zero, 0.5f);
		mRettyAnim.draw(mCursorPos + Vector2::Right * 420-setVect, Vector2::Zero, 0.5f);

		return;
	}
	else{
		for (auto lists : listBase) {
			for (auto my : lists) {
				if (isTitle_) {
					if (count > 0 && count < 5) {
						continue;
					}
				}
				else if (!isTitle_&& count >= 5)break;
				strLen = strlen(my.c_str());
				strWidth = GetDrawStringWidthToHandle(my.c_str(), strLen, FontManager::GetInstance().ChangeFont(FontName::GamePlayFont));
				if (forcount == targetPoint&&isArrive.at(0))SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(sin(sinCount*MathHelper::Pi / 180)) * 255));

				if (count == 5)SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(sin(sinCount*MathHelper::Pi / 180)) * 255));
				if (count == 0) {
					int psizex = 80;
					int psizey = 80;
					DrawExtendGraph((int)(center - (748 + psizex / 2)), (int)textPosList.at(count).y, (int)(center + (748 + psizex / 2)), (int)(textPosList.at(count).y + 155 + psizey), ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_TITLE_TEX), TRUE);
				}
				else if (count == targetPoint) {
					DrawRotaGraph2(center, (int)textPosList.at(count).y, (int)(ResourceLoader::GetInstance().GetTextureSize(textIDs.at(count)).x / 2), 0, 1.5, 0, ResourceLoader::GetInstance().getTextureID(textIDs.at(count)), TRUE);
				}
				else if (count > targetPoint) {
					DrawGraph((int)(center - ResourceLoader::GetInstance().GetTextureSize(textIDs.at(count)).x / 2), static_cast<int>(textPosList.at(count).y + ResourceLoader::GetInstance().GetTextureSize(textIDs.at(count)).y / 2), ResourceLoader::GetInstance().getTextureID(textIDs.at(count)), TRUE);
				}
				else {
					DrawGraph((int)(center - ResourceLoader::GetInstance().GetTextureSize(textIDs.at(count)).x / 2), static_cast<int>(textPosList.at(count).y), ResourceLoader::GetInstance().getTextureID(textIDs.at(count)), TRUE);
				}
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				heightPoint++;
			}
			forcount++;
			count++;
			heightPoint = 0;
		}
	}
	if (alphaCou[1]<255) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alphaCou[1]);
	}

	Vector2 ysetVect = Vector2(0, ResourceLoader::GetInstance().GetTextureSize(textIDs.at(targetPoint)).y/4);
	
	mButtyAnim.draw(mCursorPos + Vector2::Left * 420+ ysetVect, Vector2::Zero, 0.5f);
	mRettyAnim.draw(mCursorPos + Vector2::Right * 580+ ysetVect, Vector2::Zero, 0.5f);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void MainMenuScene::end() {
	delete backManager;
	StopSoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_MENU));
}

bool MainMenuScene::isEnd() const {
	return isEnd_;
}

Scene MainMenuScene::next() const {
	return nextScene.at(targetPoint);
}
