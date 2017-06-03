#include "CharacterAnmManager.h"
#include"../../../../time/Time.h"
#include"../../../../graphic/Sprite.h"
#include"../../../../Def.h"

//フレーム揺れを考慮しない場合の1フレームあたりの時間(アニメーション終了時間の取得用)
static const float defFrameTime = 0.016f;

CharacterAnmManager::CharacterAnmManager():anmChangeFrame_(5), timeCount_(0.f)
{
}

CharacterAnmManager::~CharacterAnmManager()
{
}

void CharacterAnmManager::Add(SPRITE_ID id)
{
	anmID_.push_back(id);
}

void CharacterAnmManager::ResetAnm()
{
	timeCount_ = 0.0f;
}

void CharacterAnmManager::Update()
{
	timeCount_ += Time::DeltaTime;


}

void CharacterAnmManager::Draw(const Vector2& position,const Vector2& origin,const Vector2& size,float alpha) const
{
	int targetFrame = ((int)roundf((timeCount_*60.f)/anmChangeFrame_))% anmID_.size();
	Sprite::GetInstance().Draw(anmID_.at(targetFrame),position,origin,alpha,size);


	if (BuildMode != 1)return;
	
	DrawFormatString(700, 300, GetColor(255, 255, 255), "%d", targetFrame);
}

void CharacterAnmManager::Clear()
{
}

float CharacterAnmManager::GetAnmEndTime() const
{
	return defFrameTime*anmChangeFrame_*anmID_.size();
}
