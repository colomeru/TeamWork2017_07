#pragma once
#include "IScene.h"
#include <map>
#include "../math/Vector2.h"

using namespace std;

class EndingScene : public IScene
{
public:
	// コンストラクタ
	EndingScene();
	// デストラクタ
	~EndingScene();

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
	float size;
	float vertical;
	float horizontal;
	float alpha;
	float distanceH;
	float distanceV;
	float absH;
	float absV;
	float distanceC;
	float cAlpha;
	Vector2 absC;
	Vector2 arrowSize;
	Vector2 particleSize;
	Vector2 arrowPos;
	Vector2 particlePos;
	Vector2 centerPos;
	Vector2 circlePos;
	Vector2 circleSize;
	Vector2 cCenterPos;
	bool turn;
	vector<int> v;

	//ここから振り子
	float fx; //支点x座標
	float fy; //支点y座標
	float rot; //角度
	float rot_spd; //角速度
	float length; //紐の長さ
	float g; //重力加速度
	Vector2 spherePos;
	float rad;
	float angle;
	Vector2 linePos_1;
	Vector2 linePos_2;
	float len;
	float lineRot;
	float rot2;


private:
	// ワールド用シェアドポインタ
	using WorldPtr = std::shared_ptr<World>;
	// ワールド
	WorldPtr		world_;

	// 次のシーン
	Scene			nextScene_;
};