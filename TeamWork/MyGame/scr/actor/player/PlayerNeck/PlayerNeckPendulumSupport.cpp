#include "PlayerNeckPendulumSupport.h"
#include"../../../math/Vector2.h"
#include"../../../graphic/DrawPos.h"
#include"../../../math/MyFuncionList.h"

//1ピース毎の長さ
static const float oneLength = 30.0f;
//ピースの太さ
static const float neckPieceWidth = 15.0f;
//振り子の重力
static const float gravity = 0.5f;
void PlayerNeckPendulumSupport::Init(const Vector2 & fulcrum, const Vector2 & tipPos,
	std::vector<Vector2>& fulcrum_List,
	std::vector<Vector2>& tipPos_List,
	std::vector<float>& rot_List,
	std::vector<float>& rot_Speed_List,
	std::vector<NeckPiecePoint>& neck_Point_List,
	float rot_speed) {

	fulcrum_List.clear();
	tipPos_List.clear();
	rot_List.clear();
	rot_Speed_List.clear();
	neck_Point_List.clear();

	//首の長さを設定
	float neckLength = Vector2::Distance(fulcrum, tipPos);

	//完全な首のピース数を計算
	int piecenumber = (int)floorf(neckLength / oneLength);
	//余りピースの大きさを計算
	float piecesurplus = fmodf(neckLength, oneLength);

	//始点から終点へのベクトルを作成
	Vector2 fulcrumtotipPos = tipPos - fulcrum;
	fulcrumtotipPos = fulcrumtotipPos.Normalize();
	//ベクトルの角度を計算
	float rotAngle = MathAngle(fulcrumtotipPos) + 90.f;

	//最初にあまり分の首を作成
	{
		//1つ目の支点を設定(1ピース目の始点)
		fulcrum_List.push_back(fulcrum);

		//支点から余りのサイズ分先の点をtipPosに設定(1ピース目の終点)
		Vector2 firstTipPos = fulcrum + fulcrumtotipPos*piecesurplus;
		tipPos_List.push_back(firstTipPos);

		rot_List.push_back(rotAngle);

		rot_Speed_List.push_back(rot_speed);

		NeckPiecePoint piecePoint = MathNeckPiecePoint(fulcrum, firstTipPos, neckPieceWidth);
		neck_Point_List.push_back(piecePoint);
	}

	for (int i = 0; i < piecenumber; i++) {
		//直前に作った先端を次の支点とする
		Vector2 nextFurcrumPos = tipPos_List.back();
		fulcrum_List.push_back(nextFurcrumPos);

		Vector2 nextTipPos = nextFurcrumPos + fulcrumtotipPos*oneLength;
		tipPos_List.push_back(nextTipPos);

		rot_List.push_back(rotAngle);

		rot_Speed_List.push_back(rot_speed);

		NeckPiecePoint piecePoint = MathNeckPiecePoint(fulcrum_List.back(), tipPos_List.back(), neckPieceWidth);
		neck_Point_List.push_back(piecePoint);
	}

}

Vector2 PlayerNeckPendulumSupport::Pendulum(
	std::vector<Vector2>& fulcrum_List,
	std::vector<Vector2>& tipPos_List,
	std::vector<float>& rot_List,
	std::vector<float>& rot_Speed_List,
	std::vector<NeckPiecePoint>& neck_Point_List) {

	Vector2 beforeTipPos = tipPos_List.back();

	//現在の重りの位置
	for (int i = 0; i < fulcrum_List.size(); i++)
	{
		auto px = fulcrum_List[i].x + MathHelper::Cos(rot_List[i]) * (oneLength);
		auto py = fulcrum_List[i].y + MathHelper::Sin(rot_List[i]) * (oneLength);

		//重力移動量を反映した重りの位置
		auto vx = px - fulcrum_List[i].x;
		auto vy = py - fulcrum_List[i].y;
		auto t = -(vy * gravity) / (vx * vx + vy * vy);
		auto gx = px + t * vx;
		auto gy = py + gravity + t * vy;

		//２つの重りの位置の確度差
		auto rDiff = MathHelper::ATan(gy - fulcrum_List[i].y, gx - fulcrum_List[i].x);

		//角度差を角速度に加算
		auto sub = rDiff - rot_List[i];
		sub -= std::floor(sub / 360.0f) * 360.0f;
		if (sub < -180.0f) sub += 360.0f;
		if (sub > 180.0f) sub -= 360.0f;

		if (i == 0)rot_Speed_List[i] = rot_Speed_List[i] + sub;
		else rot_Speed_List[i] = (rot_Speed_List[i - 1] + sub) * 0.8f;


		//anyPos2[i] = fPos[i];


		//mRot_spd[i] = MathHelper::Clamp(mRot_spd[i],-3.f,3.f);
		//角度に角速度を加算
		if (i == 0) rot_List[i] += rot_Speed_List[i] * 0.1f;
		else rot_List[i] = rot_List[0] + rot_Speed_List[i] * 0.1f;

		//新しい重りの位置
		px = fulcrum_List[i].x + MathHelper::Cos(rot_List[i] /*+ rot2*/) * (oneLength);
		py = fulcrum_List[i].y + MathHelper::Sin(rot_List[i] /*+ rot2*/) * (oneLength);

		//重りの座標
		tipPos_List[i].x = px;
		tipPos_List[i].y = py;

		if (i > 0) fulcrum_List[i] = tipPos_List[i - 1];

		//頂点で正規化
		if (rot_List[i] > 270.0f) rot_List[i] -= 360.0f;
		if (rot_List[i] < -90) rot_List[i] += 360.0f;


		float friction = 0.985f;

		//振り子が90°を超えたら摩擦を極端に上昇させる
		if (tipPos_List[i].y < fulcrum_List.front().y) {
			friction = 0.9f; //上の振り子の重りより高く上がったら摩擦を増やす
		}


		rot_Speed_List[i] *= friction;


		NeckPiecePoint piecePoint = MathNeckPiecePoint(fulcrum_List[i], tipPos_List[i], neckPieceWidth);
		neck_Point_List[i] = piecePoint;

	}

	return tipPos_List.back() - beforeTipPos;

}

void PlayerNeckPendulumSupport::Deformation(
	const std::vector<Vector2>& fulcrum_List,
	const std::vector<Vector2>& tipPos_List,
	std::vector<NeckPiecePoint>& neck_Point_List
	)
{

	NeckPiecePoint p;
	for (int i = 0; i < neck_Point_List.size(); i++) {
		Vector2 v = fulcrum_List[i] - tipPos_List[i];
		Vector2 n = Vector2(-v.y, v.x).Normalize();
		if (i == 0) {
			p.fulcrumLeft = fulcrum_List[i] + n * neckPieceWidth * oneLength * 1.2f;
			p.fulcrumRight = fulcrum_List[i] - n * neckPieceWidth * oneLength * 1.2f;
			if (v.Length() > 0) {
				p.tipPosRight = tipPos_List[i] - n * neckPieceWidth * 1.2f;
				p.tipPosLeft = tipPos_List[i] + n * neckPieceWidth * 1.2f;
				p.tipPosRight -= v.Normalize() * oneLength;
				p.tipPosLeft -= v.Normalize() * oneLength;
				p.fulcrumLeft += v.Normalize() * oneLength;
				p.fulcrumRight += v.Normalize() * oneLength;
				neck_Point_List[i] = p;
			}
		}
		else {
			p.fulcrumLeft = fulcrum_List[i] + n * neckPieceWidth;
			p.fulcrumRight = fulcrum_List[i] - n * neckPieceWidth;
			if (v.Length() > 0) {
				p.tipPosRight = p.fulcrumRight - v.Normalize() * 15.f * 1.f;
				p.tipPosLeft = p.fulcrumLeft - v.Normalize() * 15.f * 1.f;
				p.fulcrumLeft += v.Normalize() * oneLength;
				p.fulcrumRight += v.Normalize() * oneLength;
				p.tipPosRight -= v.Normalize() * oneLength;
				p.tipPosLeft -= v.Normalize() * oneLength;
				neck_Point_List[i] = p;
			}
		}
	}

}
