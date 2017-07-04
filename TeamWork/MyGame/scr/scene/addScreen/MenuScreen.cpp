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

//�J�[�\��
const Vector2 CursorPos[2]{ Vector2(WINDOW_WIDTH / 2.0f - 410.0f, WINDOW_HEIGHT / 2.0f),
							Vector2(380.0f, WINDOW_HEIGHT - 54.25f) };
//�w�i�F
const Vector3 BgColor[9]{ Vector3(155, 204, 255) ,Vector3(51, 204, 255) ,Vector3(0, 153, 255) ,
						  Vector3(0, 153, 204) ,Vector3(204, 153, 102) ,Vector3(204, 153, 0) ,
						  Vector3(153, 102, 51) ,Vector3(0, 51, 102) ,Vector3(0, 0, 102) };

//�R���X�g���N�^
MenuScreen::MenuScreen() :
	stageNum_(0), backSelect_(false), cursorPos_(CursorPos[0]), dis_(stageNum_ * BetDis), from_(Vector2(0.0f, stageNum_ * BetDis)),
	color_(BgColor[0]), alphaValue_(0.01f)
{
	for (int i = 0; i < 9; i++)
	{
		if (i == 0 || i == 1) panel[i] = { Vector2(WINDOW_WIDTH / 2.0f, Height - i * BetDis),true,1.0f };
		else panel[i] = { Vector2(WINDOW_WIDTH / 2.0f, Height - i * BetDis),false,0.5f };
	}

	//�{����0�ԍ��̓`���[�g���A��
	stageList_[0] = Stage::Stage1;
	for (int i = 1; i < 9; i++) {
		stageList_[i] = (Stage)(i - 1);
	}

	//��
	for (int i = 0; i < 3; i++) {
		starAlpha_[i] = 0.0f;
	}

	//�J���X
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

//�f�X�g���N�^
MenuScreen::~MenuScreen()
{
}

void MenuScreen::Init()
{
	for (int i = 0; i < 8; i++) {
		OpenNextStage(i);
	}


	stageNum_ = CheatData::getInstance().GetStartStage();
	dis_ = stageNum_ * BetDis;
	cursorPos_ = CursorPos[0];
	backSelect_ = false;
	from_ = Vector2(0.0f, stageNum_ * BetDis);

	//���ꐯ
	for (int i = 0; i < StarNum; i++) {
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

//�X�V
void MenuScreen::Update()
{
	anmManager_.Update();
	Crow();
}

//�`��
void MenuScreen::Draw() const
{
	Pattern2Draw();
}

//
void MenuScreen::Action()
{
}

//�O�̃X�e�[�W���N���A���Ă��邩�H
bool MenuScreen::CheckPreviousStage(int sNum)
{
	if ((sNum > 0 && panel[sNum].isDraw == true) || sNum == 0) return true;
	else return false;
}

//���̃X�e�[�W�̉��
void MenuScreen::OpenNextStage(int sNum)
{
	if (sNum == 0)return;
	if (CheatData::getInstance().GetClearData(sNum - 1))
	{
		panel[sNum + 1].isDraw = true;
		panel[sNum + 1].alpha = 1.0f;
	}
}

//���̃X�e�[�W���������Ă��邩�H
bool MenuScreen::CheckNextStage(int sNum)
{
	if (sNum < 8 && panel[sNum + 1].isDraw == true) return true;
	else return false;
}

//�p�^�[���Q�X�V
void MenuScreen::Pattern2Update()
{
	if (!backSelect_) {
		if (IsInputUp())
		{
			if (stageNum_ == 8 || CheckNextStage(stageNum_) == false) return;
			dis_ += BetDis;
			stageNum_++;

			TweenManager::GetInstance().Add(EaseOutExpo, &from_, Vector2(0.0f, dis_), MoveTime);
			TweenManager::GetInstance().Add(Linear, &color_, BgColor[stageNum_], MoveTime);
			for (int i = 0; i < 3; i++)
			{
				cDis_[i] += BetDis;
				TweenManager::GetInstance().Add(EaseOutExpo, &cFrom_[i], Vector2(0.0f, cDis_[i]), MoveTime);
			}
			Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);

		}
		if (IsInputDown())
		{
			if (stageNum_ == 0.0f) return;
			dis_ -= BetDis;
			stageNum_--;

			TweenManager::GetInstance().Add(EaseOutExpo, &from_, Vector2(0.0f, dis_), MoveTime);
			TweenManager::GetInstance().Add(Linear, &color_, BgColor[stageNum_], MoveTime);
			for (int i = 0; i < 3; i++)
			{
				cDis_[i] -= BetDis;
				TweenManager::GetInstance().Add(EaseOutExpo, &cFrom_[i], Vector2(0.0f, cDis_[i]), MoveTime);
			}
			Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);

		}
	}
	stageNum_ = MathHelper::Clamp(stageNum_, 0, 8);

	//���������Ɓu�߂�v�ɃJ�[�\�����ړ�
	if (IsInputLeft() && !backSelect_)
	{
		TweenManager::GetInstance().Add(EaseOutExpo, &cursorPos_, CursorPos[1], MoveTime);
		Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);
		backSelect_ = true;

	}
	else if (IsInputAny() && backSelect_)
	{
		TweenManager::GetInstance().Add(EaseOutExpo, &cursorPos_, CursorPos[0], MoveTime);
		Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);
		backSelect_ = false;

	}

	//�w�i�F
	SetBackgroundColor(color_.x, color_.y, color_.z);

	Star();

	ShootingStar();

	SE();

}

//�p�^�[���Q�`��
void MenuScreen::Pattern2Draw() const
{

	////���ꐯ
	//for (int i = 0; i < sStarNum_; i++) {
	//	Sprite::GetInstance().Draw(SPRITE_ID::STAR_SPRITE, sStar_[i].position_, Vector2::Zero, sStar_[i].isAlpha_ Vector2(sStar_[i].scale_, sStar_[i].scale_));
	//}

	//�w�i
	static auto bgSize = Sprite::GetInstance().GetSize(SPRITE_ID::STAGE_SELECT_BACK_SPRITE);
	static auto builSize = Sprite::GetInstance().GetSize(SPRITE_ID::STAGE_SELECT_M_SPRITE);
	static auto wwwSize = Sprite::GetInstance().GetSize(SPRITE_ID::WWW_SPRITE);
	static auto nightSize = Sprite::GetInstance().GetSize(SPRITE_ID::STAGE_SELECT_NIGHT1_SPRITE);

	Sprite::GetInstance().Draw(SPRITE_ID::STAGE_SELECT_BACK_SPRITE, BgPos + from_ * 0.7f, Vector2(bgSize.x / 2.0f - 100.0f, bgSize.y), Vector2(1.5f, 1.5f), 1.0f, false);

	//��
	Vector2 drawNightSize = Vector2(WINDOW_WIDTH / nightSize.x, WINDOW_HEIGHT / nightSize.y);
	/*�p�^�[���Q*/
	Sprite::GetInstance().Draw(SPRITE_ID::STAGE_SELECT_NIGHT1_SPRITE, Vector2::Zero, Vector2::Zero, starAlpha_[2], drawNightSize);
	Sprite::GetInstance().Draw(SPRITE_ID::STAGE_SELECT_NIGHT2_SPRITE, Vector2::Zero, Vector2::Zero, starAlpha_[1], drawNightSize);
	Sprite::GetInstance().Draw(SPRITE_ID::STAGE_SELECT_NIGHT3_SPRITE, Vector2::Zero, Vector2::Zero, starAlpha_[0], drawNightSize);


	//�J���X
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::BIRD_SPRITE);
	anmManager_.Draw(crowPos_[2] + cFrom_[2], origin, Vector2::One, 1.0f);

	//�r���Ƒ�
	Sprite::GetInstance().Draw(SPRITE_ID::STAGE_SELECT_M_SPRITE, BuilPos + from_, Vector2(builSize.x / 2.0f - 24.0f, builSize.y), Vector2(3.0f, 3.0f), 1.0f, false);
	Sprite::GetInstance().Draw(SPRITE_ID::WWW_SPRITE, WwwPos + from_, Vector2(wwwSize.x / 2.0f, wwwSize.y), Vector2(2.0f, 2.2f), 1.0f, false);

	//�X�e�[�W�p�l����`��
	static auto panelSize = Sprite::GetInstance().GetSize(SPRITE_ID::STAGE_SELECT_1_SPRITE);
	static auto trainingSize = Sprite::GetInstance().GetSize(SPRITE_ID::STAGE_SELECT_TRAINING_SPRITE);
	Sprite::GetInstance().Draw(SPRITE_ID::STAGE_SELECT_TRAINING_SPRITE, panel[0].position + from_, Vector2(trainingSize.x / 2.0f, trainingSize.y / 2.0f), panel[0].alpha, Vector2::One);
	static int spriteNum = SPRITE_ID::STAGE_SELECT_1_SPRITE - 1;
	for (int i = 1; i <= 8; i++) {
		Sprite::GetInstance().Draw((SPRITE_ID)(spriteNum + i), panel[i].position + from_, Vector2(panelSize.x / 2.0f, panelSize.y / 2.0f), panel[i].alpha, Vector2::One);
	}

	//�߂�p�l����`��
	Sprite::GetInstance().Draw(SPRITE_ID::TITLE_SELECT_SPRITE, Vector2(0.0f, WINDOW_HEIGHT - 108.5f), Vector2::Zero, 1.0f, Vector2(0.5f, 0.5f));

	//�J�[�\����`��
	Sprite::GetInstance().Draw(SPRITE_ID::OROCHI_CURSOR_SPRITE, cursorPos_, Vector2(48.0f, 35.0f), 1.0f, Vector2::One, true, backSelect_);

	//�J���X
	anmManager_.Draw(crowPos_[0] + cFrom_[0], origin, Vector2::One, 1.0f);
	anmManager_.Draw(crowPos_[1] + cFrom_[1], origin, Vector2::One, 1.0f);


	if (BuildMode == 1) {
		DrawFormatString(0, 40, GetColor(255, 255, 255), "stageNum:%d", stageNum_);
		DrawFormatString(0, 100, GetColor(255, 255, 255), "dis %f", dis_);
		for (int i = 0; i < 3; i++) {
			DrawFormatString(1600, 0 + i * 20, GetColor(255, 255, 255), "cTimer_ %f", cTimer_[i]);
			DrawFormatString(1600, 60 + i * 20, GetColor(255, 255, 255), "crowPos_X %f", crowPos_[i].x);
			DrawFormatString(1600, 120 + i * 20, GetColor(255, 255, 255), "cVelocity_X %f", cVelocity_[i].x);
			DrawFormatString(1600, 180 + i * 20, GetColor(255, 255, 255), "cDis_ %f", cDis_[i]);
		}
	}

	//���ꐯ
	for (int i = 0; i < StarNum; i++) {
		Sprite::GetInstance().Draw(SPRITE_ID::STAR_SPRITE, sStar_[i].position_, Vector2::Zero, sStar_[i].isAlpha_, Vector2(sStar_[i].scale_, sStar_[i].scale_));
	}

}

//"��"�����͂��ꂽ��
bool MenuScreen::IsInputUp() const
{
	return Keyboard::GetInstance().KeyTriggerDown(KEYCODE::W) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::UP);
}

//"��"�����͂��ꂽ��
bool MenuScreen::IsInputDown() const
{
	return Keyboard::GetInstance().KeyTriggerDown(KEYCODE::S) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::DOWN);
}

//"��/A"�̂����ꂩ�����͂��ꂽ��
bool MenuScreen::IsInputLeft() const
{
	return Keyboard::GetInstance().KeyTriggerDown(KEYCODE::A) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::LEFT) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM1);
}

//"��/��/�E"�̂����ꂩ�����͂��ꂽ��
bool MenuScreen::IsInputAny() const
{
	return Keyboard::GetInstance().KeyTriggerDown(KEYCODE::W) ||
		Keyboard::GetInstance().KeyTriggerDown(KEYCODE::S) ||
		Keyboard::GetInstance().KeyTriggerDown(KEYCODE::D) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::UP) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::DOWN) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::RIGHT);
}



//��
void MenuScreen::Star()
{
	alphaValue_ = 0.007 * (9 - stageNum_);

	/*�p�^�[���Q(�t���T�C�Y�摜��)*/
	if (stageNum_ == 8) {
		starAlpha_[0] += alphaValue_;
		starAlpha_[1] += alphaValue_;
		starAlpha_[2] += alphaValue_;
	}
	else if (stageNum_ == 7) {
		starAlpha_[0] += alphaValue_;
		starAlpha_[1] += alphaValue_;
		starAlpha_[2] -= alphaValue_;
	}
	else if (stageNum_ == 6) {
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

//���ꐯ
void MenuScreen::ShootingStar()
{
	if (stageNum_ >= 0) {
		for (int i = 0; i < StarNum; i++) {
			sStar_[i].timer_ += Time::DeltaTime;
			sStar_[i].isAlpha_ = MathHelper::Clamp(sStar_[i].isAlpha_, 0.0f, 1.0f);
			if (sStar_[i].timer_ >= waitTime_[i]) {
				sStar_[i].position_ += Vector2(-20.0f, 10.0f);
				sStar_[i].isAlpha_ += 0.25f;
				if (sStar_[i].isAlpha_ >= 1.0f) sStar_[i].isAlpha_ -= 0.25f;
				if (sStar_[i].timer_ >= waitTime_[i] + 0.5f) {
					sStar_[i].timer_ = 0.0f;
					sStar_[i].position_ = prevPos_[i];
					sStar_[i].isAlpha_ = 0.0f;
					sStar_[i].scale_ = Random::GetInstance().Range(0.5f, 2.5f);
				}
			}
		}
	}
}

//�J���X
void MenuScreen::Crow()
{
	for (int i = 0; i < 3; i++) {
		if (crowPos_[i].x <= -300.0f || crowPos_[i].x >= WINDOW_WIDTH + 300.0f) {
			cTimer_[i] += Time::DeltaTime;
			//���W���Z�b�g
			cDis_[i] = 0.0f;
			cFrom_[i] = 0.0f;

			crowPos_[i].x = WINDOW_WIDTH + 302.0f;
		}
		if (cTimer_[i] >= interval_[i] - 1.0f) {
			cTimer_[i] = 0.0f;
		}
		if (cTimer_[i] == 0.0f) {
			if (stageNum_ >= 7 && crowPos_[i].x == WINDOW_WIDTH + 302.0f) return;
			crowPos_[i] += cVelocity_[i];
		}

		crowPos_[i].x = MathHelper::Clamp(crowPos_[i].x, -302.0f, WINDOW_WIDTH + 302.0f);
		cTimer_[i] = MathHelper::Min(cTimer_[i], interval_[i]);
	}
}

//SE
void MenuScreen::SE()
{
	if (backSelect_ == false &&
		(Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M) ||
			GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2))) {
		Sound::GetInstance().PlaySE(SE_ID::CHECK_SE);
	}
	if (backSelect_ == true &&
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
	return stageList_[stageNum_];
}

void MenuScreen::InputSelectStage()
{
	CheatData::getInstance().SetStartStage(stageNum_);
	CheatData::getInstance().SetSelectStage(stageList_[stageNum_]);
}

//�w�i���Z�b�g
void MenuScreen::ResetBG()
{
	color_ = BgColor[stageNum_];
	if (stageNum_ <= 5) {
		for (int i = 0; i < 3; i++) {
			starAlpha_[i] = 0.0f;
		}
	}
}
