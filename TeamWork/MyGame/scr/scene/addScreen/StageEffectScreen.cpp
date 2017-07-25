#include "StageEffectScreen.h"
#include"../../math/MathHelper.h"
#include"../../Def.h"
#include<DxLib.h>


StageEffectScreen::StageEffectScreen():
	EffectColor_{
		{ Stage::Stage1 ,RGBAColorF(0.0f,0.0f,0.0f,0.0f) },
		{ Stage::Stage2 ,RGBAColorF(150.0f, 220.0f, 170.0f) },
		{ Stage::Stage3 ,RGBAColorF(255.0f, 80.0f, 80.0f) },
		{ Stage::Stage4 ,RGBAColorF(0.0f,0.0f,0.0f,0.0f) },
		{ Stage::Stage5 ,RGBAColorF(0.0f,0.0f,0.0f,0.0f) },
		{ Stage::Stage6 ,RGBAColorF(0.0f,0.0f,0.0f,0.0f) },
		{ Stage::Stage7 ,RGBAColorF(150.0f, 220.0f, 170.0f) },
		{ Stage::Stage8 ,RGBAColorF(80.0f, 80.0f,200.0f) }}{

}

void StageEffectScreen::Init(Stage stage)
{
	stage_ = stage;
	sinCount_ = 0;
}

void StageEffectScreen::Update()
{
	if (!isPlayEffect_)return;

	sinCount_ = (sinCount_ + 1);// % 360;
	
	if (sinCount_ >= 360)isPlayEffect_ = false;
}

void StageEffectScreen::Draw() const
{
	if (!isPlayEffect_)return;

	int effectAlpha = (int)(MathHelper::Sin((float)sinCount_)*EffectColor_.at(stage_).GetAlpha255I());
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, effectAlpha);

	DrawBox(
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 
		GetColor(
			EffectColor_.at(stage_).GetRedColor255I(),
			EffectColor_.at(stage_).GetGreenColor255I(),
			EffectColor_.at(stage_).GetBlueColor255I()),
		TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
}

void StageEffectScreen::SetIsPlayEffect(bool isplay)
{
	isPlayEffect_ = isplay;
}

void StageEffectScreen::StartEffect()
{
	isPlayEffect_ = true;
	sinCount_ = 0;
}

void StageEffectScreen::drawUpdate()
{
}
