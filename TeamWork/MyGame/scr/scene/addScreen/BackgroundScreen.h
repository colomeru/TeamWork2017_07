#pragma once
#include <vector>
#include"../Scene.h"
#include"../../math/Vector2.h"

class IWorld;
class BackgroundScreen
{
public:
	// コンストラクタ
	BackgroundScreen(IWorld* world);

	void Init();
	void Update();
	// 描画
	void Draw() const;
private:
	IWorld* world_;

	Vector2 drawPos_;
};