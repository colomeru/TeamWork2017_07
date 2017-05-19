#pragma once

#include"../../../actor/Actor.h"
#include<vector>

class StartCameraPointAct : public Actor//, public std::enable_shared_from_this<Player>
{
public:
	//コンストラクタ
	StartCameraPointAct(IWorld* world,float startPosx);
	//デストラクタ
	~StartCameraPointAct();
	//更新
	virtual void FastUpdate() override {
		isUpdate_ = true;
	}
	virtual void Update() override;
	virtual void StartOnlyUpdate()override;
		//描画
	virtual void Draw() const override;
	//受動更新
	virtual void OnUpdate() override;
	//衝突時のアクション
	virtual void OnCollide(Actor&, CollisionParameter colpara) override;
	//メッセージ取得
	virtual void OnMessage(EventMessage message, void* param);

	bool isArrivePoint() const{
		return position_.x <= 0;
	}
private:

};