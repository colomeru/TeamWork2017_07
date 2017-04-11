#include "PlayerTxt.h"
#include "../Game/Time.h"
#include "../Input/InputMgr.h"
PlayerTxt::PlayerTxt(char * fileName, int lineNum)
{
	TxtRead::GetInstance().ReadTxt(TXT_FILE_ID::STAGE01_TXT, fileName);
	prePlayer_ = PlayerEnum::BUTTY;
	//改行数回す
	for (int i = 0; i < lineNum; i++) {
		std::string txt = TxtRead::GetInstance().GetTex(TXT_FILE_ID::STAGE01_TXT);
		TxtState state;
		state.lineFlag = false;
		if (txt[0] == 'b') {
			state.speekPlayer = PlayerEnum::BUTTY;
			prePlayer_ = PlayerEnum::BUTTY;
			//判別用の文字を削除
			txt.erase(txt.begin());
			state.txt = txt;
		}
		else if (txt[0] == 'r')
		{
			state.speekPlayer = PlayerEnum::RETTY;
			prePlayer_ = PlayerEnum::RETTY;
			//判別用の文字を削除
			txt.erase(txt.begin());
			state.txt = txt;
		}
		else if (txt[0] == 'c') {
			state.speekPlayer = PlayerEnum::COCOCO;
			prePlayer_ = PlayerEnum::COCOCO;
			//判別用の文字を削除
			txt.erase(txt.begin());
			state.txt = txt;
		}
		else {
			state.lineFlag = true;
			state.txt = txt;
			//直前に話した奴を入れる
			state.speekPlayer = prePlayer_;
		}
		txts_.push_back(state);
	}
	//ファイルを閉じる
	TxtRead::GetInstance().CloseTxt(TXT_FILE_ID::STAGE01_TXT);
	speedEndCount_ = lineNum;
	speekCount_ = 0;
	speekTimer_ = 0.1f;
	isSpeekFlag_ = true;
	isSpeekEnd_ = false;
	isAllSpeekEnd_ = false;
	characterCount_ = 0;
	characterCountLine_ = 0;
	nextSpeekTimer_ = 0.0f;
	nowPlayer_ = txts_.begin()->speekPlayer;
}

PlayerTxt::~PlayerTxt()
{
}

void PlayerTxt::Update()
{
	if (isAllSpeekEnd_) return;
	if (isSpeekEnd_) {
		nextSpeekTimer_ += Time::GetInstance().deltaTime();
		if (nextSpeekTimer_ >= 5.0f) {
			//全てのテキストが終わったら
			drawLineTxt_.clear();
			drawTxt_.clear();
			if (speekCount_ >= speedEndCount_ - 1) {
				isAllSpeekEnd_ = true;
				return;
			}
			if (txts_[speekCount_ + 1].lineFlag) speekCount_++;
			if (speekCount_ >= speedEndCount_ - 1) {
				isAllSpeekEnd_ = true;
				return;
			}
			speekCount_++;
			nowPlayer_ = txts_[speekCount_].speekPlayer;
			speekTimer_ = 0.0f;
			characterCount_ = 0;
			characterCountLine_ = 0;
			isSpeekEnd_ = false;
			nextSpeekTimer_ = 0.0f;
		}
	}
	else {
		speekTimer_ += Time::GetInstance().deltaTime();
		if (speekTimer_ >= 0.02f&&
			txts_[speekCount_].txt.size() > characterCount_)
		{
			drawTxt_.push_back(txts_[speekCount_].txt[characterCount_]);
			characterCount_++;
			//表示し終わったかどうか
			if (txts_[speekCount_].txt.size() <= characterCount_) {
				if (speekCount_ >= speedEndCount_ - 1) {
					isSpeekEnd_ = true;
					return;
				}
				if (!txts_[speekCount_ + 1].lineFlag)
					isSpeekEnd_ = true;
			}
			speekTimer_ = 0.0f;

		}
		else if (speekTimer_ >= 0.02f&&
			txts_[speekCount_].txt.size() <= characterCount_&&
			txts_[speekCount_ + 1].txt.size() >= characterCountLine_&&
			txts_[speekCount_ + 1].lineFlag)
		{
			if (speekCount_ >= speedEndCount_-1) {
				isSpeekEnd_ = true;
				return;
			}
			int size = txts_[speekCount_ + 1].txt.size();
			drawLineTxt_.push_back(txts_[speekCount_ + 1].txt[characterCountLine_]);
			characterCountLine_++;
			speekTimer_ = 0;

			//表示終わったかどうか
			if (txts_[speekCount_ + 1].txt.size() <= characterCountLine_) isSpeekEnd_ = true;
		}
	}

}

void PlayerTxt::Draw(const Vector2& pos, float size)
{
	if (isAllSpeekEnd_) return;

	SetFontSize(size);
	if (txts_[speekCount_].speekPlayer == PlayerEnum::BUTTY) {
		DrawFormatString(pos.x, pos.y, GetColor(0, 0, 255), drawTxt_.c_str());
		DrawFormatString(pos.x, pos.y+size, GetColor(0, 0, 255), drawLineTxt_.c_str());
	}
	else if (txts_[speekCount_].speekPlayer == PlayerEnum::RETTY) {
		DrawFormatString(pos.x, pos.y, GetColor(255, 0, 0), drawTxt_.c_str());
		DrawFormatString(pos.x, pos.y + size, GetColor(255, 0, 0), drawLineTxt_.c_str());
	}
	else if (txts_[speekCount_].speekPlayer == PlayerEnum::COCOCO) {
		DrawFormatString(pos.x, pos.y, GetColor(255, 0, 0), drawTxt_.c_str());
		DrawFormatString(pos.x, pos.y + size, GetColor(255, 0, 255), drawLineTxt_.c_str());
	}
}

bool PlayerTxt::isEndEvent()
{
	return isAllSpeekEnd_;
}

PlayerEnum PlayerTxt::GetSpeekPlayer()
{
	return nowPlayer_;
}
