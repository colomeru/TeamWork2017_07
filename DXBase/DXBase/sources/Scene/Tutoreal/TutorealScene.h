#pragma once
#include <memory>
#include "../Base/IScene.h"
#include"../../ResourceLoader/ResourceLoader.h"
#include <vector>
#include <string>
#include "../GamePlay/MoveScreen.h"
#include "../GamePlay/PauseScreen.h"
#include "../../Animation/Base/Animation2D.h"
class PlayerConnector;

struct TutorealName {
	std::string csvName;
	MOVIE_ID movieID;
	std::string moviePas;
	std::vector<TextureID> textIDs;
	std::vector<TextureID> timeCountIDs;
};
struct StarState {
	Vector2 position;
	float Scale;
	float alpha;
	float angle;
};

class World;
class TutorealScene : public IScene {
public:
	TutorealScene(SceneDataKeeper* keeper);
	~TutorealScene();
	virtual void start() override;
	virtual void update() override;
	virtual void draw() const override;
	virtual void end() override;
	virtual bool isEnd() const override;
	virtual Scene next() const override;
private:
	bool EndTutoreal(int num);
	//終わるフラグ判定
	bool Tutoreal1();
	bool Tutoreal2();
	bool Tutoreal3();
	bool Tutoreal4();
private:
	//デルタタイム
	float deltaTime_;
	using WorldPtr = std::shared_ptr<World>;
	// ワールド
	WorldPtr	world_;
	//ポーズフラグ
	bool isStopped_;
	//チュートリアルサイズ
	int tutorealSize_;
	//何回繰り返したか
	int tutorealRoopCount_;
	//次のシーン
	Scene nextScene_;
	//動画ID
	MOVIE_ID movieId_;
	//チュートリアルたち
	std::vector<TutorealName> tutorels_;
	//動画を選択したか
	bool isMovie_;
	//動画の位置
	Vector2 movieResPos1_;
	Vector2 movieResPos2_;
	Vector2 moviePos_;
	//サイズ
	float size_;
	//チュートリアルのテキストたち
	std::vector<TextureID> tutorealTexs_;
	//α値
	float alpha_;
	//フェード用α
	float feedAlpha_;
	//動画移動の補間
	float movieMoveTime_;
	//見せかけのチュートリアルテキスト番号
	int tutorealTexCount_;
	//本来のテキスト番号
	int resTutorealTexCount_;
	//カウントや時間
	std::vector<TextureID> tutorealTimes_;
	//チュートリアルの時間とかカウントとか
	int countAndTime_;
	int endTutorealCount_;
	//チュートリアル1,2
	float stickTime_;
	//チュートリアル3
	int endCount_;
	float attackRagCount_;
	bool attackRagFalg;
	//クリアーした後の時間
	float clearTime_;
	//クリアーしたか
	bool isClear_;
	//最後に使うフラグ
	bool tutoreal4Flag;
	//プレイヤーコネクター
	PlayerConnector* playerConnector_;
	//プレイヤーが上に居たら
	float playerUpAlpha_;
	float playerUpCount_;
	//動画のα値
	float mMovieAlpha;
	//チュートリアルクリアーのサイズ
	float mClerScale;
	//チュートリアルクリアーのα
	float mClerAlpha;
	std::vector<StarState> starState_;
	std::vector<Vector2>starPoss_;
	Animation2D starAnim_;
	//プレイヤーが上にいるか
	bool isPlayerUp_;
};