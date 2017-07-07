#pragma once
#include "../../../actor/player/CreditPlayer.h"
#include "../../../actor/Field/Clothes/Clothes.h"
#include "../../../actor/Field/Enemys/EnemyCharas/CharacterAnmManager.h"

class CreditPostText : public Clothes/*,std::enable_shared_from_this<CreditText>*/ {
public:
	//
	CreditPostText(IWorld* world, CLOTHES_ID id, SPRITE_ID sprite, int laneNum, Vector2 position, Vector2 size, int frame);
	//
	~CreditPostText();
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

	CharacterAnmManager anmManager_;
};
