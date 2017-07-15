#pragma once

#include "IScene.h"
#include <map>
#include "../camera/Camera.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../stageGenerator/StageGeneratorManager.h"
#include "../actor/player/Player.h"
#include "GamePlayDefine.h"
#include "addScreen\BackgroundScreen.h"
#include "addScreen/TutorialTextScreen.h"
#include"../actor/Effects/PlayerEffect/ArrowEffectGenerator.h"

static const int maxTutorialNum = 5;

class TutorialManager;

class TutorialScene : public IScene
{
public:
	enum UnLockType {
		ChangeLane,
		ChangeLaneUp,
		ChangeLaneFall,
		BiteClothes,
		PlayPendulum,
		KillTapper,
		UseSword,
		StartWind,
		ChangeHead,
		ChangeHeadKey,
		ClearStage,
		PlayerShoot,
		PlayerBackShoot,
		EndStage,
		Dummy,
		Stick,
		HalfFullStick,
		FullStick,
	};

	struct LockList
	{
		UnLockType type;
		bool isLock;
		SPRITE_ID ctrl;

		LockList(UnLockType type,bool isLock,SPRITE_ID ctrl=SPRITE_ID::GAMEPAD_SPRITE) :type(type), isLock(isLock), ctrl(ctrl) {

		}
	};

public:
	// コンストラクタ
	TutorialScene();
	// デストラクタ
	~TutorialScene();

	// 初期化
	virtual void Initialize() override;
	virtual void SceneInit();
	// 更新
	virtual void Update() override;
	// 描画
	virtual void Draw() const override;
	// 終了しているか？
	virtual bool IsEnd() const override;
	// 次のシーンを返す
	virtual Scene Next() const override;
	// 終了時処理
	virtual void End() override;
	// メッセージ処理
	void handleMessage(EventMessage message, void* param);

	void addCurrentNum() {currentTutorialNum_++;}
private:
	//プレイヤーをロックする
	void SceneLock();
	//チュートリアルクリア条件から、typeと一致する条件を解除する
	void UnLock(UnLockType type);

	//シーンのロック条件を満たしているかを調べる
	bool IsCanSceneLock()const;

	//画面ロックのルールを追加する
	void SetLockList(int currentTutorial, int tutorialLockNum);

	//各チュートリアルステージ用のセット関数、テキストが変わるたびに呼ぶこと
	void SetLock1(int tutorialLockNum);
	void SetLock2(int tutorialLockNum);
	void SetLock3(int tutorialLockNum);
	void SetLock4(int tutorialLockNum);
	void SetLock5(int tutorialLockNum);

	void ReLockUpLane();
	void ReLockNeckShoot();
	void ReLockPendulum();
	void ReLockBite();
	void ReLockChangeHead();
	void ReLockUseSword();
private:
	void ChangeNextTutorial();

	void ResetLockNum();
private:
	// ワールド用シェアドポインタ
	using WorldPtr = std::shared_ptr<World>;
	using PlayerPtr = std::shared_ptr<Player>;
	// ワールド
	WorldPtr		world_;
	PlayerPtr		player_;

	BackgroundScreen bgScreen_;

	//ステージ作成マネージャー
	StageGenerateManager stageGeneratorManager;

	// 次のシーン
	Scene			nextScene_;

	TutorialTextScreen textScreen_;

	//現在のチュートリアルのベース番号
	int currentTutorialNum_;
	//チュートリアルの停止番号
	int tutorialLockNum_;

	std::string StageNameList_[maxTutorialNum];
	
	std::string TextAddList_[4];

	float dummy_;

	float timeCount_;

	int sinCount_;

	bool isAlreadyPutButton_;

	//コントローラを描画するかどうか
	bool isDrawCtrl_;
	//worldの更新を行うかどうか
	bool isUpdate_;
	bool isNext_;

	std::vector<LockList> lockList_;
	std::vector<SPRITE_ID> KeySpriteList_;
	std::vector<SPRITE_ID> LastKeySpriteList_;

	std::array<std::string, 10> stageTexts_;
	int stageTextNum_;

	std::vector<std::function<void(int)>> setLockFuncList_;

	std::shared_ptr<TutorialManager> enemGenerator_;

	ArrowEffectGenerator arrowEffectGenerator_;
};