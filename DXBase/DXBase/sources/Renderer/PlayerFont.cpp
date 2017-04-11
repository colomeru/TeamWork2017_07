#include "PlayerFont.h"
#include "../Game/Time.h"
#include "../ResourceLoader/ResourceLoader.h"

PlayerFont::PlayerFont(IWorld* world, const Vector2 & pos) :
	world_(world),
	isPlayFont_(false),
	alphaR_(0.0f),
	alphaB_(0.0f)
{
	playerB = world_->findActor("PlayerBody1").get();
	playerR = world_->findActor("PlayerBody2").get();
	bossC = world_->findActor("Boss").get();
	textSize_ = ResourceLoader::GetInstance().GetTextureSize(TextureID::HUKIDASI_TEX);
}

PlayerFont::~PlayerFont()
{
}

void PlayerFont::Update()
{
	playerPosB = (playerB->getPosition() - Vector2(0, 516))*world_->GetInv();
	playerPosR = (playerR->getPosition() - Vector2(0, 516))*world_->GetInv();
	if (bossC != nullptr)
		bossPosC = (bossC->getPosition() - Vector2(0, 516))*world_->GetInv();
	if (!isPlayFont_) {
		alphaB_ -= 3.0f*Time::GetInstance().deltaTime();
		alphaR_ -= 3.0f*Time::GetInstance().deltaTime();
		alphaC_ -= 3.0f*Time::GetInstance().deltaTime();
	}
	alphaB_ = MathHelper::Clamp(alphaB_, 0.0f, 1.0f);
	alphaR_ = MathHelper::Clamp(alphaR_, 0.0f, 1.0f);
	alphaC_ = MathHelper::Clamp(alphaC_, 0.0f, 1.0f);
	if (!isPlayFont_)return;
	eventFonts_[nowEvent_]->Update();
	if (eventFonts_[nowEvent_]->GetSpeekPlayer() == PlayerEnum::BUTTY) {
		alphaB_ += 3.0f*Time::GetInstance().deltaTime();
		alphaR_ -= 3.0f*Time::GetInstance().deltaTime();
		alphaC_ -= 3.0f*Time::GetInstance().deltaTime();
	}
	else if (eventFonts_[nowEvent_]->GetSpeekPlayer() == PlayerEnum::RETTY) {
		alphaB_ -= 3.0f*Time::GetInstance().deltaTime();
		alphaR_ += 3.0f*Time::GetInstance().deltaTime();
		alphaC_ -= 3.0f*Time::GetInstance().deltaTime();
	}
	else {
		alphaB_ -= 3.0f*Time::GetInstance().deltaTime();
		alphaR_ -= 3.0f*Time::GetInstance().deltaTime();
		alphaC_ += 3.0f*Time::GetInstance().deltaTime();
	}
	alphaB_ = MathHelper::Clamp(alphaB_, 0.0f, 1.0f);
	alphaR_ = MathHelper::Clamp(alphaR_, 0.0f, 1.0f);
	alphaC_ = MathHelper::Clamp(alphaC_, 0.0f, 1.0f);
	if (eventFonts_[nowEvent_]->isEndEvent()) {
		isPlayFont_ = false;
	}

}

void PlayerFont::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, MathHelper::Lerp(0.0f, 255.0f, alphaB_));
	DrawRotaGraph(playerPosB.x, playerPosB.y, MathHelper::Lerp(0.0f, 2.0f, alphaB_), 0.0f, ResourceLoader::GetInstance().getTextureID(TextureID::HUKIDASI_TEX), true);
	eventFonts_[nowEvent_]->Draw(playerPosB - textSize_ + Vector2(96, 96), 64);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0.0f);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, MathHelper::Lerp(0.0f, 255.0f, alphaR_));
	DrawRotaGraph(playerPosR.x, playerPosR.y, MathHelper::Lerp(0.0f, 2.0f, alphaR_), 0.0f, ResourceLoader::GetInstance().getTextureID(TextureID::HUKIDASI_TEX), true);
	eventFonts_[nowEvent_]->Draw(playerPosR - textSize_ + Vector2(96, 96), 64);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0.0f);

	if (bossC == nullptr) return;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, MathHelper::Lerp(0.0f, 255.0f, alphaC_));
	DrawRotaGraph(bossPosC.x, bossPosC.y, MathHelper::Lerp(0.0f, 2.0f, alphaC_), 0.0f, ResourceLoader::GetInstance().getTextureID(TextureID::HUKIDASI_TEX), true);
	eventFonts_[nowEvent_]->Draw(bossPosC - textSize_ + Vector2(96, 96), 64);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0.0f);
}

void PlayerFont::StartFont()
{
	isPlayFont_ = true;
}

void PlayerFont::ChangeFont(TxtEvent txtEvent)
{
	nowEvent_ = txtEvent;
}

void PlayerFont::AddEvent(TxtEvent txtEvent, char* txtFile, int lineNum)
{
	eventFonts_[txtEvent] = std::make_shared<PlayerTxt>(txtFile, lineNum);
}

bool PlayerFont::GetEndFont()
{
	return isPlayFont_;
}
