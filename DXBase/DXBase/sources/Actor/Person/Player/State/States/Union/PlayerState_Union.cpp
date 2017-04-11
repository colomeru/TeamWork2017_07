#include "PlayerState_Union.h"

#include "../../../../../../Define.h"
#include "../../../../../../Input/InputMgr.h"

// コンストラクタ
PlayerState_Union::PlayerState_Union(const PlayerBodyPtr& butty, const PlayerBodyPtr& retty) :
	butty_(butty),
	retty_(retty){}

// 全てのステートに共通する初期化処理
void PlayerState_Union::common_init(Actor& actor, const StateElement& element){
	element_ = element;
	timer_ = 0;
	isEnd_ = false;

	cntr_ = dynamic_cast<PlayerConnector*>(&actor);
}

void PlayerState_Union::input(float deltaTime){}

void PlayerState_Union::common_update(float deltaTime){

	cntr_->point_chase(deltaTime);
	
	if (cntr_->getWorld()->GetPlayerNotMove()) {
		change(PlayerState_Enum_Union::IDLE);
	}

	butty_->position() += butty_->hit_vector();
	retty_->position() += retty_->hit_vector();

	butty_->velocity() = Vector2::One;
	retty_->velocity() = Vector2::One;
	butty_->position() = Vector2::Clamp(butty_->position(), butty_->getClampPoint_(), Vector2::One * 50000);
	retty_->position() = Vector2::Clamp(retty_->position(), butty_->getClampPoint_(), Vector2::One * 50000);
	//butty_->position() = Vector2::Clamp(butty_->position(), Vector2::One * PLAYER_RADIUS, Vector2::One * 50000);
	//retty_->position() = Vector2::Clamp(retty_->position(), Vector2::One * PLAYER_RADIUS, Vector2::One * 50000);
}

// 衝突処理
void PlayerState_Union::collide(const Actor & other) {}

// 描画処理
void PlayerState_Union::draw() const {}

void PlayerState_Union::change(const PlayerState_Enum_Union & id, const ActionType & type){
	request(StateElement(static_cast<int>(id), type));
}

// パッド系フラグ
bool PlayerState_Union::holdable_padR() {
	return InputMgr::GetInstance().IsButtonOn(Buttons::BUTTON_R1) && butty_->getColliderOpponent() == HitOpponent::FLOOR_HIT;
}

bool PlayerState_Union::holdable_padL() {
	return InputMgr::GetInstance().IsButtonOn(Buttons::BUTTON_L1) && retty_->getColliderOpponent() == HitOpponent::FLOOR_HIT;
}

bool PlayerState_Union::move_padR() {
	return InputMgr::GetInstance().AnalogPadVectorR().Length() > 0/* && butty_->able_to_jump()*/;
}

bool PlayerState_Union::move_padL() {
	return InputMgr::GetInstance().AnalogPadVectorL().Length() > 0/* && retty_->able_to_jump()*/;
}

bool PlayerState_Union::jump_pad() {
	return InputMgr::GetInstance().IsButtonOn(Buttons::BUTTON_L2) && InputMgr::GetInstance().IsButtonOn(Buttons::BUTTON_R2);
}

Vector2 PlayerState_Union::speed_b(float deltaTime){
	return butty_->velocity() * PLAYER_SPEED * butty_->dump() * time(deltaTime);
}

Vector2 PlayerState_Union::speed_r(float deltaTime){
	return retty_->velocity() * PLAYER_SPEED * retty_->dump() * time(deltaTime);
}

void PlayerState_Union::chase(Vector2 &position, int pointIndex){
	Vector2::Spring(position, cntr_->get_point(pointIndex), velocity_, SPRING_STIFFNESS, SPRING_FRICTION, SPRING_MASS);
}

Vector2 PlayerState_Union::clamp(const Vector2 &position, int pointIndex) {
	return Vector2::ClampTarget(position, cntr_->get_point(pointIndex), PLAYER_MAX_DIV_LENGTH);
}

bool PlayerState_Union::isOnFloor() {
	return  (butty_->getOpponent() == HitOpponent::FLOOR_TOP || butty_->getOpponent() == HitOpponent::FLOOR_HIT) &&
			(retty_->getOpponent() == HitOpponent::FLOOR_TOP || retty_->getOpponent() == HitOpponent::FLOOR_HIT);
}

