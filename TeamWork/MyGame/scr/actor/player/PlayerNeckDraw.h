#pragma once
#include"Player.h"

class PlayerNeckDraw {
public:
	PlayerNeckDraw() {
		ids_.push_back(SPRITE_ID::NECK_CHAIN1_SPRITE);
		ids_.push_back(SPRITE_ID::NECK_CHAIN2_SPRITE);
	}
	~PlayerNeckDraw() {
		ids_.clear();
	}
	void Draw(const Vector2& basePos, const Vector2& lastPos)const {
		Vector2 toV = lastPos - basePos;
		float leng=toV.Length();
		float count=leng/Sprite::GetInstance().GetSize(SPRITE_ID::NECK_CHAIN1_SPRITE).y;
		int fulldrawCount = (int)floorf(count);
		float surp = count - (float)fulldrawCount;
		Vector2 addvec=Vector2::Zero;
		Vector2 drawpos=basePos;
		float angle = MathAngle(toV);
		Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::NECK_CHAIN1_SPRITE).x;
		origin.x /= 2.f;
		for (int i = 0; i < fulldrawCount; i++) {
			addvec = toV.Normalize()*Sprite::GetInstance().GetSize(SPRITE_ID::NECK_CHAIN1_SPRITE).y;
			addvec *= i;
			drawpos = basePos + addvec;
			int drawSpr = i % 2;
			Sprite::GetInstance().Draw(ids_.at(drawSpr), drawpos, origin, Vector2::One, angle);
		}
		addvec = toV.Normalize()*Sprite::GetInstance().GetSize(SPRITE_ID::NECK_CHAIN1_SPRITE).y*surp;
		if (surp >= 0.05f) {
			//DrawCircle((drawpos + addvec).x, (drawpos + addvec).y, 3, GetColor(255, 0, 0));
			Sprite::GetInstance().Draw(ids_.at(1), drawpos + addvec, origin, Vector2::One*surp, angle, TRUE, FALSE);
		}
	}

private:
	std::vector<SPRITE_ID> ids_;
};