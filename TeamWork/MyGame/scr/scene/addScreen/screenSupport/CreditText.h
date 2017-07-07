#pragma once
#include "../../../actor/player/CreditPlayer.h"
#include "../../../actor/Field/Clothes/Clothes.h"

class CreditText : public Clothes/*,std::enable_shared_from_this<CreditText>*/ {
public:
	//
	CreditText(IWorld* world, CLOTHES_ID id, SPRITE_ID sprite, int laneNum, Vector2 position, Vector2 size, int frame);
	//
	~CreditText();
	//
	virtual void Update() override;
	//
	virtual void Draw() const override;
	//
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;

private:
	CreditPlayer* cPlayer_;
	int frame_;
	float f2;
};
