#include "TxtRead.h"

TxtRead & TxtRead::GetInstance()
{
	static TxtRead instance;
	return instance;
}

TxtRead::~TxtRead()
{
}

void TxtRead::ReadTxt(TXT_FILE_ID id, char* pas)
{
	txtId_[id] = FileRead_open(pas);
}

std::string TxtRead::GetTex(TXT_FILE_ID id)
{
	char txt[256];
	std::string txtString;
	FileRead_gets(txt, 256, txtId_[id]);
	txtString = txt;
	return txtString;
}

void TxtRead::CloseTxt(TXT_FILE_ID id)
{
	FileRead_close(txtId_[id]);
}
