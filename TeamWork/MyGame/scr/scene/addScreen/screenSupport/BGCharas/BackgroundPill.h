#pragma once
#include"../BackgroundCharacters.h"


class BackgroundPill :public BackgroundCharacters {
public:
	BackgroundPill(IWorld* world,const Vector2& position);
	virtual void Update()override;
	virtual void FastUpdate() override {
		isUpdate_ = true;
		isDraw_ = true;
	}
	virtual void Draw()const override;
	virtual void Del()override;
private:
};