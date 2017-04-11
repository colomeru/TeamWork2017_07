#ifndef FIELD_H_
#define FIELD_H_

#include "DxLib.h"
#include "../Math/Math.h"

#include "../ResourceLoader/ResourceLoader.h"
#include"../FileReader/CsvReader.h"

#include <map>

enum class FieldEnum {
	None,
	Field,
};

class Field {
public:
	// コンストラクタ
	Field(const std::string& fileName, int colDef = 0, int rowDef = 0, int stagenum = 0);
	void load(const std::string& fileName, int colDef = 0, int rowDef = 0, int stagenum = 0);
	//更新
	void update(float deltaTime);
	// 描画
	void draw(Matrix invert = Matrix::Identity) const;

	unsigned int modelHandle();

	FieldEnum calcColl(Vector2& curPos, const Vector2& prePos, Vector2& velocity = Vector2(0,0));
	
	void calcCurColl(Vector2& curPos, const Vector2& prePos, FieldEnum& field, Vector2& velocity);
	void calcPreColl(Vector2& curPos, const Vector2& prePos, FieldEnum& field, Vector2& velocity);

	bool existCheck(int row, int column);

	FieldEnum hitCheck(Vector2& curPos, const Vector2& prePos = Vector2::Zero);

	void FieldHitCheck();

	// コピー禁止
	Field(const Field& other) = delete;
	Field& operator = (const Field& other) = delete;
private:
	//CSVのデータを引き出すためのクラス
	CsvReader reader_;
	std::map<int, TextureID> stagetexes;

	TextureID id_;

	typedef std::vector<std::vector<int>> FieldData;
	FieldData field_;
};

#endif
