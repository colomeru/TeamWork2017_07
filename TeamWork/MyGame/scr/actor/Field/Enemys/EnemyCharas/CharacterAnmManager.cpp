#include "CharacterAnmManager.h"
#include"../../../../time/Time.h"
#include"../../../../graphic/Sprite.h"
#include"../../../../Def.h"
#include"../../../../math/MathHelper.h"

//フレーム揺れを考慮しない場合の1フレームあたりの時間(アニメーション終了時間の取得用)
static const float defFrameTime = 0.016f;

CharacterAnmManager::CharacterAnmManager():anmChangeFrame_(5), timeCount_(0.f), anmEnd_(false), reverse_(1.0f), isRepeat_(false)
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
	reverse_ = 1.0f;
	anmEnd_ = false;
}

void CharacterAnmManager::ReverseAnm()
{
	reverse_ = -reverse_;
}

void CharacterAnmManager::Update()
{
	if (isRepeat_) {
		timeCount_ += Time::DeltaTime;
		return;
	}
	timeCount_ += reverse_*Time::DeltaTime;
	int timeInt = (int)roundf((timeCount_*60.f) / anmChangeFrame_);
	if (timeInt >= anmID_.size()-1||timeCount_<0.001f) {
		anmEnd_ = true;
	}
	timeCount_ =  MathHelper::Clamp(timeCount_,0.0f, anmID_.size() - 1);

}

void CharacterAnmManager::Draw(const Vector2& position,const Vector2& origin,const Vector2& size,float alpha) const
{
	if (isRepeat_) {
		int targetFrame = ((int)roundf((timeCount_*60.f) / anmChangeFrame_)) % anmID_.size();
		Sprite::GetInstance().Draw(anmID_.at(targetFrame), position, origin, alpha, size);
		return;
	}
	int targetFrame = ((int)roundf((timeCount_*60.f) / anmChangeFrame_));// % anmID_.size();
	targetFrame = MathHelper::Clamp(targetFrame, 0, anmID_.size() - 1);
	//int targetFrame = ((int)roundf((timeCount_*60.f) / anmChangeFrame_));
	Sprite::GetInstance().Draw(anmID_.at(targetFrame),position,origin,alpha,size);


	if (BuildMode != 1)return;
	int timeInt = (int)roundf((timeCount_*60.f) / anmChangeFrame_);

	DrawFormatString(700, 300, GetColor(255, 255, 255), "%d", timeInt);
}

void CharacterAnmManager::Clear()
{
}

float CharacterAnmManager::GetAnmEndTime() const
{
	return defFrameTime*anmChangeFrame_*anmID_.size();
}

bool CharacterAnmManager::IsEndAnimation() const
{
	return anmEnd_;
}

void CharacterAnmManager::SetIsRepeat(bool isrp)
{
	isRepeat_ = isrp;
}

void CharacterAnmManager::SetTimeCount(float time)
{
	timeCount_ = time;

	timeCount_ = MathHelper::Clamp(timeCount_, 0.0f, anmID_.size() - 1.f);
}

void CharacterAnmManager::SetAnmSpeed(int anmChangeFrame)
{
	anmChangeFrame_ = anmChangeFrame;
}

