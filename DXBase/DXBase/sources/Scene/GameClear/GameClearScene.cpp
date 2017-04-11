#include "GameClearScene.h"
#include"../../ResourceLoader/ResourceLoader.h"

static const int MaxTexSize = 1920;
static const int SlideSpeed = 10;
static const float DefBackSpriteChangeTime = 10.f;
static const int TitleFadeInSpeed = 10;

GameClearScene::GameClearScene(SceneDataKeeper* keeper) :
	id(0) {
	isEnd_ = false;
	keeper_ = keeper;
	name_ = "GameClear";

	nextScene[1] = MainMenu;

	textIDs[1] = TextureID::TEXT_MENUBACK_TEX;

	int listNum = 0;
	listBase.push_back(changeTextList);
	std::vector<std::string> list1;
	list1.push_back("ゲームクリア");
	listBase[0]=list1;
	textPosList.push_back(Vector2(200, 200));
	changeTextList.clear();

	listBase.push_back(changeTextList);
	std::vector<std::string> list2;
	list2.push_back("メニューに戻る");
	listBase[1] = list2;
	textPoses[1] = Vector2(500, 800);
	textPosList.push_back(textPoses[1]);
	changeTextList.clear();
	//OutputDebugString(listBase[0].at(0).c_str());

	currentTitleBackID.push_back(TextureID::TITLE_BACK4_TEX);
	currentTitleBackID.push_back(TextureID::TITLE_BACK4_TEX);

	changeTargetChecker.push_back(0);
	changeTargetChecker.push_back(0);

	baseTitleBackID[0] = TextureID::TITLE_BACK4_TEX;

	numberTexes_.push_back(TextureID::NUMBER_ZERO_TEX);
	numberTexes_.push_back(TextureID::NUMBER_ONE_TEX);
	numberTexes_.push_back(TextureID::NUMBER_TWO_TEX);
	numberTexes_.push_back(TextureID::NUMBER_THREE_TEX);
	numberTexes_.push_back(TextureID::NUMBER_FOUR_TEX);
	numberTexes_.push_back(TextureID::NUMBER_FIVE_TEX);
	numberTexes_.push_back(TextureID::NUMBER_SIX_TEX);
	numberTexes_.push_back(TextureID::NUMBER_SEVEN_TEX);
	numberTexes_.push_back(TextureID::NUMBER_EIGHT_TEX);
	numberTexes_.push_back(TextureID::NUMBER_NINE_TEX);
}

GameClearScene::~GameClearScene() {

}

void GameClearScene::start() {
	targetPoint = 1;
	sinCount = 0;
	slideSize = 0;

	// 描画先画面を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);


	for (int i = 0; i < 2; i++) {
		titleBackStageNum[i] = 1;
		//0が1枚目、1が2枚目で、1枚につきMaxTexSize移動フレーム分の時間をスライドの変更時間に追加する
		titleBackChangeTime[i] = DefBackSpriteChangeTime + ((MaxTexSize / SlideSpeed) / 60 * (i));

			currentTitleBackID[i] = TextureID::TITLE_BACK4_TEX;

		changeBackChecker[i] = false;
	}

	anmer_ = StageClearTextAnm();
	PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGECLEAR), DX_PLAYTYPE_BACK);

	mButtyAnim = PlayerAnimation2D("PlayerBody1");
	mRettyAnim = PlayerAnimation2D("PlayerBody2");
	mButtyAnim.change(PlayerAnimID::SWIM);
	mRettyAnim.change(PlayerAnimID::SWIM);
	mRettyAnim.change_dir(PlayerAnimID::SWIM, ActionType::Left);

	//mCursorPos = Vector2(500, 500);
	mCursorPos = Vector2(SCREEN_SIZE.x / 2 - 128, 500);
}

void GameClearScene::update() {
	
	anmer_.update_e(Time::GetInstance().deltaTime());
	
	sinCount += FlashTempo;
	sinCount = sinCount % 360;
	sinCount = min(max(sinCount, 0), 360);
	
	slideSize += SlideSpeed;
	if (slideSize >= MaxTexSize) {
		slideSize = 0;
	}

	if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_UP)) {
		targetPoint--;
		sinCount = 0;
	}
	if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_DOWN)) {
		targetPoint++;
		sinCount = 0;
	}
	targetPoint = min(max(targetPoint, 1), 1);

	if (isEnd_) return;
	if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_CIRCLE))
	{
		isEnd_ = true;
		PlaySound("./resources/sounds/menuse/menu_decision.mp3", DX_PLAYTYPE_BACK);
	}

	mButtyAnim.update(Time::GetInstance().deltaTime());
	mRettyAnim.update(Time::GetInstance().deltaTime());

	//mCursorPos = Vector2::Lerp(mCursorPos, textPoses.at(targetPoint), 0.5f);
	mCursorPos.y = Vector2::Lerp(mCursorPos, textPoses.at(targetPoint), 0.5f).y;

}

void GameClearScene::draw() const {

	DrawGraph(MaxTexSize - slideSize, 0, ResourceLoader::GetInstance().getTextureID(currentTitleBackID[0]), TRUE);
	DrawGraph(-slideSize, 0, ResourceLoader::GetInstance().getTextureID(currentTitleBackID[1]), TRUE);

	int strLen, strWidth, center, count, heightPoint;
	count = 0;
	heightPoint = 0;
	int forcount = 0;
	center = static_cast<int>(SCREEN_SIZE.x) / 2;

	anmer_.draw_e(Vector2(static_cast<float>(center), textPosList.at(0).y), Vector2::Zero, 1.5f);

	for (auto lists : listBase) {
		for (auto my : lists) {
			if (count == 0)break;

			strLen = strlen(my.c_str());
			strWidth = GetDrawStringWidthToHandle(my.c_str(), strLen, FontManager::GetInstance().ChangeFont(FontName::GamePlayFont));
			
			if (forcount == targetPoint && forcount != 0)SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(sin(sinCount*MathHelper::Pi / 180)) * 255));
			//DrawStringToHandle(center - (strWidth / 2), textPosList.at(count).y + ((FontManager::GetInstance().GetFontSize(FontName::GamePlayFont))*heightPoint), my.c_str(), GetColor(255, 255, 255), FontManager::GetInstance().ChangeFont(FontName::GamePlayFont));

			if (count == targetPoint) {
				DrawRotaGraph2(center, (int)textPosList.at(count).y, (int)(ResourceLoader::GetInstance().GetTextureSize(textIDs.at(count)).x / 2), 0, 1.5, 0, ResourceLoader::GetInstance().getTextureID(textIDs.at(count)), TRUE);
			}
			else if (count>targetPoint) {
				DrawGraph((int)(center - ResourceLoader::GetInstance().GetTextureSize(textIDs.at(count)).x / 2), static_cast<int>(textPosList.at(count).y + ResourceLoader::GetInstance().GetTextureSize(textIDs.at(count)).y / 2), ResourceLoader::GetInstance().getTextureID(textIDs.at(count)), TRUE);
			}
			else {
				DrawGraph((int)(center - ResourceLoader::GetInstance().GetTextureSize(textIDs.at(count)).x / 2), static_cast<int>(textPosList.at(count).y), ResourceLoader::GetInstance().getTextureID(textIDs.at(count)), TRUE);
			}

			//DrawGraph(center - 320, static_cast<int>(textPosList.at(count).y), ResourceLoader::GetInstance().getTextureID(textIDs.at(count)), TRUE);

			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			heightPoint++;
		}
		forcount++;
		count++;
		heightPoint = 0;
	}

	DrawGraph((int)(SCREEN_SIZE.x/2 - ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX).x), 450, ResourceLoader::GetInstance().getTextureID(TextureID::ITEM_TEX), TRUE);
	
	int drawNum = keeper_->GetItemCount();
	int baseNum = keeper_->GetItemCount();
	int posCount = 0;
	std::vector<int> drawNumberList;

	for (int i = 0;;) {

		if (baseNum < 10) {
			drawNumberList.push_back(baseNum);
			//DrawGraph((SCREEN_SIZE.x - CountPos)
			//	- ResourceLoader::GetInstance().GetTextureSize(numberTexes_[baseNum]).x*posCount, 50, ResourceLoader::GetInstance().getTextureID(numberTexes_[baseNum]), TRUE);
			break;
		}

		drawNum = (int)(baseNum*0.1);
		drawNum = drawNum * 10;
		int textNum = baseNum - drawNum;

		drawNumberList.push_back(textNum);
		//DrawGraph((SCREEN_SIZE.x - CountPos) 
		//	- ResourceLoader::GetInstance().GetTextureSize(numberTexes_[textNum]).x*posCount, 50, ResourceLoader::GetInstance().getTextureID(numberTexes_[textNum]), TRUE);

		baseNum = (int)(baseNum*0.1);
		posCount++;
		//DrawFormatString(SCREEN_SIZE.x - 100, 50, GetColor(0, 0, 0), "%d", );
	}
	int drawPosCount = drawNumberList.size() - 1;
	for (int i = 0; i < (int)drawNumberList.size(); i++) {

		DrawGraph((int)((SCREEN_SIZE.x/2)
			+ ResourceLoader::GetInstance().GetTextureSize(numberTexes_[drawNumberList[i]]).x*drawPosCount), 450, ResourceLoader::GetInstance().getTextureID(numberTexes_[drawNumberList[i]]), TRUE);

		drawPosCount--;
	}

	///////////
	//drawNum = keeper_->resultJumpGet();
	//baseNum = keeper_->resultJumpGet();
	posCount = 0;
	drawNumberList.clear();

	for (int i = 0;;) {

		if (baseNum < 10) {
			drawNumberList.push_back(baseNum);
			//DrawGraph((SCREEN_SIZE.x - CountPos)
			//	- ResourceLoader::GetInstance().GetTextureSize(numberTexes_[baseNum]).x*posCount, 50, ResourceLoader::GetInstance().getTextureID(numberTexes_[baseNum]), TRUE);
			break;
		}

		drawNum = (int)(baseNum*0.1);
		drawNum = drawNum * 10;
		int textNum = baseNum - drawNum;

		drawNumberList.push_back(textNum);
		//DrawGraph((SCREEN_SIZE.x - CountPos) 
		//	- ResourceLoader::GetInstance().GetTextureSize(numberTexes_[textNum]).x*posCount, 50, ResourceLoader::GetInstance().getTextureID(numberTexes_[textNum]), TRUE);

		baseNum = (int)(baseNum*0.1);
		posCount++;
		//DrawFormatString(SCREEN_SIZE.x - 100, 50, GetColor(0, 0, 0), "%d", );
	}
	drawPosCount = drawNumberList.size() - 1;
	for (int i = 0; i < (int)drawNumberList.size(); i++) {

		DrawGraph((int)((SCREEN_SIZE.x / 2)
			+ ResourceLoader::GetInstance().GetTextureSize(numberTexes_[drawNumberList[i]]).x*drawPosCount), 570, ResourceLoader::GetInstance().getTextureID(numberTexes_[drawNumberList[i]]), TRUE);

		drawPosCount--;
	}

	drawNum = keeper_->resultDamageGet();
	baseNum = keeper_->resultDamageGet();
	posCount = 0;
	drawNumberList.clear();

	for (int i = 0;;) {

		if (baseNum < 10) {
			drawNumberList.push_back(baseNum);
			//DrawGraph((SCREEN_SIZE.x - CountPos)
			//	- ResourceLoader::GetInstance().GetTextureSize(numberTexes_[baseNum]).x*posCount, 50, ResourceLoader::GetInstance().getTextureID(numberTexes_[baseNum]), TRUE);
			break;
		}

		drawNum = (int)(baseNum*0.1);
		drawNum = drawNum * 10;
		int textNum = baseNum - drawNum;

		drawNumberList.push_back(textNum);
		//DrawGraph((SCREEN_SIZE.x - CountPos) 
		//	- ResourceLoader::GetInstance().GetTextureSize(numberTexes_[textNum]).x*posCount, 50, ResourceLoader::GetInstance().getTextureID(numberTexes_[textNum]), TRUE);

		baseNum = (int)(baseNum*0.1);
		posCount++;
		//DrawFormatString(SCREEN_SIZE.x - 100, 50, GetColor(0, 0, 0), "%d", );
	}
	drawPosCount = drawNumberList.size() - 1;
	for (int i = 0; i < (int)drawNumberList.size(); i++) {

		DrawGraph((int)((SCREEN_SIZE.x / 2)
			+ ResourceLoader::GetInstance().GetTextureSize(numberTexes_[drawNumberList[i]]).x*drawPosCount), 720, ResourceLoader::GetInstance().getTextureID(numberTexes_[drawNumberList[i]]), TRUE);

		drawPosCount--;
	}
	/////////
	//DrawGraph(textPoses.at(targetPoint).x, textPoses.at(targetPoint).y, ResourceLoader::GetInstance().getTextureID(TextureID::SELECT_TARGET_TEX), TRUE);
	//mButtyAnim.draw(mCursorPos, Vector2::Zero, 0.5f);
	//mRettyAnim.draw(mCursorPos + Vector2::Right * 780, Vector2::Zero, 0.5f);
	Vector2 ysetVect = Vector2(0, ResourceLoader::GetInstance().GetTextureSize(textIDs.at(targetPoint)).y / 4);

	mButtyAnim.draw(mCursorPos + Vector2::Left * 420+ysetVect, Vector2::Zero, 0.5f);
	mRettyAnim.draw(mCursorPos + Vector2::Right * 580+ysetVect, Vector2::Zero, 0.5f);
}

void GameClearScene::end() {

}

bool GameClearScene::isEnd() const {
	return isEnd_;
}

Scene GameClearScene::next() const {
	return Scene::MainMenu;
}
