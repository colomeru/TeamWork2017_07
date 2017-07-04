#pragma once
#include "../Clothes.h"

class NotSlashClothes : public Clothes, public std::enable_shared_from_this<NotSlashClothes>
{
public:
	//コンストラクタ
	NotSlashClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, SPRITE_ID spriteId, bool is_Pin = false);
	//デストラクタ
	virtual ~NotSlashClothes() override;
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//切断状態による当たり判定のポイントの設定
	virtual void SetLocalPoints() override;
private:
	void SetPoints01();
	void SetPoints02();
};