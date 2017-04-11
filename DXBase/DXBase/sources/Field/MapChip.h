#pragma once
#ifndef MAPCHIP_H_
#define MAPCHIP_H_
#include"../Actor/Base/Actor.h"
#include"../Define.h"
#include"../ResourceLoader/ResourceLoader.h"
#include"../World/World.h"

class MapChip :public Actor{
public:
	//空のMapChipを生成
	MapChip(){}
	//マップチップの生成
	MapChip(IWorld* world, Vector2& position);
	MapChip(IWorld* world, Vector2& position, CollisionBase& base);
	MapChip(IWorld* world, Vector2& position,std::string name, CollisionBase& base);
	MapChip(std::shared_ptr<MapChip> chip, IWorld* world, Vector2& position);
	MapChip(MapChip& chip,IWorld* world,Vector2& position);
	virtual void set(Vector2& pos);
	bool isOutCamera() const{
		auto player = world_->findActor("PlayerBody1");
		if (player == nullptr)return true;
		//Vector2 pos = ((world_->findActor("PlayerBody1")->position_ + world_->findActor("PlayerBody2")->position_) / 2) + Vector2(CHIPSIZE / 2, 0);
		Vector2 pos = player->position_ + Vector2(CHIPSIZE / 2, 0.0f);

		if (pos.x - (30 * CHIPSIZE)>position_.x || pos.x + (30 * CHIPSIZE)<position_.x) {
			return true;
		}
		else if (pos.y - (20 * CHIPSIZE)>position_.y || pos.y + (20 * CHIPSIZE)<position_.y) {
			return true;
		}
		return false;
	}
	MapChip& operator = (std::shared_ptr<MapChip> other) {
		return MapChip(other,world_, Vector2(0, 0));
	};
private:
	void onUpdate(float deltaTime);
	void onDraw() const override;
	void onCollide(Actor& other);
protected:
	int rotate_;
	int width_;
	int height_;

};

#endif // !MAPCHIP_H_