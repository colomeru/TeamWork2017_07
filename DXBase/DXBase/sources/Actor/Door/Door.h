#pragma once
#include "../Base/Actor.h"
#include "../../World/IWorld.h"
#include "../../Math/Vector3.h"
#include "../../Animation/Base/Animation2D.h"
// プレーヤー
class Door : public Actor {
public:
	Door(IWorld* world, const Vector2& position);
	~Door();
	virtual void onUpdate(float deltaTime) override;
	virtual void onDraw() const override;
	virtual void onCollide(Actor& other) override;
public:
	//ドアの開閉を設定 true:開く false:閉じる
	void DoorOpen(bool openFlag);
private:
	bool mDoorFlag;
	Animation2D mLoadAnim;
	bool mEndAnim;
	bool mOpenFlag;
	bool mCloseFlag;

};