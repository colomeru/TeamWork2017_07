#include "TutorialTextScreen.h"

#include"../../world/World.h"
#include"../../graphic/Sprite.h"
#include"../../game/Random.h"
#include"../../Def.h"
#include"../GamePlayDefine.h"
#include"../../input/Keyboard.h"
#include"../../input/GamePad.h"
#include"../../time/Time.h"
#include"../../graphic/FontManager.h"

static const int textSpeed = 3;
static const float defTextChangeTime = 3.0f;

TutorialTextScreen::TutorialTextScreen(World * world) :world_(world), textCount_(0), sinCount_(0)
{

}

void TutorialTextScreen::Init(const std::string& filename)
{
	drawText_.clear();
	std::ifstream readText_("res/Text/"+filename);
	readText_.seekg(0, std::ios_base::end);
	std::size_t size = (std::size_t)readText_.tellg();
	readText_.seekg(0, std::ios_base::beg);
	std::vector<char> charbuf(size);
	readText_.read(&charbuf[0], size);
	
	drawText_.push_back(std::vector<char>());
	int drawtextline = 0;
	for (int i = 0; i < (int)size; i++) {
		char chartex = charbuf[i];

		if (chartex == '\n') {
			drawText_.push_back(std::vector<char>());
			drawText_[drawtextline].push_back(chartex);
			drawtextline++;
			continue;
		}
		drawText_[drawtextline].push_back(chartex);
	}
	maxTextSize_ = size;
	textCount_ = 0;
	currentTextLine_ = 0;
	sinCount_ = 0;
	textChangeTimeCount_ = defTextChangeTime;
}

//void TutorialTextScreen::Update()
//{
//	sinCount_+=5; sinCount_ %= 360;
//
//	textCount_++;
//	if (textCount_/textSpeed >= GetMaxTextSize()){
//		textChangeTimeCount_ -= Time::DeltaTime;
//
//	}
//	if (textChangeTimeCount_ <= 0.0f) {
//		textChangeTimeCount_ = defTextChangeTime;
//		currentTextLine_++;
//		
//		//currentTextLine_ = min(currentTextLine_, drawText_.size() - 1);
//	
//		if (currentTextLine_>=drawText_.size()/3) {
//			currentTextLine_ = 0;
//		}
//
//		textCount_ = 0;
//	}
//
//	textCount_ = min(textCount_, GetMaxTextSize()*textSpeed);
//
//}

bool TutorialTextScreen::TutorialUpdate()
{
	sinCount_ += 5; sinCount_ %= 360;

	textCount_++;
	if (textCount_ / textSpeed >= GetMaxTextSize()) {
		textChangeTimeCount_ -= Time::DeltaTime;

	}
	//if (textChangeTimeCount_ <= 0.0f) {
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M)||GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2)) {
		if (textCount_ < GetMaxTextSize()*textSpeed) {
			textCount_ = GetMaxTextSize()*textSpeed;
			return false;
		}

		textChangeTimeCount_ = defTextChangeTime;
		currentTextLine_++;


		if (currentTextLine_ >= (int)drawText_.size() / 3) {
			currentTextLine_ = drawText_.size() / 3 - 1;
			//currentTextLine_ = 0;
			//textCount_ = 0;
			return true;
		}

		textCount_ = 0;
	}

	textCount_ = min(textCount_, GetMaxTextSize()*textSpeed);

	return false;
}

void TutorialTextScreen::Draw() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (128));
	DrawBox(100, 100, WINDOW_WIDTH-100, 350, GetColor(128, 128, 128), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	std::string drawString_;
	int forcount = (int)floor(textCount_ / textSpeed);

	int alreadyDrawCount = 0;
	for (int i = currentTextLine_*3;i< currentTextLine_*3+3; i++) {
		if(i >= (int)drawText_.size())break;
		for (int x = 0; x < (int)drawText_[i].size(); x++) {
			if (alreadyDrawCount >= forcount)break;
			
			char tex= drawText_[i][x];


			drawString_.push_back(tex);

			alreadyDrawCount++;
		}
	}

	FontManager::GetInstance().DrawTextApplyFont(200, 150, GetColor(255, 255, 255), FONT_ID::TUTORIAL_FONT, drawString_.c_str());

	 
	 
	 //Sprite::GetInstance().Draw(SPRITE_ID::TO_STAGESELECT_SPRITE, Vector2(1500.f, 200.f), MathHelper::Sin((float)sinCount_));
	
}

void TutorialTextScreen::End()
{
}
