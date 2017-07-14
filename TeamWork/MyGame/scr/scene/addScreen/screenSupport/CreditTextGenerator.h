#pragma once
#include "../../../actor/Actor.h"
#include <vector>


class CreditTextGenerator : public Actor {
public:
	//コンストラクタ
	CreditTextGenerator(IWorld* world, Vector2 position);
	//デストラクタ
	~CreditTextGenerator();
	//更新
	virtual void Update() override;
	//描画
	virtual void Draw() const override;

private:
	//生成
	void ClothAdd();

private:
	int frame_;							//フレーム数
	std::vector<SPRITE_ID> id_;			//リソースID
	int idCount_;						//ID変更用カウント
	float loopHandle_;					//

	std::vector<SPRITE_ID> postId_;		//リソースID
	std::vector<Vector2> postSize_;		//リソースサイズ

};
