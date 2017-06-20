#pragma once
#include "../../../actor/player/CreditPlayer.h"
#include "../../../actor/Field/Clothes/Clothes.h"

class CreditText : public Clothes/*,std::enable_shared_from_this<CreditText>*/ {
public:
	//
	CreditText(IWorld* world, CLOTHES_ID id,int laneNum, Vector2 position);
	//
	~CreditText();
	//
	virtual void Update() override;
	//
	virtual void Draw() const override;
	//
	virtual void OnCollide(Actor& other, CollisionParameter colpara) override;

private:
	CreditPlayer* player_;
};
