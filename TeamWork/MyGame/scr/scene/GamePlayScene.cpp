#include "GamePlayScene.h"
#include "../math/MathHelper.h"
#include "../time/Time.h"
#include "../fade/FadePanel.h"

#include "../actor/SampleActor.h"

GamePlayScene::GamePlayScene() :
nextScene_(Scene::Credit)
{
	// ���[���h����
	world_ = std::make_shared<World>();
	// �C�x���g���X�i�[�o�^
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});
}

GamePlayScene::~GamePlayScene()
{
}

void GamePlayScene::Initialize()
{
	isEnd_ = false;

	// �t�F�[�h�p�l��������
	FadePanel::GetInstance().Initialize();

	// �A�N�^�[����
	world_->Add(ACTOR_ID::SAMPLE_ACTOR, std::make_shared<SampleActor>(world_.get()));
}

void GamePlayScene::Update()
{
	// �X�V
	world_->Update();

	// �I��
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;
}

void GamePlayScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "GamePlayScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);

	// �`��
	world_->Draw();
}

bool GamePlayScene::IsEnd() const
{
	return isEnd_;
}

Scene GamePlayScene::Next() const
{
	return nextScene_;
}

void GamePlayScene::End()
{
	// ������
	world_->Clear();
}

void GamePlayScene::handleMessage(EventMessage message, void * param)
{
}
