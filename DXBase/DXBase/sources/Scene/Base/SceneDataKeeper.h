#ifndef SCENE_DATA_KEEPER_H_
#define SCENE_DATA_KEEPER_H_
#include<string>
#include<DxLib.h>
#include<vector>
#include<map>

class SceneDataKeeper {
private:
	struct StageDatas {
		std::string previousSceneName_;
		int hp_;
		int itemCount_;
		int maxItemCount_;
		int maxCombo_;
		int currentCombo_;
		std::vector<int> comboCount_;
		int damageCount_;
		int holdCount_;
		int moveDistance_;
		int plusStar_;
		int minusStar_;
		float chargeTime_;
		int enemyCount_;
	};
public:
	SceneDataKeeper();
	SceneDataKeeper(std::string name);
	//直前のシーン名を書き換えます name:1つ前のGamePlayScene名(std::string)
	void setSceneName(std::string name);
	//直前のシーン名から次のシーン名を調べ、引数として受け取ったnameに代入します name:更新前のシーン名(std::string)
	int getNextSceneName(std::string& name);
	//直前のシーン名を受け取ります return:直前のシーン名(std::string)
	std::string getSceneName()const;
	void setCurrentSceneName(std::string name) {
		currentSceneName_ = name;
	}
	void start() {
		comboLimit_ = 0;
		comboCount_ = 0;
		keepItemCount_ = 0;
		isAddCountBonus_ = false;
	}
	void update(float deltaTime) {
		comboLimit_ -= deltaTime;
		if (comboLimit_ <= 0&& isAddCountBonus_) {
			isAddCountBonus_ = false;
			if (comboCount_ >= 30) {
				datas_[currentSceneName_].itemCount_ += ((comboCount_*0.1f) * (2*3));

			}
			else {
				datas_[currentSceneName_].itemCount_ += ((comboCount_*0.1f) * 2);
			}
			comboCount_ = 0;
		}
	}
	void clear() {
		datas_[currentSceneName_].itemCount_ += keepItemCount_;
		datas_[currentSceneName_].itemCount_ = max(datas_[currentSceneName_].itemCount_,0);
		keepItemCount_ = 0;
		if (comboCount_ >= 30) {
			datas_[currentSceneName_].itemCount_ += ((comboCount_*0.1f) * (2 * 3));

		}
		else {
			datas_[currentSceneName_].itemCount_ += ((comboCount_*0.1f) * 2);
		}
		comboCount_ = 0;
	}
	int getKeepCount() {
		return keepItemCount_;
	}
	void setKeepCount(int co) {
		keepItemCount_ = co;
	}
	void addKeepCount(int co) {
		keepItemCount_ += co;
	}
	void minusKeepCount(int co) {
		keepItemCount_ -= co;
	}
	void resetKeepCount() {
		keepItemCount_ = 0;
	}

	void addComboCount() {
		isAddCountBonus_ = true;
		comboCount_++;
		comboLimit_ = 3.f;
	}
	int getComboCount()const {
		return comboCount_;
	}
	float getComboLimit()const {
		return comboLimit_;
	}
	void setPlayerHP(int hp);
	int getPlayerHP() const;
	int getInt();
	void setItemCount(int itemCount);
	void addCount(int adds);
	void ItemReset();
	void ItemMinus(int minusCount);
	int GetItemCount();
	int GetItemCount(std::string stage) {
		if (stage == "All") {
			int num = 0;
			for (auto i:datas_) {
				num += i.second.itemCount_;
			}
			return num;
		}
		return datas_[stage].itemCount_;
	}
	//0なら全部の合計
	//引数に"All"を入れると全ステージの合計が返ってくる
	int GetMaxItemCount(std::string stage);
	int SceneDataKeeper::GetMaxItemCount() {
		return datas_[currentSceneName_].maxItemCount_;
	}
	void setMaxItemCount(int max, std::string stage);
	void addMaxItemCount(int add, std::string stage);
	void resetMaxItemCount();
	//comboNumがコンボ数
	int getComboCount(int comboNum);
	void setComboCount(int jumpCount,int comboNum);
	void addComboCount(int jumpCount,int comboNum);
	void comboReset(int comboNum);
	void comboMinus(int minusCount,int comboNum);
	int getDamageCount();
	void setDamageCount(int damageCount);
	void addDamageCount(int damageCount);
	void DamageReset();
	void DamageMinus(int minusCount);
	void maxComboSet(int result);
	int maxComboGet();
	void addmaxCombo(int jumpCountResult);
	void maxComboReset();
	void maxComboMinus(int minusCount);
	int resultDamageGet();
	
	void setstring(std::string stage) {
		datas_[currentSceneName_].previousSceneName_ = stage;
	}

	int getHoldCount() {
		return datas_[currentSceneName_].holdCount_;
	}
	void setHoldCount(int co) {
		datas_[currentSceneName_].holdCount_ = co;
	}
	void addHoldCount(int co) {
		datas_[currentSceneName_].holdCount_ += co;
	}
	void minusHoldCount(int co) {
		datas_[currentSceneName_].holdCount_ -= co;
	}
	void resetHoldCount() {
		datas_[currentSceneName_].holdCount_ = 0;
	}

	int getMoveDistance() {
		return datas_[currentSceneName_].moveDistance_;
	}
	void setMoveDistance(int co) {
		datas_[currentSceneName_].moveDistance_ = co;
	}
	void addMoveDistance(int co) {
		datas_[currentSceneName_].moveDistance_ += co;
	}
	void minusMoveDistance(int co) {
		datas_[currentSceneName_].moveDistance_ -= co;
	}
	void resetMoveDistance() {
		datas_[currentSceneName_].moveDistance_ = 0;
	}

	int getPlusStar() {
		return datas_[currentSceneName_].plusStar_;
	}
	void setPlusStar(int co) {
		datas_[currentSceneName_].plusStar_ = co;
	}
	void addPlusStar(int co) {
		datas_[currentSceneName_].plusStar_ += co;
	}
	void minusPlusStar(int co) {
		datas_[currentSceneName_].plusStar_ -= co;
	}
	void resetPlusStar() {
		datas_[currentSceneName_].plusStar_ = 0;
	}

	int getMinusStar() {
		datas_[currentSceneName_].minusStar_;
	}
	void setMinusStar(int co) {
		datas_[currentSceneName_].minusStar_ = co;
	}
	void addMinusStar(int co) {
		datas_[currentSceneName_].minusStar_ += co;
	}
	void minusMinusStar(int co) {
		datas_[currentSceneName_].minusStar_ -= co;
	}
	void resetMinusStar() {
		datas_[currentSceneName_].minusStar_ = 0;
	}

	int getChargeTime() {
		return static_cast<int>(datas_[currentSceneName_].chargeTime_);
	}
	void setChargeTime(float co) {
		datas_[currentSceneName_].chargeTime_ = co;
	}
	void addChargeTime(float co) {
		datas_[currentSceneName_].chargeTime_ += co;
	}
	void resetChargeTime() {
		datas_[currentSceneName_].chargeTime_ = 0;
	}
	
	int getEnemyCount() {
		return datas_[currentSceneName_].enemyCount_;
	}
	void setEnemyCount(int co) {
		datas_[currentSceneName_].enemyCount_ = co;
	}
	void addEnemyCount(int co) {
		datas_[currentSceneName_].enemyCount_ += co;
	}
	void minusEnemyCount(int co) {
		datas_[currentSceneName_].enemyCount_ -= co;
	}
	void resetEnemyCount() {
		datas_[currentSceneName_].enemyCount_ = 0;
	}
	void resetDatas(std::string stage="") {
		if (stage == "") {
			for (auto i : datas_) {
				datas_[i.first] = StageDatas();
				datas_[i.first].comboCount_.resize(6,0);
			}
		}
		else {
			datas_[stage] = StageDatas();
			datas_[stage].comboCount_.resize(6,0);
		}
	}
	int GetStageScore() {
		int ans = 0;
		for (auto i : datas_) {
			ans += GetStageScore(i.first);
		}
		return ans;
	}
	int GetStageScore(std::string stage) {
		int baseStar = datas_[stage].itemCount_;

		int multAddCount = 0;
		multAddCount += datas_[stage].maxCombo_;
		multAddCount += (datas_[stage].moveDistance_ / 96);
		multAddCount += datas_[stage].comboCount_[5];
		multAddCount += (float)datas_[stage].plusStar_ / datas_[stage].maxItemCount_ * 10;
		multAddCount += datas_[stage].comboCount_[4];
		multAddCount += datas_[stage].comboCount_[3];
		multAddCount += datas_[stage].holdCount_;
		multAddCount += datas_[stage].comboCount_[2];
		if (datas_[stage].enemyCount_ == 0)multAddCount += 100;
		multAddCount -= datas_[stage].damageCount_;
		multAddCount -= datas_[stage].chargeTime_;
		int ans= baseStar*multAddCount;
		return ans;
	}
private:
	std::map<std::string,StageDatas> datas_;

	float comboLimit_;
	int comboCount_;
	bool isAddCountBonus_;

	std::string previousSceneName_;
	std::string currentSceneName_;
	int hp_;
	int itemCount_;
	std::vector<int> maxItemCount_;
	int jumpCount_;
	int jumpCountResult_;
	int damageCount_;
	int damageCountResult_;

	int keepItemCount_;

};

#endif