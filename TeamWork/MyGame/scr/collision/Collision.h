//#pragma once
//#include "CollisionParameter.h"
//#include "../math/MathHelper.h"
//#include "../actor/Actor.h"
//
//
//// 線分
//struct Line
//{
//	Vector3 startPos = Vector3::Zero;
//	Vector3 endPos	 = Vector3::Zero;
//
//	Line(Vector3& start, Vector3& end) :
//		startPos(start), endPos(end)
//	{}
//};
//
//// 球
//struct Sphere
//{
//	Vector3	centerPos = Vector3::Zero;
//	float	radius	  = 0.0f;
//
//	Sphere(Vector3& center, float r) :
//		centerPos(center), radius(r)
//	{}
//
//	explicit Sphere(ActorParameter& param) :
//		centerPos(param.mat.Translation()), radius(param.radius)
//	{
//		//centerPos = param.mat.Translation();
//		//radius	  = param.radius;
//	}
//};
//
//// カプセル
//struct Capsule
//{
//	Vector3 startPos = Vector3::Zero;
//	Vector3 endPos	 = Vector3::Zero;
//	float	radius	 = 0.0f;
//
//	Capsule(Vector3& start, Vector3& end, float radius) :
//		startPos(start), endPos(end), radius(radius)
//	{}
//
//	explicit Capsule(ActorParameter& param) :
//		startPos(param.mat.Translation() + Vector3(0.0f, param.size.y - param.radius, 0.0f)),
//		endPos(endPos = param.mat.Translation() + Vector3(0.0f, param.radius, 0.0f)),
//		radius(param.radius)
//	{}
//};
//
//// ポリゴン
//struct Triangle
//{
//	Vector3	position[3];
//
//	Triangle(Vector3& p1, Vector3& p2, Vector3& p3)
//	{
//		position[0] = p1;
//		position[1] = p2;
//		position[2] = p3;
//	}
//};
//
//// 箱
//struct Box
//{
//	// 箱の中心点
//	Vector3 centerPos;
//	// 分離軸
//	// [0]:Right
//	// [1]:Up
//	// [2]:Backward (MyMatrix Dxlib による)
//	Vector3 rotate[3];
//	// ３辺の半分の長さ
//	Vector3 halfSize;
//
//	//
//	Box(Vector3& center, Vector3& x, Vector3& y, Vector3& z, Vector3& size) :
//		centerPos(center), rotate(), halfSize(size / 2.0f)
//	{
//		rotate[0] = x;
//		rotate[1] = y;
//		rotate[2] = z;
//	}
//
//	explicit Box(ActorParameter& param) :
//		centerPos(param.mat.Translation())
//	{
//		rotate[0] = param.mat.Right();
//		rotate[1] = param.mat.Up();
//		rotate[2] = param.mat.Backward();
//		halfSize  = param.size / 2.0f;
//	}
//};
//
//
//struct Collision
//{
//
//	// 球とOBBの衝突判定
//	static CollisionParameter Sphere_vs_OBB(CollisionParameter& colpara, Sphere& sphere, Box& box);
//	// カプセルとカプセルの衝突判定
//	static CollisionParameter Capsule_vs_Capsule(CollisionParameter& colpara, Capsule& c1, Capsule& c2);
//	
//	// OBBと点の最接近点を求める
//	static void ClosestPtPointOBB(CollisionParameter& colpara, Vector3& point, Box& box);
//
//};