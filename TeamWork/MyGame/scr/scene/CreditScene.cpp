#include "CreditScene.h"
#include "../time/Time.h"
#include "../conv/DXConverter.h"

#include "../actor/player/Player3.h"
#include "../actor/enemy/Enemy.h"
#include "../graphic/Model.h"
#include "../math/MathHelper.h"
#include "../stageGenerator/Stage.h"
#include "../stageGenerator/Stage1/Stage1.h"
#include "../actor/Field/Clothes/TestClothes.h"

CreditScene::CreditScene() :
nextScene_(Scene::Ending),
camera_pos_(0, 100, -100),
target_(0, 0, 0)
{
	// ���[���h����
	world_ = std::make_shared<World>();
	// �C�x���g���X�i�[�o�^
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});
}

CreditScene::~CreditScene()
{
}

void CreditScene::Initialize()
{
	isEnd_ = false;

	Camera::GetInstance().SetRange(0.1f, 10000.0f);
	Camera::GetInstance().SetViewAngle(60.0f);
	Camera::GetInstance().Up.Set(Vector3::Up);
	Camera::GetInstance().Position.Set(camera_pos_);
	Camera::GetInstance().Target.Set(target_);
	Camera::GetInstance().Update();

	stageGeneratorManager.Add(Stage::Stage1, std::make_shared<Stage1>(world_.get(), std::string("Stage1")));
	stageGeneratorManager.SetStage(Stage::Stage1);

	world_->Add(ACTOR_ID::PLAYER_ACTOR, std::make_shared<Player3>(world_.get()));
	//world_->Add(ACTOR_ID::ENEMY_ACTOR, std::make_shared<Enemy>(world_.get()));

}

void CreditScene::Update()
{
	// �X�V
	world_->Update();

	Camera::GetInstance().Position.Set(camera_pos_);
	Camera::GetInstance().Target.Set(target_);
	Camera::GetInstance().Update();

	// �I��
	if (Keyboard::GetInstance().KeyTriggerDown(KEYCODE::SPACE))
		isEnd_ = true;
}

void CreditScene::Draw() const
{
	DrawFormatString(0, 00, GetColor(255, 255, 255), "CreditScene");
	DrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);

	// �`��
	world_->Draw();
}

bool CreditScene::IsEnd() const
{
	return isEnd_;
}

Scene CreditScene::Next() const
{
	return nextScene_;
}

void CreditScene::End()
{
	// ������
	world_->Clear();
}

void CreditScene::handleMessage(EventMessage message, void * param)
{
}

