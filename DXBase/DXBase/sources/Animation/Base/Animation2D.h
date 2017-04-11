#pragma once

#include <DxLib.h>

#include <unordered_map>
#include <vector>
#include "../../Math/Math.h"
#include "../../Actor/Person/Player/State/Base/ActionType.h"

class Animation2D {
public:
	// コンストラクタ
	Animation2D(const ActionType& type = ActionType::Right);
	// コンストラクタ
	virtual ~Animation2D();
	// 更新処理
	virtual void update(float deltaTime);
	// アニメーションの終了フラグ
	bool end_anim();
	// 描画
	// 座標,(軸),(拡大率),(回転角度(度数法)),
	void draw(const Vector2& position, const Vector2& origin = Vector2::Zero, const float& scale = 1, const float& degree = 0, const Vector3& color = Vector3(255, 255, 255)) const;
	// 座標,軸,拡大率,回転角度(度数法),
	void draw(const Vector2& position, const Vector2& origin = Vector2::Zero, const Vector2& scale = Vector2::One, const float& degree = 0, const Vector3& color = Vector3(255, 255, 255)) const;
	//反転付き
	void drawTurn(const Vector2& position, const Vector2& origin = Vector2::Zero, const float& scale = 1, const float& degree = 0, const Vector3& color = Vector3(255, 255, 255),bool reversal=false) const;

public:
	void add_anim(const int & id, const std::vector<int>& anims);
	// アニメーションの変更(速度のみ変更も可能)
	void change_param(const int& anim_num, const float& speed = 1.0f);
	// アニメーションの方向転換
	void change_dir_type(const int& anim_num, const ActionType& type);
	// アニメーションを元に戻す
	void back_to_pre_motion();
protected:
	//分割前の画像id
	int id_;
	//現在のアニメーション
	int curr_anim_;
	int norm_anim_;
	int turn_anim_;
	float curr_speed_;
	float norm_speed_;
	float turn_speed_;

	unsigned int frame_;
	//アニメーションの再生時間
	float timer_;
	
	ActionType type_stock_;
	ActionType type_;
	//分割後のid格納用多次元配列
	std::unordered_map<int, std::vector<int>> sprites_;
};