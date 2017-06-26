#include "GameOverScreen.h"
#include"../../graphic/Sprite.h"
#include"../../Def.h"
#include"../../input/Keyboard.h"
#include"../../input/GamePad.h"
#include"../../math/MathHelper.h"
#include"../GamePlayDefine.h"
#include"../../tween/TweenManager.h"
#include"../../sound/sound.h"
#include"../../fade/FadePanel.h"

GameOverScreen::GameOverScreen() :inputCount_(0), sinCount_(defSinC)
{
	changeSceneList_.push_back(Scene::GamePlay);
	changeSceneList_.push_back(Scene::Menu);

	textAlphaList_.push_back(1.f);
	textAlphaList_.push_back(1.f);

	textSizeList_.push_back(1.f);
	textSizeList_.push_back(1.f);

	cursorPos_.push_back(Vector2(400, 600));
	cursorPos_.push_back(Vector2(400, 800));

	cursorDrawPos_ = cursorPos_[inputCount_];
}

void GameOverScreen::Init()
{
	inputCount_ = 0;
	sinCount_ = defSinC;
	for (int i = 0; i < changeSceneList_.size(); i++) {
		textAlphaList_[i] = 1.f;
		textSizeList_[i] = 1.f;
	}
	textAlphaList_[inputCount_] = 1.f;
	textSizeList_[inputCount_] = mxmSize;
	cursorDrawPos_ = cursorPos_[inputCount_];
	isTrigger_ = true;

}

bool GameOverScreen::Update(Scene& nextScene)
{
	if (!FadePanel::GetInstance().IsClearScreen()) return false;

	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::S) || (GamePad::GetInstance().Stick().y > 0.3f&&isTrigger_)) {
		isTrigger_ = false;

		inputCount_++;
		inputCount_ = MathHelper::Clamp(inputCount_, 0, (int)changeSceneList_.size() - 1);
		TweenManager::GetInstance().Cancel(&cursorDrawPos_);
		TweenManager::GetInstance().Add(EaseOutQuad, &cursorDrawPos_, cursorPos_[inputCount_], 0.2f);
		sinCount_ = defSinC;

		Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::W) || (GamePad::GetInstance().Stick().y < -0.3f&&isTrigger_)) {

		isTrigger_ = false;

		inputCount_--;
		inputCount_ = MathHelper::Clamp(inputCount_, 0, (int)changeSceneList_.size() - 1);
		TweenManager::GetInstance().Cancel(&cursorDrawPos_);
		TweenManager::GetInstance().Add(EaseOutQuad, &cursorDrawPos_, cursorPos_[inputCount_], 0.2f);
		sinCount_ = defSinC;
		Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);
	}
	if (abs(GamePad::GetInstance().Stick().y) <= 0.3f) {
		isTrigger_ = true;
	}

	drawUpdate();

	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M) || GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2)) {
		nextScene = changeSceneList_[inputCount_];
		Sound::GetInstance().PlaySE(SE_ID::CHECK_SE);

		return true;
	}
	return false;
}

void GameOverScreen::Draw() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (128));
	DrawBox(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GetColor(128, 128, 128), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	Vector2 GOposit = Vector2(WINDOW_WIDTH / 2.f, 300.f);
	Vector2 GOorig = Sprite::GetInstance().GetSize(SPRITE_ID::GAMEOVER_TEXT_SPRITE)/2;
	Sprite::GetInstance().Draw(SPRITE_ID::GAMEOVER_TEXT_SPRITE, GOposit, GOorig, 1.f, Vector2::One);

	Vector2 RTposit = Vector2(WINDOW_WIDTH / 2, 600.f);
	Vector2 RTorig = Sprite::GetInstance().GetSize(SPRITE_ID::RETRY_TEXT_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::RETRY_TEXT_SPRITE, RTposit, RTorig, textAlphaList_[0], Vector2::One/**textSizeList_[0]*/);

	Vector2 BTposit = Vector2(WINDOW_WIDTH / 2, 800.f);
	Vector2 BTorig = Sprite::GetInstance().GetSize(SPRITE_ID::CHANGE_STAGESELECT_TEXT_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::CHANGE_STAGESELECT_TEXT_SPRITE, BTposit, BTorig, textAlphaList_[1], Vector2::One/**textSizeList_[1]*/);

	Vector2 CSorig = Sprite::GetInstance().GetSize(SPRITE_ID::OROCHI_CURSOR_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::OROCHI_CURSOR_SPRITE, cursorDrawPos_, CSorig, Vector2::One);

}

void GameOverScreen::drawUpdate()
{
	sinCount_+=3;
	if (sinCount_ > 360)sinCount_ = 0;

	for (int i = 0; i < changeSceneList_.size(); i++) {
		textAlphaList_[i]=1.f;
		textSizeList_[i]=1.f;
	}
	textAlphaList_[inputCount_] = MathHelper::Sin(sinCount_);
	textSizeList_[inputCount_] = mxmSize;

}
