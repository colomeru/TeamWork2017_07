#include "EndingScene.h"
#include "../time/Time.h"

EndingScene::EndingScene() :
nextScene_(Scene::Title)
{
	// ���[���h����
	world_ = std::make_shared<World>();
	// �C�x���g���X�i�[�o�^
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});
}

EndingScene::~EndingScene()
{
}

void EndingScene::Initialize()
{
	isEnd_ = false;
}

void EndingScene::Update()
{
	// �X�V
	world_->Update();

	// �I��
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;
}

void EndingScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "EndingScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);

	// �`��
	world_->Draw();
}

bool EndingScene::IsEnd() const
{
	return isEnd_;
}

Scene EndingScene::Next() const
{
	return nextScene_;
}

void EndingScene::End()
{
	// ������
	world_->Clear();
}

void EndingScene::handleMessage(EventMessage message, void * param)
{
}
