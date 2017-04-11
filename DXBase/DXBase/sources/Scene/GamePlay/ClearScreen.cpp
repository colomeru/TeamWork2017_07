#include "ClearScreen.h"
#include"../../Input/InputMgr.h"
#include"../../Renderer/FontManager.h"
#include"../../Define.h"
#include"../../ResourceLoader/ResourceLoader.h"
#include"../../Game/Time.h"

const static int FONT_SIZE = 64;
static const int LeftSpaceSize = 70;
static const int RightSpaceSize = 80;
static const int RankPosx = 200;
static const int RankPosy = 800;
static const int StarPosx = 800;
static const int StarPosy = 900;
static const int StarPos2_Rx = 850;
static const int StarPos2_Ry = 880;
static const int StarPos_Rx = 750;
static const int StarPos_Ry = 880;
static const int StarPos_Lx = 800;
static const int StarPos_Ly = 850;
static const int StarPos2_Lx = 750;
static const int StarPos2_Ly = 900;
static const int StarPos3_Lx = 850;
static const int StarPos3_Ly = 900;

static const float LastSTime = 0.4f;

static const float centerScoreSize = 0.7f;

static const int center = (int)(SCREEN_SIZE.x) / 2;
ClearScreen::ClearScreen(SceneDataKeeper* keeper):keeper_(keeper), drawNumberController_(0), isMaxDraw_(false),baseLastDraw_(0),drawLastDraw_(0),stageName_("stage00")
{
	ints = 0;
	usingIntCount = 0;
	usingBaseIntCount = 2;
	anmer_ = StageClearTextAnm();
	lastScoreTime = LastSTime;
	isLastScoreDCou = 1;
	numberTexes_.resize(2);

	for (auto& i : numberTexes_) {
		i.push_back(TextureID::NUMBER_ZERO_TEX);
		i.push_back(TextureID::NUMBER_ONE_TEX);
		i.push_back(TextureID::NUMBER_TWO_TEX);
		i.push_back(TextureID::NUMBER_THREE_TEX);
		i.push_back(TextureID::NUMBER_FOUR_TEX);
		i.push_back(TextureID::NUMBER_FIVE_TEX);
		i.push_back(TextureID::NUMBER_SIX_TEX);
		i.push_back(TextureID::NUMBER_SEVEN_TEX);
		i.push_back(TextureID::NUMBER_EIGHT_TEX);
		i.push_back(TextureID::NUMBER_NINE_TEX);
	}

	
	int leftDefPosCount = 0;
	int rightDefPosCount = 0;
	int basePos = 240;

	int leftBasePos = 50;
	int rightBasePos = 1000;
	textPositions["stageclear"] = Vector2(center, -5);
	textPositions["getStar"] = Vector2(leftBasePos, 140);
	textPositions["getStarStar"] = Vector2(leftBasePos+440, 140);
	textPositions["getStarStarCount"] = Vector2(leftBasePos+620, 140);
	textPositions["maxDash"] = Vector2(leftBasePos, basePos+leftDefPosCount*LeftSpaceSize);
	textPositions["maxDashMult"] = Vector2(leftBasePos+520, basePos + leftDefPosCount*LeftSpaceSize);
	textPositions["maxDashMultCount"] = Vector2(leftBasePos+620, basePos + leftDefPosCount*LeftSpaceSize);
	leftDefPosCount++;

	textPositions["WalkBlockCount"] = Vector2(rightBasePos, basePos);
	textPositions["WalkBlockCountMult"] = Vector2(rightBasePos+520, basePos);
	textPositions["WalkBlockCountMultcount"] = Vector2(rightBasePos+620, basePos);
	textPositions["5Dash"] = Vector2(leftBasePos+20, basePos+leftDefPosCount*LeftSpaceSize);
	textPositions["5DashMult"] = Vector2(leftBasePos+520, basePos + leftDefPosCount*LeftSpaceSize);
	textPositions["5DashMultCount"] = Vector2(leftBasePos+620, basePos + leftDefPosCount*LeftSpaceSize);
	rightDefPosCount++;
	leftDefPosCount++;

	textPositions["YellowStar"] = Vector2(rightBasePos, basePos+rightDefPosCount*RightSpaceSize);
	textPositions["YellowStarMult"] = Vector2(rightBasePos+520, basePos + rightDefPosCount*RightSpaceSize);
	textPositions["YellowStarMultCount"] = Vector2(rightBasePos+620, basePos + rightDefPosCount*RightSpaceSize);
	textPositions["4Dash"] = Vector2(leftBasePos+20, basePos + leftDefPosCount*LeftSpaceSize);
	textPositions["4DashMult"] = Vector2(leftBasePos+520, basePos + leftDefPosCount*LeftSpaceSize);
	textPositions["4DashMultCount"] = Vector2(leftBasePos+620, basePos + leftDefPosCount*LeftSpaceSize);
	rightDefPosCount++;
	leftDefPosCount++;

	//textPositions["PurpleStar"] = Vector2(1000, basePos+rightDefPosCount*RightSpaceSize);
	//textPositions["PurpleStarMult"] = Vector2(1450, basePos+rightDefPosCount*RightSpaceSize);
	//textPositions["PurpleStarMultCount"] = Vector2(1600, basePos+rightDefPosCount*RightSpaceSize);
	textPositions["3Dash"] = Vector2(leftBasePos+20, basePos + leftDefPosCount*LeftSpaceSize);
	textPositions["3DashMult"] = Vector2(leftBasePos+520, basePos + leftDefPosCount*LeftSpaceSize);
	textPositions["3DashMultCount"] = Vector2(leftBasePos+620, basePos + leftDefPosCount*LeftSpaceSize);
	//rightDefPosCount++;
	leftDefPosCount++;

	textPositions["StickWall"] = Vector2(rightBasePos, basePos + rightDefPosCount*RightSpaceSize);
	textPositions["StickWallMult"] = Vector2(rightBasePos+520, basePos + rightDefPosCount*RightSpaceSize);
	textPositions["StickWallMultCount"] = Vector2(rightBasePos+620, basePos + rightDefPosCount*RightSpaceSize);
	textPositions["2Dash"] = Vector2(leftBasePos+20, basePos + leftDefPosCount*LeftSpaceSize);
	textPositions["2DashMult"] = Vector2(leftBasePos+520, basePos + leftDefPosCount*LeftSpaceSize);
	textPositions["2DashMultCount"] = Vector2(leftBasePos+620, basePos + leftDefPosCount*LeftSpaceSize);
	rightDefPosCount++;
	leftDefPosCount++;

	textPositions["EnemyBonus"] = Vector2(rightBasePos, basePos + rightDefPosCount*RightSpaceSize);
	rightDefPosCount++;

	textPositions["EnemyBonusMult"] = Vector2(rightBasePos, basePos + rightDefPosCount*RightSpaceSize+50);
	textPositions["EnemyBonusMultCount"] = Vector2(rightBasePos+100, basePos + rightDefPosCount*RightSpaceSize+50);
	textPositions["DamageCount"] = Vector2(leftBasePos, basePos + leftDefPosCount*LeftSpaceSize);
	textPositions["DamageCountMinus"] = Vector2(leftBasePos+520, basePos + leftDefPosCount*LeftSpaceSize);
	textPositions["DamageCountMinusCount"] = Vector2(leftBasePos+620, basePos + leftDefPosCount*LeftSpaceSize);
	rightDefPosCount++;
	leftDefPosCount++;

	textPositions["ChargeTime"] = Vector2(leftBasePos, basePos + leftDefPosCount*LeftSpaceSize);
	textPositions["ChargeTimeMinus"] = Vector2(leftBasePos+520, basePos + leftDefPosCount*LeftSpaceSize);
	textPositions["ChargeTimeMinusCount"] = Vector2(leftBasePos+620, basePos + leftDefPosCount*LeftSpaceSize);
	leftDefPosCount++;

	textPositions["Total"] = Vector2(center - 114 / 2, 750);
	textPositions["TotalMult"] = Vector2(1350, 750);
	textPositions["TotalMultCount"] = Vector2(1520, 750);
	textPositions["TotalGetStar"] = Vector2(SCREEN_SIZE.x-ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX).x, 850);
	textPositions["TotalGetStarCount"] = Vector2(SCREEN_SIZE.x, 850);
	//textPositions["TotalGetStarCount"] = Vector2(SCREEN_SIZE.x - ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX).x*11, 950);

	isDraw_["stageclear"] = false;
	isDraw_["getStar"] = false;
	isDraw_["getStarStar"] = false;
	isDraw_["getStarStarCount"] = false;
	isDraw_["maxDash"] = false;
	isDraw_["maxDashMult"] = false;
	isDraw_["maxDashMultCount"] = false;
	isDraw_["WalkBlockCount"] = false;
	isDraw_["WalkBlockCountMult"] = false;
	isDraw_["WalkBlockCountMultcount"] = false;
	isDraw_["5Dash"] = false;
	isDraw_["5DashMult"] = false;
	isDraw_["5DashMultCount"] = false;
	isDraw_["YellowStar"] = false;
	isDraw_["YellowStarMult"] = false;
	isDraw_["YellowStarMultCount"] = false;
	isDraw_["4Dash"] = false;
	isDraw_["4DashMult"] = false;
	isDraw_["4DashMultCount"] = false;
	//isDraw_["PurpleStar"] = false;
	//isDraw_["PurpleStarMult"] = false;
	//isDraw_["PurpleStarMultCount"] = false;
	isDraw_["3Dash"] = false;
	isDraw_["3DashMult"] = false;
	isDraw_["3DashMultCount"] = false;
	isDraw_["StickWall"] = false;
	isDraw_["StickWallMult"] = false;
	isDraw_["StickWallMultCount"] = false;
	isDraw_["2Dash"] = false;
	isDraw_["2DashMult"] = false;
	isDraw_["2DashMultCount"] = false;
	isDraw_["EnemyBonus"] = false;
	isDraw_["EnemyBonusMult"] = false;
	isDraw_["EnemyBonusMultCount"] = false;
	isDraw_["DamageCount"] = false;
	isDraw_["DamageCountMinus"] = false;
	isDraw_["DamageCountMinusCount"] = false;
	isDraw_["ChargeTime"] = false;
	isDraw_["ChargeTimeMinus"] = false;
	isDraw_["ChargeTimeMinusCount"] = false;
	isDraw_["Total"] = false;
	isDraw_["TotalMult"] = false;
	isDraw_["TotalMultCount"] = false;
	isDraw_["TotalGetStar"] = false;
	isDraw_["TotalGetStarCount"] = false;
	
	isBaseDraw_["stageclear"] = false;
	isBaseDraw_["getStar"] = false;
	isBaseDraw_["maxDash"] = false;
	isBaseDraw_["WalkBlockCount"] = false;
	isBaseDraw_["5Dash"] = false;
	isBaseDraw_["YellowStar"] = false;
	isBaseDraw_["4Dash"] = false;
	//isDraw_["PurpleStar"] = false;
	isBaseDraw_["3Dash"] = false;
	isBaseDraw_["StickWall"] = false;
	isBaseDraw_["2Dash"] = false;
	isBaseDraw_["EnemyBonus"] = false;
	isBaseDraw_["DamageCount"] = false;
	isBaseDraw_["ChargeTime"] = false;
	isBaseDraw_["Total"] = false;
	isBaseDraw_["TotalGetStar"] = false;

	NumberForLoop_.push_back(0);
	NumberForLoop_.push_back(1);
	NumberForLoop_.push_back(2);
	NumberForLoop_.push_back(3);
	NumberForLoop_.push_back(4);
	NumberForLoop_.push_back(5);
	NumberForLoop_.push_back(6);
	NumberForLoop_.push_back(7);
	NumberForLoop_.push_back(8);
	NumberForLoop_.push_back(9);

	//nameList.push_back("stageclear");
	//nameList.push_back("getStar");
	//nameList.push_back("getStarStar");
	//nameList.push_back("getStarStarCount");
	//nameList.push_back("maxDash");
	//nameList.push_back("maxDashMult");
	//nameList.push_back("maxDashMultCount");
	//nameList.push_back("WalkBlockCount");
	//nameList.push_back("WalkBlockCountMult");
	//nameList.push_back("WalkBlockCountMultcount");
	//nameList.push_back("5Dash");
	//nameList.push_back("5DashMult");
	//nameList.push_back("5DashMultCount");
	//nameList.push_back("YellowStar");
	//nameList.push_back("YellowStarMult");
	//nameList.push_back("YellowStarMultCount");
	//nameList.push_back("4Dash");
	//nameList.push_back("4DashMult");
	//nameList.push_back("4DashMultCount");
	////nameList.push_back("PurpleStar");
	////nameList.push_back("PurpleStarMult");
	////nameList.push_back("PurpleStarMultCount");
	//nameList.push_back("3Dash");
	//nameList.push_back("3DashMult");
	//nameList.push_back("3DashMultCount");
	//nameList.push_back("StickWall");
	//nameList.push_back("StickWallMult");
	//nameList.push_back("StickWallMultCount");
	//nameList.push_back("2Dash");
	//nameList.push_back("2DashMult");
	//nameList.push_back("2DashMultCount");
	//nameList.push_back("EnemyBonus");
	//nameList.push_back("EnemyBonusMult");
	//nameList.push_back("EnemyBonusMultCount");
	//nameList.push_back("DamageCount");
	//nameList.push_back("DamageCountMinus");
	//nameList.push_back("DamageCountMinusCount");
	//nameList.push_back("ChargeTime");
	//nameList.push_back("ChargeTimeMinus");
	//nameList.push_back("ChargeTimeMinusCount");
	//nameList.push_back("Total");
	//nameList.push_back("TotalMult");
	//nameList.push_back("TotalMultCount");
	//nameList.push_back("TotalGetStar");
	//nameList.push_back("TotalGetStarCount");
	
	nameList.push_back("getStarStarCount");
	nameList.push_back("maxDashMultCount");
	nameList.push_back("5DashMultCount");
	nameList.push_back("4DashMultCount");
	nameList.push_back("3DashMultCount");
	nameList.push_back("2DashMultCount");
	nameList.push_back("DamageCountMinusCount");
	nameList.push_back("ChargeTimeMinusCount");
	nameList.push_back("WalkBlockCountMultcount");
	nameList.push_back("YellowStarMultCount");
	//nameList.push_back("PurpleStarMultCount");
	nameList.push_back("StickWallMultCount");
	nameList.push_back("EnemyBonusMultCount");
	nameList.push_back("TotalMultCount");
	nameList.push_back("TotalGetStarCount");

	nameBaseList.push_back("stageclear");
	nameBaseList.push_back("getStar");
	nameBaseList.push_back("maxDash");
	nameBaseList.push_back("5Dash");
	nameBaseList.push_back("4Dash");
	nameBaseList.push_back("3Dash");
	nameBaseList.push_back("2Dash");
	nameBaseList.push_back("DamageCount");
	nameBaseList.push_back("ChargeTime");
	nameBaseList.push_back("WalkBlockCount");
	nameBaseList.push_back("YellowStar");
	//nameBaseList.push_back("PurpleStar");
	nameBaseList.push_back("StickWall");
	nameBaseList.push_back("EnemyBonus");
	nameBaseList.push_back("Total");
	nameBaseList.push_back("TotalGetStar");


	isBaseDraw_[nameList[0]] = true;
	isBaseDraw_[nameList[1]] = true;

	//Count_ = 3600;

	//スキップに関するフラグ、スキップキーを初めて押したかどうか
	isFirst_ = true;

	//ボタンの制限に関するフラグ、遷移してから指定フレーム分ボタン制限する
	buttonLock_=true;
	buttonLockCount_=60;

	//ランク表示時の☆の数に関するフラグ、Aなら3個使う
	isStarDraw_.resize(3);
	for (int i = 0; i < isStarDraw_.size(); i++) {
		isStarDraw_[i] = false;
	}
	//ランクそのものの表示に関するフラグ
	isRankDraw_ = false;

	//サウンドの再生が初めてかどうかを調べるフラグ
	isSoundFirst_ = true;
	isRankSoundCheckFirst_ = false;
	rankSoundCheckCount_ = 0;

	FullStarCountVec.clear();
	FullStarCountVec.resize(11);
	int starCou = 0;
	for (int i = 0; i < FullStarCountVec.size(); i++) {
		FullStarCountVec[i]=starCou;
		starCou++;
		if (starCou > 9) {
			starCou = 0;
		}
	}
	numberDrawMaster_.clear();
	numberDrawMaster_.resize(11);
	for (int i = 0; i < numberDrawMaster_.size();i++) {
		numberDrawMaster_[i] = false;
	}
	isMyssFirst_ = true;
	isMusLastFirst_ = true;
	isEnterDraw_ = false;
	isEnterDrawNum_ = 0;

	myd = 0;
	isAlreadyStart_ = true;
	stageTexes_["stage00"] = TextureID::STAGE_01_TEX;
	stageTexes_["stage01"] = TextureID::STAGE_01_TEX;
	stageTexes_["stage02"] = TextureID::STAGE_02_TEX;
	stageTexes_["stage03"] = TextureID::STAGE_03_TEX;
	stageTexes_["stage04"] = TextureID::STAGE_04_TEX;
	isCircleDraw_ = false;
}

void ClearScreen::start(const std::string& name)
{
	if (!isAlreadyStart_)return;

	stageName_ = name;

	ChangeNextPlayVolumeSoundMem(150, ResourceLoader::GetInstance().getSoundID(SoundID::SE_RESULT_ROLL));
	PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_RESULT_ROLL), DX_PLAYTYPE_LOOP);

	isAlreadyStart_ = false;
	isCircleDraw_ = false;
	sinCount = 0;
}

bool ClearScreen::update(std::string name, Scene& next)
{
	if (isCircleDraw_) {
		sinCount += 3;
		sinCount = sinCount % 360;
		sinCount = min(max(sinCount, 0), 360);
	}
	//countDigits(myd);
	anmer_.update_e(Time::GetInstance().deltaTime());

	buttonLockCount_ -= Time::GetInstance().deltaTime();
	if (buttonLockCount_ <= 0)buttonLock_ = false;

	myd += 10;
	ints += 10;

	//最終取得スター数部分のランダム数値用カウント
	for (int i = 0; i < FullStarCountVec.size(); i++) {
		FullStarCountVec[i]++;
		if (FullStarCountVec[i] > 9) {
			FullStarCountVec[i] = 0;
		}
	}
	//通常表示系ランダム数値のカウント
	for (int i = 0; i < NumberForLoop_.size(); i++) {
		NumberForLoop_[i]++;
		if (NumberForLoop_[i] > 9) {
			NumberForLoop_[i] = 0;
		}
	}

	if (usingIntCount >= nameList.size() - 1)
	{
		lastScoreTime -= 0.016;
		useCount -= 0.016;
		//int myss = drawTagertNumber(keeper_->GetStageScore(name),1,textPositions.at("TotalGetStarCount").x - (ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX).x * 11), textPositions.at("TotalGetStarCount").y, 0);
		int myss = countDigits(keeper_->GetStageScore(name));
		if (lastScoreTime <= 0){
			if (isMyssFirst_) {
				numberDrawMaster_[isLastScoreDCou - 1] = true;
				isLastScoreDCou++;
				if (isLastScoreDCou >= myss) {
					isLastScoreDCou = myss;
					drawNumberController_ = 1;
					isMyssFirst_ = false;
					if(myss!=1)useCount = LastSTime;
					else useCount = 0;
				}
				PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_RESULT_SHOW), DX_PLAYTYPE_BACK);
			}
			lastScoreTime = LastSTime;
		}
		if (!isMyssFirst_&&useCount <= 0)
		{
			isDraw_["TotalGetStarCount"] = true;
			if(isMusLastFirst_&&myss != 1)PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_RESULT_SHOW), DX_PLAYTYPE_BACK);
			isEnterDraw_ = true;
			isMusLastFirst_ = false;
		}
	}
	rankSoundCheckCount_--;
	if (rankSoundCheckCount_ <= 0) {
		isRankSoundCheckFirst_ = false;
	}
	if (isEnterDraw_) {
		isEnterDrawNum_ += 0.016f;
		if (isEnterDrawNum_ > 1.f&&!isStarDraw_[2]) {
			isRankDraw_ = true;
			isStarDraw_[0] = true;
			isStarDraw_[1] = true;
			isStarDraw_[2] = true;
			rankSoundCheckCount_ = 1;
			isRankSoundCheckFirst_ = true;
			//Count_ = 120;
			isCircleDraw_ = true;
		}
	}

	if (InputMgr::GetInstance().IsPushButton()&&isFirst_&&!buttonLock_&&!isRankDraw_) {
		isFirst_ = false;
		rankSoundCheckCount_ = 1;
		isRankSoundCheckFirst_ = true;
		//Count_ = 120;
		for (auto& i : isBaseDraw_) {
			isBaseDraw_[i.first] = true;
		}
		for (auto& i : isDraw_) {
			isDraw_[i.first] = true;
		}
		for (auto& i : numberDrawMaster_) {
			i = true;
		}
		for (int i = 0; i < isStarDraw_.size(); i++) {
			isStarDraw_[i] = true;
		}
		isRankDraw_ = true;
		//if(isSoundFirst_)PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_RESULT_SHOWMAX), DX_PLAYTYPE_BACK);
		StopSoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_RESULT_ROLL));
		ChangeVolumeSoundMem(0, ResourceLoader::GetInstance().getSoundID(SoundID::SE_RESULT_SHOW));
		isSoundFirst_ = false;
		buttonLockCount_ = 30;
		buttonLock_ = true;
		isEnterDrawNum_ = 2;
		isEnterDraw_ = true;
		isDraw_["TotalGetStarCount"] = true;
		isCircleDraw_ = true;
		isMaxDraw_ = true;
	}
	else if (InputMgr::GetInstance().IsPushButton() && !buttonLock_ && isRankDraw_) {
		next = Scene::StageClear;
		return true;
	}
	drawNumberController_ += 0.016f;

	if (drawNumberController_>0.2f&&!isMaxDraw_) {
		isBaseDraw_[nameBaseList[usingBaseIntCount]] = true;
		isDraw_[nameList[usingIntCount]]=true;
		if (nameList[usingIntCount] == "TotalGetStarCount") {
			isDraw_[nameList[usingIntCount]] = false;
		}
		else {
			PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_RESULT_SHOW), DX_PLAYTYPE_BACK);
		}
		usingIntCount++;
		usingBaseIntCount++;
		if (usingIntCount >= nameList.size()) {
			usingIntCount = nameList.size() - 1;
			isMaxDraw_ = true;
			StopSoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_RESULT_ROLL));
			isSoundFirst_ = false;
		}

		if (usingBaseIntCount >= nameBaseList.size())usingBaseIntCount = nameBaseList.size() - 1;
		drawNumberController_ = 0;
	}
	//Count_--;
	
	//if (Count_<=0) {
	//	next = Scene::StageClear;
	//	return true;
	//}
	return false;
}

void ClearScreen::draw() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(ints));
	DrawGraph(0, 0, ResourceLoader::GetInstance().getTextureID(TextureID::PAUSE_BACK_TEX), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (isCircleDraw_) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(abs(sin(sinCount*MathHelper::Pi / 180)) * 255));
		DrawRotaGraph2(center, SCREEN_SIZE.y - (ResourceLoader::GetInstance().GetTextureSize(TextureID::TEXT_MARU_TEX).y / 4),
			ResourceLoader::GetInstance().GetTextureSize(TextureID::TEXT_MARU_TEX).x / 2, ResourceLoader::GetInstance().GetTextureSize(TextureID::TEXT_MARU_TEX).y / 2, 0.5f, 0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_MARU_TEX), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	int count, heightPoint, multAddCount;
	multAddCount = 0;
	//int strLen, strWidth, center, count, heightPoint;
	count = 0;
	heightPoint = 0;
	int forcount = 0;
	//center = static_cast<int>(SCREEN_SIZE.x) / 2;
	int setLoopNum = 0;

	int stagepos = ResourceLoader::GetInstance().GetTextureSize(stageTexes_.at(stageName_)).x*0.6;
	
	DrawRotaGraph2(50, textPositions.at("getStar").y,0,0,0.6,0, ResourceLoader::GetInstance().getTextureID(stageTexes_.at(stageName_)), TRUE);

	//anmer_.draw_e(Vector2(static_cast<float>(center), 0), Vector2::Zero, 1);
	anmer_.draw_e(textPositions.at("stageclear"));

	DrawGraph(textPositions.at("getStar").x+ stagepos, textPositions.at("getStar").y, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_GETSTAR_TEX), TRUE);
	DrawGraph(textPositions.at("getStarStar").x+ stagepos, textPositions.at("getStarStar").y, ResourceLoader::GetInstance().getTextureID(TextureID::ITEM_TEX), TRUE);
	int drawNum = keeper_->GetItemCount();
	int baseNum = keeper_->GetItemCount();
	if (isDraw_.at("getStarStarCount")) {
		drawNumber(baseNum,
			textPositions.at("getStarStarCount").x + stagepos, textPositions.at("getStarStarCount").y, 0);
	}
	else {
		LoopNumber(textPositions.at("getStarStarCount").x + stagepos, textPositions.at("getStarStarCount").y, setLoopNum, 0);
		setLoopNum++;
		if (setLoopNum > 9) {
			setLoopNum = 0;
		}
	}
	if (isBaseDraw_.at("maxDash")) {
		DrawRotaGraph2(textPositions.at("maxDash").x, textPositions.at("maxDash").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_MAXCOMBO_TEX), TRUE);
		DrawRotaGraph2(textPositions.at("maxDashMult").x, textPositions.at("maxDashMult").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_MULTIPLY_TEX), TRUE);
		drawNum = keeper_->maxComboGet();
		baseNum = keeper_->maxComboGet();
		multAddCount += keeper_->maxComboGet();
		if (isDraw_.at("maxDashMultCount")) {
			drawNumber(baseNum,
				textPositions.at("maxDashMultCount").x, textPositions.at("maxDashMultCount").y,1,3, centerScoreSize);
		}
		else {
			LoopNumber(textPositions.at("maxDashMultCount").x, textPositions.at("maxDashMultCount").y, setLoopNum,1,3, centerScoreSize);
			setLoopNum++;
			if (setLoopNum > 9) {
				setLoopNum = 0;
			}
		}
	}

	if (isBaseDraw_.at("WalkBlockCount")) {
		DrawRotaGraph2(textPositions.at("WalkBlockCount").x, textPositions.at("WalkBlockCount").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_WALK_BLOCK_COUNT_TEX), TRUE);
		DrawRotaGraph2(textPositions.at("WalkBlockCountMult").x, textPositions.at("WalkBlockCountMult").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_MULTIPLY_TEX), TRUE);
		drawNum = keeper_->getMoveDistance() / CHIPSIZE;
		baseNum = keeper_->getMoveDistance() / CHIPSIZE;
		multAddCount += (keeper_->getMoveDistance() / CHIPSIZE);
		if (isDraw_.at("WalkBlockCountMultcount")) {
			drawNumber(baseNum,
				textPositions.at("WalkBlockCountMultcount").x, textPositions.at("WalkBlockCountMultcount").y,1,3, centerScoreSize);
		}
		else {
			LoopNumber(textPositions.at("WalkBlockCountMultcount").x, textPositions.at("WalkBlockCountMultcount").y, setLoopNum,1,3, centerScoreSize);
			setLoopNum++;
			if (setLoopNum > 9) {
				setLoopNum = 0;
			}
		}
	}
	if (isBaseDraw_.at("5Dash")) {
		DrawRotaGraph2(textPositions.at("5Dash").x, textPositions.at("5Dash").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_COMBO5_TEX), TRUE);
		DrawRotaGraph2(textPositions.at("5DashMult").x, textPositions.at("5DashMult").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_MULTIPLY_TEX), TRUE);
		drawNum = keeper_->getComboCount(5);
		baseNum = keeper_->getComboCount(5);
		multAddCount += keeper_->getComboCount(5);
		if (isDraw_.at("5DashMultCount")) {
			drawNumber(baseNum,
				textPositions.at("5DashMultCount").x, textPositions.at("5DashMultCount").y,1,3, centerScoreSize);
		}
		else {
			LoopNumber(textPositions.at("5DashMultCount").x, textPositions.at("5DashMultCount").y, setLoopNum,1,3, centerScoreSize);
			setLoopNum++;
			if (setLoopNum > 9) {
				setLoopNum = 0;
			}
		}
	}

	if (isBaseDraw_.at("YellowStar")) {
		DrawRotaGraph2(textPositions.at("YellowStar").x, textPositions.at("YellowStar").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_STAR_COUNT_TEX), TRUE);
		DrawRotaGraph2(textPositions.at("YellowStarMult").x, textPositions.at("YellowStarMult").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_MULTIPLY_TEX), TRUE);
		if (keeper_->GetMaxItemCount() != 0) {
			drawNum = (float)keeper_->getPlusStar() / keeper_->GetMaxItemCount() * 10;
			baseNum = (float)keeper_->getPlusStar() / keeper_->GetMaxItemCount() * 10;
			multAddCount += (float)keeper_->getPlusStar() / keeper_->GetMaxItemCount() * 10;
		}
		else {
			drawNum = 0;
			baseNum = 0;
		}
		if (isDraw_.at("YellowStarMultCount")) {
			drawNumber(baseNum,
				textPositions.at("YellowStarMultCount").x, textPositions.at("YellowStarMultCount").y,1,3, centerScoreSize);
		}
		else {
			LoopNumber(textPositions.at("YellowStarMultCount").x, textPositions.at("YellowStarMultCount").y, setLoopNum,1,3, centerScoreSize);
			setLoopNum++;
			if (setLoopNum > 9) {
				setLoopNum = 0;
			}
		}
	}

	if (isBaseDraw_.at("4Dash")) {
		DrawRotaGraph2(textPositions.at("4Dash").x, textPositions.at("4Dash").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_COMBO4_TEX), TRUE);
		DrawRotaGraph2(textPositions.at("4DashMult").x, textPositions.at("4DashMult").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_MULTIPLY_TEX), TRUE);
		drawNum = keeper_->getComboCount(4);
		baseNum = keeper_->getComboCount(4);
		multAddCount += keeper_->getComboCount(4);
		if (isDraw_.at("4DashMultCount")) {
			drawNumber(baseNum,
				textPositions.at("4DashMultCount").x, textPositions.at("4DashMultCount").y,1,3, centerScoreSize);
		}
		else {
			LoopNumber(textPositions.at("4DashMultCount").x, textPositions.at("4DashMultCount").y, setLoopNum,1,3, centerScoreSize);
			setLoopNum++;
			if (setLoopNum > 9) {
				setLoopNum = 0;
			}
		}
	}

	//DrawGraph(textPositions.at("PurpleStar").x, textPositions.at("PurpleStar").y, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_MINUS_STAR_COUNT_TEX), TRUE);
	//DrawGraph(textPositions.at("PurpleStarMult").x, textPositions.at("PurpleStarMult").y, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_MULTIPLY_TEX), TRUE);
	//drawNum = keeper_->getMinusStar();
	//baseNum = keeper_->getMinusStar();
	//multAddCount+= keeper_->getMinusStar();
	//drawNumber(drawNum, baseNum,
	//	textPositions.at("PurpleStarMultCount").x, textPositions.at("PurpleStarMultCount").y);
	if (isBaseDraw_.at("3Dash")) {
		DrawRotaGraph2(textPositions.at("3Dash").x, textPositions.at("3Dash").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_COMBO3_TEX), TRUE);
		DrawRotaGraph2(textPositions.at("3DashMult").x, textPositions.at("3DashMult").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_MULTIPLY_TEX), TRUE);
		drawNum = keeper_->getComboCount(3);
		baseNum = keeper_->getComboCount(3);
		multAddCount += keeper_->getComboCount(3);
		if (isDraw_.at("3DashMultCount")) {
			drawNumber(baseNum,
				textPositions.at("3DashMultCount").x, textPositions.at("3DashMultCount").y,1,3, centerScoreSize);
		}
		else {
			LoopNumber(textPositions.at("3DashMultCount").x, textPositions.at("3DashMultCount").y, setLoopNum,1,3, centerScoreSize);
			setLoopNum++;
			if (setLoopNum > 9) {
				setLoopNum = 0;
			}
		}
	}

	if (isBaseDraw_.at("StickWall")) {
		DrawRotaGraph2(textPositions.at("StickWall").x, textPositions.at("StickWall").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_STICK_COUNT_TEX), TRUE);
		DrawRotaGraph2(textPositions.at("StickWallMult").x, textPositions.at("StickWallMult").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_MULTIPLY_TEX), TRUE);
		drawNum = keeper_->getHoldCount();
		baseNum = keeper_->getHoldCount();
		multAddCount += keeper_->getHoldCount();
		if (isDraw_.at("StickWallMultCount")) {
			drawNumber(baseNum,
				textPositions.at("StickWallMultCount").x, textPositions.at("StickWallMultCount").y,1,3, centerScoreSize);
		}
		else {
			LoopNumber(textPositions.at("StickWallMultCount").x, textPositions.at("StickWallMultCount").y, setLoopNum,1,3, centerScoreSize);
			setLoopNum++;
			if (setLoopNum > 9) {
				setLoopNum = 0;
			}
		}

	}
	if (isBaseDraw_.at("2Dash")) {
		DrawRotaGraph2(textPositions.at("2Dash").x, textPositions.at("2Dash").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_COMBO2_TEX), TRUE);
		DrawRotaGraph2(textPositions.at("2DashMult").x, textPositions.at("2DashMult").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_MULTIPLY_TEX), TRUE);
		drawNum = keeper_->getComboCount(2);
		baseNum = keeper_->getComboCount(2);
		multAddCount += keeper_->getComboCount(2);
		if (isDraw_.at("2DashMultCount")) {
			drawNumber(baseNum,
				textPositions.at("2DashMultCount").x, textPositions.at("2DashMultCount").y,1,3, centerScoreSize);
		}
		else {
			LoopNumber(textPositions.at("2DashMultCount").x, textPositions.at("2DashMultCount").y, setLoopNum,1,3, centerScoreSize);
			setLoopNum++;
			if (setLoopNum > 9) {
				setLoopNum = 0;
			}
		}
	}

	if (keeper_->getEnemyCount() == 0) {
		if (isBaseDraw_.at("EnemyBonus")) {
			DrawRotaGraph2(textPositions.at("EnemyBonus").x, textPositions.at("EnemyBonus").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_NoENEMY_DEATH_TEX), TRUE);
			DrawRotaGraph2(textPositions.at("EnemyBonusMult").x, textPositions.at("EnemyBonusMult").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_MULTIPLY_TEX), TRUE);
			drawNum = 100;
			baseNum = 100;
			multAddCount += 100;
			if (isDraw_.at("EnemyBonusMultCount")) {
				drawNumber(baseNum,
					textPositions.at("EnemyBonusMultCount").x, textPositions.at("EnemyBonusMultCount").y,1,3, centerScoreSize);
			}
			else {
				LoopNumber(textPositions.at("EnemyBonusMultCount").x, textPositions.at("EnemyBonusMultCount").y, setLoopNum,1,3, centerScoreSize);
				setLoopNum++;
				if (setLoopNum > 9) {
					setLoopNum = 0;
				}
			}
		}
	}
	if (isBaseDraw_.at("DamageCount")) {
		DrawRotaGraph2(textPositions.at("DamageCount").x, textPositions.at("DamageCount").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_DAMAGE_COUNT_TEX), TRUE);
		DrawRotaGraph2(textPositions.at("DamageCountMinus").x, textPositions.at("DamageCountMinus").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_MINUS_MULT_TEX), TRUE);
		drawNum = keeper_->getDamageCount();
		baseNum = keeper_->getDamageCount();
		multAddCount -= keeper_->getDamageCount();
		if (isDraw_.at("DamageCountMinusCount")) {
			drawNumber(baseNum,
				textPositions.at("DamageCountMinusCount").x, textPositions.at("DamageCountMinusCount").y,1,3, centerScoreSize);
		}
		else {
			LoopNumber(textPositions.at("DamageCountMinusCount").x, textPositions.at("DamageCountMinusCount").y, setLoopNum,1,3, centerScoreSize);
			setLoopNum++;
			if (setLoopNum > 9) {
				setLoopNum = 0;
			}
		}
	}

	if (isBaseDraw_.at("ChargeTime")) {
		DrawRotaGraph2(textPositions.at("ChargeTime").x, textPositions.at("ChargeTime").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_CHARGETIME_TEX), TRUE);
		DrawRotaGraph2(textPositions.at("ChargeTimeMinus").x, textPositions.at("ChargeTimeMinus").y,0,0, centerScoreSize,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_MINUS_MULT_TEX), TRUE);
		drawNum = keeper_->getChargeTime();
		baseNum = keeper_->getChargeTime();
		multAddCount -= keeper_->getChargeTime();
		if (isDraw_.at("ChargeTimeMinusCount")) {
			drawNumber(baseNum,
				textPositions.at("ChargeTimeMinusCount").x, textPositions.at("ChargeTimeMinusCount").y,1,3, centerScoreSize);
		}
		else {
			LoopNumber(textPositions.at("ChargeTimeMinusCount").x, textPositions.at("ChargeTimeMinusCount").y, setLoopNum,1,3, centerScoreSize);
			setLoopNum++;
			if (setLoopNum > 9) {
				setLoopNum = 0;
			}
		}
	}

	if (isBaseDraw_.at("Total")) {
		DrawRotaGraph2(textPositions.at("Total").x, textPositions.at("Total").y,0,0, 1,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_TOTAL_TEX), TRUE);
		DrawRotaGraph2(textPositions.at("TotalMult").x, textPositions.at("TotalMult").y,0,0, 1,0, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_MULTIPLY_TEX), TRUE);
		drawNum = multAddCount;
		baseNum = multAddCount;
		if (isDraw_.at("TotalMultCount")) {
			drawNumber(baseNum,
				textPositions.at("TotalMultCount").x, textPositions.at("TotalMultCount").y, 0,3, 1);
		}
		else {
			LoopNumber(textPositions.at("TotalMultCount").x, textPositions.at("TotalMultCount").y, setLoopNum, 0,3, 1);
			setLoopNum++;
			if (setLoopNum > 9) {
				setLoopNum = 0;
			}
		}
	}

	if (isBaseDraw_.at("TotalGetStar")) {
		drawNum = multAddCount*keeper_->GetItemCount();
		baseNum = multAddCount*keeper_->GetItemCount();
		if (isDraw_.at("TotalGetStarCount")) {
			int drawPNum = drawNumber(baseNum,
				textPositions.at("TotalGetStarCount").x - (ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX).x * 11), textPositions.at("TotalGetStarCount").y, 0, 11, 1);
			DrawRotaGraph2(textPositions.at("TotalGetStar").x - (ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX).x * drawPNum), textPositions.at("TotalGetStar").y,0,0, 1,0, ResourceLoader::GetInstance().getTextureID(TextureID::ITEM_TEX), TRUE);
		}
		else {
			//LoopNumber(textPositions.at("TotalGetStarCount").x - (ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX).x * 11), textPositions.at("TotalGetStarCount").y, setLoopNum, 0, 11);
			//LoopPointNumber(textPositions.at("TotalGetStar").x, textPositions.at("TotalGetStar").y, FullStarCountVec.at(0),10,)
			for (int i = 0; i < isLastScoreDCou; i++)
			{
				if (numberDrawMaster_[i]) {
					drawTagertNumber(baseNum, i, textPositions.at("TotalGetStarCount").x/* - (ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX).x * 11)*/, textPositions.at("TotalGetStarCount").y, 0);
				}
				else
				{
					DrawGraph(textPositions.at("TotalGetStarCount").x - ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX).x * (i + 1), textPositions.at("TotalGetStarCount").y, ResourceLoader::GetInstance().getTextureID(numberTexes_[0].at(FullStarCountVec[i])), TRUE);
				}
			}//DrawGraph(textPositions.at("TotalGetStar").x-(ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX).x*11), textPositions.at("TotalGetStar").y, ResourceLoader::GetInstance().getTextureID(TextureID::ITEM_TEX), TRUE);

			setLoopNum++;
			if (setLoopNum > 9) {
				setLoopNum = 0;
			}
		}
	}
	if (baseNum < 100000) {
		if (isRankSoundCheckFirst_)PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_RESULT_RANKC), DX_PLAYTYPE_BACK);
		if (isRankDraw_) {
			DrawGraph(RankPosx, RankPosy, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_RANK_C_TEX), TRUE);
		}
		if (isStarDraw_[0]) {
			Vector2 cent = ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX);
			DrawRotaGraph2(StarPosx, StarPosy, cent.x, cent.y, 1, 0, ResourceLoader::GetInstance().getTextureID(TextureID::ITEM_TEX), TRUE);
		}
	}
	else if (baseNum < 200000) {
		if (isRankSoundCheckFirst_)PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_RESULT_RANKB), DX_PLAYTYPE_BACK);
		if (isRankDraw_) {
			DrawGraph(RankPosx, RankPosy, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_RANK_B_TEX), TRUE);
		}
		if (isStarDraw_[0]) {
			Vector2 cent = ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX);
			DrawRotaGraph2(StarPos_Rx, StarPos_Ry, cent.x, cent.y, 0.7, 0, ResourceLoader::GetInstance().getTextureID(TextureID::ITEM_TEX), TRUE);
		}
		if (isStarDraw_[1]) {
			Vector2 cent = ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX);
			DrawRotaGraph2(StarPos2_Rx, StarPos2_Ry, cent.x, cent.y, 0.7, 0, ResourceLoader::GetInstance().getTextureID(TextureID::ITEM_TEX), TRUE);
		}
	}
	else {
		if (isRankSoundCheckFirst_)PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_RESULT_RANKA), DX_PLAYTYPE_BACK);
		if (isRankDraw_) {
			DrawGraph(RankPosx, RankPosy, ResourceLoader::GetInstance().getTextureID(TextureID::TEXT_RANK_A_TEX), TRUE);
		}
		if (isStarDraw_[0]) {
			Vector2 cent = ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX);
			DrawRotaGraph2(StarPos_Lx, StarPos_Ly, cent.x, cent.y, 0.5, 0, ResourceLoader::GetInstance().getTextureID(TextureID::ITEM_TEX), TRUE);
		}
		if (isStarDraw_[1]) {
			Vector2 cent = ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX);
			DrawRotaGraph2(StarPos2_Lx, StarPos2_Ly, cent.x, cent.y, 0.5, 0, ResourceLoader::GetInstance().getTextureID(TextureID::ITEM_TEX), TRUE);
		}
		if (isStarDraw_[2]) {
			Vector2 cent = ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX);
			DrawRotaGraph2(StarPos3_Lx, StarPos3_Ly, cent.x, cent.y, 0.5, 0, ResourceLoader::GetInstance().getTextureID(TextureID::ITEM_TEX), TRUE);
		}

	}
}