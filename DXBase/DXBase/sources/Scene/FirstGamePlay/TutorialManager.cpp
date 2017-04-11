#include "TutorialManager.h"

#include "../../World/World.h"

#include "../../FileReader/CsvReader.h"

#include "../../ResourceLoader/ResourceLoader.h"
#include "../../Input/InputMgr.h"
#include "../../Define.h"

#include "../../Math/Easing.h"

#include <algorithm>

TutorialManager::TutorialManager() :
	timer_(0),
	textIndex_(0),
	currentTutorial_(1),
	currentTurn_(0),
	drawY_(0),
	color_(GetColor(255, 255, 255)),
	text1_(""),
	text2_(""),
	scale_(0),
	state_(TutoState::POP_UP),
	sound_(0){
}

TutorialManager::~TutorialManager(){}

void TutorialManager::load_csv(const std::string& file_name){
	contents_.clear();

	CsvReader reader;
	reader.load(file_name);

	for (int rowN = 0; rowN < reader.rows() - 1; rowN++){
		Elements elements;
		elements.posX = reader.geti(rowN, POSITION);
		elements.speaker = reader.get(rowN, SPEEKER);
		elements.text1 = reader.get(rowN, TEXT_FIRST);
		if (reader.columns(rowN) - 1 >= TEXT_SECOND) {
			elements.text2 = reader.get(rowN, TEXT_SECOND);
		}
		contents_[reader.geti(rowN, NUMBER)].push_back(elements);
	}
}

void TutorialManager::init(){
	timer_ = 0;
	textIndex_ = 0;
	currentTutorial_ = 1;
	currentTurn_ = 0;
	drawY_ = 100;
	color_ = GetColor(0, 0, 255);
	sound_ = ResourceLoader::GetInstance().getSoundID(SoundID::SE_BUDDY);
	text1_ = "";
	text2_ = "";
	scale_ = 0;
	state_ = TutoState::POP_UP;
}

void TutorialManager::update(IWorld* world, float deltaTime){

	tutoStateUpdate(world, deltaTime);

	auto butty = world->findActor("PlayerBody1");
	auto retty = world->findActor("PlayerBody2");

	if (butty == nullptr || retty == nullptr)return;

	if (contents_[currentTutorial_][currentTurn_].speaker == "b") {
		drawPos_ = Vector2::Clamp(butty->getPosition() * world->GetInv() - Vector2(-100, 300), Vector2(600, 200), Vector2(1300, 800));
	}
	else if (contents_[currentTutorial_][currentTurn_].speaker == "r") {
		drawPos_ = Vector2::Clamp(retty->getPosition() * world->GetInv() - Vector2(-100, 300), Vector2(600, 200), Vector2(1300, 800));
	}
}

void TutorialManager::tutoStateUpdate(IWorld * world, float deltaTime) {
	switch (state_) {
	case TutoState::POP_UP:popUp(world, deltaTime);
		break;
	case TutoState::TEXT:stepText(deltaTime);
		break;
	case TutoState::POP_BACK:popBack(world, deltaTime);
		break;
	case TutoState::CHANGE:change(world, deltaTime);
		break;
	case TutoState::MOVE:move(world, deltaTime);
		break;
	default:
		break;
	}
}

void TutorialManager::popUp(IWorld * world, float deltaTime){
	scale_ = MathHelper::Lerp(scale_, 2.0f, 0.7f);
	if (scale_ >= 2.0f) {
		PlaySoundMem(ResourceLoader::GetInstance().getSoundID(SoundID::SE_KAIWA), DX_PLAYTYPE_BACK);

		state_ = TutoState::TEXT;
		timer_ = 0;
	}
}

void TutorialManager::popBack(IWorld * world, float deltaTime){
	scale_ = MathHelper::Lerp(scale_, 0.0f, 0.7f);
	if (scale_ >= 0.0f) {
		state_ = TutoState::CHANGE;
		timer_ = 0;
	}
}

void TutorialManager::change(IWorld * world, float deltaTime) {
	if (currentTurn_ >= contents_[currentTutorial_].size() - 1) {
		world->PlayerNotMove(false);
		state_ = TutoState::MOVE;
	}
	else {
		changeTurn(world);
	}
}

void TutorialManager::move(IWorld * world, float deltaTime){
	auto butty = world->findActor("PlayerBody1");
	auto retty = world->findActor("PlayerBody2");

	if (butty == nullptr || retty == nullptr)return;

	if (butty->getPosition().x > contents_[std::min<int>(currentTutorial_ + 1, contents_.size())][0].posX * CHIPSIZE &&
		retty->getPosition().x > contents_[std::min<int>(currentTutorial_ + 1, contents_.size())][0].posX * CHIPSIZE) {

		if (currentTutorial_ != contents_.size()) {
			changeTutorial(world);
			world->PlayerNotMove(true);
		}
	}
}

void TutorialManager::draw() const{

	if (state_ == TutoState::MOVE)return;
	DrawRotaGraph(drawPos_.x, drawPos_.y, scale_, 0.0f, ResourceLoader::GetInstance().getTextureID(TextureID::HUKIDASI_TEX), true);

	int size = 64;
	SetFontSize(size);
	DrawFormatString(drawPos_.x - 425, drawPos_.y - drawY_, color_, text1_.c_str());
	DrawFormatString(drawPos_.x - 425, drawPos_.y - drawY_ + size, color_, text2_.c_str());
}

void TutorialManager::changeTutorial(IWorld* world){
	currentTurn_ = 0;
	textIndex_ = 0;
	text1_ = "";
	text2_ = "";
	currentTutorial_ = std::min<int>(currentTutorial_ + 1, contents_.size());

	state_ = TutoState::POP_UP;

	if (contents_[currentTutorial_][currentTurn_].text2.size() <= 0) {
		drawY_ = 55;
	}
	else {
		drawY_ = 105;
	}
	auto butty = world->findActor("PlayerBody1");
	auto retty = world->findActor("PlayerBody2");

	if (butty == nullptr || retty == nullptr)return;

	if (contents_[currentTutorial_][currentTurn_].speaker == "b") {
		color_ = GetColor(0, 0, 255);
		sound_ = ResourceLoader::GetInstance().getSoundID(SoundID::SE_BUDDY);
	}
	else if (contents_[currentTutorial_][currentTurn_].speaker == "r") {
		color_ = GetColor(255, 0, 0);
		sound_ = ResourceLoader::GetInstance().getSoundID(SoundID::SE_REDDY);
	}
}

void TutorialManager::changeTurn(IWorld* world){
	text1_ = "";
	text2_ = "";
	textIndex_ = 0;
	currentTurn_ = std::min<int>(currentTurn_ + 1, contents_[currentTutorial_].size() - 1);

	state_ = TutoState::POP_UP;

	if (contents_[currentTutorial_][currentTurn_].text2.size() <= 0) {
		drawY_ = 60;
	}
	else {
		drawY_ = 110;
	}
	
	auto butty = world->findActor("PlayerBody1");
	auto retty = world->findActor("PlayerBody2");

	if (butty == nullptr || retty == nullptr)return;

	if (contents_[currentTutorial_][currentTurn_].speaker == "b") {
		color_ = GetColor(0, 0, 255);
		sound_ = ResourceLoader::GetInstance().getSoundID(SoundID::SE_BUDDY);
	}
	else if (contents_[currentTutorial_][currentTurn_].speaker == "r") {
		color_ = GetColor(255, 0, 0);
		sound_ = ResourceLoader::GetInstance().getSoundID(SoundID::SE_REDDY);
	}
}

bool TutorialManager::isTutorialEnd(){
	bool tuto = contents_.size() == currentTutorial_;
	bool turn = contents_[currentTutorial_].size() - 1 == currentTurn_;

	return tuto && turn && timer_ >= 5.0f;
}

void TutorialManager::stepText(float deltaTime){
	timer_ += deltaTime * 10;
	
	if (contents_[currentTutorial_][currentTurn_].text1.size() <= text1_.size() &&
		contents_[currentTutorial_][currentTurn_].text2.size() <= text2_.size()) {
		if (timer_ >= 10.0f) {
			state_ = TutoState::POP_BACK;
			timer_ = 0.0f;
		}
	}
	else {
		if (contents_[currentTutorial_][currentTurn_].text1.size() > text1_.size()) {
			if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_CIRCLE)) {
				textIndex_ = contents_[currentTutorial_][currentTurn_].text1.size() / 2;
			}
		}
		else {
			if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_CIRCLE)) {
				textIndex_ = contents_[currentTutorial_][currentTurn_].text2.size() / 2;
			}
		}

		if (timer_ > 1.0f) {
			if (contents_[currentTutorial_][currentTurn_].text1.size() > text1_.size()) {
				if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_CIRCLE)) {
					textIndex_ = contents_[currentTutorial_][currentTurn_].text1.size() / 2;
				}

				text1_ = contents_[currentTutorial_][currentTurn_].text1.substr(0, textIndex_ * 2);
				PlaySoundMem(sound_, DX_PLAYTYPE_BACK);
			}
			else {
				if (text2_.size() == 0) {
					textIndex_ = 1;
				}
				if (InputMgr::GetInstance().IsButtonDown(Buttons::BUTTON_CIRCLE)) {
					textIndex_ = contents_[currentTutorial_][currentTurn_].text2.size() / 2;
				}
				text2_ = contents_[currentTutorial_][currentTurn_].text2.substr(0, textIndex_ * 2);
				PlaySoundMem(sound_, DX_PLAYTYPE_BACK);
			}
			textIndex_++;
			timer_ = 0.0f;
		}
	}
}


