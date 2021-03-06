#pragma once

#include "IScene.h"
#include"addScreen\TitleBGScreen.h"

class TitleScene : public IScene
{
public:
	// コンストラクタ
	TitleScene();
	// デストラクタ
	~TitleScene();
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
	void SetNextPanel();
private:
	int selectNum_;
	Vector2 selectPos_;
	float timer;
	float dummy_;
private:
	// ワールド用シェアドポインタ
	using WorldPtr = std::shared_ptr<World>;
	// ワールド
	WorldPtr		world_;
	
	TitleBGScreen bgScreen_;
	// 次のシーン
	Scene			nextScene_;

	std::vector<float> alpha_;
	std::vector<Vector2> posList_;
	int sinCount_;

	bool isPushKey_;
	bool isStartSetPanel_;

	bool isTrigger_;
};