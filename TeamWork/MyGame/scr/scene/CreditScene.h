#pragma once
#include "IScene.h"
#include <map>
#include "../camera/Camera.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"

class CreditScene : public IScene
{
public:
	// コンストラクタ
	CreditScene();
	// デストラクタ
	~CreditScene();

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
	//振り子の計算
	Vector2 pendulum(
		Vector2 fulcrum,		//支点の位置
		float length			//紐の長さ
		);

private:
	// ワールド用シェアドポインタ
	using WorldPtr = std::shared_ptr<World>;
	// ワールド
	WorldPtr		world_;

	Vector3 camera_pos_;
	Vector3 target_;

	//支点座標
	Vector2 fulcrum_;
	//重りの座標
	Vector2 weight_pos_;
	//角度
	float rot_;
	//角速度
	float rot_spd_;
	//紐の長さ
	float length_;
	//重力加速度
	float gravity_;

	// 次のシーン
	Scene			nextScene_;
};