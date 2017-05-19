#pragma once
#include "IScene.h"
#include <map>
#include "../math/Vector2.h"
#include <vector>
#include <array>
#include "../stageGenerator/StageGeneratorManager.h"

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
	//単振り子
	virtual void Pendulum();
	//二重振り子
	virtual void Double(Vector2 fPos);
	//多重振り子
	virtual void Multiple();


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
	float aAlpha;
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
	float friction; //摩擦

	//ここから振り子
	float fx; //支点x座標
	float fy; //支点y座標
	float rot; //角度
	float rot_spd; //角速度
	float length; //紐の長さ
	float g; //重力加速度
	Vector2 spherePos; //重りの位置

	float len; //首の長さ
	float rot2;
	float r; //重りの半径
	float stageLen; //ステージの長さ
	float meterLen; //進行度メーターの長さ
	Vector2 meterPos; //進行度メーターの位置
	StageGenerateManager stageManager;

	std::array<Vector2, 8> fulcrum; //支点
	std::array<Vector2, 8> inPos; //内側
	std::array<Vector2, 8> outPos; //外側
	std::array<float, 8> lineRot; //それぞれの首の角度
	std::array<float, 8> neckLen; //それぞれの首の長さ
	int vec; //現在の首番号
	//const float spdLimit = 2.75f; //
	float spdLimit; //
	bool rotDirection;; //回転方向：trueなら右回り、falseなら左回り
	int r1; //1フレーム前のrot
	int r2; //1フレーム後のrot

	Vector2 v1;
	Vector2 v2;
	float f;

	//2重振り子
	Vector2 doublePos;
	float dRot;
	float dRot_spd;
	float dFriction;
	float dSub;

	float any; //デバッグ表示用
	float any1;
	float any2;
	Vector2 anyPos1;
	array<Vector2, 8> anyPos2;

	//多重振り子
	int fNum;
	std::array<Vector2, 99> fPos;
	std::array<Vector2, 99> multiplePos;
	std::array<float, 99> mRot;
	std::array<float, 99> mRot_spd;
	float mFriction;
	float mSub;
	std::array<float, 99> mLimit;

	

private:
	// ワールド用シェアドポインタ
	using WorldPtr = std::shared_ptr<World>;
	// ワールド
	WorldPtr		world_;

	// 次のシーン
	Scene			nextScene_;
};