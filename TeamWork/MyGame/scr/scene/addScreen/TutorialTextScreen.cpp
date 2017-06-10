#include "TutorialTextScreen.h"

#include"../../world/World.h"
#include"../../graphic/Sprite.h"
#include"../../game/Random.h"
#include"../../Def.h"
#include"../GamePlayDefine.h"
#include"../../input/Keyboard.h"
#include"../../time/Time.h"
#include"../../graphic/FontManager.h"

static const int textSpeed = 3;
static const float defTextChangeTime = 3.0f;

TutorialTextScreen::TutorialTextScreen(World * world) :world_(world), textCount_(0)
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
	for (int i = 0; i < size; i++) {
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
	textChangeTimeCount_ = defTextChangeTime;
}

void TutorialTextScreen::Update()
{
	textCount_++;
	if (textCount_/textSpeed >= GetMaxTextSize()){
		textChangeTimeCount_ -= Time::DeltaTime;

	}
	if (textChangeTimeCount_ <= 0.0f) {
		textChangeTimeCount_ = defTextChangeTime;
		currentTextLine_++;
		
		currentTextLine_ = min(currentTextLine_, drawText_.size() - 1);
	
		textCount_ = 0;
	}

	textCount_ = min(textCount_, GetMaxTextSize()*textSpeed);


}

void TutorialTextScreen::Draw() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (128));
	DrawBox(100, 50, WINDOW_WIDTH-100, 300, GetColor(128, 128, 128), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	std::string drawString_;
	int forcount = floor(textCount_ / textSpeed);

	int alreadyDrawCount = 0;
	for (int i = currentTextLine_*3;i< currentTextLine_*3+3; i++) {
		if(i >= drawText_.size())break;
		for (int x = 0; x < drawText_[i].size(); x++) {
			if (alreadyDrawCount >= forcount)break;
			
			char tex= drawText_[i][x];


			drawString_.push_back(tex);

			alreadyDrawCount++;
		}
	}

	FontManager::GetInstance().DrawTextApplyFont(200, 100, GetColor(255, 255, 255), FONT_ID::TUTORIAL_FONT, drawString_.c_str());
}

void TutorialTextScreen::End()
{
}
