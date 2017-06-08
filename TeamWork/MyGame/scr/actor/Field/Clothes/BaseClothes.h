#pragma once
#include "Clothes.h"
#include "../MyGame/scr/stageGenerator/StageGeneratorManager.h"

//服の画像のパターン
enum BaseDrawPattern
{
	Orange,
	SkyBlue,
};

class BaseClothes : public Clothes, public std::enable_shared_from_this<BaseClothes>
{
public:
	//コンストラクタ
	BaseClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, bool is_Pin = false);
	//デストラクタ
	~BaseClothes();
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;

private:
	void GraphicPattern();
	void PatternDraw() const;

private:
	StageGenerateManager* stageManager_;
	BaseDrawPattern pattern_;
};