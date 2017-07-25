#include "TGSManager.h"
#include"../input/InputChecker.h"
#include"../time/Time.h"

//#include"TGSMode.h"//TGS提出版でない場合はコメントアウトする事

TGSManager::TGSManager():
	timeCount_(0.0f), isActive_(true){
}

TGSManager & TGSManager::GetInstance() {
	static TGSManager tgsManager;
	return tgsManager;
}


#ifdef TGSMODE_ACTIVE//TGS提出に対応する場合
void TGSManager::Initialize()
{
	ResetTime();
	isActive_ = true;
}

bool TGSManager::Update() {
	AddTime();
	CheckIsReset();

	return CheckExit_Input()||CheckExit_Timer();
}

bool TGSManager::CheckExit_Input() const{
	return InputChecker::GetInstance().KeyStateDown(InputChecker::Input_Key::Start)
		&& InputChecker::GetInstance().KeyStateDown(InputChecker::Input_Key::Back);
}

bool TGSManager::CheckExit_Timer() const{
	return timeCount_ >= EXIT_TIME_;
}

void TGSManager::GameEnd() {
	DxLib_End();
	exit(0);
}

void TGSManager::AddTime() {
	if (!isActive_)return;
	timeCount_ += Time::DeltaTime;
}

void TGSManager::CheckIsReset()
{
	if (InputChecker::GetInstance().AnyInputStateDown()) {
		ResetTime();
	}
}

void TGSManager::ResetTime()
{
	timeCount_ = 0.0f;
}

void TGSManager::SetActive(bool isActive) {
	isActive_ = isActive;
}

#else//TGS提出に対応しない場合

void TGSManager::Initialize()
{
}

bool TGSManager::Update() {
	return false;
}

bool TGSManager::CheckExit_Input() const {
	return false;
}

bool TGSManager::CheckExit_Timer() const {
	return false;
}

void TGSManager::GameEnd() {
}

void TGSManager::AddTime() {
}

void TGSManager::CheckIsReset(){
}

void TGSManager::ResetTime(){
}

void TGSManager::SetActive(bool isActive) {
}

#endif