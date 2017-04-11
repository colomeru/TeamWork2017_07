#include "PuyoCollision.h"
#include "../../Body/CollisionBase.h"
#include "../../../Math/Math.h"
#include "../../Body/BoundingSegment.h"
#include "../../../Game/Time.h"
PuyoCollision::PuyoCollision(IWorld * world, Vector2 & position, Vector2 arrayState, Vector2& center) :
	Actor(world, "PuyoCollision", position, CollisionBase(center, position)),
	mIntersection(position),
	mResIntersection(position),
	mVelo(Vector2::Zero),
	mArrayState(arrayState),
	mCenter(center),
	mIsCol(false),
	mPlayerPos1(Vector2::Zero),
	mPlayerPos2(Vector2::Zero),
	mNoColTimer(0.0f)
{
	for (int i = 0; i < 3; i++) {
		mIntersections[i] = Vector2::Zero;
	}
}

PuyoCollision::~PuyoCollision()
{
}

void PuyoCollision::onUpdate(float deltaTime)
{
	if (Vector2::Distance(mCenter1, position_) <= Vector2::Distance(mCenter2, position_))
		mCenter = mCenter1;
	else
		mCenter = mCenter2;


	//あたり判定更新
	body_.setSegmentPoint(mCenter, mCenter, position_);
	//当たっていない場合は交点は本来の位置へ
	if (!mIsCol) {
		mResIntersection = position_;
		mVec = Vector2::Zero;
		mIntersectionsVector.clear();
	}
	else
	{
		if (!mIntersectionsVector.empty()) {
			mResIntersection = mIntersectionsVector.front();
			for (auto i : mIntersectionsVector) {
				if (Vector2::Distance(mCenter, i) < Vector2::Distance(mCenter, mResIntersection)) {
					mResIntersection = i;
				}
			}
			//ベクトルを計算
			mVec = mIntersection - position_;
			mIntersectionsVector.clear();
		}
	}
	//生成された瞬間は当たらない
	if (mNoColTimer <= 0.2f) {
		mNoColTimer += Time::GetInstance().deltaTime();
		mVec = 0.0f;
	}

	Vector2::Spring(mIntersection, mResIntersection, mVelo, 0.5f,0.6f,2.5f);
	//フラグ初期化
	mIsCol = false;
}

void PuyoCollision::onLateUpdate(float deltaTime)
{
}

void PuyoCollision::onDraw() const
{
	//DrawCircle((position_ * inv_).x, (position_ * inv_).y, 5, GetColor(0, 0, 255));
	//DrawLine((position_ * inv_).x, (position_ * inv_).y, (mCenter*inv_).x, (mCenter*inv_).y, GetColor(255, 0, 0));
	//for (int i = 0; i < 3; i++) {
	//	DrawCircle((mIntersections[i] *inv_).x, (mIntersections[i] *inv_).y, 5, GetColor(0, 0, 255));
	//}
	//for (auto i : mIntersectionsVector) {
	//	DrawCircle((i * inv_).x, (i * inv_).y, 5, GetColor(0, 0, 255));
	//	DrawLine((mCenter*inv_).x, (mCenter*inv_).y, (i*inv_).x, (i*inv_).y, GetColor(255, 255, 0));
	//}
	//DrawFormatString(25, 500, GetColor(0, 0, 0), "交差している交点の数:%d", mIntersectionsVector.size());
	//if (mIntersectionsVector.empty())return;
	//DrawCircle((mIntersectionsVector.front() * inv_).x, (mIntersectionsVector.front() * inv_).y, 5, GetColor(0, 0, 255));
	//DrawLine((mCenter*inv_).x, (mCenter*inv_).y, (mIntersectionsVector.front()*inv_).x, (mIntersectionsVector.front()*inv_).y, GetColor(255, 0, 0));
	//DrawCircle((mIntersection * inv_).x, (mIntersection * inv_).y, 5, GetColor(255, 0, 0));
	//DrawCircle((position_*inv_).x, (position_ *inv_).y, 2, GetColor(0, 0, 0));
	//if (top)
	//	DrawFormatString(25, 500, GetColor(0, 0, 0), "上が当たっている");
	//if (bot)
	//	DrawFormatString(25, 500, GetColor(0, 0, 0), "下が当たっている");
	//if (right)
	//	DrawFormatString(25, 500, GetColor(0, 0, 0), "右が当たっている");
	//if (left)
	//	DrawFormatString(25, 500, GetColor(0, 0, 0), "左が当たっている");
}

void PuyoCollision::onCollide(Actor & other)
{
	if (other.getName() == "MovelessFloor" || other.getName() == "SticklessFloor" ||
		other.getName() == "MoveFloorUpDown" || other.getName() == "MoveFloorRightLeft" ||
		other.getName() == "TurnFloor" || other.getName() == "TranslessTurnFloor" || other.getName() == "Door"
		|| other.getName() == "BlockParticle") {

		//当たっているフラグ
		mIsCol = true;
		//当たっている短形の4頂点
		auto t_left = other.getBody().GetBox().component_.point[0];
		auto t_right = other.getBody().GetBox().component_.point[1];
		auto b_left = other.getBody().GetBox().component_.point[2];
		auto b_right = other.getBody().GetBox().component_.point[3];

		//線のあたり判定＆交点を求める関数
		SegmentCol(mCenter, position_, t_left, t_right, mIntersections[0], false);
		SegmentCol(mCenter, position_, b_right, b_left, mIntersections[1], false);
		SegmentCol(mCenter, position_, b_right, t_right, mIntersections[2], true);
		SegmentCol(mCenter, position_, b_left, t_left, mIntersections[3], true);

		if (mIntersections[0].x != Vector2::Zero.x&&
			mIntersections[0].y != Vector2::Zero.y)
			mIntersectionsVector.push_back(mIntersections[0]);
		if (mIntersections[1].x != Vector2::Zero.x&&
			mIntersections[1].y != Vector2::Zero.y)
			mIntersectionsVector.push_back(mIntersections[1]);

		if (mIntersections[2].x != Vector2::Zero.x&&
			mIntersections[2].y != Vector2::Zero.y)
			mIntersectionsVector.push_back(mIntersections[2]);

		if (mIntersections[3].x != Vector2::Zero.x&&
			mIntersections[3].y != Vector2::Zero.y)
			mIntersectionsVector.push_back(mIntersections[3]);
	}

	if (other.getName() == "SegmentCollider")
	{
		mIsCol = true;
		Vector2 intersection;
		auto left = other.getBody().GetSegment().component_.point[0];
		auto right = other.getBody().GetSegment().component_.point[1];
		SegmentCol(mCenter, position_, left, right, intersection, false);
		if (intersection.x != Vector2::Zero.x&&
			intersection.y != Vector2::Zero.y)
			mIntersectionsVector.push_back(intersection);

	}
}

bool PuyoCollision::SegmentCol(const Vector2& A, const Vector2& B, const Vector2& C, const Vector2& D, Vector2 & couten, bool rightLeft)
{
	////必要なベクトルを計算
	Vector2 AB = B - A;
	Vector2 AC = C - A;
	Vector2 AD = D - A;
	Vector2 CD = D - C;
	Vector2 CA = A - C;
	Vector2 CB = B - C;
	//必要な外積を計算
	float cross1 = Vector2::Cross(AB, AC);
	float cross2 = Vector2::Cross(AB, AD);
	float cross3 = Vector2::Cross(CD, CA);
	float cross4 = Vector2::Cross(CD, CB);


	float cross5 = std::abs(Vector2::Cross(CD, CA));
	float cross6 = std::abs(Vector2::Cross(CD, CB));
	if (cross1*cross2 < 0.0f&&
		cross3*cross4 < 0.0f) {
		//当たっていた場合交点を求める
		//couten = C + CD*(Vector2::Cross(AB,CA)/Vector2::Cross(AB,CD));
		couten = A + AB*(cross5 / (cross5 + cross6));
		return true;
	}
	////当たってなかったら0を返す
	couten = Vector2::Zero;
	return false;
}
