#include "MenuScreen.h"
#include "../../time/Time.h"
#include "../../math/MathHelper.h"
#include "../../graphic/Sprite.h"
#include "../../tween/TweenManager.h"
#include "../../Def.h"
#include "../../game/Random.h"
#include "../../sound/sound.h"
#include "../GamePlayDefine.h"
#include "../../cheat/CheatData.h"
#include "../../debugdata/DebugDraw.h"
#include "../../input/InputChecker.h"

//カーソル
const Vector2 CursorPos[2]{ Vector2(WINDOW_WIDTH / 2.0f - 410.0f, WINDOW_HEIGHT / 2.0f),
							Vector2(380.0f, WINDOW_HEIGHT - 54.25f) };
//背景色
const Vector3 BgColor[9]{ Vector3(155, 204, 255) ,Vector3(51, 204, 255) ,Vector3(0, 153, 255) ,
						  Vector3(0, 153, 235) ,Vector3(0, 153, 204) ,Vector3(204, 153, 0) ,
						  Vector3(153, 102, 51) ,Vector3(0, 51, 102) ,Vector3(0, 0, 102) };
//流れ星待機時間
const float WaitTime[10]{ 10.0f,7.0f,20.0f,12.0f,34.0f,16.0f,19.0,24.0f,19.0f,15.0f };


//コンストラクタ
MenuScreen::MenuScreen() :
	stageNum_(0), backSelect_(false), cursorPos_(CursorPos[0]), dis_(stageNum_ * BetDis), from_(Vector2(0.0f, stageNum_ * BetDis)),
	color_(BgColor[0]), alphaValue_(0.01f), crow_({}), isUseKey_(true)
{
	//パネル
	for (int i = 0; i < 9; i++)
	{
		if (i == 0 || i == 1) panel[i] = { Vector2(WINDOW_WIDTH / 2.0f, Height - i * BetDis),true,1.0f };
		else panel[i] = { Vector2(WINDOW_WIDTH / 2.0f, Height - i * BetDis),false,0.5f };
	}

	//ステージセット
	stageList_[0] = Stage::Stage1;
	for (int i = 1; i < 9; i++) {
		stageList_[i] = (Stage)(i - 1);
	}

	//星
	for (int i = 0; i < 3; i++) {
		starAlpha_[i] = 0.0f;
	}

	//アニメーション読み込み
	int crowIdNum = CROW_ANM_01_SPRITE;
	for (int i = 0; i < 8; i++) {
		anmManager_.Add((SPRITE_ID)(crowIdNum + i));
	}
	anmManager_.SetIsRepeat(true);

	//カラス初期化
	for (int i = 0; i < crow_.size(); i++) {
		crow_[i].Initialize(Vector2(WINDOW_WIDTH + 300.0f, 300.0f + i * 200.0f), 5.0f + i * 4.0f);
	}
}

//デストラクタ
MenuScreen::~MenuScreen()
{
}

//初期化
void MenuScreen::Init()
{
	//ステージのクリア情報を描画情報に反映
	for (int i = 0; i < 8; i++) {
		OpenNextStage(i);
	}

	stageNum_ = CheatData::getInstance().GetStartStage();
	dis_ = stageNum_ * BetDis;
	cursorPos_ = CursorPos[0];
	backSelect_ = false;
	from_ = Vector2(0.0f, stageNum_ * BetDis);

	//流れ星
	for (int i = 0; i < StarNum; i++) {
		int randX = Random::GetInstance().Range(0, WINDOW_WIDTH);
		int randY = Random::GetInstance().Range(0, WINDOW_HEIGHT);
		sStar_[i].position_ = Vector2(randX, randY);
		sStar_[i].isAlpha_ = 0.0f;
		sStar_[i].timer_ = 0.0f;
		sStar_[i].prevPos_ = sStar_[i].position_;
		sStar_[i].scale_ = 1.0f;
	}

	//背景リセット
	ResetBG();

	//BGM
	if (!Sound::GetInstance().IsPlayBGM()) { //タイトルのBGMを引き継ぐ
		Sound::GetInstance().PlayBGM(BGM_ID::TITLE_BGM, DX_PLAYTYPE_LOOP);
		Sound::GetInstance().SetBGMVolume(BGM_ID::TITLE_BGM, 1.0f);
	}
}

//更新
void MenuScreen::Update()
{
	//戻るが選択されていなければ
	if (!backSelect_) {
		if (IsInputUp())
		{//上が押されたら
			if (stageNum_ == 8 || CheckNextStage(stageNum_) == false) return;
			stageNum_++;
			//パネルを下にずらす
			dis_ += BetDis;
			TweenManager::GetInstance().Add(EaseOutExpo, &from_, Vector2(0.0f, dis_), MoveTime);
			//背景カラーを変化
			TweenManager::GetInstance().Add(Linear, &color_, BgColor[stageNum_], MoveTime);
			//カラスをパネルに合わせてずらす
			for (int i = 0; i < 3; i++)
			{
				crow_[i].AddDistance(BetDis);
			}
			Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);
		}
		if (IsInputDown())
		{//下が押されたら
			if (stageNum_ == 0) return;
			stageNum_--;
			//パネルを上にずらす
			dis_ -= BetDis;
			TweenManager::GetInstance().Add(EaseOutExpo, &from_, Vector2(0.0f, dis_), MoveTime);
			//背景カラーを変化
			TweenManager::GetInstance().Add(Linear, &color_, BgColor[stageNum_], MoveTime);
			//カラスをパネルに合わせてずらす
			for (int i = 0; i < 3; i++)
			{
				crow_[i].AddDistance(-BetDis);
			}
			Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);
		}
	}
	//ステージ番号を0～8に固定
	stageNum_ = MathHelper::Clamp(stageNum_, 0, 8);

	if (IsInputLeft() && !backSelect_)
	{//左を押すと「戻る」にカーソルを移動
		TweenManager::GetInstance().Add(EaseOutExpo, &cursorPos_, CursorPos[1], MoveTime);
		Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);
		backSelect_ = true;
	}
	else if (IsInputAny() && backSelect_)
	{//"上/下/右"のいずれかでパネルにカーソルを移動
		TweenManager::GetInstance().Add(EaseOutExpo, &cursorPos_, CursorPos[0], MoveTime);
		Sound::GetInstance().PlaySE(SE_ID::MOVE_CURSOR_SE);
		backSelect_ = false;

	}

	//背景色セット
	SetBackgroundColor(color_.x, color_.y, color_.z);

	Star();

	SE();
}

//描画
void MenuScreen::Draw() const
{
	Sprite& ins = Sprite::GetInstance();

	//リソースサイズ取得
	static auto bgSize = ins.GetSize(SPRITE_ID::STAGE_SELECT_BACK_SPRITE);
	static auto builSize = ins.GetSize(SPRITE_ID::STAGE_SELECT_M_SPRITE);
	static auto wwwSize = ins.GetSize(SPRITE_ID::WWW_SPRITE);
	static auto nightSize = ins.GetSize(SPRITE_ID::STAGE_SELECT_NIGHT1_SPRITE);

	//星（3枚重ねて描画）
	Vector2 drawNightSize = Vector2(WINDOW_WIDTH / nightSize.x, WINDOW_HEIGHT / nightSize.y);
	ins.Draw(SPRITE_ID::STAGE_SELECT_NIGHT1_SPRITE, Vector2::Zero, Vector2::Zero, starAlpha_[2], drawNightSize);
	ins.Draw(SPRITE_ID::STAGE_SELECT_NIGHT2_SPRITE, Vector2::Zero, Vector2::Zero, starAlpha_[1], drawNightSize);
	ins.Draw(SPRITE_ID::STAGE_SELECT_NIGHT3_SPRITE, Vector2::Zero, Vector2::Zero, starAlpha_[0], drawNightSize);

	////流れ星
	for (int i = 0; i < StarNum; i++) {
		ins.Draw(SPRITE_ID::STAR_SPRITE, sStar_[i].position_, Vector2::Zero, sStar_[i].isAlpha_, Vector2(sStar_[i].scale_, sStar_[i].scale_));
	}

	//背景
	ins.Draw(SPRITE_ID::STAGE_SELECT_BACK_SPRITE, BgPos + from_ * 0.7f, Vector2(bgSize.x / 2.0f - 100.0f, bgSize.y), Vector2(1.5f, 1.5f), 1.0f, false);

	//カラス
	crow_[1].Draw();

	//ビルと草
	ins.Draw(SPRITE_ID::STAGE_SELECT_M_SPRITE, BuilPos + from_, Vector2(builSize.x / 2.0f - 24.0f, builSize.y), Vector2(3.0f, 3.0f), 1.0f, false);
	ins.Draw(SPRITE_ID::WWW_SPRITE, WwwPos + from_, Vector2(wwwSize.x / 2.0f, wwwSize.y), Vector2(2.0f, 2.2f), 1.0f, false);

	//ステージパネル
	static auto panelSize = ins.GetSize(SPRITE_ID::STAGE_SELECT_1_SPRITE);
	static auto trainingSize = ins.GetSize(SPRITE_ID::STAGE_SELECT_TRAINING_SPRITE);
	ins.Draw(SPRITE_ID::STAGE_SELECT_TRAINING_SPRITE, panel[0].position + from_, Vector2(trainingSize.x / 2.0f, trainingSize.y / 2.0f), panel[0].alpha, Vector2::One);
	static int spriteNum = SPRITE_ID::STAGE_SELECT_1_SPRITE - 1;
	for (int i = 1; i <= 8; i++) {
		ins.Draw((SPRITE_ID)(spriteNum + i), panel[i].position + from_, Vector2(panelSize.x / 2.0f, panelSize.y / 2.0f), panel[i].alpha, Vector2::One);
	}

	//カラス
	crow_[0].Draw();
	crow_[2].Draw();

	//戻るパネル
	ins.Draw(SPRITE_ID::TITLE_SELECT_SPRITE, Vector2(0.0f, WINDOW_HEIGHT - 108.5f), Vector2::Zero, 1.0f, Vector2(0.5f, 0.5f));

	//カーソル
	ins.Draw(SPRITE_ID::OROCHI_CURSOR_SPRITE, cursorPos_, Vector2(48.0f, 35.0f), 1.0f, Vector2::One, true, backSelect_);

	//デバッグ表示
	DebugDraw::DebugDrawFormatString(0, 40, GetColor(255, 255, 255), "stageNum:%d", stageNum_);
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

//"上"が入力されたか
bool MenuScreen::IsInputUp() const
{
	return isUseKey_.StickTriggerDown(InputChecker::Input_Stick::Up);
}

//"下"が入力されたか
bool MenuScreen::IsInputDown() const
{
	return isUseKey_.StickTriggerDown(InputChecker::Input_Stick::Down);
}

//"左/A"のいずれかが入力されたか
bool MenuScreen::IsInputLeft() const
{
	return isUseKey_.StickTriggerDown(InputChecker::Input_Stick::Left);
}

//"上/下/右"のいずれかが入力されたか
bool MenuScreen::IsInputAny() const
{
	return isUseKey_.StickTriggerDown(InputChecker::Input_Stick::Up) ||
		isUseKey_.StickTriggerDown(InputChecker::Input_Stick::Down) ||
		isUseKey_.StickTriggerDown(InputChecker::Input_Stick::Right);
}

//星
void MenuScreen::Star()
{
	//下に行くほど透過処理を早くする
	alphaValue_ = 0.007 * (9 - stageNum_);

	//ステージ番号ごとの透過処理
	switch (stageNum_)
	{
	case 8:
		starAlpha_[0] += alphaValue_;
		starAlpha_[1] += alphaValue_;
		starAlpha_[2] += alphaValue_;
		break;
	case 7:
		starAlpha_[0] += alphaValue_;
		starAlpha_[1] += alphaValue_;
		starAlpha_[2] -= alphaValue_;
		break;
	case 6:
		starAlpha_[0] += alphaValue_;
		starAlpha_[1] -= alphaValue_;
		starAlpha_[2] -= alphaValue_;
		break;
	default:
		starAlpha_[0] -= alphaValue_;
		starAlpha_[1] -= alphaValue_;
		starAlpha_[2] -= alphaValue_;
		break;
	}
	for (int i = 0; i < 3; i++) {
		starAlpha_[i] = MathHelper::Clamp(starAlpha_[i], 0.0f, 1.0f);
	}
}

//流れ星
void MenuScreen::ShootingStar()
{
	for (int i = 0; i < StarNum; i++) {
		if (sStar_[i].isAlpha_ >= 1.0f) sStar_[i].isAlpha_ -= 0.25f;
		//0.5間移動
		if (sStar_[i].timer_ > WaitTime[i] + 0.5f) {
			sStar_[i].timer_ = 0.0f;
			sStar_[i].position_ = sStar_[i].prevPos_;
			sStar_[i].isAlpha_ = 0.0f;
			sStar_[i].scale_ = Random::GetInstance().Range(0.5f, 2.0f);
		}
		if (stageNum_ <= 6 && sStar_[i].isAlpha_ == 0.0f) continue;
		sStar_[i].timer_ += Time::DeltaTime;
		//待機じかんを超えたら移動開始
		if (sStar_[i].timer_ > WaitTime[i]) {
			sStar_[i].position_ += Vector2(-20.0f, 10.0f);
			sStar_[i].isAlpha_ += 0.25f;
		}
		sStar_[i].isAlpha_ = MathHelper::Clamp(sStar_[i].isAlpha_, 0.0f, 1.0f);
	}
}

//カラス更新
void MenuScreen::Crow()
{
	for (int i = 0; i < 3; i++) {
		crow_[i].Update(stageNum_);
	}
}

//SE
void MenuScreen::SE()
{
	//決定
	if (!backSelect_ && isUseKey_.KeyTriggerDown(InputChecker::Input_Key::B)) {
		Sound::GetInstance().PlaySE(SE_ID::CHECK_SE);
	}
	//戻る
	if (backSelect_ && isUseKey_.KeyTriggerDown(InputChecker::Input_Key::B)) {
		Sound::GetInstance().PlaySE(SE_ID::CANCEL_SE);
	}
}

//終了
void MenuScreen::End()
{
	Sound::GetInstance().StopBGM();
}

//ステージを取得
Stage MenuScreen::GetGamePlayStage() const
{
	return stageList_[stageNum_];
}

//「戻る」が選択されているか？
bool MenuScreen::GetIsBackSelect() const {
	return backSelect_;
}

//チュートリアルステージが選択されているか？
bool MenuScreen::GetIsTutorialSelect() const {
	return stageNum_ == 0;
}

//ステージをセット
void MenuScreen::InputSelectStage()
{
	CheatData::getInstance().SetStartStage(stageNum_);
	CheatData::getInstance().SetSelectStage(stageList_[stageNum_]);
}

//背景リセット
void MenuScreen::ResetBG()
{
	color_ = BgColor[stageNum_];
	if (stageNum_ <= 5) {
		for (int i = 0; i < 3; i++) {
			starAlpha_[i] = 0.0f;
		}
	}
}
