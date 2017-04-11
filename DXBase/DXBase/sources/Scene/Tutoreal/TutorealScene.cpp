#include "TutorealScene.h"
#include "../../Field/MapGenerator.h"
#include "../../World/World.h"
#include "../../Field/Field.h"
#include "../../Actor/Base/ActorGroup.h"
#include "../../Actor/Person/Player/Player.h"
#include "../../ResourceLoader/Movie.h"
#include "../../Input/InputMgr.h"
#include "../../Math/MathHelper.h"
#include "../../Actor/Person/Player/PlayerConnector.h"
TutorealScene::TutorealScene(SceneDataKeeper* keeper) :
	tutorealRoopCount_(0)
{
	keeper_ = keeper;
	isEnd_ = false;
	deltaTime_ = 1.0f / 60.0f;
	//チュートリアル設定
	//1番
	TutorealName name1;
	name1.csvName = "tutoreal01";
	name1.moviePas = "./resources/Movie/tutorial01.wmv";
	name1.movieID = MOVIE_ID::TUTPREAL_01_MOVIE;
	name1.textIDs.push_back(TextureID::TUTOREAL1_1_TXT_TEX);
	//name1.textIDs.push_back(TextureID::TUTOREAL1_2_TXT_TEX);
	//name1.timeCountIDs.push_back(TextureID::TUTOREAL_TIME_1_TEX);
	//name1.timeCountIDs.push_back(TextureID::TUTOREAL_TIME_2_TEX);
	//name1.timeCountIDs.push_back(TextureID::TUTOREAL_TIME_3_TEX);
	//2番
	TutorealName name2;
	name2.csvName = "tutoreal02";
	name2.moviePas = "./resources/Movie/tutorial02.wmv";
	name2.movieID = MOVIE_ID::TUTPREAL_02_MOVIE;
	name2.textIDs.push_back(TextureID::TUTOREAL2_1_TXT_TEX);
	//name2.textIDs.push_back(TextureID::TUTOREAL2_2_TXT_TEX);
	//name2.timeCountIDs.push_back(TextureID::TUTOREAL_TIME_1_TEX);
	//name2.timeCountIDs.push_back(TextureID::TUTOREAL_TIME_2_TEX);
	//name2.timeCountIDs.push_back(TextureID::TUTOREAL_TIME_3_TEX);
	//3番
	TutorealName name3;
	name3.csvName = "tutoreal03";
	name3.moviePas = "./resources/Movie/tutorial03.wmv";
	name3.movieID = MOVIE_ID::TUTPREAL_03_MOVIE;
	name3.textIDs.push_back(TextureID::TUTOREAL3_1_TXT_TEX);
	//name3.textIDs.push_back(TextureID::TUTOREAL3_2_TXT_TEX);
	//name3.timeCountIDs.push_back(TextureID::TUTOREAL_COUNT_1_TEX);
	//name3.timeCountIDs.push_back(TextureID::TUTOREAL_COUNT_2_TEX);
	////4番
	//TutorealName name4;
	//name4.csvName = "tutoreal02";
	//name4.movieID = MOVIE_ID::TEST_MOVE;
	//name4.textIDs.push_back(TextureID::TUTOREAL4_1_TXT_TEX);
	//name4.textIDs.push_back(TextureID::TUTOREAL4_2_TXT_TEX);
	////name4.textIDs.push_back(TextureID::)
	//name4.textIDs.push_back(TextureID::TUTOREAL4_3_TXT_TEX);

	//設定したやつを入れる
	tutorels_.push_back(name1);
	tutorels_.push_back(name2);
	tutorels_.push_back(name3);
	//tutorels_.push_back(name4);

	Vector2 center = SCREEN_SIZE / 2;

	starPoss_.push_back(Vector2(400, 300));
	starPoss_.push_back(Vector2(750, 400));
	starPoss_.push_back(Vector2(1200, 300));
	starPoss_.push_back(Vector2(1500, 400));
	starPoss_.push_back(Vector2(1300, 600));
	starPoss_.push_back(Vector2(580, 700));
	starPoss_.push_back(Vector2(900, 730));
	starPoss_.push_back(Vector2(400, 800));
	starPoss_.push_back(Vector2(1030, 530));
	starPoss_.push_back(Vector2(1470, 840));
	starPoss_.push_back(Vector2(1360, 670));
	//サイズを入れる
	tutorealSize_ = tutorels_.size();


	starState_.clear();
	for (const auto& i : starPoss_) {
		StarState state;
		state.position = i;
		state.angle = GetRand(360);
		state.Scale = (GetRand(100) / 100.0f) + 0.8f;
		starState_.push_back(state);
	}
}

TutorealScene::~TutorealScene()
{
}

void TutorealScene::start()
{
	//初期化たち
	isEnd_ = false;
	isMovie_ = false;
	isClear_ = false;
	tutoreal4Flag = false;
	isPlayerUp_ = false;
	tutorealTexCount_ = 0;
	resTutorealTexCount_ = 0;
	countAndTime_ = 0;
	stickTime_ = 3.0f;
	endCount_ = 2;
	attackRagCount_ = 0.0f;
	clearTime_ = 0.0f;
	feedAlpha_ = 1.0f;
	mMovieAlpha = 1.0f;
	mClerAlpha = 0.0f;
	//終わるフラグの番号を設定
	endTutorealCount_ = tutorealRoopCount_;
	//α値を初期化
	alpha_ = 1.0f;
	//補間初期化
	size_ = 0.2f;
	movieMoveTime_ = 0.0f;
	Movie::GetInstance().Load(tutorels_[tutorealRoopCount_].movieID, tutorels_[tutorealRoopCount_].moviePas);

	//動画サイズ取得
	Vector2 movieSize = Movie::GetInstance().GetMovieSize(tutorels_[tutorealRoopCount_].movieID)*0.1f;
	//動画の位置設定
	moviePos_ = Vector2(SCREEN_SIZE.x - movieSize.x - 16, 150);
	movieResPos1_ = moviePos_;
	movieResPos2_ = Vector2(SCREEN_SIZE.x / 2, SCREEN_SIZE.y / 2);
	//デルタタイム
	deltaTime_ = Time::GetInstance().deltaTime();
	//次も同じ
	nextScene_ = Scene::Tutoreal;
	//ポーズしない
	isStopped_ = false;
	//
	//ワールド
	world_ = std::make_shared<World>(keeper_);
	MapGenerator gener = MapGenerator(world_.get());
	if (tutorealRoopCount_==0)
		PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_123), DX_PLAYTYPE_LOOP);
	//ネームをセット
	name_ = tutorels_[tutorealRoopCount_].csvName;
	//チュートリアル動画をセット
	movieId_ = tutorels_[tutorealRoopCount_].movieID;
	//テキストをセット
	tutorealTexs_ = tutorels_[tutorealRoopCount_].textIDs;
	//時間とカウントをセット
	tutorealTimes_ = tutorels_[tutorealRoopCount_].timeCountIDs;
	//動画を再生
	Movie::GetInstance().Play(movieId_);
	//セットしたら次の名前にしておく
	if (tutorealSize_ - 1 > tutorealRoopCount_)
		tutorealRoopCount_++;
	//テュートリアルがない場合タイトルへ
	else
		nextScene_ = Scene::MainMenu;
	//プレイヤーの座標をセット
	world_->SetPlayerPos(gener.findStartPoint("./resources/file/" + name_ + ".csv"));
	//プレイヤーを追加
	world_->addActor(ActorGroup::Player, std::make_shared<Player>
		(world_.get(), gener.findStartPoint("./resources/file/" + name_ + ".csv")));

	world_->CollisitionOffOn(true);
	//マップ生成
	gener.create("./resources/file/" + name_ + ".csv", 0, 0);
	//スクロールセット
	world_->SetScroolJudge(Vector2(0, 0), Vector2::Zero, Vector2(9999, 9999));
	//プレイヤーコネクター
	playerConnector_ = dynamic_cast<PlayerConnector*>(world_->findActor("PlayerConnector").get());
	starAnim_.add_anim(0, ResourceLoader::GetInstance().getAnimationIDs(AnimationID::ITEM_ANM));
	starAnim_.change_param(0, 2.0f);
	
}

void TutorealScene::update()
{
	if (!Movie::GetInstance().IsPlay(movieId_)) {
		Movie::GetInstance().Seek(movieId_, 0.0f);
		Movie::GetInstance().Play(movieId_);
	}
	//Tを押したらチュートリアル動画が再生される
	if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_SELECT) && !isClear_&&!isPlayerUp_) {
		isStopped_ ? deltaTime_ = Time::GetInstance().deltaTime() : deltaTime_ = 0;
		isStopped_ = !isStopped_;
		isMovie_ = !isMovie_;
		PlaySound("./resources/sounds/menuse/menu_decision.mp3", DX_PLAYTYPE_BACK);
		if (isMovie_) {
			Movie::GetInstance().Stop(movieId_);
			Movie::GetInstance().Seek(movieId_, 0.0f);
		}
	}

	//補間時間関係
	if (isMovie_) {
		movieMoveTime_ += 90 * Time::GetInstance().deltaTime();
		playerUpCount_ -= movieMoveTime_ / 90.0f;
	}
	else {
		movieMoveTime_ -= 90 * Time::GetInstance().deltaTime();
	}

	//クランプ
	movieMoveTime_ = MathHelper::Clamp(movieMoveTime_, 0.0f, 90.0f);
	//拡大し終わったら再生する
	if (movieMoveTime_ >= 90) Movie::GetInstance().Play(movieId_);
	//α値を線形保管
	alpha_ = MathHelper::Lerp(255.0f, 0.0f, movieMoveTime_ / 90.0f);
	//サイズを線形補間
	size_ = MathHelper::Lerp(0.2f, 0.8f, MathHelper::Sin(movieMoveTime_));
	//移動を線形補間
	moviePos_ = Vector2::Lerp(movieResPos1_, movieResPos2_, MathHelper::Sin(movieMoveTime_));
	//クリアーしてたらプレイヤーをみない
	if (!isClear_) {
		if (world_->findActor("Player")->getPosition().y <= 700.0f) {
			isPlayerUp_ = true;
			playerUpCount_ -= 3.0f* Time::GetInstance().deltaTime();
			if (!isMovie_)
				mMovieAlpha -= 3.0f*Time::GetInstance().deltaTime();
		}
		else {
			isPlayerUp_ = false;
			mMovieAlpha += 3.0f*Time::GetInstance().deltaTime();
			if (!isMovie_)
				playerUpCount_ += 3.0f*Time::GetInstance().deltaTime();

		}
	}


	//クリアー関係
	if (isClear_&&endTutorealCount_ != 3) {
		clearTime_ += Time::GetInstance().deltaTime();

		if (tutorealTexCount_ == 0)
			feedAlpha_ -= 2.0f*Time::GetInstance().deltaTime();
		else
			feedAlpha_ += 2.0f*Time::GetInstance().deltaTime();

		if (feedAlpha_ <= 0.0f) /*tutorealTexCount_ = 1*/;

		if (clearTime_ >= 4.0f) isEnd_ = true;
	}
	//最後のチュートリアルだけの処理
	else if (isClear_) {
		if (tutorels_.back().textIDs.size() - 1 == tutorealTexCount_&&
			InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_CIRCLE)) {
			isEnd_ = true;
		}
		//クリアーした時だけ強制
		if (resTutorealTexCount_ == 0) resTutorealTexCount_++;

		if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_CIRCLE) && feedAlpha_ >= 1.0f) {
			resTutorealTexCount_++;
		}
		if (resTutorealTexCount_ != tutorealTexCount_) {
			feedAlpha_ -= 2.0f*Time::GetInstance().deltaTime();
			if (feedAlpha_ <= 0.0f) {
				tutorealTexCount_ = resTutorealTexCount_;
			}
		}
		if (resTutorealTexCount_ == tutorealTexCount_)
			feedAlpha_ += 2.0f*Time::GetInstance().deltaTime();
		//クランプ
		feedAlpha_ = MathHelper::Clamp(feedAlpha_, 0.0f, 1.0f);
	}

	//if (InputMgr::GetInstance().IsKeyDown(KeyCode::J))
	//	isClear_ = true;

	if (!isClear_)
		EndTutoreal(endTutorealCount_);


	if (!isStopped_)
		world_->update(deltaTime_);

	if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_START)) {
		nextScene_ = Scene::MainMenu;
		isEnd_ = true;
	}

	if (isClear_) {
		mMovieAlpha -= 3.0f*Time::GetInstance().deltaTime();
		playerUpCount_ -= 3.0f*Time::GetInstance().deltaTime();
		if (mClerAlpha == 0.0f)PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGECLEAR), DX_PLAYTYPE_BACK);
		mClerAlpha += 3.0f* Time::GetInstance().deltaTime();
	}
	mClerAlpha = MathHelper::Clamp(mClerAlpha, 0.0f, 2.0f);
	playerUpCount_ = MathHelper::Clamp(playerUpCount_, 0.0f, 1.0f);
	playerUpAlpha_ = MathHelper::Lerp(0.0f, 255.0f, playerUpCount_);
	mMovieAlpha = MathHelper::Clamp(mMovieAlpha, 0.0f, 1.0f);
	starAnim_.update(Time::GetInstance().deltaTime());
}

void TutorealScene::draw() const
{
	DrawGraph(0, 0, ResourceLoader::GetInstance().getTextureID(TextureID::TITLE_BACK_TEX), true);
	world_->draw();

	//αブレンド
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, playerUpAlpha_);
	//テキスト部分
	Vector2 size = ResourceLoader::GetInstance().GetTextureSize(TextureID::TUTOREAL_BACK_TEX)*0.93f / 2.0f;
	DrawRotaGraphF(32 + size.x, 16 + size.y, 0.93f, 0, ResourceLoader::GetInstance().getTextureID(TextureID::TUTOREAL_BACK_TEX), TRUE);
	//if (isClear_)SetDrawBlendMode(DX_BLENDMODE_ALPHA, MathHelper::Lerp(0, 255, feedAlpha_));
	DrawGraph(64, 64, ResourceLoader::GetInstance().getTextureID(tutorealTexs_[tutorealTexCount_]), TRUE);
	////時間とカウント部分
	//if (!tutorealTimes_.empty() && (int)stickTime_ != -1 && (int)endCount_ != -1)
	//	DrawGraph(32, 256 + 128, ResourceLoader::GetInstance().getTextureID(tutorealTimes_[countAndTime_]), TRUE);
	//αブレンド終わり
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, MathHelper::Lerp(0.0f, 255.0f, mMovieAlpha));
	//動画
	Movie::GetInstance().DrawRotaMovie(movieId_, moviePos_, size_);
	//動画の後ろの画像
	int id = ResourceLoader::GetInstance().getTextureID(TextureID::MOVIEBACK_TEX);
	DrawRotaGraph(moviePos_.x, moviePos_.y, size_, 0.0f, id, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, MathHelper::Lerp(0.0f, 255.0f, mClerAlpha));

	for (const auto& i : starState_) {
		starAnim_.draw(i.position, Vector2::Zero, Vector2(i.Scale, i.Scale), i.angle);
	}

	DrawRotaGraph((SCREEN_SIZE / 2).x, (SCREEN_SIZE / 2).y, mClerAlpha, 0.0f,
		ResourceLoader::GetInstance().getTextureID(TextureID::TUTOREAL_CLER_TEX), TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0.0f);


}

void TutorealScene::end()
{
	//再生時間を0に戻す
	Movie::GetInstance().Seek(movieId_, 0.0f);
	//再生ストップ
	Movie::GetInstance().Stop(movieId_);
	Movie::GetInstance().Clear();

	//チュートリアルの進みをリセット
	if (nextScene_ == Scene::MainMenu) {
		StopSoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_123));
		tutorealRoopCount_ = 0;
	}
}

bool TutorealScene::isEnd() const
{
	return isEnd_;
}

Scene TutorealScene::next() const
{
	return nextScene_;
}

bool TutorealScene::EndTutoreal(int num)
{
	if (world_->findActors(ActorGroup::Item).empty())isClear_ = true;
	//switch (num)
	//{
	//case 0: {

	//	break;
	//}
	//case 1: {
	//	if (world_->getCount() >= 1)return true;
	//	break;
	//}
	//case 2: {
	//	if (world_->getCount() >= 1)return true;
	//	break;
	//}
	//case 3: {
	//	if (world_->getCount() >= 1)return true;
	//	break;
	//}
	//}
	return false;
}

bool TutorealScene::Tutoreal1()
{
	if (std::fabsf(InputMgr::GetInstance().AnalogPadVectorL().x) != 0.0f ||
		std::fabsf(InputMgr::GetInstance().AnalogPadVectorR().x) != 0.0f) {
		stickTime_ -= Time::GetInstance().deltaTime();
	}
	else {
		//離したら3秒に戻る
		stickTime_ = 3.9f;
	}
	//カウント
	countAndTime_ = (int)stickTime_ - 1;
	//0秒になったら終わる
	if (stickTime_ < 1.0f) {
		isClear_ = true;
		return true;
	}
	return false;
}

bool TutorealScene::Tutoreal2()
{
	if (playerConnector_->stateMgr_.currentActionType(ActionType::Right) ||
		playerConnector_->stateMgr_.currentActionType(ActionType::Left) &&
		playerConnector_->stateMgr_.currentState((unsigned int)PlayerState_Enum_Union::HOLD)) {
		stickTime_ -= Time::GetInstance().deltaTime();
	}
	else {
		//離したら3秒に戻る
		stickTime_ = 3.9f;
	}
	//カウント
	countAndTime_ = (int)stickTime_ - 1;
	//0秒になったら終わる
	if (stickTime_ < 1.0f) {
		isClear_ = true;
		return true;
	}
	return false;
}

bool TutorealScene::Tutoreal3()
{
	//ラグタイムがないと一瞬で終わるため
	if ((playerConnector_->stateMgr_.currentState((unsigned int)PlayerState_Enum_Union::ATTACK/*_R) ||
		playerConnector_->stateMgr_.currentState((unsigned int)PlayerState_Enum_Union::ATTACK_L*/)) &&
		!attackRagFalg) {
		endCount_--;
		attackRagFalg = true;
	}

	if (attackRagFalg) {
		attackRagCount_ += Time::GetInstance().deltaTime();
		if (attackRagCount_ >= 3.0f) {
			attackRagCount_ = 0.0f;
			attackRagFalg = false;
		}
	}
	countAndTime_ = endCount_ - 1;
	if (endCount_ <= 0) {
		isClear_ = true;
		return true;
	}
	return false;
}

bool TutorealScene::Tutoreal4()
{
	if (world_->is_clear()) {
		isClear_ = true;
		return true;
	}
	return false;
}
