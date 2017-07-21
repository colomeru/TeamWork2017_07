#pragma once
#include "../../Actor.h"
#include "../../player/Player.h"
#include "../../../game/ID.h"

enum CuttingState
{
	Normal,					//通常
	RightUpSlant,			//右斜め上に向かって切れてる
	LeftUpSlant,			//左斜め上に向かって切れてる
};

class Clothes : public Actor
{
protected:
	enum ClothesState
	{
		WINDLESS,				//風が吹いていない状態
		BEGIN_WIND,				//風の吹き始め
		BEGIN_STRONG_WIND,		//強い風の吹き始め
		STRONG_WIND,			//強い風が吹いている
		ATTENUATE_WIND,			//風の威力の減衰
		POSSIBLE_BITE,			//プレイヤーが噛めるようになる
		END_WIND,				//風が終了
	};
	using CLPoints = std::map<CuttingState, std::vector<Vector3>>;
	//コンストラクタ
	Clothes(IWorld* world, CLOTHES_ID clothes, int laneNum, float weight,
		const CLPoints& localPoints = std::map<CuttingState, std::vector<Vector3>>());

public:
	//デストラクタ
	virtual ~Clothes() override;
	//更新
	virtual void Update() = 0;
	//描画
	virtual void Draw() const = 0;
	// 当たり判定処理
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	// メッセージ処理
	virtual void OnMessage(EventMessage message, void* param) override;
	//服の当たり判定の設定
	virtual void SetPointsUpdate();

	//IDの取得
	CLOTHES_ID GetClothesID() const;
	//風を受けているかの取得
	bool GetIsWind() const;
	//支点の取得
	Vector2 GetFulcrum() const;
	//当たり判定のポイントの取得
	std::vector<Vector2> GetCollisionPoints() const;
	//振り子運動
	void Pendulum(Vector2 fulcrum, float length);
	//風による服揺らし
	void ShakesClothes();
	//強い風によるプレイヤーへの作用
	void WindSwing();
	//服に付着した鳥の糞の更新
	void UpdateClothesFeces();
	//服に付着した鳥の糞の描画
	void DrawClothesFeces() const;
	//補助線描画をするかどうか
	void DrawRangeUpdate();
	//補助線描画
	void DrawRange() const;
	//補助線描画（ハンガー用）
	void DrawHangerRange(Vector2 startPos, Vector2 endPos) const;
	//プレイヤーを服に同期させる振り子
	void Synchronize();
	//服の揺れる確率の設定
	void SetFrequencyWind(int wind);
	//現在のステージ番号の設定
	void SetCurrentStage(Stage currentStage);

	//コピー禁止
	Clothes(const Clothes& other) = delete;
	Clothes& operator = (const Clothes& other) = delete;

protected:
	//服ID
	CLOTHES_ID clothes_ID;
	//当たり判定のポイント
	std::vector<Vector2> collisionPoints;
	//当たり判定のポイントのローカル座標
	CLPoints localPoints_;
	//重さ
	float weight_;
	//画像のコマ番号
	int drawFrame_;
	//糞がついているか
	bool isDroping_;
	//服の状態
	ClothesState clothesState_;
	//服の切断状態
	CuttingState cuttingState_;
	//Delay用の値
	float dNumber_;
	//服に付着した糞
	ActorPtr clothesFeces_;
	//プレイヤー
	Player* player_;
	//振り子の移動量
	Vector2 pendulumVec_;
	//振り子の移動前の位置
	Vector2 beforePos_;
	//服が揺れる確率
	int frequencyWind_;
	//現在のステージ番号
	Stage currentStage_;
	//補助線描画条件
	bool isDrawRange_;

	//振り子関連(服用)
	//紐の長さ
	const float LENGTH = 125.0f;
	//摩擦が増加か減衰か
	bool isFriction_;
	//風を受けているか
	bool isWind_;
	//支点座標
	Vector2 fulcrum_;
	//角度
	float rot_;
	//角速度
	float rot_spd_;
	//摩擦
	float friction_;
	//振り子カウント
	int count_;
};