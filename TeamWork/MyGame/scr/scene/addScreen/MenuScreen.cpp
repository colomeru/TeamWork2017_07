#include "MenuScreen.h"
#include "../../time/Time.h"
#include "../../math/MathHelper.h"
#include "../../graphic/Sprite.h"
#include "../../input/Keyboard.h"
#include "../../input/GamePad.h"
#include "../../math/Easing.h"
#include "../../tween/TweenManager.h"
#include "../../Def.h"
#include "../../game/Random.h"
#include "../../sound/sound.h"
#include"../GamePlayDefine.h"
#include"../../cheat/CheatData.h"

const Vector2 CursorPos[2]{ Vector2(WINDOW_WIDTH / 2.0f - 410.0f, WINDOW_HEIGHT / 2.0f),
							Vector2(380.0f, WINDOW_HEIGHT - 54.25f) };

//コンストラクタ
MenuScreen::MenuScreen() :
	stageNum(0)
{
	betDis_ = 339.0f;
	for (int i = 0; i < 9; i++)
	{
		if (i == 0 || i == 1) panel[i] = { Vector2(WINDOW_WIDTH / 2.0f, height - i * betDis_),true,1.0f };
		else panel[i] = { Vector2(WINDOW_WIDTH / 2.0f, height - i * betDis_),false,0.5f };
	}
	//本来は0番号はチュートリアル
	stageList_[0] = Stage::Stage1;
	stageList_[1] = Stage::Stage1;
	stageList_[2] = Stage::Stage2;
	stageList_[3] = Stage::Stage3;
	stageList_[4] = Stage::Stage4;
	stageList_[5] = Stage::Stage5;
	stageList_[6] = Stage::Stage6;
	stageList_[7] = Stage::Stage7;
	stageList_[8] = Stage::Stage8;

	//背景色
	bgColor_[0] = Vector3(155, 204, 255);
	bgColor_[1] = Vector3(51, 204, 255);
	bgColor_[2] = Vector3(0, 153, 255);
	bgColor_[3] = Vector3(0, 153, 204);
	bgColor_[4] = Vector3(204, 153, 102);
	bgColor_[5] = Vector3(204, 153, 0);
	bgColor_[6] = Vector3(153, 102, 51);
	bgColor_[7] = Vector3(0, 51, 102);
	bgColor_[8] = Vector3(0, 0, 102);
	color_ = bgColor_[0];

	//星
	starNum_ = 50;
	alphaValue_ = 0.01f;
	for (int i = 0; i < 3; i++) {
		starAlpha_[i] = 0.0f;
	}
	for (int i = 0; i < starNum_; i++) {
		int randX = Random::GetInstance().Range(0, WINDOW_WIDTH);
		int randY = Random::GetInstance().Range(0, WINDOW_HEIGHT);
		star_[i].position_ = Vector2(randX, randY);
		star_[i].isAlpha_ = 0.0f;
		star_[i].timer_ = 0.0f;
		star_[i].scale_ = 1.0f;
	}


	//カラス
	int crowIdNum = CROW_ANM_01_SPRITE;
	for (int i = 0; i < 8; i++) {
		anmManager_.Add((SPRITE_ID)(crowIdNum + i));
	}
	anmManager_.SetIsRepeat(true);

	interval_ = { 5.0f,12.0f,8.0f };
	cTimer_ = { 3.0f,0.0f,0.0f };
	crowPos_ = { Vector2(WINDOW_WIDTH + 300.0f, 300.0f),Vector2(-300.0f, 500.0f), Vector2(WINDOW_WIDTH + 300.0f, 700.0f) };
	cVelocity_ = { Vector2(-5.0f, 0.0f),Vector2(-5.0f, 0.0f),Vector2(-5.0f, 0.0f) };
	cFrom_ = { 0.0f,0.0f,0.0f };
	cDis_ = { 0.0f,0.0f,0.0f };
}

//デストラクタ
MenuScreen::~MenuScreen()
{
}

void MenuScreen::Init()
{
	for (int i = 0; i < 8; i++) {
		OpenNextStage(i);
	}


	stageNum = CheatData::getInstance().GetStartStage();
	dis = stageNum * betDis_;
	cursorPos = CursorPos[0];
	backSelect = false;
	from = Vector2(0.0f, stageNum * betDis_);
	builPos_ = Vector2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 285.0f);
	bgPos_ = Vector2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 475.0f);
	wwwPos_ = Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT);

	//流れ星
	sStarNum_ = 10;
	for (int i = 0; i < sStarNum_; i++) {
		int randX = Random::GetInstance().Range(0, WINDOW_WIDTH);
		int randY = Random::GetInstance().Range(0, WINDOW_HEIGHT);
		sStar_[i].position_ = Vector2(randX, randY);
		sStar_[i].isAlpha_ = 0.0f;
		sStar_[i].timer_ = 0.0f;
		prevPos_[i] = sStar_[i].position_;
		ssAlpha_[i] = 0.0f;
		sStar_[i].scale_ = 1.0f;
	}
	waitTime_ = { 10.0f,7.0f,20.0f,12.0f,34.0f,16.0f,19.0,24.0f,19.0f,15.0f };

	ResetBG();

	if (!Sound::GetInstance().IsPlayBGM()) {
		Sound::GetInstance().PlayBGM(BGM_ID::TITLE_BGM, DX_PLAYTYPE_LOOP);
		Sound::GetInstance().SetBGMVolume(BGM_ID::TITLE_BGM, 1.0f);
	}

}

//更新
void MenuScreen::Update()
{
	anmManager_.Update();
	Crow();
}

//描画
void MenuScreen::Draw() const
{
	Pattern2Draw();
}

//
void MenuScreen::Action()
{
}

//前のステージをクリアしているか？
bool MenuScreen::CheckPreviousStage(int sNum)
{
	if ((sNum > 0 && panel[sNum].isDraw == true) || sNum == 0) return true;
	else return false;
}

//次のステージの解放
void MenuScreen::OpenNextStage(int sNum)
{
	if (sNum == 0)return;
	if (CheatData::getInstance().GetClearData(sNum - 1))
	{
		panel[sNum + 1].isDraw = true;
		panel[sNum + 1].alpha = 1.0f;
	}
}

//次のステージが解放されているか？
bool MenuScreen::CheckNextStage(int sNum)
{
	if (sNum < 8 && panel[sNum + 1].isDraw == true) return true;
	else return false;
}

//パターン２更新
void MenuScreen::Pattern2Update()
{
	if (!backSelect) {
		if (IsInputUp())
		{
			if (stageNum == 8 || CheckNextStage(stageNum) == false) return;
			dis += betDis_;
			stageNum++;

			TweenManager::GetInstance().Add(EaseOutExpo, &from, Vector2(0.0f, dis), MoveTime);
			TweenManager::GetInstance().Add(Linear, &color_, bgColor_[stageNum], MoveTime);
			for (int i = 0; i < 3; i++)
			{
				cDis_[i] += betDis_;
				TweenManager::GetInstance().Add(EaseOutExpo, &cFrom_[i], Vector2(0.0f, cDis_[i]), MoveTime);
			}
			Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);

		}
		if (IsInputDown())
		{
			if (stageNum == 0.0f) return;
			dis -= betDis_;
			stageNum--;

			TweenManager::GetInstance().Add(EaseOutExpo, &from, Vector2(0.0f, dis), MoveTime);
			TweenManager::GetInstance().Add(Linear, &color_, bgColor_[stageNum], MoveTime);
			for (int i = 0; i < 3; i++)
			{
				cDis_[i] -= betDis_;
				TweenManager::GetInstance().Add(EaseOutExpo, &cFrom_[i], Vector2(0.0f, cDis_[i]), MoveTime);
			}
			Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);

		}
	}
	stageNum = MathHelper::Clamp(stageNum, 0, 8);

	//左を押すと「戻る」にカーソルを移動
	if (IsInputLeft() && !backSelect)
	{
		TweenManager::GetInstance().Add(EaseOutExpo, &cursorPos, CursorPos[1], MoveTime);
		Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);
		backSelect = true;

	}
	else if (IsInputAny() && backSelect)
	{
		TweenManager::GetInstance().Add(EaseOutExpo, &cursorPos, CursorPos[0], MoveTime);
		Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);
		backSelect = false;

	}

	//背景色
	SetBackgroundColor(color_.x, color_.y, color_.z);

	Star();

	ShootingStar();

	SE();

}

//パターン２描画
void MenuScreen::Pattern2Draw() const
{

	////流れ星
	//for (int i = 0; i < sStarNum_; i++) {
	//	Sprite::GetInstance().Draw(SPRITE_ID::STAR_SPRITE, sStar_[i].position_, Vector2::Zero, ssAlpha_[i], Vector2(sStar_[i].scale_, sStar_[i].scale_));
	//}

	//背景
	static auto bgSize = Sprite::GetInstance().GetSize(SPRITE_ID::STAGE_SELECT_BACK_SPRITE);
	static auto builSize = Sprite::GetInstance().GetSize(SPRITE_ID::STAGE_SELECT_M_SPRITE);
	static auto wwwSize = Sprite::GetInstance().GetSize(SPRITE_ID::WWW_SPRITE);
	static auto nightSize = Sprite::GetInstance().GetSize(SPRITE_ID::STAGE_SELECT_NIGHT1_SPRITE);

	Sprite::GetInstance().Draw(SPRITE_ID::STAGE_SELECT_BACK_SPRITE, bgPos_ + from * 0.7f, Vector2(bgSize.x / 2.0f - 100.0f, bgSize.y), Vector2(1.5f, 1.5f), 1.0f, false);

	//星
	Vector2 drawNightSize = Vector2(WINDOW_WIDTH / nightSize.x, WINDOW_HEIGHT / nightSize.y);
	/*パターン２*/
	Sprite::GetInstance().Draw(SPRITE_ID::STAGE_SELECT_NIGHT1_SPRITE, Vector2::Zero, Vector2::Zero, starAlpha_[2], drawNightSize);
	Sprite::GetInstance().Draw(SPRITE_ID::STAGE_SELECT_NIGHT2_SPRITE, Vector2::Zero, Vector2::Zero, starAlpha_[1], drawNightSize);
	Sprite::GetInstance().Draw(SPRITE_ID::STAGE_SELECT_NIGHT3_SPRITE, Vector2::Zero, Vector2::Zero, starAlpha_[0], drawNightSize);


	//カラス
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::BIRD_SPRITE);
	anmManager_.Draw(crowPos_[2] + cFrom_[2], origin, Vector2::One, 1.0f);

	//ビルと草
	Sprite::GetInstance().Draw(SPRITE_ID::STAGE_SELECT_M_SPRITE, builPos_ + from, Vector2(builSize.x / 2.0f - 24.0f, builSize.y), Vector2(3.0f, 3.0f), 1.0f, false);
	Sprite::GetInstance().Draw(SPRITE_ID::WWW_SPRITE, wwwPos_ + from, Vector2(wwwSize.x / 2.0f, wwwSize.y), Vector2(2.0f, 2.2f), 1.0f, false);

	//ステージパネルを描画
	static auto panelSize = Sprite::GetInstance().GetSize(SPRITE_ID::STAGE_SELECT_1_SPRITE);
	static auto trainingSize = Sprite::GetInstance().GetSize(SPRITE_ID::STAGE_SELECT_TRAINING_SPRITE);
	Sprite::GetInstance().Draw(SPRITE_ID::STAGE_SELECT_TRAINING_SPRITE, panel[0].position + from, Vector2(trainingSize.x / 2.0f, trainingSize.y / 2.0f), panel[0].alpha, Vector2::One);
	static int spriteNum = SPRITE_ID::STAGE_SELECT_1_SPRITE - 1;
	for (int i = 1; i <= 8; i++) {
		Sprite::GetInstance().Draw((SPRITE_ID)(spriteNum + i), panel[i].position + from, Vector2(panelSize.x / 2.0f, panelSize.y / 2.0f), panel[i].alpha, Vector2::One);
	}

	//戻るパネルを描画
	Sprite::GetInstance().Draw(SPRITE_ID::TITLE_SELECT_SPRITE, Vector2(0.0f, WINDOW_HEIGHT - 108.5f), Vector2::Zero, 1.0f, Vector2(0.5f, 0.5f));

	//カーソルを描画
	Sprite::GetInstance().Draw(SPRITE_ID::OROCHI_CURSOR_SPRITE, cursorPos, Vector2(48.0f, 35.0f), 1.0f, Vector2::One, true, backSelect);

	//カラス
	anmManager_.Draw(crowPos_[0] + cFrom_[0], origin, Vector2::One, 1.0f);
	anmManager_.Draw(crowPos_[1] + cFrom_[1], origin, Vector2::One, 1.0f);


	if (BuildMode == 1) {
		DrawFormatString(0, 40, GetColor(255, 255, 255), "stageNum:%d", stageNum);
		DrawFormatString(0, 100, GetColor(255, 255, 255), "dis %f", dis);
		for (int i = 0; i < 30; i++) {
			DrawFormatString(0, 200 + i * 20, GetColor(255, 255, 255), "isAlpha %f", star_[i].isAlpha_);
		}
		for (int i = 0; i < 3; i++) {
			DrawFormatString(1600, 0 + i * 20, GetColor(255, 255, 255), "cTimer_ %f", cTimer_[i]);
			DrawFormatString(1600, 60 + i * 20, GetColor(255, 255, 255), "crowPos_X %f", crowPos_[i].x);
			DrawFormatString(1600, 120 + i * 20, GetColor(255, 255, 255), "cVelocity_X %f", cVelocity_[i].x);
			DrawFormatString(1600, 180 + i * 20, GetColor(255, 255, 255), "cDis_ %f", cDis_[i]);
		}
	}

	//流れ星
	for (int i = 0; i < sStarNum_; i++) {
		Sprite::GetInstance().Draw(SPRITE_ID::STAR_SPRITE, sStar_[i].position_, Vector2::Zero, ssAlpha_[i], Vector2(sStar_[i].scale_, sStar_[i].scale_));
	}

}

//"上"が入力されたか
bool MenuScreen::IsInputUp() const
{
	return Keyboard::GetInstance().KeyTriggerDown(KEYCODE::W) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::UP);
}

//"下"が入力されたか
bool MenuScreen::IsInputDown() const
{
	return Keyboard::GetInstance().KeyTriggerDown(KEYCODE::S) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::DOWN);
}

//"左/A"のいずれかが入力されたか
bool MenuScreen::IsInputLeft() const
{
	return Keyboard::GetInstance().KeyTriggerDown(KEYCODE::A) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::LEFT) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM1);
}

//"上/下/右"のいずれかが入力されたか
bool MenuScreen::IsInputAny() const
{
	return Keyboard::GetInstance().KeyTriggerDown(KEYCODE::W) ||
		Keyboard::GetInstance().KeyTriggerDown(KEYCODE::S) ||
		Keyboard::GetInstance().KeyTriggerDown(KEYCODE::D) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::UP) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::DOWN) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::RIGHT);
}



//星
void MenuScreen::Star()
{
	alphaValue_ = 0.007 * (9 - stageNum);

	/*パターン２(フルサイズ画像版)*/
	if (stageNum == 8) {
		starAlpha_[0] += alphaValue_;
		starAlpha_[1] += alphaValue_;
		starAlpha_[2] += alphaValue_;
	}
	else if (stageNum == 7) {
		starAlpha_[0] += alphaValue_;
		starAlpha_[1] += alphaValue_;
		starAlpha_[2] -= alphaValue_;
	}
	else if (stageNum == 6) {
		starAlpha_[0] += alphaValue_;
		starAlpha_[1] -= alphaValue_;
		starAlpha_[2] -= alphaValue_;
	}
	else {
		starAlpha_[0] -= alphaValue_;
		starAlpha_[1] -= alphaValue_;
		starAlpha_[2] -= alphaValue_;
	}
	for (int i = 0; i < 3; i++) {
		starAlpha_[i] = MathHelper::Clamp(starAlpha_[i], 0.0f, 1.0f);
	}

}

//流れ星
void MenuScreen::ShootingStar()
{
	if (stageNum >= 0) {
		for (int i = 0; i < sStarNum_; i++) {
			sStar_[i].timer_ += Time::DeltaTime;
			ssAlpha_[i] = MathHelper::Clamp(ssAlpha_[i], 0.0f, 1.0f);
			if (sStar_[i].timer_ >= waitTime_[i]) {
				sStar_[i].position_ += Vector2(-20.0f, 10.0f);
				ssAlpha_[i] += 0.25f;
				if (ssAlpha_[i] >= 1.0f) ssAlpha_[i] -= 0.25f;
				if (sStar_[i].timer_ >= waitTime_[i] + 0.5f) {
					sStar_[i].timer_ = 0.0f;
					sStar_[i].position_ = prevPos_[i];
					ssAlpha_[i] = 0.0f;
					sStar_[i].scale_ = Random::GetInstance().Range(0.5f, 2.5f);
				}
			}
		}
	}
}

//カラス
void MenuScreen::Crow()
{
	for (int i = 0; i < 3; i++) {
		if (crowPos_[i].x <= -300.0f || crowPos_[i].x >= WINDOW_WIDTH + 300.0f) {
			cTimer_[i] += Time::DeltaTime;
			//座標リセット
			cDis_[i] = 0.0f;
			cFrom_[i] = 0.0f;

			crowPos_[i].x = WINDOW_WIDTH + 302.0f;
		}
		if (cTimer_[i] >= interval_[i] - 1.0f) {
			cTimer_[i] = 0.0f;
		}
		if (cTimer_[i] == 0.0f) {
			if (stageNum >= 7 && crowPos_[i].x == WINDOW_WIDTH + 302.0f) return;
			crowPos_[i] += cVelocity_[i];
		}

		crowPos_[i].x = MathHelper::Clamp(crowPos_[i].x, -302.0f, WINDOW_WIDTH + 302.0f);
		cTimer_[i] = MathHelper::Min(cTimer_[i], interval_[i]);
	}
}

//SE
void MenuScreen::SE()
{
	if (backSelect == false &&
		(Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M) ||
			GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2))) {
		Sound::GetInstance().PlaySE(SE_ID::CHECK_SE);
	}
	if (backSelect == true &&
		(Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M) ||
			GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2))) {
		Sound::GetInstance().PlaySE(SE_ID::CANCEL_SE);
	}
}

void MenuScreen::End()
{
	Sound::GetInstance().StopBGM();
}

Stage MenuScreen::GetGamePlayStage() const
{
	return stageList_[stageNum];
}

void MenuScreen::InputSelectStage()
{
	CheatData::getInstance().SetStartStage(stageNum);
	CheatData::getInstance().SetSelectStage(stageList_[stageNum]);
}

//背景リセット
void MenuScreen::ResetBG()
{
	color_ = bgColor_[stageNum];
	if (stageNum <= 5) {
		for (int i = 0; i < 3; i++) {
			starAlpha_[i] = 0.0f;
		}
	}
}
