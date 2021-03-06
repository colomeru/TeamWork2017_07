#include "AllClearScreen.h"
#include"../../graphic/Sprite.h"
#include"../../Def.h"
#include"../../input/Keyboard.h"
#include"../../input/GamePad.h"
#include"../../math/MathHelper.h"
#include"../GamePlayDefine.h"
#include"../../tween/TweenManager.h"
#include"screenSupport\DrawScore.h"
#include"../../sound/sound.h"
#include"../../fade/FadePanel.h"

AllClearScreen::AllClearScreen() :inputCount_(0), sinCount_(defSinC), fscore_(0.f), fheadCount_(0.f)
{
	changeSceneList_.push_back(Scene::Title);

	textAlphaList_.push_back(1.f);

	textSizeList_.push_back(1.f);

	cursorPos_.push_back(Vector2(400, 800));

	stageNumList_[Stage::Stage1] = 0;
	stageNumList_[Stage::Stage2] = 1;
	stageNumList_[Stage::Stage3] = 2;
	stageNumList_[Stage::Stage4] = 3;
	stageNumList_[Stage::Stage5] = 4;
	stageNumList_[Stage::Stage6] = 5;
	stageNumList_[Stage::Stage7] = 6;
	stageNumList_[Stage::Stage8] = 7;

	cursorDrawPos_ = cursorPos_[inputCount_];
}

AllClearScreen::~AllClearScreen()
{
	TweenManager::GetInstance().Cancel(&cursorDrawPos_);
	TweenManager::GetInstance().Cancel(&fscore_);
	TweenManager::GetInstance().Cancel(&fheadCount_);

}


void AllClearScreen::Init()
{
	score_ = 0;
	headCount_ = 0;
	starCount_ = 0;
	fscore_ = 0.f;
	fheadCount_ = 0.f;
	cursorDrawPos_ = cursorPos_[inputCount_];
	stage_ = Stage::Stage1;
	isKeyOnceScore_ = false;
	isShowScore_ = true;
	isHeadDraw_ = false;
	inputCount_ = 0;
	sinCount_ = defSinC;
	for (int i = 0; i < changeSceneList_.size(); i++) {
		textAlphaList_[i] = 1.f;
		textSizeList_[i] = 1.f;
	}
	textAlphaList_[inputCount_] = 1.f;
	textSizeList_[inputCount_] = mxmSize;

	dstar_ = DrawStar();
	cursorDrawPos_ = cursorPos_[inputCount_];


}

bool AllClearScreen::Update(Scene & nextScene)
{
	if (isShowScore_) {
		ScoreUpdate();
		return false;
	}

	if (!FadePanel::GetInstance().IsClearScreen()) return false;

	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::S) || GamePad::GetInstance().Stick().y > 0.3f) {
		inputCount_++;
		if (inputCount_ <= (int)changeSceneList_.size() - 1) {
			Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);
		}
		inputCount_ = MathHelper::Clamp(inputCount_, 0, (int)changeSceneList_.size() - 1);
		TweenManager::GetInstance().Cancel(&cursorDrawPos_);
		TweenManager::GetInstance().Add(EaseOutQuad, &cursorDrawPos_, cursorPos_[inputCount_], 0.2f);

		sinCount_ = defSinC;

	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::W) || GamePad::GetInstance().Stick().y < -0.3f) {
		inputCount_--;
		if (inputCount_ >= 0) {
			Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);
		}
		inputCount_ = MathHelper::Clamp(inputCount_, 0, (int)changeSceneList_.size() - 1);
		TweenManager::GetInstance().Cancel(&cursorDrawPos_);
		TweenManager::GetInstance().Add(EaseOutQuad, &cursorDrawPos_, cursorPos_[inputCount_], 0.2f);

		sinCount_ = defSinC;
	}
	//inputCount_ = MathHelper::Clamp(inputCount_, 0, (int)changeSceneList_.size() - 1);

	drawUpdate();

	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M) || GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2)) {
		nextScene = changeSceneList_[inputCount_];
		Sound::GetInstance().PlaySE(SE_ID::CHECK_SE);

		return true;
	}
	return false;
}

void AllClearScreen::Draw() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (128));
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(128, 128, 128), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	Vector2 GOposit = Vector2(WINDOW_WIDTH / 2.f, 300.f);
	Vector2 GOorig = Sprite::GetInstance().GetSize(SPRITE_ID::GAMECLEAR_TEXT_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::GAMECLEAR_TEXT_SPRITE, GOposit, GOorig, 1.f, Vector2::One);

	if (isShowScore_) {
		ScoreDraw();
		return;
	}

	Vector2 RTposit = Vector2(WINDOW_WIDTH / 2, 800.f);
	Vector2 RTorig = Sprite::GetInstance().GetSize(SPRITE_ID::PRESS_B_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::PRESS_B_SPRITE, RTposit, RTorig, textAlphaList_[0], Vector2::One/**textSizeList_[0]*/);


	//Vector2 CSorig = Sprite::GetInstance().GetSize(SPRITE_ID::OROCHI_CURSOR_SPRITE) / 2;
	//Sprite::GetInstance().Draw(SPRITE_ID::OROCHI_CURSOR_SPRITE, cursorDrawPos_, CSorig, Vector2::One);

}

void AllClearScreen::SetScore(int score, int count, Stage stage) {
	score_ = score;
	headCount_ = count;
	TweenManager::GetInstance().Add(Linear, &fscore_, (float)score, 1.f, [=] {SetHeadCount(); });
	stage_ = stage;
}

void AllClearScreen::SetHeadCount() {
	isHeadDraw_ = true;
	Sound::GetInstance().PlaySE(SE_ID::CHECK_SE);
	TweenManager::GetInstance().Add(Linear, &fheadCount_, (float)headCount_, 1.f, [=] {SetStarCount(); });
}

void AllClearScreen::SetStarCount() {
	starCount_ = 1;
	if (score_ >= ScoreList[stageNumList_[stage_]]) {
		starCount_++;
	}
	if (headCount_ >= 5) {
		starCount_++;
	}
	dstar_.SetStarCount(starCount_);
	Sound::GetInstance().PlaySE(SE_ID::CHECK_SE);

}
void AllClearScreen::SetFullStarCount() {
	starCount_ = 1;
	if (score_ >= ScoreList[stageNumList_[stage_]]) {
		starCount_++;
	}
	if (headCount_ >= 5) {
		starCount_++;
	}
	dstar_.SetFullStarCount(starCount_);
}

void AllClearScreen::ScoreUpdate()
{
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M) || GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2)) {
		if (!isKeyOnceScore_) {
			TweenManager::GetInstance().Cancel(&fscore_);
			TweenManager::GetInstance().Cancel(&fheadCount_);
			fscore_ = score_;
			fheadCount_ = headCount_;
			SetFullStarCount();
			isHeadDraw_ = true;
			dstar_.SetFull();
			isKeyOnceScore_ = true;
			Sound::GetInstance().PlaySE(SE_ID::CHECK_SE);

		}
		else {
			isShowScore_ = false;
		}
	}
	if (isKeyOnceScore_) {
		fscore_ = score_;
		fheadCount_ = headCount_;

	}
	dstar_.Update();

	if (dstar_.GetStarFull())isKeyOnceScore_ = true;
}

void AllClearScreen::ScoreDraw() const
{
	int bcposx = DrawScore::getInstance().Draw(Vector2(1500, 760.f),
		(int)roundf(fscore_), 5, Vector2::One);
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::BITECOUNT_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::BITECOUNT_SPRITE, Vector2(580, 750), origin, Vector2::One);

	if (!isHeadDraw_)return;

	//DrawScore::getInstance().Draw(Vector2(WINDOW_WIDTH / 2 - Sprite::GetInstance().GetSplitPieceSize(SPRITE_ID::NUMBER_SPRITE).x * 0.5f, 900.f),
	int hcposx = DrawScore::getInstance().Draw(Vector2(1500, 960.f),
		(int)roundf(fheadCount_), 1, Vector2::One);
	Vector2 horigin = Sprite::GetInstance().GetSize(SPRITE_ID::HEADCOUNT_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::HEADCOUNT_SPRITE, Vector2(hcposx - horigin.x, 950), horigin, Vector2::One);

	dstar_.Draw(Vector2(Vector2(WINDOW_WIDTH / 2, 400.f)));
}

void AllClearScreen::drawUpdate()
{
	sinCount_ += 3;
	if (sinCount_ > 360)sinCount_ = 0;

	for (int i = 0; i < changeSceneList_.size(); i++) {
		textAlphaList_[i] = 1.f;
		textSizeList_[i] = 1.f;
	}
	textAlphaList_[inputCount_] = MathHelper::Sin(sinCount_);
	textSizeList_[inputCount_] = mxmSize;

}
