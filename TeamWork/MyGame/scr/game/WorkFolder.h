#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class WorkFolder
{
public:
	///<summary>
	/// ファイルが格納されているフォルダー名を設定する
	///</summary>
	static void SetWorkFolder(std::string foldername);

	///<summary>
	/// ファイル名が登録されているかチェックする(単体格納変数)
	///</summary>
	template <typename T>
	static void CheckName(std::string assetname,  const std::unordered_map<std::string, T>& maps)
	{
		if (maps.count(assetname) == 0)
		{
			throw std::string(assetname + "は存在しません\n");
		}
	}

	///<summary>
	/// ファイル名が登録されているかチェックする(複数格納変数)
	///</summary>
	static void CheckName(std::string assetname, std::unordered_map<std::string, std::vector<int>> maps);

	static std::string Name;
};