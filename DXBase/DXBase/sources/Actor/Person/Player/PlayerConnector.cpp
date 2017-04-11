#include "PlayerConnector.h"

#include"../../Body/CollisionBase.h"

#include "PlayerBody.h"
#include "PlayerBodyPoint.h"

#include "../Player/Effect/PlayerEffectObj.h"

#include "../../../Define.h"
#include "../../../Input/InputMgr.h"

#include "../../../Game/Time.h"
#include "../../../Scene/Base/SceneDataKeeper.h"

PlayerConnector::PlayerConnector(IWorld * world, const Vector2 & position, PlayerBodyPtr& butty, PlayerBodyPtr& retty) :
	Actor(world, "PlayerConnector", position), butty_(butty), retty_(retty),
	mPower(0.0f),
	mPuyoTimer(0.0f),
	mPuyoFlag(false),
	timer_(0),
	stateMgr_(butty, retty){
	// ポイントの生成
	create_point(PLAYER_CNTR_DIV_NUM);
	stateMgr_.change(*this, PlayerState_Enum_Union::STAND_BY);

	Vector2 texSize = ResourceLoader::GetInstance().GetTextureSize(TextureID::PUYO_TEST_TEX);
	//inv();
	mPuyo = new PuyoTextureK(world, TextureID::PUYO_TEST_TEX, position_*inv_, 1, 0);
}

PlayerConnector::~PlayerConnector() {
	StopSoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_NOBI));
	delete mPuyo;
}

void PlayerConnector::onUpdate(float deltaTime) {
	//ぷよテクスチャUPdate
	puyoUpdate();
	
	//if (world_->GetPlayerNotMove()) return;

	timer_ += deltaTime;
	position_ = (butty_->getPosition() + retty_->getPosition()) / 2;

	butty_->collider();
	retty_->collider();
	stateMgr_.action(*this, deltaTime);
	butty_->hit_vector() = Vector2::Zero;
	retty_->hit_vector() = Vector2::Zero;

	if (is_damaged()) {
		world_->GetKeeper()->addDamageCount(1);
		dead();
	}
	if (is_cleared()) world_->clear(true);
}

void PlayerConnector::onDraw() const {
	if (world_->isEntered())return;

	retty_->drawBody();
	butty_->drawBody();
	mPuyo->PuyoDraw();
}

void PlayerConnector::point_chase(float deltaTime){
	for (int i = 0; i < points.size(); i++) {
		if (stateMgr_.currentActionType(ActionType::Right)) {
			auto point = std::dynamic_pointer_cast<PlayerBodyPoint>(points[i]);
			point->attract_update(deltaTime);
			point->clamp_update(SIGN_MINUS);
			point->clamp_update(SIGN_PLUS);
		}
		else {
			auto point = std::dynamic_pointer_cast<PlayerBodyPoint>(points[points.size() - 1 - i]);
			point->attract_update(deltaTime);
			point->clamp_update(SIGN_PLUS);
			point->clamp_update(SIGN_MINUS);
		}
	}
}

void PlayerConnector::create_point(int point_num) {
	for (int i = 0; i < point_num; i++) {
		auto point = std::make_shared<PlayerBodyPoint>(world_, position_, i);
		addChild(point);
		points.push_back(point);
	}
}

bool PlayerConnector::is_damaged() {
	bool is_attack_state = stateMgr_.get_state(PlayerState_Enum_Union::ATTACK/*_R) || stateMgr_.get_state(PlayerState_Enum_Union::ATTACK_L*/);
	bool is_event_state = stateMgr_.get_state(PlayerState_Enum_Union::EVENT);
	bool is_leanback_state = stateMgr_.get_state(PlayerState_Enum_Union::LEAN_BACK);

	bool is_main_target_enemy = butty_->hit_enemy() == HitOpponent::ENEMY ||
								butty_->hit_enemy() == HitOpponent::BOSS;
	bool is_sub_target_enemy = retty_->hit_enemy() == HitOpponent::ENEMY ||
								retty_->hit_enemy() == HitOpponent::BOSS;

	bool for_debug = InputMgr::GetInstance().IsKeyDown(KeyCode::P);
	if (timer_ <= 3.0f) {
		butty_->reset_enemy();
		retty_->reset_enemy();
		return false;
	}
	return !is_event_state && !is_leanback_state && !is_attack_state && (is_main_target_enemy || is_sub_target_enemy || for_debug);
}

bool PlayerConnector::is_cleared() {
	bool is_main_target_partner = butty_->hit_enemy() == HitOpponent::CLEAR;
	bool is_sub_target_partner = retty_->hit_enemy() == HitOpponent::CLEAR;
	return is_main_target_partner || is_sub_target_partner;
}

Vector2 PlayerConnector::get_point(int index) {
	if (index == points.size()) return retty_->getPosition();
	if (index == -1) return butty_->getPosition();
	return points[index]->getPosition();
}

void PlayerConnector::ForcedMove(Vector2 velocity){
	for (int i = 0; i < points.size(); i++) {
		points[i]->position() += velocity * Time::GetInstance().deltaTime();
	}
}

void PlayerConnector::puyoUpdate() {
	float x = ((butty_->getPosition() + retty_->getPosition()) / 2).x;
	float y = ((butty_->getPosition() + retty_->getPosition()) / 2).y;
	Vector3 pos = Vector3(x, y, 0);
	Vector2 center = points[(int)(points.size() / 2)]->getPosition();

	Vector3 color;

	pos = pos*inv_;

	Vector2 posVec2 = Vector2(pos.x, pos.y);
	Vector2 vec1 = butty_->getPosition() - Vector2(x, y);
	Vector2 vec2 = retty_->getPosition() - Vector2(x, y);

	mPower = vec1.Length();

	mPuyoResPos = Vector2::Zero;
	mPuyo->PuyoAddPowerEx(vec2.Normalize(), vec2, mPower / 3.0f, 175.0f);
	mPuyo->PuyoAddPowerEx(vec1.Normalize(), vec1, mPower / 3.0f, 175.0f);
	mPuyo->SetPosition(posVec2 + mPuyoPos, 1.0f, 0.0f, center);


	//左のやつが引っ張ってる
	//if (stateMgr_.currentActionType(ActionType::Right) &&
	//	stateMgr_.currentState((unsigned int)PlayerState_Enum_Union::HOLD)) {
	//	mPuyoResPos = vec2*0.3f;
	//	mPuyo->PuyoAddPowerEx(vec1.Normalize(), vec1, mPower*0.4f, 175.0f);
	//	mPuyo->SetPosition(posVec2 + mPuyoPos, 1.0f, 0.0f, center);
	//	mPuyoTimer = 0.0f;
	//	mPuyoFlag = true;
	//}
	////右のやつが引っ張ってる
	//else if (stateMgr_.currentActionType(ActionType::Left) &&
	//	stateMgr_.currentState((unsigned int)PlayerState_Enum_Union::HOLD)) {
	//	mPuyoResPos = vec1*0.3f;
	//	mPuyo->PuyoAddPowerEx(vec2.Normalize(), vec2, mPower*0.4f, 175.0f);
	//	mPuyo->SetPosition(posVec2 + mPuyoPos, 1.0f, 0.0f, center);
	//	mPuyoTimer = 0.0f;
	//	mPuyoFlag = true;
	//}
	////同時押し
	//else if (stateMgr_.currentState((unsigned int)PlayerState_Enum_Union::HOLD_BOTH))
	//{
	//	mPuyoResPos = Vector2::Zero;
	//	mPuyo->SetPosition(posVec2 + mPuyoPos, 1.0f, 0.0f, center);
	//	mPuyo->PuyoAddPowerEx(vec2.Normalize(), vec2, mPower / 7.0f, 200.0f);
	//	mPuyo->PuyoAddPowerEx(vec1.Normalize(), vec1, mPower / 7.0f, 200.0f);
	//}
	////何もしていない
	//else
	//{
	//	mPuyoResPos = Vector2::Zero;
	//	if (!mPuyoFlag) {
	//		mPuyo->PuyoAddPowerEx(vec2.Normalize(), vec2, mPower / 7.0f, 175.0f);
	//		mPuyo->PuyoAddPowerEx(vec1.Normalize(), vec1, mPower / 7.0f, 175.0f);
	//	}
	//	mPuyo->SetPosition(posVec2 + mPuyoPos, 1.0f, 0.0f, center);
	//}

	Vector2::Spring(mPuyoPos, mPuyoResPos, mPuyoVelo, 0.3f);
	////飛ぶ瞬間は力をどこも加えない
	//if (mPuyoFlag) {
	//	mPuyoTimer += Time::GetInstance().deltaTime();
	//	if (mPuyoTimer >= 0.5f) {
	//		mPuyoFlag = false;
	//	}
	//}
	//距離が330だったため
	float dis = Vector2::Distance(butty_->getPosition(), retty_->getPosition()) / 330.0f;
	color = Vector3(255, MathHelper::Lerp(255, 0, dis), MathHelper::Lerp(255, 0, dis));

	mPuyo->PuyoPlayerPos(butty_->getPosition()*inv_, retty_->getPosition()*inv_, color,
		stateMgr_.currentState((unsigned int)PlayerState_Enum_Union::HOLD) || stateMgr_.currentState((unsigned int)PlayerState_Enum_Union::HOLD_FULL));
	mPuyo->PuyoUpdate();
}


