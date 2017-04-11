#include "SceneChange.h"
#include"../../ResourceLoader/ResourceLoader.h"
#include"../../Game/Time.h"
#include"../../Define.h"

static const float SdefTime = 0.05f;
static const float SfallSpeed = 30;
static const float SSlimePadd = 46;
static const Vector2 SSlimeSize = Vector2(290, 130);
static const float defPosY= -100;
static const int LineSize = 8;
static const int colorMaxIndex = 12;
SceneChange::SceneChange():isMainMenu_(false),isEnd_(true),isSlimeMax_(true),slimeCount(0),timeCount(SdefTime),deltaTime_(0),slimesetline(0), switchCount(0), mt(random_()),
rand256(0, 255), randR(150, 255), randG(150, 200), randB(0, 0),randL(0, LineSize-1),changeCount(0)
{
	spawnPoses[0][0] = (Vector2((SSlimeSize.x - SSlimePadd)*(-0.5f), defPosY));
	spawnPoses[0][1] = (Vector2((SSlimeSize.x - SSlimePadd) * 0.5f, defPosY));
	spawnPoses[0][2] = (Vector2((SSlimeSize.x - SSlimePadd) * 1.5f, defPosY));
	spawnPoses[0][3] = (Vector2((SSlimeSize.x - SSlimePadd) * 2.5f, defPosY));
	spawnPoses[0][4] = (Vector2((SSlimeSize.x - SSlimePadd) * 3.5f, defPosY));
	spawnPoses[0][5] = (Vector2((SSlimeSize.x - SSlimePadd) * 4.5f, defPosY));
	spawnPoses[0][6] = (Vector2((SSlimeSize.x - SSlimePadd) * 5.5f, defPosY));
	spawnPoses[0][7] = (Vector2((SSlimeSize.x - SSlimePadd) * 6.5f, defPosY));
	//spawnPoses[0][8] = (Vector2((SSlimeSize.x - SSlimePadd) * 7.5, 0));

	spawnPoses[1][0] = (Vector2((spawnPoses[0].at(1).x-(SSlimeSize.x/2)) + SSlimePadd, defPosY));
	spawnPoses[1][1] = (Vector2((spawnPoses[0].at(2).x - (SSlimeSize.x / 2)) + SSlimePadd, defPosY));
	spawnPoses[1][2] = (Vector2((spawnPoses[0].at(3).x - (SSlimeSize.x / 2) + SSlimePadd), defPosY));
	spawnPoses[1][3] = (Vector2((spawnPoses[0].at(4).x - (SSlimeSize.x / 2) + SSlimePadd), defPosY));
	spawnPoses[1][4] = (Vector2((spawnPoses[0].at(5).x - (SSlimeSize.x / 2) + SSlimePadd), defPosY));
	spawnPoses[1][5] = (Vector2((spawnPoses[0].at(6).x - (SSlimeSize.x / 2) + SSlimePadd), defPosY));
	spawnPoses[1][6] = (Vector2((spawnPoses[0].at(7).x - (SSlimeSize.x / 2) + SSlimePadd), defPosY));
	//spawnPoses[1][7] = (Vector2((spawnPoses[0].at(8).x - (SSlimeSize.x / 2) + SSlimePadd), 0));
	spawnPoses[1][7] = (Vector2((((SSlimeSize.x - SSlimePadd) *7.5f) - (SSlimeSize.x / 2) + SSlimePadd), defPosY));
	//spawnPoses[1][8] = (Vector2((((SSlimeSize.x - SSlimePadd) * 8.5) - (SSlimeSize.x / 2) + SSlimePadd), 0));

	allSizes = spawnPoses[0].size() + spawnPoses[1].size();

	randMaster.resize(colorMaxIndex+1);
	randMaster[0].push_back(std::uniform_int_distribution<>(100, 120));
	randMaster[0].push_back(std::uniform_int_distribution<>(150, 200));
	randMaster[0].push_back(std::uniform_int_distribution<>(150, 200));
	randMaster[1].push_back(std::uniform_int_distribution<>(100, 180));
	randMaster[1].push_back(std::uniform_int_distribution<>(200, 255));
	randMaster[1].push_back(std::uniform_int_distribution<>(0, 0));
	randMaster[2].push_back(std::uniform_int_distribution<>(200, 255));
	randMaster[2].push_back(std::uniform_int_distribution<>(100, 170));
	randMaster[2].push_back(std::uniform_int_distribution<>(0, 0));
	randMaster[3].push_back(std::uniform_int_distribution<>(0, 0));
	randMaster[3].push_back(std::uniform_int_distribution<>(130, 200));
	randMaster[3].push_back(std::uniform_int_distribution<>(130, 200));
	randMaster[4].push_back(std::uniform_int_distribution<>(255, 255));
	randMaster[4].push_back(std::uniform_int_distribution<>(50, 130));
	randMaster[4].push_back(std::uniform_int_distribution<>(0, 70));
	randMaster[5].push_back(std::uniform_int_distribution<>(0, 150));
	randMaster[5].push_back(std::uniform_int_distribution<>(200, 250));
	randMaster[5].push_back(std::uniform_int_distribution<>(0, 50));
	randMaster[6].push_back(std::uniform_int_distribution<>(230, 255));
	randMaster[6].push_back(std::uniform_int_distribution<>(170, 210));
	randMaster[6].push_back(std::uniform_int_distribution<>(40, 100));
	randMaster[7].push_back(std::uniform_int_distribution<>(230, 255));
	randMaster[7].push_back(std::uniform_int_distribution<>(100, 150));
	randMaster[7].push_back(std::uniform_int_distribution<>(0, 0));
	randMaster[8].push_back(std::uniform_int_distribution<>(50, 100));
	randMaster[8].push_back(std::uniform_int_distribution<>(150, 200));
	randMaster[8].push_back(std::uniform_int_distribution<>(0, 0));
	randMaster[9].push_back(std::uniform_int_distribution<>(200, 255));
	randMaster[9].push_back(std::uniform_int_distribution<>(0, 50));
	randMaster[9].push_back(std::uniform_int_distribution<>(0, 50));
	randMaster[10].push_back(std::uniform_int_distribution<>(150, 200));
	randMaster[10].push_back(std::uniform_int_distribution<>(200, 255));
	randMaster[10].push_back(std::uniform_int_distribution<>(30, 100));
	randMaster[11].push_back(std::uniform_int_distribution<>(150, 220));
	randMaster[11].push_back(std::uniform_int_distribution<>(50, 120));
	randMaster[11].push_back(std::uniform_int_distribution<>(130, 180));
	randMaster[12].push_back(std::uniform_int_distribution<>(212, 212));
	randMaster[12].push_back(std::uniform_int_distribution<>(212, 212));
	randMaster[12].push_back(std::uniform_int_distribution<>(212, 212));

}

void SceneChange::start(std::string next,std::string tonexScene)
{

	changeCount = 0;
	isEnd_ = false;
	isSlimeMax_ = false;

	if (next != "MainMenu"||tonexScene=="GameEnd") {
		isMainMenu_ = false;
		return;
	}
	isMainMenu_ = true;

	std::uniform_int_distribution<>::param_type lange(200,255);



		slimeCount = 0;
		switchCount = 0;
		slimesetline = 0;
		timeCount = SdefTime;
		slimes.clear();
		slimeColors.clear();
		deltaTime_ = Time::GetInstance().deltaTime();

		numberMap.clear();
		for (int i = 0; i < LineSize; i++) {
 			numberMap.push_back(i);
		}
		randL = std::uniform_int_distribution<>(0, numberMap.size() - 1);

		//anmer_ = SceneChangeAnm();

		anmer_.set();
		//anmer_ = StageClearTextAnm();

		int count=0;
		for (auto i : isArrive_) {
			isArrive_[count] = false;
			count++;
		}
		soundVol = 255;
}

void SceneChange::update()
{

	if (isEnd_)return;


	ChangeVolumeSoundMem(soundVol, ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_123));
	ChangeVolumeSoundMem(soundVol, ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_4));
	ChangeVolumeSoundMem(soundVol, ResourceLoader::GetInstance().getSoundID(SoundID::BGM_STAGE_5));
	ChangeVolumeSoundMem(soundVol, ResourceLoader::GetInstance().getSoundID(SoundID::BGM_MENU));
	
		
	if (!isMainMenu_) {
		changeCount++;
		if (!isMainMenu_) {
			changeCount >= fadeTime ? isEnd_ = true : isEnd_ = false;
		}if (!isMainMenu_) {
			changeCount >= fadeTime / 2 ? isSlimeMax_ = true : isSlimeMax_ = false;
		}
		return;
	}
	if (isSlimeMax_)
	{
		fallSlimes();
		return;
	}

	deltaTime_ = Time::GetInstance().deltaTime();
	timeCount -= deltaTime_;

	float setline = 0;

	for (int i = 0; i < slimeCount; i++) {
		slimes[i] += Vector2(0,SfallSpeed);

		if (slimes[i].y >= SCREEN_SIZE.y-SSlimeSize.y - (SSlimeSize.y*setline)) {
			slimes[i].y = (SCREEN_SIZE.y-SSlimeSize.y)-(SSlimeSize.y*setline);
			
			isArrive_[i] = true;
			
			if (i % spawnPoses[slimesetline % 2].size() == spawnPoses[slimesetline % 2].size() -1&&i!=0) {
				setline++;


				if (setline > (int)(SCREEN_SIZE.y/SSlimeSize.y)) {
					isSlimeMax_ = true;
				}
			}
		}
	}

	if (timeCount < 0) {
		if (slimes.size() % spawnPoses[slimesetline % 2].size() == 0) {
			slimesetline++;

		}
		timeCount = SdefTime;

		//if (i % spawnPoses[slimesetline % 2].size() == spawnPoses[slimesetline % 2].size() - 1 && i != 0) {}

		//slimes[slimeCount] = spawnPoses[slimesetline % 2].at(switchCount%spawnPoses[slimesetline % 2].size());

		int setInt = randL(mt);
			

		slimes[slimeCount] = spawnPoses[slimesetline%2].at(numberMap[setInt]);


		if(numberMap.size()>0)numberMap.erase(numberMap.begin()+ setInt);

		//生成されたスライムが使用するカラーパレットの種類を決定する
		{
			std::uniform_int_distribution<> colorP = std::uniform_int_distribution<>(0, colorMaxIndex);
			int colorPalletNum = colorP(mt);
			useRand_.push_back(colorPalletNum);
		}
		slimeColors[slimeCount] = ChangeSColor(randMaster[useRand_[slimeCount]].at(0)(mt), randMaster[useRand_[slimeCount]].at(1)(mt), randMaster[useRand_[slimeCount]].at(2)(mt));
		slimeCount++;
		switchCount++;
		// /8=7
		if (slimes.size() % spawnPoses[slimesetline % 2].size() ==0/* spawnPoses[slimesetline % 2].size()*/ && slimes.size() > 1) {
			for (int i = 0; i < LineSize; i++) {
				numberMap.push_back(i);
			}
		}
		randL = std::uniform_int_distribution<>(0, numberMap.size() - 1);

	}
	anmer_.update_e(0.016f,isArrive_);

}

void SceneChange::fallSlimes()
{
	for (int i = 0; i < static_cast<int>(slimes.size()); i++) {
		slimes[i] += Vector2(0, 80);
	}
	if (slimes.at(slimes.size()-1).y >= SCREEN_SIZE.y) isEnd_ = true;
}
void SceneChange::draw() const

{
	if (isEnd_)return;



	for (int i = 0; i < slimeCount; i++) {
		//SetDrawBright(slimeColors.at(i).Red, slimeColors.at(i).Green, slimeColors.at(i).Blue);

		anmer_.draw_e(i,slimes.at(i),Vector2::Zero,1,Vector3((float)slimeColors.at(i).Red, (float)slimeColors.at(i).Green, (float)slimeColors.at(i).Blue));
		//DrawGraph(static_cast<int>(slimes.at(i).x), static_cast<int>(slimes.at(i).y), ResourceLoader::GetInstance().getTextureID(TextureID::CHANGE_SCENE_TEX), TRUE);
		//SetDrawBright(255, 255, 255);
	}
}
