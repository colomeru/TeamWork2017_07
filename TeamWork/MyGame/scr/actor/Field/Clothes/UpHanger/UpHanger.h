#pragma once
#include "../Clothes.h"

//ハンガー
class UpHanger : public Clothes, public std::enable_shared_from_this<UpHanger>
{
public:
	//コンストラクタ
	UpHanger(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos);
	//デストラクタ
	virtual ~UpHanger() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//衝突時のアクション
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param) override;

private:
	//ばねの計算
	void Spring(const Vector2& pos, float stiffnes, float friction, float mass);
	//プレイヤーが噛んでいない状態ではリセットする処理
	void Cancel();
	//プレイヤーが上に上がる一連の処理
	void UpPlayer();

private:
	//ハンガーの機能が動いているか
	bool isMove_;
	//引っ張っているかどうか
	bool isPull_;
	//紐の中心の基準点
	Vector2 baseCenter_;
	//プレイヤーの頭の更新座標
	Vector2 pHeadPos_;
	//ハンガーのしなりの位置
	Vector2 bonePos_[2];
	//紐の描画用の位置
	Vector2 codePos_[6];
	//紐の中心点
	Vector2 codeCenterPos_;
};