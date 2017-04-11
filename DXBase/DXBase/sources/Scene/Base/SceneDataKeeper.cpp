#include "SceneDataKeeper.h"

#include <algorithm>

SceneDataKeeper::SceneDataKeeper() :currentSceneName_("stage00"),previousSceneName_("stage04"), hp_(0),itemCount_(0),jumpCount_(0),damageCount_(0),isAddCountBonus_(false),keepItemCount_(0)
{
	maxItemCount_.resize(7);
	datas_.clear();
	datas_["stage00"].comboCount_.resize(6);
	datas_["stage01"].comboCount_.resize(6);
	datas_["stage02"].comboCount_.resize(6);
	datas_["stage03"].comboCount_.resize(6);
	datas_["stage04"].comboCount_.resize(6);
	datas_["bossStage01"].comboCount_.resize(6);
	datas_["clear_stage"].comboCount_.resize(6);

	//初期化処理
	for (auto& i : datas_) {
		i.second.chargeTime_ = 0;
		i.second.comboCount_.clear();
		i.second.comboCount_.resize(6,0);
		i.second.currentCombo_ = 0;
		i.second.damageCount_ = 0;
		i.second.enemyCount_ = 0;
		i.second.holdCount_ = 0;
		i.second.hp_ = 0;
		i.second.itemCount_ = 0;
		i.second.maxCombo_ = 0;
		i.second.maxItemCount_ = 0;
		i.second.minusStar_ = 0;
		i.second.moveDistance_ = 0;
		i.second.plusStar_ = 0;
	}
}

SceneDataKeeper::SceneDataKeeper(std::string name):previousSceneName_(name),hp_(0), itemCount_(0), jumpCount_(0), damageCount_(0){
}

//直前のシーン名を書き換えます name:1つ前のGamePlayScene名(std::string)

void SceneDataKeeper::setSceneName(std::string name){
	if (name == "bossStage01") {
		previousSceneName_ = name;
		return;
	}
	std::string::size_type pos = name.find("stage");
	if (pos == std::string::npos) return;

	previousSceneName_ = name;
	//OutputDebugString(name.c_str());
}

//直前のシーン名から次のシーン名を調べ、引数として受け取ったnameに代入します name:更新前のシーン名(std::string)

int SceneDataKeeper::getNextSceneName(std::string & name){
	for (int i = 1; i < 5; i++) {
		if (previousSceneName_.find(std::to_string(i)) != std::string::npos) {
			int plus = 1;
			if (i == 4) plus = -3;
			name = "stage0" + std::to_string(i + plus);
			return i + plus;
		}
	}
	name = previousSceneName_;
	return 0;
}

//直前のシーン名を受け取ります return:直前のシーン名(std::string)

std::string SceneDataKeeper::getSceneName() const{
	return previousSceneName_;
	//for (int i = 0; i < 3; i++) {
	//	if (previousSceneName_.find("stage01") != std::string::npos) {
	//		previousSceneName_ = "stage02";
	//		}
	//	else if (previousSceneName_.find("stage02") != std::string::npos) {
	//		previousSceneName_ = "stage03";
	//	}
	//	else if (previousSceneName_.find("stage03") != std::string::npos) {
	//		previousSceneName_ = "stage01";
	//	}
}

void SceneDataKeeper::setPlayerHP(int hp){
	hp_ = hp;
}

int SceneDataKeeper::getPlayerHP() const{
	return hp_;
}

int SceneDataKeeper::getInt(){
	for (int i = 1; i < 5; i++) {
		if (previousSceneName_.find(std::to_string(i)) != std::string::npos) {
			int myi;
			int plus = 1;
			if (i == 4) plus = -3;
			myi = i + plus;
			return myi;
		}
	}
}

void SceneDataKeeper::setItemCount(int itemCount) {
	//itemCount_ = itemCount;
	datas_[currentSceneName_].itemCount_ = itemCount;
}

void SceneDataKeeper::addCount(int adds) {
	//itemCount_ += adds;
	keepItemCount_ -= adds;
	int subKeep = datas_[currentSceneName_].itemCount_;
	datas_[currentSceneName_].itemCount_ += adds;
	if (datas_[currentSceneName_].itemCount_ < 0) {
		keepItemCount_ += subKeep-abs(adds);
		datas_[currentSceneName_].itemCount_ = 0;
	}
}

void SceneDataKeeper::ItemReset() {
	//itemCount_ = 0;
	datas_[currentSceneName_].itemCount_ = 0;
}

void SceneDataKeeper::ItemMinus(int minusCount) {
	//itemCount_ -= minusCount;
	datas_[currentSceneName_].itemCount_ -= minusCount;
}

int SceneDataKeeper::GetItemCount() {
	//return itemCount_;
	return datas_[currentSceneName_].itemCount_;
}

//0なら全部の合計

int SceneDataKeeper::GetMaxItemCount(std::string stage) {
	if (stage == "All") {
		int mas = 0;
		for (auto i:datas_) {
			mas += i.second.maxItemCount_;
		}
		return mas;
	}
	//return maxItemCount_[num];
	return datas_[stage].maxItemCount_;
}

void SceneDataKeeper::setMaxItemCount(int max,std::string stage) {
	//maxItemCount_[num] = max;
	datas_[currentSceneName_].maxItemCount_=max;
}

void SceneDataKeeper::addMaxItemCount(int add, std::string stage) {
	//maxItemCount_[num] += add;
	datas_[currentSceneName_].maxItemCount_ += add;
}

void SceneDataKeeper::resetMaxItemCount() {
	for (int i = 0; i < datas_.size(); i++) {
		datas_[currentSceneName_].maxItemCount_ = 0;
	}
}

int SceneDataKeeper::getComboCount(int comboNum) {
	//return jumpCount_;
	if (comboNum < 2)comboNum = 0;
	if (comboNum > 5)comboNum = 5;

	return datas_[currentSceneName_].comboCount_[comboNum];
}

void SceneDataKeeper::setComboCount(int jumpCount, int comboNum){
	datas_[currentSceneName_].comboCount_[comboNum] = jumpCount;
}

void SceneDataKeeper::addComboCount(int jumpCount, int comboNum){
	datas_[currentSceneName_].currentCombo_ += jumpCount;
	
	if (maxComboGet() < datas_[currentSceneName_].currentCombo_) {
		maxComboSet(datas_[currentSceneName_].currentCombo_);
	}
	
	//datas_[currentSceneName_].comboCount_[comboNum] += jumpCount;
}

void SceneDataKeeper::comboReset(int comboNum){
	int combo = std::min<int>(datas_[currentSceneName_].currentCombo_, 5);
	datas_[currentSceneName_].comboCount_[combo] += 1;

	datas_[currentSceneName_].currentCombo_ = 0;
}

void SceneDataKeeper::comboMinus(int minusCount, int comboNum){
	datas_[currentSceneName_].comboCount_[comboNum] -= minusCount;
}


int SceneDataKeeper::getDamageCount() {
	//return damageCount_;
	return datas_[currentSceneName_].damageCount_;
}

void SceneDataKeeper::setDamageCount(int damageCount) {
	//damageCount_ = damageCount;
	datas_[currentSceneName_].damageCount_ = damageCount;
}

void SceneDataKeeper::addDamageCount(int damageCount) {
	//damageCount_ += damageCount;
	datas_[currentSceneName_].damageCount_ += damageCount;
}

void SceneDataKeeper::DamageReset() {
	//damageCount_ = 0;
	datas_[currentSceneName_].damageCount_ = 0;
}

void SceneDataKeeper::DamageMinus(int minusCount) {
	//damageCount_ -= minusCount;
	datas_[currentSceneName_].damageCount_ -= minusCount;
}

void SceneDataKeeper::maxComboSet(int result)
{
	datas_[currentSceneName_].maxCombo_ = result;
}

int SceneDataKeeper::maxComboGet()
{
	return datas_[currentSceneName_].maxCombo_;
}

void SceneDataKeeper::addmaxCombo(int jumpCountResult)
{
	datas_[currentSceneName_].maxCombo_ += jumpCountResult;
}

void SceneDataKeeper::maxComboReset()
{
	datas_[currentSceneName_].maxCombo_ = 0;
}

void SceneDataKeeper::maxComboMinus(int minusCount)
{
	datas_[currentSceneName_].maxCombo_ -= minusCount;
}

int SceneDataKeeper::resultDamageGet() {
	//return damageCountResult_;
	int mas = 0;
	for (auto i:datas_) {
		mas += i.second.damageCount_;
	}
	return mas;
}

