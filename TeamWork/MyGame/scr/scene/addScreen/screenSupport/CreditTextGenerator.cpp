#include "CreditTextGenerator.h"
#include "../../../tween/TweenManager.h"
#include "CreditText.h"
#include "../../../time/Time.h"
#include "CreditPostText.h"

//コンストラクタ
CreditTextGenerator::CreditTextGenerator(IWorld * world, Vector2 position) :
	Actor(world)
{
	position_ = position;
	frame_ = 0;
	idCount_ = 0;
	int nameIdNum = CREDIT_SAITO_SPRITE;
	for (int i = 0; i < 7; i++) {
		id_.push_back((SPRITE_ID)(nameIdNum + i));
	}

	postId_.push_back(SPRITE_ID::CREDIT_MPL_SPRITE);
	postId_.push_back(SPRITE_ID::CREDIT_MPG_SPRITE);
	postId_.push_back(SPRITE_ID::CREDIT_PG_SPRITE);
	postId_.push_back(SPRITE_ID::CREDIT_PG_SPRITE);
	postId_.push_back(SPRITE_ID::CREDIT_PG_SPRITE);
	postId_.push_back(SPRITE_ID::CREDIT_SPG_SPRITE);
	postId_.push_back(SPRITE_ID::CREDIT_DESIGNER_SPRITE);

	for (int i = 0; i < 7; i++) {
		postSize_.push_back(Sprite::GetInstance().GetSize(postId_.at(i)));
	}

	auto count = id_.size();
	TweenManager::GetInstance().LoopCount(Linear, count, &loopHandle_, 1.0f, 6.0f, [=]() {ClothAdd(); });
}

//デストラクタ
CreditTextGenerator::~CreditTextGenerator()
{
}

//更新
void CreditTextGenerator::Update()
{
}

//描画
void CreditTextGenerator::Draw() const
{
}

//生成
void CreditTextGenerator::ClothAdd()
{
	//名前
	auto nameCount = Sprite::GetInstance().GetSliptFrameSize(id_[idCount_]);
	for (int i = nameCount - 1; i >= 0; i--) {
		world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<CreditText>(world_, CLOTHES_ID::TEXT_CLOTHES, id_[idCount_], world_->GetKeepDatas().playerLane_, Vector2(WINDOW_WIDTH + i * 370.0f, 257.0f), Vector2(300, 300), i));
	}

	//役職名
	world_->Add(ACTOR_ID::STAGE_ACTOR, std::make_shared<CreditPostText>(world_, CLOTHES_ID::TEXT_CLOTHES, postId_[idCount_], world_->GetKeepDatas().playerLane_, Vector2(WINDOW_WIDTH + 520.0f, -140.0f), postSize_[idCount_], 0));

	idCount_++;
}
