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
	float friction; //摩擦
	float x1; //支点のｘ座標
	float x2; //支点のy座標

	//ここから振り子
	float fx; //支点x座標
	float fy; //支点y座標
	float rot; //角度
	float rot_spd; //角速度
	float length; //紐の長さ
	float g; //重力加速度
	Vector2 spherePos; //重りの位置
	float rad;
	Vector2 line1_Pos_In;  //緑:内側
	Vector2 line1_Pos_Out; //緑:外側
	Vector2 line2_Pos_In;  //赤:内側
	Vector2 line2_Pos_Out; //赤:外側
	Vector2 line3_Pos_In;  //青:内側
	Vector2 line3_Pos_Out; //青:外側
	Vector2 line4_Pos_In;  //黄:内側
	Vector2 line4_Pos_Out; //黄:外側
	Vector2 line5_Pos_In;  //紫:内側
	Vector2 line5_Pos_Out; //紫:外側
	Vector2 line6_Pos_In;  //水:内側
	Vector2 line6_Pos_Out; //水:外側
	Vector2 line7_Pos_In;  //橙:内側
	Vector2 line7_Pos_Out; //橙:外側
	Vector2 line8_Pos_In;  //白:内側
	Vector2 line8_Pos_Out; //白:外側

	float len; //首の長さ
	float line1_Rot; //緑
	float line2_Rot; //赤
	float line3_Rot; //青
	float line4_Rot; //黄色
	float line5_Rot; //紫
	float line6_Rot; //水色
	float line7_Rot; //オレンジ
	float line8_Rot; //白
	float rot2;
	float r; //重りの半径
	float stageLen; //ステージの長さ
	float meterLen; //進行度メーターの長さ
	Vector2 meterPos; //進行度メーターの位置
	
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