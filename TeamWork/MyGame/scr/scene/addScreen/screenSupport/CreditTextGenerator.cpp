#include "CreditTextGenerator.h"
#include "../../../tween/TweenManager.h"
#include "CreditText.h"
#include "../../../time/Time.h"

CreditTextGenerator::CreditTextGenerator(IWorld * world, Vector2 position) :
	Actor(world)
{
	position_ = position;
	timer_ = 0.0f;
	frame_ = 0;
	idCount_ = 0;
	interval_ = 3.0f;
	test_ = false;
	id_.push_back(SPRITE_ID::CREDIT_SAITO_SPRITE);
	id_.push_back(SPRITE_ID::CREDIT_MIYAUCHI_SPRITE);
	id_.push_back(SPRITE_ID::CREDIT_NEGISHI_SPRITE);
	id_.push_back(SPRITE_ID::CREDIT_SIM_SPRITE);
	id_.push_back(SPRITE_ID::CREDIT_SUGIURA_SPRITE);
	id_.push_back(SPRITE_ID::CREDIT_KUWABARA_SPRITE);
	id_.push_back(SPRITE_ID::CREDIT_SAITO_SPRITE);

	auto count = id_.size();
	TweenManager::GetInstance().LoopCount(Linear, count, &loopHandle_, 1.0f, 6.0f, [=]() {ClothAdd(); });
}

CreditTextGenerator::~CreditTextGenerator()
{
}

void CreditTextGenerator::Update()
{
	//if (idCount_ >= 7)idCount_ = 0;

	//if (!test_) {
	//	timer_ += Time::DeltaTime;
	//}
	//else {
	//	interval_ -= Time::DeltaTime;
	//	if (interval_ <= 0.0f) {
	//		test_ = !test_;
	//		interval_ = 3.0f;
	//	}
	//}

	//if (!test_) {

	//}

	//for (int i = 0; i <= Sprite::GetInstance().GetSliptFrameSize(id_[idCount_]); i++) {
	//		world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<CreditText>(world_, CLOTHES_ID::TEXT_CLOTHES, id_[idCount_], world_->GetKeepDatas().playerLane_, Vector2(WINDOW_WIDTH + i * 160.0f, 0.0f), frame_));
	//		frame_++;
	//		//timer_ = 0.0f;
	//		if (Sprite::GetInstance().GetSliptFrameSize(id_[idCount_]) <= frame_) {
	//			frame_ = 0;
	//			idCount_++;
	//			test_ = true;
	//		}
	//}
	//if (timer_ >= DefTime) {
	//	world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<CreditText>(world_, CLOTHES_ID::TEXT_CLOTHES, id_[idCount_], world_->GetKeepDatas().playerLane_, Vector2(WINDOW_WIDTH, 0.0f), frame_));
	//	frame_++;
	//	timer_ = 0.0f;
	//	if (Sprite::GetInstance().GetSliptFrameSize(id_[idCount_]) <= frame_) {
	//		frame_ = 0;
	//		idCount_++;
	//		test_ = true;
	//	}

	//}

	//ClothAdd();
}

void CreditTextGenerator::Draw() const
{
}

void CreditTextGenerator::ClothAdd()
{
	auto nameCount = Sprite::GetInstance().GetSliptFrameSize(id_[idCount_]);
	for (int i = 0; i < nameCount; i++) {
		world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<CreditText>(world_, CLOTHES_ID::TEXT_CLOTHES, id_[idCount_], world_->GetKeepDatas().playerLane_, Vector2(WINDOW_WIDTH + i * 370.0f, 150.0f), i));

	}
	idCount_++;
	//timer_ = 0.0f;
	//if (Sprite::GetInstance().GetSliptFrameSize(id_[idCount_]) <= frame_) {
	//	frame_ = 0;
	//	idCount_++;
	//	test_ = true;
	//}


	//if (idCount_ >= 7)idCount_ = 0;
	//if (!test_) {
	//	for (int i = 0; i <= Sprite::GetInstance().GetSliptFrameSize(id_[idCount_]); i++) {
	//		world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<CreditText>(world_, CLOTHES_ID::TEXT_CLOTHES, id_[idCount_], world_->GetKeepDatas().playerLane_, Vector2(WINDOW_WIDTH + i * 320.0f, 0.0f), frame_));
	//		frame_++;
	//		if (Sprite::GetInstance().GetSliptFrameSize(id_[idCount_]) <= frame_) {
	//			frame_ = 0;
	//			idCount_++;
	//			test_ = true;
	//		}
	//	}

	//}
	//else {
	//	interval_ -= Time::DeltaTime;
	//	if (interval_ <= 0.0f) {
	//		test_ = !test_;
	//		interval_ = 3.0f;
	//	}
	//}

}
