#include "Field.h"

#include "../Define.h"
#include <algorithm>

#include "../Actor/Collision/Collision.h"

// コンストラクタ
Field::Field(const std::string& fileName, int colDef, int rowDef, int stagenum) {
	load(fileName, colDef, rowDef, stagenum);

	stagetexes[0] = TextureID::FLOOR_STAGE4_TEX;
	stagetexes[1] = TextureID::FLOOR_STAGE1_TEX;
	stagetexes[2] = TextureID::FLOOR_STAGE2_TEX;
	stagetexes[3] = TextureID::FLOOR_STAGE3_TEX;
	stagetexes[4] = TextureID::FLOOR_STAGE4_TEX;

	id_ = stagetexes[stagenum];
}

void Field::load(const std::string& fileName, int colDef, int rowDef, int stagenum){
	reader_.load(fileName);

	for (int rowN = rowDef; rowN < (reader_.rows() - std::max<int>(rowDef - 1, 0)); rowN++)	{
		std::vector<int> column;
		for (int colN = colDef; colN < (reader_.columns(rowN) - std::max<int>(colDef - 1, 0)); colN++){
			if (reader_.geti(rowN, colN) == 1 || reader_.geti(rowN, colN) == 20) {
				column.push_back(reader_.geti(rowN, colN));
			}
			else {
				column.push_back(0);
			}
		}
		field_.push_back(column);
	}
}

void Field::update(float deltaTime) {

}

// 描画
void Field::draw(Matrix invert) const {
	if (this == nullptr)return;
	for (int row = 0; row < field_.size(); row++) {
		for (int column = 0; column < field_[0].size(); column++) {
			Vector2 drawPos = Vector2(column, row) * CHIPSIZE * invert;
			if (field_[row][column] == 1) {
				DrawGraph(drawPos.x, drawPos.y, ResourceLoader::GetInstance().getTextureID(id_), TRUE);
			}
			else if (field_[row][column] == 20) {
				DrawGraph(drawPos.x, drawPos.y, ResourceLoader::GetInstance().getTextureID(TextureID::FLOOR_WOOD_TEX), TRUE);
			}
		}
	}
}

unsigned int Field::modelHandle(){
	return 0;
}

FieldEnum Field::calcColl(Vector2& curPos, const Vector2& prePos, Vector2& velocity){
	if (this == nullptr)return FieldEnum::None;
	
	FieldEnum fieldEnum = FieldEnum::None;
	
	calcPreColl(curPos, prePos, fieldEnum, velocity);

	if(fieldEnum == FieldEnum::None)	calcCurColl(curPos, prePos, fieldEnum, velocity);

	return fieldEnum;
}

void Field::calcCurColl(Vector2 & curPos, const Vector2 & prePos, FieldEnum& field, Vector2& velocity){
	int column_p = MathHelper::Clamp(static_cast<int>(curPos.x) / CHIPSIZE, 0,  field_[0].size() - 1);
	int row_p = MathHelper::Clamp(static_cast<int>(curPos.y) / CHIPSIZE, 0, field_.size() - 1);

	int row_t = std::max<int>(row_p - 1, 0);
	int row_b = std::min<int>(row_p + 1, field_.size() - 1);
	int column_l = std::max<int>(column_p - 1, 0);
	int column_r = std::min<int>(column_p + 1, field_[0].size() - 1);

	//上
	if (existCheck(row_t, column_p) && !existCheck(row_p, column_p)) {
		float cellY = (row_t + 1) * CHIPSIZE;
		if (MathHelper::Distance(cellY, curPos.y) < PLAYER_RADIUS) {
			velocity.y = 0;
			curPos.y = cellY + PLAYER_RADIUS;
			field = FieldEnum::Field;
		}
	}
	//下
	else if (existCheck(row_b, column_p) && !existCheck(row_p, column_p)) {
		float cellY = row_b * CHIPSIZE;
		if (MathHelper::Distance(cellY, curPos.y) < PLAYER_RADIUS) {
			velocity.y = 0;
			curPos.y = cellY - PLAYER_RADIUS;
			field = FieldEnum::Field;
		}
	}
	//左
	else if (existCheck(row_p, column_l) && !existCheck(row_p, column_p)) {
		float cellX = (column_l + 1) * CHIPSIZE;
		if (MathHelper::Distance(cellX, curPos.x) < PLAYER_RADIUS) {
			velocity.x = 0;
			curPos.x = cellX + PLAYER_RADIUS;
			field = FieldEnum::Field;
		}
	}
	//右
	else if (existCheck(row_p, column_r) && !existCheck(row_p, column_p)) {
		float cellX = column_r * CHIPSIZE;
		if (MathHelper::Distance(cellX, curPos.x) < PLAYER_RADIUS) {
			velocity.x = 0;
			curPos.x = cellX - PLAYER_RADIUS;
			field = FieldEnum::Field;
		}
	}
	////左上
	//if (existCheck(row_t, column_l)) {
	//	Vector2 bottom_r = Vector2(column_p, row_p) * CHIPSIZE;
	//	if (Vector2::DistanceSquared(bottom_r, curPos) < PLAYER_RADIUS * PLAYER_RADIUS) {
	//		curPos = bottom_r + Vector2::One * PLAYER_RADIUS;
	//	}
	//}
	////右上
	//if (existCheck(row_t, column_r)) {
	//	Vector2 bottom_l = Vector2(column_p + 1, row_p) * CHIPSIZE;
	//	if (Vector2::DistanceSquared(bottom_l, curPos) < PLAYER_RADIUS * PLAYER_RADIUS) {
	//		curPos = bottom_l + Vector2(-1, 1) * PLAYER_RADIUS;
	//	}
	//}
	////左下
	//if (existCheck(row_b, column_l)) {
	//	Vector2 top_r = Vector2(column_p, row_p + 1) * CHIPSIZE;
	//	if (Vector2::DistanceSquared(top_r, curPos) < PLAYER_RADIUS * PLAYER_RADIUS) {
	//		curPos = top_r + Vector2(1, -1) * PLAYER_RADIUS;
	//	}
	//}
	////右下
	//if (existCheck(row_t, column_r)) {
	//	Vector2 top_l = Vector2(column_p + 1, row_p + 1) * CHIPSIZE;
	//	if (Vector2::DistanceSquared(top_l, curPos) < PLAYER_RADIUS * PLAYER_RADIUS) {
	//		curPos = top_l - Vector2::One * PLAYER_RADIUS;
	//	}
	//}
}

void Field::calcPreColl(Vector2 & curPos, const Vector2 & prePos, FieldEnum& field, Vector2& velocity) {
	int column_p = MathHelper::Clamp(static_cast<int>(prePos.x) / CHIPSIZE, 0.0f, static_cast<float>(field_[0].size() - 1));
	int row_p    = MathHelper::Clamp(static_cast<int>(prePos.y) / CHIPSIZE, 0.0f, static_cast<float>(field_.size() - 1));

	int row_t = std::max<int>(row_p - 1, 0);
	int row_b = std::min<int>(row_p + 1, field_.size() - 1);
	int column_l = std::max<int>(column_p - 1, 0);
	int column_r = std::min<int>(column_p + 1, field_[0].size() - 1);

	//if (existCheck(row_p, column_p)) {
	//	curPos = prePos;
	//}

	//上
	if (existCheck(row_t, column_p) && !existCheck(row_p, column_p)) {
		Vector2 bottom_l = Vector2(column_p, row_p) * CHIPSIZE;
		Vector2 bottom_r = Vector2(column_p + 1, row_p) * CHIPSIZE;
		if (Collision::seg_seg(curPos, prePos, bottom_l, bottom_r)) {
			velocity.y = 0;
			curPos.y = bottom_l.y + PLAYER_RADIUS;
			field = FieldEnum::Field;
		}
	}
	//下
	else if (existCheck(row_b, column_p) && !existCheck(row_p, column_p)) {
		Vector2 top_l = Vector2(column_p, row_p + 1) * CHIPSIZE;
		Vector2 top_r = Vector2(column_p + 1, row_p + 1) * CHIPSIZE;
		if (Collision::seg_seg(curPos, prePos, top_l, top_r)) {
			velocity.y = 0;
			curPos.y = top_l.y - PLAYER_RADIUS;
			field = FieldEnum::Field;
		}
	}
	//左
	else if (existCheck(row_p, column_l) && !existCheck(row_p, column_p)) {
		Vector2 right_t = Vector2(column_p, row_p) * CHIPSIZE;
		Vector2 right_b = Vector2(column_p, row_p + 1) * CHIPSIZE;
		if (Collision::seg_seg(curPos, prePos, right_t, right_b)) {
			velocity.x = 0;
			curPos.x = right_t.x + PLAYER_RADIUS;
			field = FieldEnum::Field;
		}
	}
	//右
	else if (existCheck(row_p, column_r) && !existCheck(row_p, column_p)) {
		Vector2 left_t = Vector2(column_p + 1, row_p) * CHIPSIZE;
		Vector2 left_b = Vector2(column_p + 1, row_p + 1) * CHIPSIZE;
		if (Collision::seg_seg(curPos, prePos, left_t, left_b)) {
			velocity.x = 0;
			curPos.x = left_t.x - PLAYER_RADIUS;
			field = FieldEnum::Field;
		}
	}
	//左上
	else if (existCheck(row_t, column_l)) {
		if ((curPos - prePos).y < 0) {
			Vector2 bottom_l = Vector2(column_l, row_p) * CHIPSIZE;
			Vector2 bottom_r = Vector2(column_l + 1, row_p) * CHIPSIZE;
			if (Collision::seg_seg(curPos, prePos, bottom_l, bottom_r)) {
				velocity.y = 0;
				curPos.y = bottom_l.y + PLAYER_RADIUS;
				field = FieldEnum::Field;
			}
		}
		if ((curPos - prePos).x < 0) {
			Vector2 right_t = Vector2(column_p, row_t) * CHIPSIZE;
			Vector2 right_b = Vector2(column_p, row_t + 1) * CHIPSIZE;
			if (Collision::seg_seg(curPos, prePos, right_t, right_b)) {
				velocity.x = 0;
				curPos.x = right_t.x + PLAYER_RADIUS;
				field = FieldEnum::Field;
			}
		}
	}
	//右上
	else if (existCheck(row_t, column_r)) {
		if ((curPos - prePos).y < 0) {
			Vector2 bottom_l = Vector2(column_r, row_p) * CHIPSIZE;
			Vector2 bottom_r = Vector2(column_r + 1, row_p) * CHIPSIZE;
			if (Collision::seg_seg(curPos, prePos, bottom_l, bottom_r)) {
				velocity.y = 0;
				curPos.y = bottom_l.y + PLAYER_RADIUS;
				field = FieldEnum::Field;
			}
		}
		if ((curPos - prePos).x > 0) {
			Vector2 left_t = Vector2(column_p + 1, row_t) * CHIPSIZE;
			Vector2 left_b = Vector2(column_p + 1, row_t + 1) * CHIPSIZE;
			if (Collision::seg_seg(curPos, prePos, left_t, left_b)) {
				velocity.x = 0;
				curPos.x = left_t.x - PLAYER_RADIUS;
				field = FieldEnum::Field;
			}
		}
	}
	//左下
	else if (existCheck(row_b, column_l)) {
		if ((curPos - prePos).y > 0) {
			Vector2 top_l = Vector2(column_l, row_p + 1) * CHIPSIZE;
			Vector2 top_r = Vector2(column_l + 1, row_p + 1) * CHIPSIZE;
			if (Collision::seg_seg(curPos, prePos, top_l, top_r)) {
				velocity.y = 0;
				curPos.y = top_l.y - PLAYER_RADIUS;
				field = FieldEnum::Field;
			}
		}
		if ((curPos - prePos).x < 0) {
			Vector2 right_t = Vector2(static_cast<float>(column_p), static_cast<float>(row_t)) * CHIPSIZE;
			Vector2 right_b = Vector2(static_cast<float>(column_p), static_cast<float>(row_t) + 1.0f) * CHIPSIZE;
			if (Collision::seg_seg(curPos, prePos, right_t, right_b)) {
				velocity.x = 0;
				curPos.x = right_t.x + PLAYER_RADIUS;
				field = FieldEnum::Field;
			}
		}
	}
	//右下
	else if (existCheck(row_t, column_r)) {
		if ((curPos - prePos).y > 0) {
			Vector2 top_l = Vector2(static_cast<float>(column_r), static_cast<float>(row_p + 1)) * CHIPSIZE;
			Vector2 top_r = Vector2(static_cast<float>(column_r) + 1.0f, static_cast<float>(row_p + 1)) * CHIPSIZE;
			if (Collision::seg_seg(curPos, prePos, top_l, top_r)) {
				velocity.y = 0;
				curPos.y = top_l.y - PLAYER_RADIUS;
				field = FieldEnum::Field;
			}
		}
		if ((curPos - prePos).x > 0) {
			Vector2 right_t = Vector2(column_r, row_b) * CHIPSIZE;
			Vector2 right_b = Vector2(column_r, row_b + 1) * CHIPSIZE;
			if (Collision::seg_seg(curPos, prePos, right_t, right_b)) {
				velocity.x = 0;
				curPos.x = right_t.x - PLAYER_RADIUS;
				field = FieldEnum::Field;
			}
		}
	}
}

bool Field::existCheck(int row, int column){
	return field_[row][column] == 1 || field_[row][column] == 20;
}

FieldEnum Field::hitCheck(Vector2& curPos, const Vector2& prePos){
	if (this == nullptr)return FieldEnum::None;;

	int column = static_cast<int>(curPos.x) / CHIPSIZE;
	int row = static_cast<int>(curPos.y) / CHIPSIZE;

	//上
	if (existCheck(std::max<int>(row - 1, 0), column)) {
		float cellY = (std::max<int>(row - 1, 0) + 1) * CHIPSIZE;
		if (MathHelper::Distance(cellY, curPos.y) < PLAYER_COLLIDER_RADIUS) {
			return FieldEnum::Field;
		}
	}
	//下
	if (existCheck(std::min<int>(row + 1, field_.size() - 1), column)) {
		float cellY = std::min<int>(row + 1, field_.size() - 1) * CHIPSIZE;
		if (MathHelper::Distance(cellY, curPos.y) < PLAYER_COLLIDER_RADIUS) {
			return FieldEnum::Field;
		}
	}
	//左
	if (existCheck(row, std::max<int>(column - 1, 0))) {
		float cellX = (std::max<int>(column - 1, 0) + 1) * CHIPSIZE;
		if (MathHelper::Distance(cellX, curPos.x) < PLAYER_COLLIDER_RADIUS) {
			return FieldEnum::Field;
		}
	}
	//右
	if (existCheck(row, std::min<int>(column + 1, field_[0].size() - 1))) {
		float cellX = (std::min<int>(column + 1, field_[0].size() - 1)) * CHIPSIZE;
		if (MathHelper::Distance(cellX, curPos.x) < PLAYER_COLLIDER_RADIUS) {
			return FieldEnum::Field;
		}
	}


	return FieldEnum::None;

}

void Field::FieldHitCheck(){
}
