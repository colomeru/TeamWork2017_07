#include "Player.h"
#include "../../input/Keyboard.h"
#include "../../conv/DXConverter.h"
#include "../../graphic/Model.h"
#include"../../math/MathHelper.h"
#include"Player_Head.h"
#include"../../math/Matrix.h"
#include"../../math/MyFuncionList.h"

static const float headShotPower = 0.3f;
static const float defMaxChainLength = 16.f;
static const float defJumpShotPower = 1.f;

Player::Player(IWorld * world)
	:Actor(world),
	isHit_(false), fulcrum_(500.0f, 200.0f), rot_(135.f), rot_spd_(-3.0f), length_(300.0f), gravity_(0.3f), currentHead_(0),
	headChangeTime_(0), pGrav_(defPGravPow), maxChainLength_(defMaxChainLength), isBiteMode_(false), isShootMode_(0), isNextPushKey_(true),
	pendulumVect_(Vector2::Zero), slipCount_(defSlipCount), jumpShotPower_(defJumpShotPower), isSlipped_(false)
{
	laneNum_ = 1;

	parameter_.ID = ACTOR_ID::PLAYER_ACTOR;
	parameter_.radius = 32.0f;
	parameter_.size = Vector2(96.0f, 96.0f);
	parameter_.HP = 10;
	parameter_.mat
		= Matrix::CreateScale(Vector3::One)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3::Zero);

	position_ = Vector2(0,200);

	pHeads_.resize(8);
	for (int i = 0; i < 8; i++)
	{
		//8方向のうち、各頭に対応した位置を作成
		Vector3 tgtRot= Vector3(pHDist.x,pHDist.y)*Matrix::CreateRotationZ((i+ headAngleSetter) * 45);
		Vector2 cgToV2 = position_+Vector2(tgtRot.x, tgtRot.y);
		pHeadPoses_.push_back(cgToV2);

		pHeadLength_.push_back(defHeadLength);

		pHeads_[i]=(std::make_shared<Player_Head>(world,this, pHeadPoses_[i],i));
		world_->Add(ACTOR_ID::PLAYER_ACTOR,pHeads_[i]);

		SetMyHeadLaneNum(i);
	}
	worldSetMyDatas();
}

Player::~Player()
{
}

void Player::Update()
{
	pendulumVect_ -= pendulumVect_*0.05f;

	length_ = Vector2::Distance(pHeads_[currentHead_]->GetPosition(), position_);
	//if(!pHeads_[currentHead_]->getIsHit())pGrav_+= defPGravPow;
	if (!isBiteMode_)pGrav_ += defPGravPow;
	velocity_ = Vector2::Zero;
	isHit_ = false;
	auto pos = parameter_.mat.Translation();

	//重力
	velocity_.y += pGrav_*pGrav_*defGravAddPow;
	//入力による動作をまとめた関数
	PlayerInputControl();





	//自分,相手のID,Colの種類
	//world_->SetCollideSelect(shared_from_this(), ACTOR_ID::STAGE_ACTOR, COL_ID::BOX_CLOTHES_COL);

	//行列にangleをかける
	parameter_.mat += Matrix::CreateRotationZ(angle_);
	parameter_.mat.NormalizeRotationMatrix();

	//正面への移動量を追加(DXライブラリだからZにマイナス)
	//velocity_ += parameter_.mat.Backward() * speed;
	//velocity_ += parameter_.mat.Up() * jumpVec;
	//velocityをpositionに追加
	pos += Vector3(velocity_.x, velocity_.y);

	//if (pos.y < 0)pos.y = 0;

	if (!isBiteMode_) {
		position_ += velocity_ + pendulumVect_;

		slipCount_ = defSlipCount;
	}
	parameter_.mat.Translation(pos);

	HeadPosUpdate();


	if (isBiteMode_) {
		slipCount_ -= 0.016f;
		if (slipCount_ <= 0.f) {
			isBiteMode_ = false;
			isSlipped_ = true;
			//スリップモードに移行すると同時に、その時点のベクトルをHeadに格納する
			pHeads_[currentHead_]->SetPosAddVect(pHeads_[currentHead_]->GetPosition() - position_);
		}
		slipCount_ = max(slipCount_,0.f);
	}
}

void Player::Draw() const
{
	//auto pos_1 = DXConverter::GetInstance().ToVECTOR(parameter_.mat.Translation());
	//auto pos_2 = DXConverter::GetInstance().ToVECTOR(parameter_.mat.Translation() + Vector3(0, 10, 0));
	//DrawCapsule3D(pos_1, pos_2, 5.0f, 4, GetColor(255, 0, 0), GetColor(255, 0, 0), true);

	//Model::GetInstance().Draw(MODEL_ID::PLAYER_MODEL, parameter_.mat);

	auto is = Matrix::CreateRotationZ(angle_);
	auto pos = drawPos_;
	auto sizeVec = Vector3((parameter_.size.x / 2), (parameter_.size.y / 2));

	auto box1 = Vector3(-sizeVec.x, -sizeVec.y)*is;
	auto box2 = Vector3(+sizeVec.x, -sizeVec.y)*is;
	auto box3 = Vector3(-sizeVec.x, +sizeVec.y)*is;
	auto box4 = Vector3(+sizeVec.x, +sizeVec.y)*is;
	//auto seg = Vector3(+sizeVec.x, 0)*is;

	//左上,右上,左下,右下
	auto pos1 = Vector3(pos.x + box1.x, pos.y + box1.y);
	auto pos2 = Vector3(pos.x + box2.x, pos.y + box2.y);
	auto pos3 = Vector3(pos.x + box3.x, pos.y + box3.y);
	auto pos4 = Vector3(pos.x + box4.x, pos.y + box4.y);
	//Model::GetInstance().Draw(MODEL_ID::PLAYER_MODEL, parameter_.mat);
	//DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, GetColor(255, 255, 255));
	//DrawLine(pos1.x, pos1.y, pos3.x, pos3.y, GetColor(255, 255, 255));
	//DrawLine(pos2.x, pos2.y, pos4.x, pos4.y, GetColor(255, 255, 255));
	//DrawLine(pos3.x, pos3.y, pos4.x, pos4.y, GetColor(255, 255, 255));
	DrawCircle(drawPos_.x, drawPos_.y, pHDist.Length(), GetColor(255, 255, 255));
	//DrawLine(pos.x - seg.x, pos.y-seg.y, pos.x + seg.x, pos.y+seg.y, GetColor(255, 255, 255));
	DrawFormatString(0, 60, GetColor(255, 255, 255), "position x:%f y:%f z:%f", pos.x, pos.y);
	DrawFormatString(0, 80, GetColor(255, 255, 255), "angle %f", velocity_.y);
	DrawFormatString(0, 100, GetColor(255, 255, 255), "%d",laneNum_);
	if (isShootMode_>=1)DrawFormatString(0, 700, GetColor(255, 255, 255), "true");
	else DrawFormatString(0, 700, GetColor(255, 255, 255), "false");
	DrawFormatString(0, 700, GetColor(255, 255, 255), "%f:%f", pHeadPoses_[currentHead_].x, pHeadPoses_[currentHead_].y);

	for (auto sgT : pHeads_) {
		DrawLine(drawPos_.x, drawPos_.y, sgT->GetDrawPos().x, sgT->GetDrawPos().y, GetColor(255, 255, 255));
	}
	int count = 0;
	for (auto sgT : pHeadLength_) {
		DrawFormatString(300, 300 + (30 * count),GetColor(255,255,255),"%f",sgT );
		count++;
	}


}

void Player::OnUpdate()
{
}

void Player::OnCollide(Actor& other, CollisionParameter colpara)
{
	isHit_ = true;
}

void Player::OnMessage(EventMessage message, void * param)
{
}
void Player::Pendulum(Vector2 fulcrum, float length)
{
	float friction = 0.998f;								//摩擦
	//現在の重りの位置
	position_.x = fulcrum.x + MathHelper::Cos(rot_) * length;
	position_.y = fulcrum.y + MathHelper::Sin(rot_) * length;
	
	Vector2 curdefPos = position_;

	//重力移動量を反映した重りの位置
	auto length_vec = position_ - fulcrum;
	auto t = -(length_vec.y * gravity_) / (length_vec.x * length_vec.x + length_vec.y * length_vec.y);
	auto move_weightX = position_.x + t * length_vec.x;
	auto move_weightY = position_.y + gravity_ + t * length_vec.y;

	//2つの重りの位置の角度差
	auto r = MathHelper::ATan(move_weightY - fulcrum.y, move_weightX - fulcrum.x);
	//角度差を角速度に加算
	auto sub = r - rot_;
	sub -= std::floor(sub / 360.0f) * 360.0f;
	if (sub < -180.0f) sub += 360.0f;
	if (sub > 180.0f) sub -= 360.0f;
	rot_spd_ += sub;

	//摩擦
	rot_ *= friction;

	//角度に角速度を加算
	rot_ += rot_spd_;

	//新しい重りの位置
	position_.x = fulcrum.x + MathHelper::Cos(rot_) * length;
	position_.y = fulcrum.y + MathHelper::Sin(rot_) * length;

	//ここまで振り子
	//
	Vector2 pendulumAngleVec = fulcrum - position_;
	
	float angleDtData = Vector2::Dot(Vector2::Right, pendulumAngleVec.Normalize());

	angle_ = (angleDtData * 180)/2;
	
	pendulumVect_ = (position_ - curdefPos);
	pendulumVect_.x = pendulumVect_.x*jumpShotPower_;
}

void Player::HeadPosUpdate()
{
	headChangeTime_ -= 0.016f*sign(headChangeTime_);

	rotTimer=0;
	if (MathHelper::Abs(headChangeTime_) <= 0.01f)headChangeTime_ = 0;
	else if (MathHelper::Abs(headChangeTime_) > 0)rotTimer = headChangeTime_ * 5;//MathHelper::Abs(defHeadChangeTime/1.f);
	
	for (int i = 0; i < pHeadPoses_.size(); i++) {
		Vector3 tgtRot = Vector3(pHDist.x, pHDist.y)*Matrix::CreateRotationZ(((i+headAngleSetter - currentHead_) * 45)/*+angle_*/ +((rotTimer)*45));
		Vector2 cgToV2 = position_ + Vector2(tgtRot.x, tgtRot.y);
		pHeadPoses_[i] = cgToV2;
	}

}


//Headのレーンを本体のレーンに合わせる

void Player::SetMyHeadLaneNum(int targetNum) {
		pHeads_[targetNum]->SetLaneNum(laneNum_);
}

inline void Player::SetAllHeadLaneNum() {
	for (auto& pHs : pHeads_)
	{
		pHs->SetLaneNum(laneNum_);
	}
}

inline void Player::worldSetMyDatas() {
	//共有データに、自身の現レーン位置を保存
	//world_->SetKeepDatas(KeepDatas(laneNum_));
	world_->GetKeepDatas().SetPlayerLane(laneNum_);
}

void Player::PlayerInputControl()
{
	//if (pHeads_[currentHead_]->getIsHit()) {
	//if (isBiteMode_&&pHeads_[currentHead_]->getIsHit()) {
	if (pHeads_[currentHead_]->getIsHit()) {
		OutputDebugString(std::to_string(pHeads_[currentHead_]->GetPosition().x).c_str());
		OutputDebugString(":");
		OutputDebugString(std::to_string(pHeads_[currentHead_]->GetPosition().y).c_str());
		OutputDebugString("\n");
		//Pendulum(pHeadPoses_[currentHead_]+(pHeads_[currentHead_]->GetPosition()- pHeadPoses_[currentHead_]), length_);
		Pendulum(pHeads_[currentHead_]->GetPosition(), length_);
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::UP)) {
		velocity_.y -= 20.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::DOWN)) {
		velocity_.y += 20.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::RIGHT)) {
		velocity_.x += 20.0f;
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::LEFT)) {
		velocity_.x -= 20.0f;
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::W)) {
		UpdateLaneNum(1);
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::S)) {
		UpdateLaneNum(-1);
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::Q)) {
		jumpShotPower_ -= 1.f;
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::E)) {
		jumpShotPower_ += 1.f;
	}

	SetAllHeadLaneNum();
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::Z)&&!isBiteMode_) {
		isShootMode_ = 0;
		isBiteMode_ = false;
		//キーを押し直したかの判断
		PHeadLengthReset();
		changeHead();
	}
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::X) && !isBiteMode_) {
		isShootMode_ = 0;
		isBiteMode_ = false;
		//キーを押し直したかの判断
		PHeadLengthReset();
		backChangeHead();
	}

	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
		isNextPushKey_ = true;
		if (!isBiteMode_&&isNextPushKey_) {
			isShootMode_ = 1;
			isNextPushKey_ = false;
		}
		if (isBiteMode_) {
			isBiteMode_ = false;
			PHeadLengthReset();
			//場所を戻してからHeadを交代する
			changeHead();
		}
	}
	if (Keyboard::GetInstance().KeyStateDown(KEYCODE::M)) {
		if(!isBiteMode_&&isShootMode_==1)CurPHeadLengPlus(headShotPower);
	}
	else if(Keyboard::GetInstance().KeyTriggerUp(KEYCODE::M)){
		isShootMode_ = 2;
	}
	else {
		isShootMode_ = 0;
	}
	//if (Keyboard::GetInstance().KeyStateDown(KEYCODE::M)) {
	//	if (!isBiteMode_)isShootMode_ = true;
	//	if (!isBiteMode_&&isNextPushKey_) {
	//		isShootMode_ = true;

	//		CurPHeadLengPlus(headShotPower);
	//	}
	//	isBiteMode_ = false;
	//}
	//if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)) {
	//	if (pHeads_[currentHead_]->getIsHit()) {
	//		isShootMode_ = false;
	//		//キーを押し直したかの判断
	//		isNextPushKey_ = false;
	//		PHeadLengthReset();
	//		//場所を戻してからHeadを交代する
	//		changeHead();
	//	}
	//	else {
	//		isNextPushKey_ = true;
	//		isShootMode_ = false;
	//	}
	//}
	//if(!Keyboard::GetInstance().KeyStateDown(KEYCODE::M)){
	//	if (isShootMode_) {
	//		if (pHeads_[currentHead_]->getIsBitePoint()) {
	//			//isBiteMode_ = true;
	//		}
	//	}
	//	//isShootMode_ = false;
	//}

}

void Player::CurPHeadLengPlus(float addPow) {

	if (pHeadLength_[currentHead_]>=16.f){
		return;
	}

	//現在のHead以外の長さを伸ばした分だけマイナスする
	pHeadLength_[currentHead_] += addPow;
	if (pHeadLength_[currentHead_] > 16.f)pHeadLength_[currentHead_] = 16.f;

	int targetNum = currentHead_ - 1;

	float LengthKeepNum = 1.f;
	if (targetNum<0) {
		targetNum= targetNum + 8;
	}
	for (;;) {
		if (LengthKeepNum < 0.f)break;

		if (pHeadLength_[targetNum] < LengthKeepNum)
		{
			targetNum--;

			if (targetNum<0) {
				targetNum = targetNum + 8;
			}

			if (targetNum == currentHead_) {
				LengthKeepNum -= 1.f;
				targetNum--;

				if (targetNum<0) {
					targetNum = targetNum + 8;
				}
			}

			continue;
		}
		pHeadLength_[targetNum] -= addPow;
		if (pHeadLength_[targetNum] < 0)pHeadLength_[targetNum] = 0;
		break;
	}
}
