#pragma once
#include "../../ResourceLoader/ResourceLoader.h"
#include <list>
#include <vector>
#include "../../Math/Vector2.h"
#include "../../World/IWorld.h"
#include "../../Field/MovelessFloor.h"
#include "../../Scene/StageClear/SceneChangeBossAnm.h"
class TestPlayer;
class Player;

struct IndexPos 
{
	int index;
	Vector2 position;
};
struct KonohaState {
	Vector2 position = Vector2::Zero;
	Vector2 lerpPosition = Vector2::Zero;
	Vector2 velo = Vector2::Zero;
	int index;
	float lerpTimer = 0.0f;
};
struct BackGraundState
{
	//テクスチャid
	int id = 0;
	//Actorの前に表示するか
	bool frontGraundFlag=false;
	//テクスチャサイズ
	Vector2 size = Vector2::Zero;
	//2つ画像を張るための座標
	std::list<IndexPos> indexPos;
	//画像のスケール
	Vector2 scale = Vector2(1, 1);
	//レイヤー
	int layer;
	//stage2か
	bool stage2;

};

class BackGraundManager
{
public:
	BackGraundManager(IWorld* world);
	BackGraundManager();
	~BackGraundManager();
	//背景を設定する（ロードしたテクスチャ限定）
	void SetBackGraund(TextureID id1, TextureID id2, float heightY = 0.0f, bool frontGraund = false, bool stage2 = false,const Vector2& scale=Vector2(1,1));
	//縦の背景を設定する（ロードしたテクスチャ限定）
	void SetTateBackGraund(TextureID id1, TextureID id2, const Vector2& scale=Vector2(1.0f,1.0f));
	//縦の横の背景を設定する(ロードしたテクスチャ限定)
	void SetTateYokoBackGraund(TextureID id1);
	//背景（空）を設定する（ロードしたテクスチャ限定）
	void SetUpBackGraund(TextureID id, int layer,bool flag=false);
	//地面を設定する(ロードしたテクスチャ限定)
	void SetDownBackGraund(TextureID id);

	//背景を全部消す
	void AllDeleteBackGraund();
	//横用スクロールUpdate
	void Update(float deltatime, bool title = false);
	//縦用スクロールUpdate
	void TateUpdate(float deltaTime);

	void Draw(bool title=false) const;

	void BackDraw()const;

	//バネ
	void Spring(Vector2& pos, Vector2& resPos, Vector2& velo, float stiffness = 0.05f, float friction = 0.5f, float mass = 2.0f);
public:
	//この葉の初期設定
	void AddKonoha(const TextureID& id);
	//ボスを出現される場合はtrue
	void BossFlag(bool flag=false);
private:
	void konohaUpdate();
	//ボスのUpdate
	void BossUpdate();
private:
	//ワールド
	IWorld* mWorld;
	//ブロック
	Player* mFloor;
	//地上が表示されているか？
	bool stageFlag;
	//地上の背景
	std::list<BackGraundState> backStates;
	//空の背景
	std::list<BackGraundState> upBackStates;
	//地面の背景
	BackGraundState downBackStates;
	//縦スクロールの横の背景
	BackGraundState tateYokoState;
	//このはのID
	std::vector<TextureID> konohaIds;
	//このはステート
	std::list<KonohaState>konohaStates;
	//このはタイマー
	float konohaTimer;
	//このはランダムタイム
	float konohaRandTime;
	//背景のボスのアニメーション
	SceneChangeBossAnm anim;
	//ボスの子分アニメーション
	SceneChangeBossAnm childanim;
	//ボス出現時間
	float bossTimer;
	//ボス出現周期カウント
	float bossCount;
	//ボスポジション
	Vector2 bossPos;
	//ボスが出現するかどうか
	bool bossFlag;
	//ボスの子分ポジション
	std::vector<Vector2> mBossChilds;
	//子分が出現するかフラグ
	int mChildFlag;
	//子分がいたときボスが振り向く時間
	float bossTurn;
	//子分がいたときにボスが振り向いている時間
	float bossTurnTime;
	float bossTurnCount;
	bool bossTurnFlag;
};