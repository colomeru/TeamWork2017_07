#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include "../ResourceLoader/ResourceID.h"
#include <DxLib.h>
#include <string>
class TxtRead {
public:
	static TxtRead& GetInstance();

	~TxtRead();
	void ReadTxt(TXT_FILE_ID id,char* pas);
	//àÍçsñ⁄Çï‘Ç∑
	std::string GetTex(TXT_FILE_ID id);
	void CloseTxt(TXT_FILE_ID id);
private:
	std::map<TXT_FILE_ID, int>txtId_;
};