#pragma once
#include "../../../actor/Field/Enemys/EnemyCharas/CharacterAnmManager.h"


class MenuCrow {
public:
	enum class State {
		Move,
		Idle,
	};
	//コンストラクタ
	MenuCrow() = default;
	//コンストラクタ
	MenuCrow(Vector2 position, float interval);
	//デストラクタ
	~MenuCrow();
	//更新
	void Update(const int stageNum);
	//描画
	void Draw() const;
	//目的地加算
	void AddDistance(float betDis);
	//
	void Initialize(Vector2 position, float interval);

private:
	void Move();
	void Idle();

private:
	const float MoveTime = 0.5f;	//移動にかかる時間
	const float OffSet = 300.0f;	//画面外

	Vector2 position_;	//座標
	float interval_;	//繰り返し時間
	float timer_;		//タイマ
	Vector2 velocity_;	//移動量
	Vector2 from_;		//目標値
	float dis_;			//目的地

	CharacterAnmManager anmManager_;	//アニメーション
	State state_;
};
