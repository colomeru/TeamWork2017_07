#include "WorkFolder.h"


std::string WorkFolder::Name = "";

void WorkFolder::SetWorkFolder(std::string foldername)
{
	Name = foldername;
}

// ファイル名が登録されているかチェックする(単体格納変数)

// ファイル名が登録されているかチェックする(複数格納変数)
void WorkFolder::CheckName(std::string assetname, std::unordered_map<std::string, std::vector<int>> maps)
{
	if (maps.count(assetname) == 0)
	{
		throw std::string(assetname + "は存在しません\n");
	}
}