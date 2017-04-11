#include "SceneMgr.h"
#include "SceneNull.h"
#include"../../ResourceLoader/ResourceLoader.h"

static const int fadeoutSpeed = 20;
static const int fadeinSpeed = 20;

SceneMgr::SceneMgr(SceneDataKeeper* keeper) :
	currentScene_(std::make_shared<SceneNull>()),absnum(0) {
	keeper_ = keeper;
	isFirst_ = false;
	isMove_ = false;
}

void SceneMgr::init() {
	scenes_.clear();
}

void SceneMgr::update() {
	//フェードが終わるまでInputを制限する
	if(!changer_.getEnd())
	{
		changer_.update(); 
	}
	else {
		isFirst_ = true;
		isMove_ = true;
	}
	if (currentScene_->isEnd())
	{
		sceneChanger();
	}

	if (changer_.getIsSlimeMax()) {
		currentScene_->update();
	}
	

	changer_.getIsSlimeMax() ? absnum-= fadeinSpeed : absnum+=fadeoutSpeed;
	absnum = max(min(absnum, 255), 0);

}

void SceneMgr::draw() const {
	currentScene_->draw();

	if (!changer_.getEnd())
	{
		int mas = SetDrawBlendMode(DX_BLENDMODE_ALPHA, abs(absnum));
		DrawGraph(0, 0, ResourceLoader::GetInstance().getTextureID(TextureID::BLACK_BACK_TEX), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, mas);

		changer_.draw();
	}
}

void SceneMgr::end() {
	currentScene_->end();
	currentScene_ = std::make_shared<SceneNull>();
}

void SceneMgr::add(Scene name, const IScenePtr& scene) {
	scenes_[name] = scene;
}

void SceneMgr::change(Scene name) {
	keeper_->setSceneName(currentScene_->getName());
	currentScene_->isEnd_ = false;
	end();
	currentScene_ = scenes_[name];
	currentScene_->start();
}

void SceneMgr::sceneChanger()
{
	if (isMove_)
	{
		changer_.start(currentScene_->getName(),currentScene_->nextName());
		isMove_ = false;
	}
	if (isFirst_&&changer_.getIsSlimeMax()) {
		change(currentScene_->next());
		isFirst_ = false;
	}

}
