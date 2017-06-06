#pragma once
#include"../BackgroundCharacters.h"


class BackgroundPill :public BackgroundCharacters {
public:
	BackgroundPill(IWorld* world,const Vector2& position);
	virtual void Update()override;
	virtual void FastUpdate() override {
		laneNum_ = world_->GetKeepDatas().playerLane_;
		isUpdate_ = true;
		isDraw_ = true;
	}
	virtual void LateUpdate() override {
		isDraw_ = true;
	}
	virtual void Draw()const override;
	virtual void Del()override;
private:
};