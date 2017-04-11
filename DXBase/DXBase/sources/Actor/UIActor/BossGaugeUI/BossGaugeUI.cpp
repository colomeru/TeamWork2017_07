#include "BossGaugeUI.h"
#include "../../../ResourceLoader/ResourceLoader.h"
#include "../../../Input/InputMgr.h"
#include "../../../Game/Time.h"
#include "../../../Math/Math.h"
BossGaugeUI::BossGaugeUI(IWorld* world, const Vector2& position) :
	ActorUI(world, position),
	velo(0),
	mHp(250),
	mResHp(mHp),
	mGaugeHp(100){
	//ゲージ情報をセット
	GaugeState state;
	state.Color = Vector3(255, 255, 255);
	state.Hp = 100.0f;
	state.GaugeNum = 0;
	gauges.push_back(state);
	state.Color = Vector3(255, 255, 0);
	state.Hp = 100.0f;
	state.GaugeNum = 1;
	gauges.push_back(state);
	state.Color = Vector3(0, 255, 0);
	state.Hp = 100.0f;
	state.GaugeNum = 2;
	gauges.push_back(state);
	state.Color = Vector3(0, 255, 255);
	state.Hp = 100.0f;
	state.GaugeNum = 3;
	gauges.push_back(state);

	//使用するゲージ初期化
	mGaugeNum = (int)((mHp + 1000.0f) / 100.0f) % 10;
	//テクスチャサイズ
	textureSize = ResourceLoader::GetInstance().GetTextureSize(TextureID::BOSS_GAUGE_IN_TEX);
	//割る
	mTexDivision = textureSize.x / 100.0f;
}

BossGaugeUI::~BossGaugeUI()
{
}

void BossGaugeUI::onUpdate(float deltaTime)
{
	//クランプ
	mResHp = MathHelper::Clamp(mResHp, 0.0f, 300.0f);
	//補間
	MathHelper::Spring(mHp, mResHp, velo, 0.3f, 0.5f, 2.0f);
	mGaugeNum = ((int)(mHp + 1000) / 100) % 10;
	mGaugeHp = ((((int)(mHp + 100) / 10) % 10) * 10) +
		((int)mHp % 10);
	//配列外参照防止
	if (mGaugeNum < 3)
	{
		//補間の際ずれる場合があるのでそれ防止
		for (auto& i : gauges)
		{
			if (gauges[mGaugeNum + 1].GaugeNum == i.GaugeNum)
				i.Hp = mGaugeHp;
			else if (i.GaugeNum>gauges[mGaugeNum + 1].GaugeNum)
				i.Hp = 0;
			else
				i.Hp = 100;
		}
	}
}

void BossGaugeUI::onDraw() const
{
	//ゲージ外
	DrawGraph(position_.x, position_.y, ResourceLoader::GetInstance().getTextureID(TextureID::BOSS_GAUGE_OUT_TEX), TRUE);
	//ゲージ中
	for (auto i : gauges){
		SetDrawBright(i.Color.x, i.Color.y, i.Color.z);
		DrawRectGraphF(position_.x, position_.y, 0, 0, i.Hp*mTexDivision, textureSize.y,
		ResourceLoader::GetInstance().getTextureID(TextureID::BOSS_GAUGE_IN_TEX), TRUE, FALSE);
		SetDrawBright(255, 255, 255);
	}
	//DrawFormatString(200, 550, GetColor(255, 255, 255), "Hp:%f",mHp);
}
