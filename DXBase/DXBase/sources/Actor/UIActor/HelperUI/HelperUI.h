#ifndef HELPER_UI_H_
#define HELPER_UI_H_

#include "../../Base/ActorUI.h"
#include <vector>

class HelperUI : public ActorUI {
public:
	HelperUI(IWorld* world, const Vector2& position);
	~HelperUI();
	void onUpdate(float deltaTime) override;
	void onDraw() const override;

public:
	// 表示するテキストの番号を設定します
	void setTextNumver(const int number);
	// 文字を描画するかを返します
	void setIsTextDraw(const bool isDraw);

private:
	int textNum_;		// 描画する文字番号
	bool isTextDraw_;	// 文字を描画するか 
	// 文字コンテナ
	typedef std::vector<int> TextContainer;
	TextContainer drawTexes_;
};

#endif