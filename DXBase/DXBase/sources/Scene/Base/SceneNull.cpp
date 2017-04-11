#include "SceneNull.h"

void SceneNull::start() {}

void SceneNull::update(){}

void SceneNull::draw() const{}

void SceneNull::end() {}

bool SceneNull::isEnd() const { return false; }

Scene SceneNull::next() const { return Scene::Null; }

