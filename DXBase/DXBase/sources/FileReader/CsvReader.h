#pragma once

#include <vector>
#include <string>

// CSVリーダークラス
class CsvReader {
public:
	// コンストラクタ
	CsvReader();
	// コンストラクタ
	CsvReader(const std::string& fileName);
	// ファイルの読み込み
	void load(const std::string& fileName);
	// データの取得
	const std::string& get(int row, int column) const;
	// データの取得
	int geti(int row, int column) const;
	// データの取得
	float getf(int row, int column) const;
	// 行数を返す
	int rows() const;
	// 列数を返す
	int columns(int row = 0) const;

private:
	using Row = std::vector<std::string>;
	using Rows = std::vector<Row>;
	Rows rows_;
};


