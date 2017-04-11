#ifndef STAGE_CLEAR_SCENE_H_
#define STAGE_CLEAR_SCENE_H_

#include "../Base/IScene.h"
#include<map>
#include"../../ResourceLoader/ResourceLoader.h"
#include"StageClearTextAnm.h"
#include "../../Animation/Player/PlayerAnimation2D.h"

class BackGraundManager;

class StageClearScene : public IScene {
public:
	StageClearScene(SceneDataKeeper* keeper);
	~StageClearScene();
	virtual void start() override;
	virtual void update() override;
	virtual void draw() const override;
	virtual void end() override;
	virtual bool isEnd() const override;
	virtual Scene next() const override;
private:
	int id;
	int slideSize;
	int targetPoint;
	int sinCount;

	std::map<int, Scene> nextScene;
	std::map<int, Vector2> textPoses;

	std::map<int, TextureID> textIDs;

	StageClearTextAnm anmer_;

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


	//背景クラス
	BackGraundManager* backManager;

	// キャラアニメーション
	PlayerAnimation2D mButtyAnim;
	PlayerAnimation2D mRettyAnim;
	// 選択時の座標
	Vector2 mCursorPos;
};

#endif