#pragma once

#include "DxLib.h"
#include"../Base/Scene.h"
#include<vector>
#include<map>
#include"../StageClear/StageClearTextAnm.h"
#include"../../ResourceLoader/ResourceLoader.h"
#include"../Base/SceneDataKeeper.h"

class ClearScreen {
public:
	ClearScreen() {}
	ClearScreen(SceneDataKeeper* keeper);
	~ClearScreen() {
		ChangeVolumeSoundMem(255, ResourceLoader::GetInstance().getSoundID(SoundID::SE_RESULT_SHOW));
		StopSoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_RESULT_ROLL));
	}
	void start(const std::string& name);
	bool update(std::string name, Scene& next);
	void draw() const;
	//bool isCounT() const{
	//	return Count_ <= 0;
	//};	
	//受け取った値の桁数を返す baseNum:桁数を求めたい値
	int countDigits(int baseNum) const{
		if (baseNum == 0)return 1;

		int mathNum = abs(baseNum);
		int ans = (int)log10(mathNum) + 1;
		return ans;
	}

	//受け取った値のうち、指定された桁番号の値のみを描画する(posx,yは、それぞれ値が全桁描画される場合の左端とする)
	int drawTagertNumber(int targetNum, int targetDigit, int posx, int posy, int NumTexType = 1) const{
		int baseNum = targetNum;
		int drawNum = targetNum;

		int posCount = 0;
		std::vector<int> drawNumberList;

		//drawNumberListは、[0]が1の位
		for (;;) {

			//計算値が10を下回った時点で、最後の値(最も高い位)をdrawNumberListに代入する
			if (baseNum < 10) {
				drawNumberList.push_back(baseNum);
				break;
			}
			//1の位から順にdrawNumberListに代入していく
			drawNum = (int)(baseNum*0.1);
			drawNum = drawNum * 10;
			int textNum = baseNum - drawNum;
			drawNumberList.push_back(textNum);
			baseNum = (int)(baseNum*0.1);
			posCount++;
		}

		//drawNumberListのsizeは桁数、左端の描画位置は座標通りのため、sizeから-1する事で、posx,yを基準とした右端までの桁数を算出する
		int drawPosCount = drawNumberList.size() - 1;

		//右端の描画位置を計算する(posxを、drawPosCount分右にずらす)
		//int drawx = posx + ResourceLoader::GetInstance().GetTextureSize(numberTexes_[NumTexType][drawNumberList[0]]).x*(drawPosCount);
		int drawx = posx - ResourceLoader::GetInstance().GetTextureSize(numberTexes_[NumTexType][drawNumberList[0]]).x;

		for (int i = 0; i < drawNumberList.size(); i++) {
			//drawxから、描画される対象の桁分左へズラす(drawNumberListは0が右端の値なため)
			if (i == targetDigit)DrawGraph(drawx - ResourceLoader::GetInstance().GetTextureSize(numberTexes_[NumTexType][drawNumberList[i]]).x*i, posy, ResourceLoader::GetInstance().getTextureID(numberTexes_[NumTexType][drawNumberList[i]]), TRUE);	
		}
		//桁数を返す
		int ret = drawNumberList.size();
		return ret;

	}
	//1で白、0で赤
	int drawNumber(int targetNum,int posx,int posy,int NumTexType=1,int maxSize=3,float drawSize=1.f) const{
		int drawNum = targetNum;
		int baseNum = targetNum;
		int posCount = 0;
		std::vector<int> drawNumberList;

		for (int i = 0;i<maxSize;i++) {

			if (baseNum < 10) {
				drawNumberList.push_back(baseNum);
				break;
			}

			drawNum = (int)(baseNum*0.1);
			drawNum = drawNum * 10;
			int textNum = baseNum - drawNum;
			drawNumberList.push_back(textNum);
			baseNum = (int)(baseNum*0.1);
			posCount++;
		}

		int drawx = posx +ResourceLoader::GetInstance().GetTextureSize(numberTexes_[NumTexType][drawNumberList[0]]).x*(maxSize-1);

		for (int i = 0; i < maxSize; i++) {
			//drawNumberListの桁数がmaxSizeを下回っていた場合、桁数以上のloopをカットする
			if (i>=drawNumberList.size()) {
				continue;
			}
			DrawRotaGraph2(drawx -(ResourceLoader::GetInstance().GetTextureSize(numberTexes_[NumTexType][drawNumberList[i]]).x*i*drawSize),posy,0,0,drawSize,0, ResourceLoader::GetInstance().getTextureID(numberTexes_[NumTexType][drawNumberList[i]]), TRUE);
		}
		int ret = drawNumberList.size();
		return ret;
	}
	//指定posに、maxSize桁数分のスロットを並べて描画する
	void LoopNumber(int posx,int posy,int useLoopType, int NumTexType = 1,int maxSize=3,float drawSize=1.f) const{
		int LoopCount = useLoopType;
		int drawx = posx + ResourceLoader::GetInstance().GetTextureSize(numberTexes_[NumTexType][0]).x*(maxSize - 1);
		
		for (int i = 0; i < maxSize; i++) {
			DrawRotaGraph2(drawx - (ResourceLoader::GetInstance().GetTextureSize(numberTexes_[NumTexType][NumberForLoop_[LoopCount]]).x*i*drawSize), posy,0,0,drawSize,0, ResourceLoader::GetInstance().getTextureID(numberTexes_[NumTexType][NumberForLoop_[LoopCount]]), TRUE);
			LoopCount++;
			if (LoopCount > 9) {
				LoopCount=0;
			}
		}
	}
	//指定posから、numPos桁番目の位置にスロットを描画する
	void LoopPointNumber(int posx, int posy, int& useLoopType, int numPos, int NumTexType = 1,float drawSize=1.f) const {
		DrawRotaGraph2(posx + (ResourceLoader::GetInstance().GetTextureSize(numberTexes_[NumTexType][NumberForLoop_[useLoopType]]).x*numPos*drawSize), posy,0,0,drawSize,0, ResourceLoader::GetInstance().getTextureID(numberTexes_[NumTexType][NumberForLoop_[useLoopType]]), TRUE);
		useLoopType++;
		if (useLoopType > 9) {
			useLoopType = 0;
		}
	}
private:
	std::vector<std::string> changeTextList;
	std::vector<std::vector<std::string>> listBase;
	std::vector<Vector2> textPosList;
	std::map<int, TextureID> textIDs;

	std::map<std::string, Vector2> textPositions;

	std::vector<std::vector<TextureID>> numberTexes_;
	
	std::map<std::string,TextureID> stageTexes_;
	std::string stageName_;
	StageClearTextAnm anmer_;
	SceneDataKeeper* keeper_;
	//int Count_;
	int ints;
	int sinCount;
	bool isCircleDraw_;
	float drawNumberController_;

	std::vector<int> NumberForLoop_;

	std::map<std::string, bool> isDraw_;
	std::map<std::string, bool> isBaseDraw_;
	std::vector<std::string> nameList;
	std::vector<std::string> nameBaseList;
	int usingIntCount;
	int usingBaseIntCount;

	bool isFirst_;
	bool isSoundFirst_;
	bool buttonLock_;
	int buttonLockCount_;

	bool isMaxDraw_;
	bool isRankDraw_;
	std::vector<bool> isStarDraw_;
	int baseLastDraw_;
	int drawLastDraw_;

	bool isRankSoundCheckFirst_;
	int rankSoundCheckCount_;

	std::vector<int> FullStarCountVec;

	float lastScoreTime;
	int isLastScoreDCou;
	float lastScoreAnsTime;
	int isLastScoreAnsDCou;

	std::vector<bool> numberDrawMaster_;

	float useCount;
	bool isMyssFirst_;

	bool isMusLastFirst_;

	bool isEnterDraw_;
	float isEnterDrawNum_;

	int myd;

	bool isAlreadyStart_;

};