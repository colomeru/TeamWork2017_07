#pragma once

#include "../Base/IScene.h"
#include"../../Math/Vector2.h"
#include"SceneChange.h"
#include<map>
#include"../../ResourceLoader/ResourceLoader.h"
#include "../../Animation/Player/PlayerAnimation2D.h"

class BackGraundManager;

class MainMenuScene : public IScene {
public:
	MainMenuScene(SceneDataKeeper* keeper);
	~MainMenuScene();
	virtual void start() override;
	virtual void update() override;
	void slideText(int targettext);
	void moveText(int targettext);
	virtual void draw() const override;
	virtual void end() override;
	virtual bool isEnd() const override;
	virtual Scene next() const override;
	virtual std::string nextName()  override {
		if (nextScene.at(targetPoint) == Scene::GameEnd)return "GameEnd";
		else {
			return "next";
		}
	}
private:
	bool isTitle_;
	bool isDrawAlphaBack_;
	bool isSelectGamePlay_;
	Vector2 gamePlaySlimePos_;
	//チェンジするカウント
	float alphaSlideCount_;
	//一回しか入らないようにするフラグ
	bool oneFlag_;
	int slideSize;
	
	int id;
	int targetPoint;
	int sinCount;
	int alphaCou[2];
	int alphadefSpeeds[2];

	float titleBackAlpha_;
	//チェンジするバック画面
	int backNum_;
	BackGraundManager* backManager;


	std::map<int,Vector2> textPoses;
	std::map<int,Scene> nextScene;
	std::map<int, Vector2> lastPoses;

	std::map<int, Vector2> shotPos;
	std::map<int, int> defposlist;
	std::map<int, Vector2> setPoses;
	std::map<int, int> boundCou;
	std::map<int, bool> isPoint;
	std::map<int, bool> isShotArrive;
	std::map<int, bool> isArrive;

	std::map<int, int> titleBackStageNum;
	//背景を変更する時間を設定する
	std::map<int, float> titleBackChangeTime;
	//各番号で表示される画像のIDをセットしておく
	std::map<int, TextureID> baseTitleBackID;
	//現在の背景画像のID
	std::vector<TextureID> currentTitleBackID;
	//背景画像を変更するタイミングを決める
	std::map<int, bool> changeBackChecker;
	//背景画像の移動先を決める
	std::vector<int> changeTargetChecker;
	std::map<int, TextureID> textIDs;
	//背景画像たち
	typedef std::vector<TextureID> BackTitles;
	std::vector<BackTitles> titleTexs;

	// キャラアニメーション
	PlayerAnimation2D mButtyAnim;
	PlayerAnimation2D mRettyAnim;
	// 選択時の座標
	Vector2 mCursorPos;
};

