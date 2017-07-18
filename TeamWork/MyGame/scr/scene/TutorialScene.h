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

	//シーン毎の最初のステージ番号に戻る
	void ResetLockNum();
private:
	// ワールド用シェアドポインタ
	using WorldPtr = std::shared_ptr<World>;
	using PlayerPtr = std::shared_ptr<Player>;
	// ワールド
	WorldPtr		world_;
	PlayerPtr		player_;
	//背景描画クラス
	BackgroundScreen bgScreen_; 
	//ステージ作成マネージャー
	StageGenerateManager stageGeneratorManager;
	// 次のシーン
	Scene			nextScene_;
	//チュートリアル用テキスト描画クラス
	TutorialTextScreen textScreen_;
	//現在のチュートリアルのベース番号
	int currentTutorialNum_;
	//チュートリアルの停止番号
	int tutorialLockNum_;
	//シーン毎の共通ファイル名リスト
	std::string StageNameList_[maxTutorialNum];
	//シーン毎のステージファイル切り分け文字列
	std::string TextAddList_[4];

	float timeCount_;
	int sinCount_;

	//既にテキストを全て飛ばし、ボタンを入力したか
	bool isAlreadyPutButton_;
	//コントローラを描画するかどうか
	bool isDrawCtrl_;
	//worldの更新を行うかどうか
	bool isUpdate_;
	//次のシーンへの遷移を開始しているかどうか
	bool isNext_;
	//チュートリアルクリア項目リスト
	std::vector<LockList> lockList_;
	//チュートリアルステージ名リスト
	std::array<std::string, 10> stageTexts_;
	//現在のステージ番号
	int stageTextNum_;
	//ロック設定関数リスト
	std::vector<std::function<void(int)>> setLockFuncList_;
	//チュートリアル時エネミー生成クラス
	std::shared_ptr<TutorialManager> enemGenerator_;

};