#pragma once
#include <vector>
#include <memory>
#include "../ResourceLoader/ResourceLoader.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Matrix.h"
#include "../World/IWorld.h"
#include "../Actor/Base/Actor.h"

struct VertexPos {
	//左上
	Vector2 LeftTopPos=Vector2::Zero;
	//左下
	Vector2 LeftDownPos = Vector2::Zero;
	//右上
	Vector2 RightTopPos = Vector2::Zero;
	//右下
	Vector2 RightDownPos = Vector2::Zero;
};
struct SpringState {
	float veloNum=0.0f;                                 //補間用バネ
	float resNum=0.0f;                                  //補間用現在
	float num = 0.0f;
	float power = 0.0f;                            //加える力
	//float maxPower = 0.0f;
	float time=0.0f;						       //経過時間
	//float vibrationTime = 0.0f;                    //振動するまでの時間
	//float vibrationTimer = 0.0f;                   //振動するまでのカウント
	Vector2 position=Vector2::Zero;		           //振動する中心座標
	Vector2 velocity = Vector2(1, 1);              //振動している方向と大きさ
	//bool PowerFlag = true;                         //振動してる種類フラグ
	Vector2 springVelocity = Vector2::Zero;
	Vector2 springResPos = Vector2::Zero;
	Vector2 colWallVec = Vector2::Zero;           //壁に当たっている方向
	float colWallPower = 0.0f;                  //壁に当たっている量
};


class PuyoTextureK {
public:
	//揺らすテクスチャIDと座標と大きさと回転
	PuyoTextureK(IWorld* world,TextureID tex,Vector2 pos,Vector2 scale,float rotate);
	~PuyoTextureK();
	//座標とスケールと回転をセットする
	void SetPosition(Vector2 pos, Vector2 scale, float rotate,Vector2 center);
	//力を加える頂点(pos:テクスチャから見た力を加える座標,velo:揺らす方向と大きさ)
	void PuyoAddPower(Vector2 pos,Vector2 velo);
	//力を加え続ける頂点（周りににも）(pos:テクスチャから見た力を加える座標,velo:揺らす方向と大きさ)※毎フレーム呼ぶこと
	void PuyoAddPowerDx(Vector2 pos, Vector2 velo);
	//片岡に聞いてください
	void PuyoAddPowerEx(Vector2 pos,Vector2 velo,float power,float eikyo);
	//全タイマーをゼロにする(ぷよぷよOFF)
	void PuyoTimerZero();
	//主人公のポジションをセットする
	void PuyoPlayerPos(Vector2 pos1, Vector2 pos2, Vector3 color,bool isHold);
	void PuyoUpdate();
	void PuyoDraw();
private:
	void PuyoGraphSplit();
	void PuyoGraphVertex();
	void PuyoVertexSet();
	void PuyoVertexSetInit();
	//これは違う
	void PuyoAddPowerDxSub(int x,int y,Vector2 velo,float power,float eikyo);

private:
	IWorld* mWorld;
	//あたり判定付き頂点情報
	Actor commonVertexActor[30][30];
	//頂点情報
	VertexPos  spriteVertexH[30][30];
	//インデックス情報
	int  spriteIndexsH[30][30];
	//共通の頂点情報
	SpringState commonVertexH[30][30];
	//共通の頂点情報（移動しないよ）
	SpringState commonVertexHNoMove[32][32];
	//共通の頂点情報(あたり判定ないよ)
	SpringState commonVertexHNoCol[32][32];
	//元のテクスチャ情報
	int textureIndex;
	//元のテクスチャサイズ
	Vector2 textureSize;
	//ループ関係
	int loopX;
	int loopY;
	//画像の座標
	Vector2 mPosition;
	//画像のスケール
	Vector2 mScale;
	//画像の回転
	float mRotate;
	//ぶってぃとえってぃの中心
	Vector2 mCenter;
	//ぷってぃとえってぃの座標
	Vector2 mPlayerPos1;
	Vector2 mPlayerPos2;
	float time;
	//カラー
	Vector3 mColor;
	Vector3 mResColor;
	Vector3 mColorVelo;

	std::vector<ActorPtr> puyoCols;
	float mAlpha;
	bool mIsHold;
	


};