#pragma once
#include "../../../actor/Actor.h"
#include <vector>


class CreditTextGenerator : public Actor {
public:
	//
	CreditTextGenerator(IWorld* world, Vector2 position);
	//
	~CreditTextGenerator();
	//
	virtual void Update() override;
	//
	virtual void Draw() const override;

private:
	void ClothAdd();
	void PostAdd();

private:
	float timer_;
	const float DefTime = 0.5f;
	int frame_;
	std::vector<SPRITE_ID> id_;
	int idCount_;
	float interval_;
	float loopHandle_;

	std::vector<SPRITE_ID> postId_;
	std::vector<Vector2> postSize_;

};
