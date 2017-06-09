#include "CollisionFunction.h"
#include "../actor/Actor.h"
#include "../actor/Field/Clothes/Clothes.h"
#include <vector>
#include "../collision/MyCol.h"
#include "../actor/player/Player.h"
#include "../actor/player/Player_Sword.h"
#include"../actor/Field/Enemys/EnemyCharas/ClothesTapper.h"

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

CollisionParameter CollisionFunction::IsHit_PHead_Goal(const Actor & sprite1, const Actor & sprite2)
{
	OBB obb1;
	OBB obb2;
	Vector2 sp1Translation = sprite1.GetPosition();
	Vector2 sp2Translation = sprite2.GetPosition();

	MyCol::CreateOBB(&obb1, sp1Translation, Matrix::CreateRotationZ(sprite1.GetAngle()), Vector2(sprite1.GetParameter().size.x / 2.0f, sprite1.GetParameter().size.y / 2.0f));
	MyCol::CreateOBB(&obb2, sp2Translation, Matrix::CreateRotationZ(sprite2.GetAngle()), Vector2(sprite2.GetParameter().size.x / 2.0f, sprite2.GetParameter().size.y / 2.0f));

	return CollisionParameter(COL_ID::PHEAD_GOAL_COL, MyCol::TestOBBOBB(obb1, obb2), sprite1.GetPosition());
}

CollisionParameter CollisionFunction::IsHit_PHead_Hanger(const Actor & sprite1, const Actor & sprite2)
{
	OBB obb1;
	OBB obb2;
	Vector2 sp1Translation = sprite1.GetPosition();
	Vector2 sp2Translation = sprite2.GetPosition();
	auto sp2CY = MathHelper::Abs(MathHelper::Cos(sprite2.GetAngle())) * sprite2.GetParameter().size.y / 8.0f;

	MyCol::CreateOBB(&obb1, sp1Translation, Matrix::CreateRotationZ(sprite1.GetAngle()), Vector2(sprite1.GetParameter().size.x / 2.0f, sprite1.GetParameter().size.y / 2.0f));
	MyCol::CreateOBB(&obb2, sp2Translation + Vector2(0, sp2CY), Matrix::CreateRotationZ(sprite2.GetAngle()), Vector2(sprite2.GetParameter().size.x / 2.0f, sprite2.GetParameter().size.y / 12.0f));

	return CollisionParameter(COL_ID::BOX_HANGER_COL, MyCol::TestOBBOBB(obb1, obb2), sprite1.GetPosition());
}

CollisionParameter CollisionFunction::IsHit_Circle_Capsules(const Actor & sprite1, const Actor & sprite2)
{
	std::vector<Vector2> collisionPoints = static_cast<Clothes*>(const_cast<Actor*>(&sprite2))->GetCollisionPoints();
	if(collisionPoints.empty()) return CollisionParameter(COL_ID::PHEAD_CLOTHES_COL, false);
	Circle head;
	Vector3 headTranslation = sprite1.GetPose().Translation();

	for (int i = 0; i < 3; i++) {
		auto circleCenter = DXConverter::GetInstance().ToVECTOR(headTranslation);
		auto capPos1 = DXConverter::GetInstance().ToVECTOR(Vector3(collisionPoints[i].x, collisionPoints[i].y, 0));
		auto capPos2 = DXConverter::GetInstance().ToVECTOR(Vector3(collisionPoints[i+1].x, collisionPoints[i+1].y, 0));
		if (HitCheck_Sphere_Capsule(circleCenter, sprite1.GetParameter().radius, capPos1, capPos2, sprite2.GetParameter().radius)) {
			return CollisionParameter(COL_ID::PHEAD_CLOTHES_COL, true);
		}
	}

	return CollisionParameter(COL_ID::PHEAD_CLOTHES_COL, false);
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

CollisionParameter CollisionFunction::IsHit_PSword_Clothes(const Actor & sprite1, const Actor & sprite2)
{
	std::vector<Vector2> collisionPoints = static_cast<Clothes*>(const_cast<Actor*>(&sprite2))->GetCollisionPoints();
	if (collisionPoints.empty()) return CollisionParameter(COL_ID::BOX_SEGMENT_COL, false, Vector2());

	Segment seg1;
	auto player_Sword = static_cast<Player_Sword*>(const_cast<Actor*>(&sprite1));
	auto startPos = player_Sword->GetSwordStartPos();
	auto endPos = player_Sword->GetSwordEndPos();
	MyCol::CreateSegment(&seg1, startPos, endPos);

	std::vector<Segment> seg2;
	seg2.reserve(4);
	for (int i = 0; i < 3; i++) {
		Segment seg;
		MyCol::CreateSegment(&seg, collisionPoints[i], collisionPoints[i + 1]);
		seg2.push_back(seg);
	}

	Vector2 segPoint;
	bool isHitCheck = false;
	for (int i = 0; i < 3; i++) {
		if (MyCol::Col_Segment_Segment(seg1, seg2[i], segPoint)) {
			auto sword_Parent = static_cast<Player*>(const_cast<Actor*>(player_Sword->GetParent()));
			if(sword_Parent->GetPlayerSwordAngle() > 90 && sword_Parent->GetIsSwordActive())
				isHitCheck = true;
		}
	}

	return CollisionParameter(COL_ID::PSWORD_CLOTHES_COL, isHitCheck, segPoint);

}

CollisionParameter CollisionFunction::IsHit_Hairball_PSword(const Actor & sprite1, const Actor & sprite2)
{
	Circle crc;
	Vector2 translation = sprite1.GetPosition();
	MyCol::CreateCircle(&crc, translation, sprite1.GetParameter().radius);

	Segment seg;
	auto player_Sword = static_cast<Player_Sword*>(const_cast<Actor*>(&sprite2));
	auto startPos = player_Sword->GetSwordStartPos();
	auto endPos = player_Sword->GetSwordEndPos();
	MyCol::CreateSegment(&seg, startPos, endPos);

	Vector2 segPoint;
	bool isHitCheck = false;
	if (MyCol::Col_Circle_Segment(crc, seg, segPoint)) {
		auto player = static_cast<Player*>(const_cast<Actor*>(player_Sword->GetParent()));
		if (player->GetIsSwordActive()) {
			isHitCheck = true;
		}
	}

	return CollisionParameter(COL_ID::PSWORD_HAIRBALL_COL, isHitCheck, segPoint);
}

CollisionParameter CollisionFunction::IsHit_Droping_Clothes(const Actor & sprite1, const Actor & sprite2)
{
	Circle crc;
	Vector2 sp1Translation = sprite1.GetPosition();
	MyCol::CreateCircle(&crc, sp1Translation, sprite1.GetParameter().radius);

	OBB obb;
	Vector2 size = sprite2.GetParameter().size;
	Vector2 sp2Translation = sprite2.GetPosition() + Vector2(0, size.y / 12);

	if (sp1Translation.y < sp2Translation.y)
		return CollisionParameter(COL_ID::DROP_CLOTHES_COL, false);

	MyCol::CreateOBB(&obb, Vector2::Zero, Matrix::CreateRotationZ(sprite2.GetAngle()), Vector2(size.x / 8.0f, size.y / 4.0f));

	Vector2 segPoint;
	std::vector<Vector2> points = MyCol::GetOBBPoints(sprite2);
	if (MyCol::Col_OBB_Circle(obb, crc, points, segPoint)) {
		auto clothes = static_cast<Clothes*>(const_cast<Actor*>(&sprite2));
		if (!clothes->GetIsWind()) {
			return CollisionParameter(COL_ID::DROP_CLOTHES_COL, true, segPoint);
		}
	}

	return CollisionParameter(COL_ID::DROP_CLOTHES_COL, false);
}

CollisionParameter CollisionFunction::IsHit_Tapper_PSword(const Actor & sprite1, const Actor & sprite2)
{
	Segment seg1;
	auto player_Sword = static_cast<Player_Sword*>(const_cast<Actor*>(&sprite2));
	if (!player_Sword->GetUseSword())return CollisionParameter(COL_ID::TAPPER_PSWORD_COL, false, Vector2());
	auto startPos = player_Sword->GetSwordStartPos();
	auto endPos = player_Sword->GetSwordEndPos();
	MyCol::CreateSegment(&seg1, startPos, endPos);

	Circle crc1;
	auto tapper_ = static_cast<ClothesTapper*>(const_cast<Actor*>(&sprite1));
	auto rad = sprite1.GetParameter().radius;
	auto pos = sprite1.GetPosition() + tapper_->GetShiftPos();
	MyCol::CreateCircle(&crc1, pos, rad);


	Vector2 segPoint;
	bool isHitCheck = false;
	if (MyCol::Col_Circle_Segment(crc1, seg1, segPoint)) {
		//auto sword_Parent = static_cast<Player*>(const_cast<Actor*>(player_Sword->GetParent()));
		//if (sword_Parent->GetPlayerSwordAngle() > 90 && sword_Parent->GetIsSwordActive())
		isHitCheck = true;
	}

	return CollisionParameter(COL_ID::TAPPER_PSWORD_COL, isHitCheck, segPoint);

}

//CollisionParameter CollisionFunction::IsHit_Tapper_PSword(const Actor & sprite1, const Actor & sprite2)
//{
//	Segment seg1;
//	auto player_Sword = static_cast<Player_Sword*>(const_cast<Actor*>(&sprite2));
//	if(!player_Sword->GetUseSword())return CollisionParameter(COL_ID::TAPPER_PSWORD_COL, false, Vector2());
//	auto startPos = player_Sword->GetSwordStartPos();
//	auto endPos = player_Sword->GetSwordEndPos();
//	MyCol::CreateSegment(&seg1, startPos, endPos);
//
//	Circle crc1;
//	auto rad=sprite1.GetParameter().radius;
//	auto pos = sprite1.GetPosition();
//	MyCol::CreateCircle(&crc1, pos, rad);
//
//
//	Vector2 segPoint;
//	bool isHitCheck = false;
//		if (MyCol::Col_Circle_Segment(crc1,seg1, segPoint)) {
//			//auto sword_Parent = static_cast<Player*>(const_cast<Actor*>(player_Sword->GetParent()));
//			//if (sword_Parent->GetPlayerSwordAngle() > 90 && sword_Parent->GetIsSwordActive())
//				isHitCheck = true;
//		}
//
//	return CollisionParameter(COL_ID::TAPPER_PSWORD_COL, isHitCheck, segPoint);
//
//}

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
		//ClothSegPointsの最終値に到達したら、
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
			//現在の最短距離よりも、当たった接点の距離の方が短かったら値の更新
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
		//ClothSegPointsの最終値に到達したら、
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
			//現在の最短距離よりも、当たった接点の距離の方が短かったら値の更新
			if (allsegDist[minDist] >= Vector2::Distance(sprite2.GetPosition(), allSegPoint[count])) {
				minDist = count;
			}
		}
		count++;
	}
	Vector2 segPoint = allSegPoint[minDist];

	return CollisionParameter(COL_ID::CLOTHES_BOX_COL, isHitCheck, segPoint);

}
