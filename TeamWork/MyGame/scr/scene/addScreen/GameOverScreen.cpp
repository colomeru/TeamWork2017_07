#include "GameOverScreen.h"
#include"../../graphic/Sprite.h"
#include"../../Def.h"
#include"../../input/Keyboard.h"
#include"../../input/GamePad.h"
#include"../../math/MathHelper.h"

static const float mxmSize = 1.5f;
static const int defSinC = 90;
GameOverScreen::GameOverScreen() :inputCount_(0), sinCount_(defSinC)
{
	changeSceneList_.push_back(Scene::GamePlay);
	changeSceneList_.push_back(Scene::Title);

	textAlphaList_.push_back(1.f);
	textAlphaList_.push_back(1.f);

	textSizeList_.push_back(1.f);
	textSizeList_.push_back(1.f);
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

}

bool GameOverScreen::Update(Scene& nextScene)
{
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::S) || GamePad::GetInstance().Stick().y > 0.3f) {
		inputCount_++;

		sinCount_ = defSinC;
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::W) || GamePad::GetInstance().Stick().y < -0.3f) {
		inputCount_--;

		sinCount_ = defSinC;
	}
	inputCount_ = MathHelper::Clamp(inputCount_, 0, (int)changeSceneList_.size()-1);

	drawUpdate();

	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE) || GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2)) {
		nextScene = changeSceneList_[inputCount_];
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
	Sprite::GetInstance().Draw(SPRITE_ID::RETRY_TEXT_SPRITE, RTposit, RTorig, textAlphaList_[0], Vector2::One*textSizeList_[0]);

	Vector2 BTposit = Vector2(WINDOW_WIDTH / 2, 800.f);
	Vector2 BTorig = Sprite::GetInstance().GetSize(SPRITE_ID::BACKTITLE_TEXT_SPRITE) / 2;
	Sprite::GetInstance().Draw(SPRITE_ID::BACKTITLE_TEXT_SPRITE, BTposit, BTorig, textAlphaList_[1], Vector2::One*textSizeList_[1]);

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
