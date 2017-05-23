#include "ResultScene.h"
#include "../Def.h"
#include "../camera/Camera.h"
ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
}

void ResultScene::Update()
{
}

void ResultScene::Draw() const
{
}

bool ResultScene::IsEnd() const
{
	return false;
}

Scene ResultScene::Next() const
{
	return Scene();
}

void ResultScene::End()
{
}

void ResultScene::handleMessage(EventMessage message, void * param)
{
}
