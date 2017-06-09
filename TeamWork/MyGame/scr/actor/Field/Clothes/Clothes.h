#pragma once
#include "../../Actor.h"
#include "../MyGame/scr/game/ID.h"
#include <array>
#include <vector>
#include <map>

class Clothes : public Actor
{
protected:
	enum ClothesState
	{
		BEGIN_WIND,				//風の吹き始め
		BEGIN_STRONG_WIND,		//強い風の吹き始め
		STRONG_WIND,			//強い風が吹いている
		ATTENUATE_WIND,			//風の威力の減衰
		POSSIBLE_BITE,			//プレイヤーが噛めるようになる
		END_WIND,				//風が終了
	};

	enum ClothesCuttingState
	{
		Normal,					//通常
		RightUpSlant,			//右斜め上に向かって切れてる
		LeftUpSlant,			//左斜め上に向かって切れてる
	};

public:
	//コンストラクタ
	Clothes(IWorld* world, CLOTHES_ID clothes, int laneNum, float weight);
	//デストラクタ
	virtual ~Clothes();
	//更新
	virtual void Update() = 0;
	//描画
	virtual void Draw() const = 0;

	//IDの取得
	CLOTHES_ID GetClothesID() const {
		return clothes_ID;
	}
	//風を受けているかの取得
	bool GetIsWind() const {
		return isWind_;
	}
	//支点の取得
	Vector2 GetFulcrum() const {
		return fulcrum_;
	}
	//当たり判定のポイントの取得
	std::vector<Vector2> GetCollisionPoints() const {
		return collisionPoints;
	}
	//切断状態の取得
	ClothesCuttingState GetCuttingState() {
		return cuttingState_;
	}
	//画像のコマ番号の取得
	int GetDrawFrame() const {
		return drawFrame_;
	}

	// 当たり判定処理
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	// メッセージ処理
	virtual void OnMessage(EventMessage message, void* param) override;

	//振り子運動(風用)
	void Pendulum(Vector2 fulcrum, float length);
	//風による服揺らし
	void ShakesClothes();
	//強い風によるプレイヤーへの作用
	void WindSwing();
	//服の当たり判定の設定
	void SetPointsUpdate();
	//切断状態による当たり判定のポイントの設定
	void SetLocalPoints();

	//コピー禁止
	Clothes(const Clothes& other) = delete;
	Clothes& operator = (const Clothes& other) = delete;


private:
	//切断状態による当たり判定のポイントの変更
	void SetNormal();
	void SetRightUpSlant();
	void SetLeftUpSlant();
	//服のIDによるスプライトのID設定
	void SetSpriteID();

protected:
	//衝突しているか
	bool isHit_;
	//服ID
	CLOTHES_ID clothes_ID;
	//当たり判定のポイント
	std::vector<Vector2> collisionPoints;
	//当たり判定のポイントのローカル座標
	std::vector<Vector3> localPoints;
	//重さ
	float weight_;
	//画像のコマ番号
	int drawFrame_;
	//糞がついているか
	bool is_Droping_;
	//服の状態
	ClothesState clothesState_;
	//服の切断状態
	ClothesCuttingState cuttingState_;

	//振り子関連(服用)
	//振り子フラグ
	bool isPendulum_;
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
	//紐の長さ
	float length_;
	//重力加速度
	float gravity_;
	//振り子前の固定位置
	Vector2 basePosition_;
	//摩擦
	float friction_;
	//振り子カウント
	int count_;

	float dNumber_;
};