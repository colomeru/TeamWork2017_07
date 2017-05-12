#pragma once
#include <vector>
#include"../Scene.h"
#include"../../math/Vector2.h"

class IWorld;
class BackgroundScreen
{
public:
	// �R���X�g���N�^
	BackgroundScreen(IWorld* world);

	void Init();
	void Update();
	// �`��
	void Draw() const;
private:
	IWorld* world_;

	Vector2 drawPos_;
};