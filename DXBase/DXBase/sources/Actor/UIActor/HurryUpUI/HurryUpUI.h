#pragma once
#include "../../Base/ActorUI.h"
#include "../../../ResourceLoader/ResourceID.h"

#include<map>

class HurryUpUI : public ActorUI {
public:
	HurryUpUI(IWorld* world);
	~HurryUpUI();
	virtual void onUpdate(float deltaTime) override;
	virtual void onDraw() const override;
private:
	std::map<int, TextureID> NumIDs;

	TextureID button_R1_;
	TextureID button_L1_;

	float timer_;
};