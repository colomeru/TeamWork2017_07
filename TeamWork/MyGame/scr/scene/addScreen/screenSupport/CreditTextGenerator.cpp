#include "CreditTextGenerator.h"
#include "../../../tween/TweenManager.h"
#include "CreditText.h"
#include "../../../time/Time.h"
#include "CreditPostText.h"

CreditTextGenerator::CreditTextGenerator(IWorld * world, Vector2 position) :
	Actor(world)
{
	position_ = position;
	timer_ = 0.0f;
	frame_ = 0;
	idCount_ = 0;
	interval_ = 3.0f;
	id_.push_back(SPRITE_ID::CREDIT_SAITO_SPRITE);
	id_.push_back(SPRITE_ID::CREDIT_MIYAUCHI_SPRITE);
	id_.push_back(SPRITE_ID::CREDIT_SIM_SPRITE);
	id_.push_back(SPRITE_ID::CREDIT_SUGIURA_SPRITE);
	id_.push_back(SPRITE_ID::CREDIT_KUWABARA_SPRITE);
	id_.push_back(SPRITE_ID::CREDIT_NEGISHI_SPRITE);
	id_.push_back(SPRITE_ID::CREDIT_NAGAO_SPRITE);

	postId_.push_back(SPRITE_ID::CREDIT_MPL_SPRITE);
	postId_.push_back(SPRITE_ID::CREDIT_MPG_SPRITE);
	postId_.push_back(SPRITE_ID::CREDIT_PG_SPRITE);
	postId_.push_back(SPRITE_ID::CREDIT_PG_SPRITE);
	postId_.push_back(SPRITE_ID::CREDIT_PG_SPRITE);
	postId_.push_back(SPRITE_ID::CREDIT_SPG_SPRITE);
	postId_.push_back(SPRITE_ID::CREDIT_DESIGNER_SPRITE);

	postSize_.push_back(Vector2(786, 150));
	postSize_.push_back(Vector2(1302, 150));
	postSize_.push_back(Vector2(899, 150));
	postSize_.push_back(Vector2(899, 150));
	postSize_.push_back(Vector2(899, 150));
	postSize_.push_back(Vector2(1291, 150));
	postSize_.push_back(Vector2(775, 150));

	auto count = id_.size();
	TweenManager::GetInstance().LoopCount(Linear, count, &loopHandle_, 1.0f, 6.0f, [=]() {ClothAdd(); });
}

CreditTextGenerator::~CreditTextGenerator()
{
}

void CreditTextGenerator::Update()
{
}

void CreditTextGenerator::Draw() const
{
}

void CreditTextGenerator::ClothAdd()
{
	auto nameCount = Sprite::GetInstance().GetSliptFrameSize(id_[idCount_]);
	for (int i = nameCount - 1; i >= 0; i--) {
		world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<CreditText>(world_, CLOTHES_ID::TEXT_CLOTHES, id_[idCount_], world_->GetKeepDatas().playerLane_, Vector2(WINDOW_WIDTH + i * 370.0f, 257.0f), Vector2(300, 300), i));
	}

	world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<CreditPostText>(world_, CLOTHES_ID::TEXT_CLOTHES, postId_[idCount_], world_->GetKeepDatas().playerLane_, Vector2(WINDOW_WIDTH + 520.0f, -140.0f), postSize_[idCount_], 0));

	idCount_++;
}

void CreditTextGenerator::PostAdd()
{

}
