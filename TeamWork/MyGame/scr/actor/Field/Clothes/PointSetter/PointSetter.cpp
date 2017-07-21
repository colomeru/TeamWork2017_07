#include "PointSetter.h"

PointSetter::PointSetter()
{
}

PointSetter::~PointSetter()
{
}

void PointSetter::Load(const std::string& fileName)
{
	csvReader_.load("res/csv/" + fileName + ".csv");
}

void PointSetter::SetLocalPoints(const SPRITE_ID& startSpriteId, float length)
{
	int row = csvReader_.rows();
	int col = csvReader_.columns();
	std::vector<Vector3> points;
	int cutId = 0;

	for (int i = 2; i < row; i += 2) {
		auto id = csvReader_.geti(i, 0);
		for (int j = 1; j < col; j++) {
			Vector3 point = Vector3::Zero;
			point.x = csvReader_.getf(i, j);
			point.y = csvReader_.getf(i + 1, j) + length;
			points.push_back(point);
			if (j % 4 == 0) {
				spritePoints_[(SPRITE_ID)(id + startSpriteId)][(CuttingState)cutId] = points;
				points.clear();
				cutId++;
			}
		}
		cutId = 0;
	}
}

std::map<CuttingState, std::vector<Vector3>> PointSetter::GetLocalPoints(const SPRITE_ID & spriteId) const
{
	return spritePoints_.at(spriteId);
}
