#pragma once
#include "IScene.h"
#include <map>
#include"../actor/CameraAct/TPSCamera.h"

class Player;
class Player2;

class GamePlayScene : public IScene
{	
public:
	// コンストラクタ
	GamePlayScene();
	// デストラクタ
	~GamePlayScene();

	// 初期化
	virtual void Initialize() override;
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
	
private:


private:
	// ワールド用シェアドポインタ
	using WorldPtr = std::shared_ptr<World>;
	using PlayerPtr = std::shared_ptr<Player>;
	using Player2Ptr = std::shared_ptr<Player2>;
	// ワールド
	WorldPtr		world_;

	// 次のシーン
	Scene			nextScene_;

	PlayerPtr ply1;
	Player2Ptr ply2;

	//Vector3 posit;

	//Vector3 camera_pos_;
	//Vector3 target_;

};