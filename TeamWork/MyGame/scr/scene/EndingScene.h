#pragma once
#include "IScene.h"
#include <map>
#include "../math/Vector2.h"
#include <vector>

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
	float friction;
	float x1;
	float x2;

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
	Vector2 line1_Pos_In;
	Vector2 line1_Pos_Out;
	Vector2 line2_Pos_In;
	Vector2 line2_Pos_Out;
	Vector2 line3_Pos_In;
	Vector2 line3_Pos_Out;
	Vector2 line4_Pos_In;
	Vector2 line4_Pos_Out;
	Vector2 line5_Pos_In;
	Vector2 line5_Pos_Out;
	Vector2 line6_Pos_In;
	Vector2 line6_Pos_Out;
	Vector2 line7_Pos_In;
	Vector2 line7_Pos_Out;
	Vector2 line8_Pos_In;
	Vector2 line8_Pos_Out;

	float len;
	float line1_Rot;
	float line2_Rot;
	float line3_Rot;
	float line4_Rot;
	float line5_Rot;
	float line6_Rot;
	float line7_Rot;
	float line8_Rot;
	float rot2;
	
	vector<Vector2> fulcrum;
	int vec;

private:
	// ワールド用シェアドポインタ
	using WorldPtr = std::shared_ptr<World>;
	// ワールド
	WorldPtr		world_;

	// 次のシーン
	Scene			nextScene_;
};