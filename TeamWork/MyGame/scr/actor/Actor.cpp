#include "Actor.h"
#include "../collision/Collision.h"
#include "../collision/CollisionDX.h"
#include "../conv/DXConverter.h"
#include "../graphic/Model.h"
#include"../collision/MyCol.h"

Actor::Actor(IWorld * world, Actor * parent) :
	world_(world),
	parent_(parent),
	angle_(0.f),
	position_(Vector2::Zero),
	laneNum_(0),
	isDraw_(true)
{
	colFunc_[COL_ID::TEST_COL] = std::bind(&Actor::IsHit_Circle_Circle,this,std::placeholders::_1);
	colFunc_[COL_ID::BOX_SEGMENT_COL] = std::bind(&Actor::IsHit_OBB_Segment, this, std::placeholders::_1);
	colFunc_[COL_ID::BOX_CLOTHES_COL] = std::bind(&Actor::IsHit_OBB_Clothes, this, std::placeholders::_1);
	colFunc_[COL_ID::BOX_BOX_COL] = std::bind(&Actor::IsHit_OBB_OBB, this, std::placeholders::_1);
}

Actor::~Actor()
{
}

void Actor::Collide(COL_ID id, Actor & other)
{
	if (laneNum_ != other.laneNum_)return;

	CollisionParameter colpara = colFunc_[id](other);
	if (colpara.colFrag)
	{
		OnCollide(other, colpara);
		other.OnCollide(*this, colpara);
	}
}

ActorParameter Actor::GetParameter() const
{
	return parameter_;
}

Matrix Actor::GetPose() const
{
	return Matrix(parameter_.mat.RotationMatrix())
		 * Matrix::CreateTranslation(parameter_.mat.Translation());
}

void Actor::SetPose(const Matrix& mat)
{
	parameter_.mat = mat;
	position_ = Vector2(parameter_.mat.Translation().x, parameter_.mat.Translation().y);
}

void Actor::OnUpdate()
{
}

// 自分取得
Actor* Actor::GetActor() const
{
	return const_cast<Actor*>(this);
}

Actor* Actor::GetParent() const
{
	return parent_;
}

// メッセージ処理
void Actor::handleMessage(EventMessage message, void * param)
{
	OnMessage(message, param);
}

void Actor::OnCollide(Actor & other, CollisionParameter colpara)
{
}

void Actor::OnMessage(EventMessage message, void * param)
{
}

CollisionParameter Actor::Test_Col(const Actor & other) const
{
	CollisionParameter colpara;
	colpara.colID = COL_ID::TEST_COL;
	//colpara.colFrag = CollisionDX::Capsule_vs_Capsule(Capsule(GetParameter()), Capsule(other.GetParameter()));

	return colpara;
}

// スプライトの当たり判定（２次元のOBB vs OBB）

CollisionParameter Actor::IsHit_OBB_OBB(const Actor & sprite2)
{
	OBB obb1;
	OBB obb2;
	Vector2 sp1Translation = GetPosition();
	Vector2 sp2Translation = sprite2.GetPosition();

	//MyCol::CreateOBB(&obb1, sp1Translation, GetParameter().mat.RotationMatrix(), Vector2(GetParameter().size.x / 2.0f, GetParameter().size.y / 2.0f));
	//MyCol::CreateOBB(&obb2, sp2Translation, sprite2.GetParameter().mat.RotationMatrix(), Vector2(sprite2.GetParameter().size.x / 2.0f, sprite2.GetParameter().size.y / 2.0f));
	MyCol::CreateOBB(&obb1, sp1Translation, Matrix::CreateRotationZ(angle_), Vector2(GetParameter().size.x / 2.0f, GetParameter().size.y / 2.0f));
	MyCol::CreateOBB(&obb2, sp2Translation, Matrix::CreateRotationZ(sprite2.GetAngle()), Vector2(sprite2.GetParameter().size.x / 2.0f, sprite2.GetParameter().size.y / 2.0f));

	return CollisionParameter(COL_ID::BEGIN_COL, MyCol::TestOBBOBB(obb1, obb2),position_);

}

CollisionParameter Actor::IsHit_OBB_Segment(const Actor & sprite2)
{
	OBB obb;
	Segment seg;
	Vector2 sp1Translation = GetPosition();
	Vector2 sp2Translation = sprite2.GetPosition();

	MyCol::CreateOBB(&obb, GetPosition(), Matrix::CreateRotationZ(GetAngle()), GetParameter().size);
	MyCol::CreateSegment(&seg, sprite2.GetPosition(), Matrix::CreateRotationZ(sprite2.GetAngle()), sprite2.GetParameter().size.x);
	std::vector<Vector2> points = MyCol::GetOBBPoints(*this);
	Vector2 segPoint;

	int intSet[][2] = { { 0,1 },{ 0,2 },{ 1,3 },{ 2,3 } };

	for (int i = 0; i < 4; i++) {
		//DrawLine(points[intSet[i][0]].x, points[intSet[i][0]].y, points[intSet[i][1]].x, points[intSet[i][1]].y, GetColor(255, 0, 0));
	}
	//DrawLine(seg.startPoint.x, seg.startPoint.y, seg.endPoint.x, seg.endPoint.y, GetColor(255, 255, 255));

	bool isHitCheck = false;
	if (MyCol::Col_OBB_Segment(obb, seg, points, segPoint)) {
		isHitCheck = true;
	}
	return CollisionParameter(COL_ID::BEGIN_COL, isHitCheck, segPoint);

}

CollisionParameter Actor::IsHit_Segment_Segment(const Actor & sprite2)
{
	Vector2 intersectPoint = Vector2();
	Segment seg1;
	Segment seg2;
	Vector2 sp1Translation = GetPosition();
	Vector2 sp2Translation = sprite2.GetPosition();

	MyCol::CreateSegment(&seg1, sp1Translation, Matrix::CreateRotationZ(GetAngle()), GetParameter().size.x);
	MyCol::CreateSegment(&seg2, sp2Translation, Matrix::CreateRotationZ(sprite2.GetAngle()), sprite2.GetParameter().size.x);
	//CeateOBB(&obb1, sp1Translation, sprite1.mat.RotationMatrix(), Vector2(sprite1.size.x / 2.0f, sprite1.size.y / 2.0f));
	//CreateOBB(&obb2, sp2Translation, sprite2.mat.RotationMatrix(), Vector2(sprite2.size.x / 2.0f, sprite2.size.y / 2.0f));

	//DrawLine(seg1.startPoint.x, seg1.startPoint.y, seg1.endPoint.x, seg1.endPoint.y, GetColor(255, 255, 255));
	//DrawLine(seg2.startPoint.x, seg2.startPoint.y, seg2.endPoint.x, seg2.endPoint.y, GetColor(255, 255, 255));

	Vector2 segPoint;
	bool colPF = false;
	if (MyCol::Col_Segment_Segment(seg1, seg2, segPoint)) {
		intersectPoint = segPoint;
		colPF = true;
	}
	return CollisionParameter(COL_ID::BEGIN_COL, colPF, intersectPoint);
}

CollisionParameter Actor::IsHit_Circle_Circle(const Actor & sprite2)
{
	Circle crc1;
	Circle crc2;
	Vector2 sp1Translation = GetPosition();
	Vector2 sp2Translation = sprite2.GetPosition();

	MyCol::CreateCircle(&crc1, sp1Translation, GetParameter().radius);
	MyCol::CreateCircle(&crc2, sp2Translation, sprite2.GetParameter().radius);
	//CeateOBB(&obb1, sp1Translation, sprite1.mat.RotationMatrix(), Vector2(sprite1.size.x / 2.0f, sprite1.size.y / 2.0f));
	//CreateOBB(&obb2, sp2Translation, sprite2.mat.RotationMatrix(), Vector2(sprite2.size.x / 2.0f, sprite2.size.y / 2.0f));

	//DrawCircle(sp1Translation.x, sp1Translation.y, GetParameter().radius, GetColor(255, 255, 255));
	//DrawCircle(sp2Translation.x, sp2Translation.y, sprite2.GetParameter().radius, GetColor(255, 255, 255));

	Vector2 segPoint;
	bool colPF = false;
	if (MyCol::Col_Circle_Circle(crc1, crc2, segPoint)) {
		colPF = true;
	}
	return CollisionParameter(COL_ID::BEGIN_COL, colPF, segPoint);
}

CollisionParameter Actor::IsHit_Circle_Segment(const Actor & sprite2)
{
	Circle crc;
	Segment seg;
	Vector2 sp1Translation = GetPosition();
	Vector2 sp2Translation = sprite2.GetPosition();

	MyCol::CreateCircle(&crc, sp1Translation, GetParameter().radius);
	MyCol::CreateSegment(&seg, sp2Translation, Matrix::CreateRotationZ(sprite2.GetAngle()), sprite2.GetParameter().size.x);

	//DrawCircle(sp1Translation.x, sp1Translation.y, GetParameter().radius, GetColor(255, 255, 255));
	//DrawLine(seg.startPoint.x, seg.startPoint.y, seg.endPoint.x, seg.endPoint.y, GetColor(255, 255, 255));

	Vector2 segPoint;
	return CollisionParameter(COL_ID::BEGIN_COL, MyCol::Col_Circle_Segment(crc, seg, segPoint), segPoint);

}

CollisionParameter Actor::IsHit_OBB_Circle(const Actor & sprite2) {

	OBB obb;
	Circle crc;
	Vector2 sp1Translation = GetPosition();
	Vector2 sp2Translation = sprite2.GetPosition();

	MyCol::CreateOBB(&obb, sp1Translation, Matrix::CreateRotationZ(GetAngle()), GetParameter().size);
	MyCol::CreateCircle(&crc, sp2Translation, sprite2.GetParameter().radius);

	//DrawCircle(sp2Translation.x, sp2Translation.y, sprite2.GetParameter().radius, GetColor(255, 255, 255));

	Vector2 segPoint;
	std::vector<Vector2> points = MyCol::GetOBBPoints(*this);

	return CollisionParameter(COL_ID::BEGIN_COL, MyCol::Col_OBB_Circle(obb, crc, points, segPoint), segPoint);
}

CollisionParameter Actor::IsHit_OBB_Clothes(const Actor & sprite2)
{
	if (sprite2.GetParameter().ClothSegmentPoints_.size()==0) {
		return CollisionParameter(COL_ID::BOX_SEGMENT_COL, false);
	}
	OBB obb;
	std::vector<Segment> segs;
	segs.resize(sprite2.GetParameter().ClothSegmentPoints_.size());
	Vector2 sp1Translation = GetPosition();
	Vector2 sp2Translation = sprite2.GetPosition();

	MyCol::CreateOBB(&obb, GetPosition(), Matrix::CreateRotationZ(GetAngle()), GetParameter().size);

	int count = 0;
	for (auto csp : sprite2.GetParameter().ClothSegmentPoints_)
	{
		//ClothSegPointsの最終値に到達したら、
		if (count >= sprite2.GetParameter().ClothSegmentPoints_.size()-1) {
			Vector3 targetMPos1=Vector3(csp.x,csp.y)*Matrix::CreateRotationZ(sprite2.GetAngle());
			Vector3 targetMPos2= Vector3(sprite2.GetParameter().ClothSegmentPoints_[0].x, sprite2.GetParameter().ClothSegmentPoints_[0].y)
				* Matrix::CreateRotationZ(sprite2.GetAngle());
			
			Vector2 targetPos1 = Vector2(targetMPos1.x, targetMPos1.y);
			Vector2 targetPos2 = Vector2(targetMPos2.x, targetMPos2.y);

			MyCol::CreateSegment(&segs[count], targetPos1 + sp2Translation, targetPos2 + sp2Translation);
			break;
		}
		MyCol::CreateSegment(&segs[count],csp+sp2Translation, sprite2.GetParameter().ClothSegmentPoints_[count+1]+ sp2Translation);
		
		count++;
	}
	std::vector<Vector2> points = MyCol::GetOBBPoints(*this);

	int intSet[][2] = { { 0,1 },{ 0,2 },{ 1,3 },{ 2,3 } };

	//for (int i = 0; i < 4; i++) {
	//	DrawLine(points[intSet[i][0]].x, points[intSet[i][0]].y, points[intSet[i][1]].x, points[intSet[i][1]].y, GetColor(255, 0, 0));
	//}
	//DrawLine(seg.startPoint.x, seg.startPoint.y, seg.endPoint.x, seg.endPoint.y, GetColor(255, 255, 255));

	bool isHitCheck = false;
	std::vector<Vector2> allSegPoint;
	std::vector<float> allsegDist;
	int minDist=0;

	count = 0;
	allSegPoint.resize(segs.size(), Vector2(999999, 999999));
	allsegDist.resize(segs.size(), 99999999);
	for (auto sg : segs) {
		if (MyCol::Col_OBB_Segment(obb, sg, points, allSegPoint[count])) {
			isHitCheck = true;
			//現在の最短距離よりも、当たった接点の距離の方が短かったら値の更新
			if (allsegDist[minDist] >= Vector2::Distance(position_, allSegPoint[count])) {
				minDist = count;
			}
		}
		count++;
	}
	Vector2 segPoint= allSegPoint[minDist];
	
	return CollisionParameter(COL_ID::BEGIN_COL, isHitCheck, segPoint);

}
