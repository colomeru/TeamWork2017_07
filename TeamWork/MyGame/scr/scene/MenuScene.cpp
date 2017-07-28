#include "MenuScene.h"
#include "../time/Time.h"
#include "../math/MathHelper.h"
#include "../graphic/Sprite.h"
#include "../fade/FadePanel.h"
#include "../sound/sound.h"

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
	menu.Init();
	nextScene_ = Scene::GamePlay;
	FadePanel::GetInstance().SetInTime(0.5f);
	FadePanel::GetInstance().FadeIn();
}

void MenuScene::Update()
{

	// �X�V
	world_->Update();

	menu.Crow();
	menu.ShootingStar();

	if (!FadePanel::GetInstance().IsClearScreen()) return;

	// �I��
	if ((Keyboard::GetInstance().KeyTriggerDown(KEYCODE::M) ||
		GamePad::GetInstance().ButtonTriggerDown(PADBUTTON::NUM2)))
	{
		if (menu.GetIsBackSelect())
			nextScene_ = Scene::Title;
		else if (menu.GetIsTutorialSelect())
			nextScene_ = Scene::Tutorial;
		FadePanel::GetInstance().AddCollBack([=] {
			isEnd_ = true;
		});
		FadePanel::GetInstance().FadeOut();
		menu.InputSelectStage();
	}

	menu.Update();
}

void MenuScene::Draw() const
{
	menu.Draw();
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
	menu.End();
	world_->Clear();
}

void MenuScene::handleMessage(EventMessage message, void * param)
{
}

Stage MenuScene::SendStage() const
{
	return menu.GetGamePlayStage();
}
