#pragma once

class IWorld;
class Actor;
struct CollisionParameter;

class CollisionFunction
{
public:
	// コンストラクタ
	explicit CollisionFunction(IWorld* world); 

	// スプライトの当たり判定（２次元のOBB vs OBB）
	CollisionParameter IsHit_OBB_OBB(const Actor& sprite1, const Actor& sprite2);
	// スプライトの当たり判定（２次元のPlayerの頭 vs 服）
	CollisionParameter IsHit_PHead_Clothes(const Actor& sprite1, const Actor& sprite2);
	// スプライトの当たり判定（２次元のPlayerの頭 vs ハンガー）
	CollisionParameter IsHit_PHead_Hanger(const Actor& sprite1, const Actor& sprite2);
	// スプライトの当たり判定（２次元のプレイヤーの頭（円） vs 服（カプセル３つ））
	CollisionParameter IsHit_Circle_Capsules(const Actor& sprite1, const Actor& sprite2);
	// スプライトの当たり判定（２次元のOBB vs 線分）
	CollisionParameter IsHit_OBB_Segment(const Actor& sprite1, const Actor& sprite2);
	// スプライトの当たり判定（服（線分））vs ２次元のプレイヤーの剣（線分）
	CollisionParameter IsHit_PSword_Clothes(const Actor& sprite1, const Actor& sprite2);
	// スプライトの当たり判定（２次元のプレイヤーの剣（線分）vs (洗濯物叩き(円)）
	CollisionParameter IsHit_Tapper_PSword(const Actor& sprite1, const Actor& sprite2);
	// スプライトの当たり判定（毛玉（円） vs ２次元のプレイヤーの剣（線分））
	CollisionParameter IsHit_Hairball_PSword(const Actor& sprite1, const Actor& sprite2);
	// スプライトの当たり判定（鳥の糞（円）vs 服（OBB））
	CollisionParameter IsHit_Droping_Clothes(const Actor& sprite1, const Actor& sprite2);
	// スプライトの当たり判定（２次元の線分 vs 線分）
	CollisionParameter IsHit_Segment_Segment(const Actor& sprite1, const Actor& sprite2);
	// スプライトの当たり判定（２次元の円 vs 円）
	CollisionParameter IsHit_Circle_Circle(const Actor& sprite1, const Actor& sprite2);
	// スプライトの当たり判定（２次元の円 vs 線分）
	CollisionParameter IsHit_Circle_Segment(const Actor& sprite1, const Actor& sprite2);
	// スプライトの当たり判定（２次元のOBB vs 円）
	CollisionParameter IsHit_OBB_Circle(const Actor& sprite1, const Actor& sprite2);
	// スプライトの当たり判定（２次元のOBB vs 服）
	CollisionParameter IsHit_OBB_Clothes(const Actor& sprite1, const Actor& sprite2);
	CollisionParameter IsHit_Clothes_OBB(const Actor& sprite1, const Actor& sprite2);


private:
	// ワールド
	IWorld*	world_;
};