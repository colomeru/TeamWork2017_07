#pragma once
#include"../Base/Actor.h"
#include"../../ResourceLoader/ResourceLoader.h"
#include"BadItemAnm.h"
#include"../../World/World.h"

class BadItems :public Actor {
public:
	BadItems(IWorld* world, Vector2 position) :Actor(world, "BadItem", position, CollisionBase(position, CHIPSIZE)), spriteType_(TextureID::ITEM_MINUS_TEX), spriteID_(ResourceLoader::GetInstance().getTextureID(TextureID::ITEM_MINUS_TEX)), ismyHit_(false), size_(1), lCount(0), speed_(0) {
		anmer_ = BadItemAnm();
	}
	virtual void onUpdate(float deltaTime) override {
		if (ismyHit_) {
			if (size_ >= 0.8f)
			{
				size_ -= deltaTime*5.f;
			}
			else if (size_ >= 0.4f) {
				size_ -= deltaTime*5.f;

				lCount += 0.00001f;
				//lCount -= 0.003f;
				position_ = position_.Lerp(position_, Vector2(SCREEN_SIZE.x - 100 - ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX).x / 2, 50), lCount);//GamePlaySceneのドロー位置と合わせる事

			}
			else {
				speed_ += deltaTime*0.05f;
				lCount += speed_;
				//drawPos=position_.Lerp(position_, Vector2(SCREEN_SIZE.x - 100 - ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX).x/2, 50), lCount);//GamePlaySceneのドロー位置と合わせる事
			}
			lCount = min(lCount, 1);

			if (lCount >= 1)
			{
				world_->GetKeeper()->addCount(-10);
				dead();
			}
		}
		else {
			anmer_.update_e(deltaTime * 10);
		}
	}
	virtual void onDraw()const override {
		//Vector3 center = Vector3(drawPos.x,drawPos.y)*inv_;
		//Vector3 center = Vector3(position_.x- ResourceLoader::GetInstance().GetTextureSize(spriteType_).x/2, position_.y- ResourceLoader::GetInstance().GetTextureSize(spriteType_).y/2)*inv_;
		Vector3 drawPos = Vector3(position_.x, position_.y)*inv_;
		Vector2 itemDefPos = Vector2(SCREEN_SIZE.x - CountPos - ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_MINUS_TEX).x / 2, 50);
		Vector3 ItemPos = Vector3(itemDefPos.x, itemDefPos.y);
		Vector3 center = center.Lerp(drawPos, ItemPos, lCount);
		//Vector3 center= Vector3(position_.Lerp(position_, Vector2(SCREEN_SIZE.x - 100 - ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX).x / 2, 50), lCount).x, position_.Lerp(position_, Vector2(SCREEN_SIZE.x - 100 - ResourceLoader::GetInstance().GetTextureSize(TextureID::ITEM_TEX).x / 2, 50), lCount).y)*inv_;
		if (ismyHit_)DrawRotaGraph2((int)center.x, (int)center.y, (int)ResourceLoader::GetInstance().GetTextureSize(spriteType_).x / 2, (int)ResourceLoader::GetInstance().GetTextureSize(spriteType_).y / 2, size_, 0, spriteID_, TRUE);
		else {
			anmer_.draw_e(Vector2(drawPos.x, drawPos.y));
		}
		//DrawGraph(center.x, center.y, spriteID_, TRUE);
	}
	virtual void onCollide(Actor& other) override {
		if (ismyHit_)return;
		body_.enabled(false);
		ismyHit_ = true;
		ChangeNextPlayVolumeSoundMem(180, ResourceLoader::GetInstance().getSoundID(SoundID::SE_GETCOIN));
		PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_GETMINUSCOIN), DX_PLAYTYPE_BACK);
		world_->GetKeeper()->addKeepCount(-10);
	}
	int spriteID_;
	float size_;
	float lCount;
	float speed_;
	TextureID spriteType_;
	bool ismyHit_;
	Vector2 drawPos;

	BadItemAnm anmer_;

};