#include "MenuScene.h"
#include "../time/Time.h"

MenuScene::MenuScene() :
nextScene_(Scene::GamePlay)
{
	// ���[���h����
	world_ = std::make_shared<World>();
	// �C�x���g���X�i�[�o�^
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});
}

MenuScene::~MenuScene()
{
}

void MenuScene::Initialize()
{
	isEnd_ = false;
}

void MenuScene::Update()
{
	// �X�V
	world_->Update();

	// �I��
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;
}

void MenuScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "MenuScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);

	// �`��
	world_->Draw();
}

bool MenuScene::IsEnd() const
{
	return isEnd_;
}

Scene MenuScene::Next() const
{
	return nextScene_;
}

void MenuScene::End()
{
	// ������
	world_->Clear();
}

void MenuScene::handleMessage(EventMessage message, void * param)
{
}
