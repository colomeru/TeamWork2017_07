#include "HurryUpUI.h"

#include "../../../ResourceLoader/ResourceLoader.h"
#include "../../../Input/InputMgr.h"
#include "../../../Define.h"
#include "../../../World/IWorld.h"

HurryUpUI::HurryUpUI(IWorld * world) :
	ActorUI(world, Vector2::Zero),
	timer_(0.0f){
	NumIDs[0] = TextureID::NUMBER_ZERO_TEX;
	NumIDs[1] = TextureID::NUMBER_ONE_TEX;
	NumIDs[2] = TextureID::NUMBER_TWO_TEX;
	NumIDs[3] = TextureID::NUMBER_THREE_TEX;
	NumIDs[4] = TextureID::NUMBER_FOUR_TEX;
	NumIDs[5] = TextureID::NUMBER_FIVE_TEX;

	button_R1_ = TextureID::BUTTON_R1_UP_TEX;
	button_L1_ = TextureID::BUTTON_L1_UP_TEX;
}

HurryUpUI::~HurryUpUI(){}

void HurryUpUI::onUpdate(float deltaTime){
	timer_ += deltaTime;

	if (InputMgr::GetInstance().IsKeyOn(KeyCode::L_SHIFT)) {
		button_L1_ = TextureID::BUTTON_L1_PUSH_TEX;
	}
	else {
		button_L1_ = TextureID::BUTTON_L1_UP_TEX;
	}
	if (InputMgr::GetInstance().IsKeyOn(KeyCode::R_SHIFT)) {
		button_R1_ = TextureID::BUTTON_R1_PUSH_TEX;
	}
	else {
		button_R1_ = TextureID::BUTTON_R1_UP_TEX;
	}

	if (InputMgr::GetInstance().IsButtonOn(Buttons::BUTTON_L1)) {
		button_L1_ = TextureID::BUTTON_L1_PUSH_TEX;
	}
	else {
		button_L1_ = TextureID::BUTTON_L1_UP_TEX;
	}
	if (InputMgr::GetInstance().IsButtonOn(Buttons::BUTTON_R1)) {
		button_R1_ = TextureID::BUTTON_R1_PUSH_TEX;
	}
	else {
		button_R1_ = TextureID::BUTTON_R1_UP_TEX;
	}

	auto cntr = world_->findActor(std::string("PlayerConnector"));
	if (cntr != nullptr && timer_ != 0.0f) {
		timer_ = 0.0f;
	}
}

void HurryUpUI::onDraw() const{
	auto cntr = world_->findActor(std::string("PlayerConnector"));
	if (cntr != nullptr)return;

	int graphNum = (int)ceil(PLAYER_DEAD_LIMIT - timer_);
	graphNum = min((int)PLAYER_DEAD_LIMIT, max(0, graphNum));

	Vector2 screen_center = SCREEN_SIZE * 0.5f;

	DrawRotaGraph(static_cast<int>(screen_center.x), static_cast<int>(screen_center.y), (((MathHelper::Sin(timer_ * 60.0f * 5.5f) + 1.0f) * 0.5f) + 1.0f) * 2.0f, 0.0f, ResourceLoader::GetInstance().getTextureID(NumIDs.at(graphNum)), TRUE);
	DrawRotaGraph(static_cast<int>(screen_center.x), static_cast<int>(screen_center.y) - 300, (((MathHelper::Sin(timer_ * 60.0f * 5.5f) + 1.0f) * 0.5f) + 1.0f), 0.0f, ResourceLoader::GetInstance().getTextureID(TextureID::HURRY_TEX), TRUE);
	DrawRotaGraph(static_cast<int>(screen_center.x) + 500, static_cast<int>(screen_center.y), 1.5f, 0.0f, ResourceLoader::GetInstance().getTextureID(button_R1_), TRUE);
	DrawRotaGraph(static_cast<int>(screen_center.x) - 500, static_cast<int>(screen_center.y), 1.5f, 0.0f, ResourceLoader::GetInstance().getTextureID(button_L1_), TRUE);
}
