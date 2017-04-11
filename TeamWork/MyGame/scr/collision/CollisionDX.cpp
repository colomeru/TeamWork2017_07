#include "CollisionDX.h"
#include "../DxLib/DXConverter.h"
#include "../math/MathHelper.h"
#include "../math/Vector3.h"

bool CollisionDX::Capsule_vs_Capsule(const Capsule & c1, const Capsule & c2)
{
	int result = HitCheck_Capsule_Capsule(
		DXConverter::GetInstance().ToVECTOR(c1.startPos),
		DXConverter::GetInstance().ToVECTOR(c1.endPos),
		c1.radius,
		DXConverter::GetInstance().ToVECTOR(c2.startPos),
		DXConverter::GetInstance().ToVECTOR(c2.endPos),
		c2.radius);

	if (result != 0)	
		return true;	
	else				
		return false;
}

CollisionParameter CollisionDX::Capsule_vs_Capsule(CollisionParameter & colpara, const Capsule & c1, const Capsule & c2)
{
	int result = HitCheck_Capsule_Capsule(
		DXConverter::GetInstance().ToVECTOR(c1.startPos),
		DXConverter::GetInstance().ToVECTOR(c1.endPos),
		c1.radius,
		DXConverter::GetInstance().ToVECTOR(c2.startPos),
		DXConverter::GetInstance().ToVECTOR(c2.endPos),
		c2.radius);

	if (result != 0)
		colpara.colFrag = true;
	else
		colpara.colFrag = false;

	return colpara;
}

bool CollisionDX::Capsule_vs_Triangle(const Capsule & capsule, const Triangle & poly)
{
	int result = HitCheck_Capsule_Triangle(
		DXConverter::GetInstance().ToVECTOR(capsule.startPos),
		DXConverter::GetInstance().ToVECTOR(capsule.endPos),
		capsule.radius,
		DXConverter::GetInstance().ToVECTOR(poly.position[0]),
		DXConverter::GetInstance().ToVECTOR(poly.position[1]),
		DXConverter::GetInstance().ToVECTOR(poly.position[2]));

	if (result != 0)	return true;
	return false;
}

CollisionParameter CollisionDX::Capsule_vs_Triangle(CollisionParameter & colpara, const Capsule & capsule, const Triangle & poly)
{
	int result = HitCheck_Capsule_Triangle(
		DXConverter::GetInstance().ToVECTOR(capsule.startPos),
		DXConverter::GetInstance().ToVECTOR(capsule.endPos),
		capsule.radius,
		DXConverter::GetInstance().ToVECTOR(poly.position[0]),
		DXConverter::GetInstance().ToVECTOR(poly.position[1]),
		DXConverter::GetInstance().ToVECTOR(poly.position[2]));

	if (result != 0)
		colpara.colFrag = true;
	else
		colpara.colFrag = false;

	return colpara;
}

CollisionParameter CollisionDX::Model_vs_Line(CollisionParameter & colpara, const ModelData & model, const Line & line)
{
	MV1_COLL_RESULT_POLY HitPoly;

	HitPoly = MV1CollCheck_Line(
		model.modelHandle, model.modelFrameIndex,
		DXConverter::GetInstance().ToVECTOR(line.startPos),
		DXConverter::GetInstance().ToVECTOR(line.endPos));

	if (HitPoly.HitFlag)
	{
		colpara.colFrag = true;
		colpara.colPos = DXConverter::GetInstance().ToVector3(HitPoly.HitPosition);
	}

	return colpara;
}

CollisionParameter CollisionDX::Model_vs_Sphere(CollisionParameter & colpara, const ModelData & model, const Sphere & sphere)
{
	return CollisionParameter();
}

CollisionParameter CollisionDX::Model_vs_Capsule(CollisionParameter & colpara, std::vector<MV1_COLL_RESULT_POLY>& colpoly, const ModelData & model, const Capsule & capsule, ModelCollType type)
{
	MV1_COLL_RESULT_POLY_DIM HitPolyDim;
	Capsule c(capsule);

	HitPolyDim = MV1CollCheck_Capsule(
		model.modelHandle, model.modelFrameIndex,
		DXConverter::GetInstance().ToVECTOR(c.startPos),
		DXConverter::GetInstance().ToVECTOR(c.endPos),
		c.radius);

	if (HitPolyDim.HitNum != 0)
	{
		for (int i = 0; i < HitPolyDim.HitNum; i++)
		{
			switch (type)
			{
			case All:
			{
				colpoly.push_back(HitPolyDim.Dim[i]);
				break;
			}
			case Wall:
			{
				auto angle = Vector3::Dot(Vector3::Up, DXConverter::GetInstance().ToVector3(HitPolyDim.Dim[i].Normal));
				if (MathHelper::Abs(angle) <= 0.5f)
					colpoly.push_back(HitPolyDim.Dim[i]);
				break;
			}
			case Floor:
			{
				auto angle = Vector3::Dot(Vector3::Up, DXConverter::GetInstance().ToVector3(HitPolyDim.Dim[i].Normal));
				if (MathHelper::Abs(angle) > 0.5f)
					colpoly.push_back(HitPolyDim.Dim[i]);
				break;
			}}
		}

		colpara.colFrag = true;
	}

	MV1CollResultPolyDimTerminate(HitPolyDim);
	return colpara;
}
