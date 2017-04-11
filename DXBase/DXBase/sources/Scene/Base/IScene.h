#ifndef _ISCENE_H_
#define _ISCENE_H_

#include "DxLib.h"

#include "Scene.h"

#include "../../Game/Time.h"
#include"../Base/SceneDataKeeper.h"
#include"../../Renderer/FontManager.h"
#include"../../Define.h"
#include<vector>

//シーン抽象インターフェース
class IScene{
public:
	//仮想デストラクタ
	virtual ~IScene(){}
	//初期化
	virtual void start() = 0;
	//更新
	virtual void update() = 0;
	//描画
	virtual void draw() const = 0;
	//終了フラグ
	virtual bool isEnd() const = 0;
	//次のシーン
	virtual Scene next() const = 0;
	//終了
	virtual void end() = 0;
	std::string getName() {
		return name_;
	}
	virtual std::string nextName() {
		return "next";
	}
	void setName(std::string name) {
		name_=name;
	}
public:
	bool isEnd_;
	SceneDataKeeper* keeper_;
	std::string name_;
	std::vector<std::string> changeTextList;
	std::vector<std::vector<std::string>> listBase;
	std::vector<Vector2> textPosList;
};

#endif