#include "CollisionFunction.h"
#include "../actor/Actor.h"
#include "../collision/MyCol.h"

CollisionFunction::CollisionFunction(IWorld * world) :
world_(world)
{
}

CollisionParameter CollisionFunction::IsHit_OBB_OBB(const Actor & sprite1, const Actor & sprite2)
{
	OBB obb1;
	OBB obb2;
	Vector2 sp1Translation = sprite1.GetPosition();
	Vector2 sp2Translation = sprite2.GetPosition();

	MyCol::CreateOBB(&obb1, sp1Translation, Matrix::CreateRotationZ(sprite1.GetAngle()), Vector2(sprite1.GetParameter().size.x / 2.0f, sprite1.GetParameter().size.y / 2.0f));
	MyCol::CreateOBB(&obb2, sp2Translation, Matrix::CreateRotationZ(sprite2.GetAngle()), Vector2(sprite2.GetParameter().size.x / 2.0f, sprite2.GetParameter().size.y / 2.0f));

	return CollisionParameter(COL_ID::BOX_BOX_COL, MyCol::TestOBBOBB(obb1, obb2), sprite1.GetPosition());
}

CollisionParameter CollisionFunction::IsHit_OBB_Segment(const Actor & sprite1, const Actor & sprite2)
{
	OBB obb;
	Segment seg;
	Vector2 sp1Translation = sprite1.GetPosition();
	Vector2 sp2Translation = sprite2.GetPosition();

	MyCol::CreateOBB(&obb, sprite1.GetPosition(), Matrix::CreateRotationZ(sprite1.GetAngle()), sprite1.GetParameter().size);
	MyCol::CreateSegment(&seg, sprite2.GetPosition(), Matrix::CreateRotationZ(sprite2.GetAngle()), sprite2.GetParameter().size.x);
	std::vector<Vector2> points = MyCol::GetOBBPoints(sprite1);
	Vector2 segPoint;

	int intSet[][2] = { { 0,1 },{ 0,2 },{ 1,3 },{ 2,3 } };

	for (int i = 0; i < 4; i++) {
		DrawLine(points[intSet[i][0]].x, points[intSet[i][0]].y, points[intSet[i][1]].x, points[intSet[i][1]].y, GetColor(255, 0, 0));
	}
	DrawLine(seg.startPoint.x, seg.startPoint.y, seg.endPoint.x, seg.endPoint.y, GetColor(255, 255, 255));

	bool isHitCheck = false;
	if (MyCol::Col_OBB_Segment(obb, seg, points, segPoint)) {
		isHitCheck = true;
	}

	return CollisionParameter(COL_ID::BOX_SEGMENT_COL, isHitCheck, segPoint);
}

CollisionParameter CollisionFunction::IsHit_Segment_Segment(const Actor & sprite1, const Actor & sprite2)
{
	Vector2 intersectPoint = Vector2();
	Segment seg1;
	Segment seg2;
	Vector2 sp1Translation = sprite1.GetPosition();
	Vector2 sp2Translation = sprite2.GetPosition();

	MyCol::CreateSegment(&seg1, sp1Translation, Matrix::CreateRotationZ(sprite1.GetAngle()), sprite1.GetParameter().size.x);
	MyCol::CreateSegment(&seg2, sp2Translation, Matrix::CreateRotationZ(sprite2.GetAngle()), sprite2.GetParameter().size.x);

	Vector2 segPoint;
	bool colPF = false;
	if (MyCol::Col_Segment_Segment(seg1, seg2, segPoint)) {
		intersectPoint = segPoint;
		colPF = true;
	}

	return CollisionParameter(COL_ID::BEGIN_COL, colPF, intersectPoint);
}

CollisionParameter CollisionFunction::IsHit_Circle_Circle(const Actor & sprite1, const Actor & sprite2)
{
	Circle crc1;
	Circle crc2;
	Vector2 sp1Translation = sprite1.GetPosition();
	Vector2 sp2Translation = sprite2.GetPosition();

	MyCol::CreateCircle(&crc1, sp1Translation, sprite1.GetParameter().radius);
	MyCol::CreateCircle(&crc2, sp2Translation, sprite2.GetParameter().radius);

	Vector2 segPoint;
	bool colPF = false;
	if (MyCol::Col_Circle_Circle(crc1, crc2, segPoint)) {
		colPF = true;
	}

	return CollisionParameter(COL_ID::BEGIN_COL, colPF, segPoint);
}

CollisionParameter CollisionFunction::IsHit_Circle_Segment(const Actor & sprite1, const Actor & sprite2)
{
	Circle crc;
	Segment seg;
	Vector2 sp1Translation = sprite1.GetPosition();
	Vector2 sp2Translation = sprite2.GetPosition();

	MyCol::CreateCircle(&crc, sp1Translation, sprite1.GetParameter().radius);
	MyCol::CreateSegment(&seg, sp2Translation, Matrix::CreateRotationZ(sprite2.GetAngle()), sprite2.GetParameter().size.x);

	Vector2 segPoint;

	return CollisionParameter(COL_ID::BEGIN_COL, MyCol::Col_Circle_Segment(crc, seg, segPoint), segPoint);
}

CollisionParameter CollisionFunction::IsHit_OBB_Circle(const Actor & sprite1, const Actor & sprite2)
{
	OBB obb;
	Circle crc;
	Vector2 sp1Translation = sprite1.GetPosition();
	Vector2 sp2Translation = sprite2.GetPosition();

	MyCol::CreateOBB(&obb, sp1Translation, Matrix::CreateRotationZ(sprite1.GetAngle()), sprite1.GetParameter().size);
	MyCol::CreateCircle(&crc, sp2Translation, sprite2.GetParameter().radius);

	Vector2 segPoint;
	std::vector<Vector2> points = MyCol::GetOBBPoints(sprite1);

	return CollisionParameter(COL_ID::BEGIN_COL, MyCol::Col_OBB_Circle(obb, crc, points, segPoint), segPoint);
}

CollisionParameter CollisionFunction::IsHit_OBB_Clothes(const Actor & sprite1, const Actor & sprite2)
{
	if (sprite2.GetParameter().ClothSegmentPoints_.size() == 0) {
		return CollisionParameter(COL_ID::BOX_SEGMENT_COL, false);
	}
	OBB obb;
	std::vector<Segment> segs;
	segs.resize(sprite2.GetParameter().ClothSegmentPoints_.size());
	Vector2 sp1Translation = sprite1.GetPosition();
	Vector2 sp2Translation = sprite2.GetPosition();

	MyCol::CreateOBB(&obb, sprite1.GetPosition(), Matrix::CreateRotationZ(sprite1.GetAngle()), sprite1.GetParameter().size);

	int count = 0;
	for (auto csp : sprite2.GetParameter().ClothSegmentPoints_)
	{
		//ClothSegPoints�̍ŏI�l�ɓ��B������A
		if (count >= sprite2.GetParameter().ClothSegmentPoints_.size() - 1) {
			Vector3 targetMPos1 = Vector3(csp.x, csp.y)*Matrix::CreateRotationZ(sprite2.GetAngle());
			Vector3 targetMPos2 = Vector3(sprite2.GetParameter().ClothSegmentPoints_[0].x, sprite2.GetParameter().ClothSegmentPoints_[0].y)
				* Matrix::CreateRotationZ(sprite2.GetAngle());

			Vector2 targetPos1 = Vector2(targetMPos1.x, targetMPos1.y);
			Vector2 targetPos2 = Vector2(targetMPos2.x, targetMPos2.y);

			MyCol::CreateSegment(&segs[count], targetPos1 + sp2Translation, targetPos2 + sp2Translation);
			break;
		}
		MyCol::CreateSegment(&segs[count], csp + sp2Translation, sprite2.GetParameter().ClothSegmentPoints_[count + 1] + sp2Translation);

		count++;
	}
	std::vector<Vector2> points = MyCol::GetOBBPoints(sprite1);

	int intSet[][2] = { { 0,1 },{ 0,2 },{ 1,3 },{ 2,3 } };

	bool isHitCheck = false;
	std::vector<Vector2> allSegPoint;
	std::vector<float> allsegDist;
	int minDist = 0;

	count = 0;
	allSegPoint.resize(segs.size(), Vector2(999999, 999999));
	allsegDist.resize(segs.size(), 99999999);
	for (auto sg : segs) {
		if (MyCol::Col_OBB_Segment(obb, sg, points, allSegPoint[count])) {
			isHitCheck = true;
			//���݂̍ŒZ���������A���������ړ_�̋����̕����Z��������l�̍X�V
			if (allsegDist[minDist] >= Vector2::Distance(sprite1.GetPosition(), allSegPoint[count])) {
				minDist = count;
			}
		}
		count++;
	}
	Vector2 segPoint = allSegPoint[minDist];

	return CollisionParameter(COL_ID::BOX_CLOTHES_COL, isHitCheck, segPoint);
}

CollisionParameter CollisionFunction::IsHit_Clothes_OBB(const Actor & sprite1, const Actor & sprite2)
{
	if (sprite1.GetParameter().ClothSegmentPoints_.size() == 0) {
		return CollisionParameter(COL_ID::BOX_SEGMENT_COL, false);
	}
	OBB obb;
	std::vector<Segment> segs;
	segs.resize(sprite1.GetParameter().ClothSegmentPoints_.size());
	Vector2 sp1Translation = sprite2.GetPosition();
	Vector2 sp2Translation = sprite1.GetPosition();

	MyCol::CreateOBB(&obb, sprite2.GetPosition(), Matrix::CreateRotationZ(sprite2.GetAngle()), sprite2.GetParameter().size);

	int count = 0;
	for (auto csp : sprite1.GetParameter().ClothSegmentPoints_)
	{
		//ClothSegPoints�̍ŏI�l�ɓ��B������A
		if (count >= sprite1.GetParameter().ClothSegmentPoints_.size() - 1) {
			Vector3 targetMPos1 = Vector3(csp.x, csp.y)*Matrix::CreateRotationZ(sprite1.GetAngle());
			Vector3 targetMPos2 = Vector3(sprite1.GetParameter().ClothSegmentPoints_[0].x, sprite1.GetParameter().ClothSegmentPoints_[0].y)
				* Matrix::CreateRotationZ(sprite1.GetAngle());

			Vector2 targetPos1 = Vector2(targetMPos1.x, targetMPos1.y);
			Vector2 targetPos2 = Vector2(targetMPos2.x, targetMPos2.y);

			MyCol::CreateSegment(&segs[count], targetPos1 + sp2Translation, targetPos2 + sp2Translation);
			break;
		}
		MyCol::CreateSegment(&segs[count], csp + sp2Translation, sprite1.GetParameter().ClothSegmentPoints_[count + 1] + sp2Translation);

		count++;
	}
	std::vector<Vector2> points = MyCol::GetOBBPoints(sprite2);

	int intSet[][2] = { { 0,1 },{ 0,2 },{ 1,3 },{ 2,3 } };

	bool isHitCheck = false;
	std::vector<Vector2> allSegPoint;
	std::vector<float> allsegDist;
	int minDist = 0;

	count = 0;
	allSegPoint.resize(segs.size(), Vector2(999999, 999999));
	allsegDist.resize(segs.size(), 99999999);
	for (auto sg : segs) {
		if (MyCol::Col_OBB_Segment(obb, sg, points, allSegPoint[count])) {
			isHitCheck = true;
			//���݂̍ŒZ���������A���������ړ_�̋����̕����Z��������l�̍X�V
			if (allsegDist[minDist] >= Vector2::Distance(sprite2.GetPosition(), allSegPoint[count])) {
				minDist = count;
			}
		}
		count++;
	}
	Vector2 segPoint = allSegPoint[minDist];

	return CollisionParameter(COL_ID::CLOTHES_BOX_COL, isHitCheck, segPoint);

}