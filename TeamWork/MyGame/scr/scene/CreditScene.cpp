#include "CreditScene.h"
#include "../conv/DXConverter.h"
#include "../fade/FadePanel.h"
#include "../math/MathHelper.h"
#include "../stageGenerator/Stage.h"
#include "../stageGenerator/Stage1/Stage1.h"
#include "../graphic/Sprite.h"
#include"../actor/Field/ClothesLine.h"
#include "../tween/TweenManager.h"
#include "addScreen/screenSupport/CreditTextGenerator.h"
#include "../time/Time.h"
#include "../Def.h"
#include "../actor/player/CreditPlayer.h"
#include "../scene/addScreen/screenSupport/CreditText.h"
#include "../sound/sound.h"
#include "../debugdata/DebugDraw.h"
#include "../actor/player/PlayerNeck/PlayerNeckPendulumSupport.h"
#include "../input/InputChecker.h"

//�w�i�F
const Vector3 BgColor[9]{ Vector3(155, 204, 255) ,Vector3(51, 204, 255) ,Vector3(0, 153, 255) ,
						  Vector3(0, 153, 235) ,  Vector3(204, 153, 0) , Vector3(153, 102, 51) ,
						  Vector3(0, 51, 102) , Vector3(0, 0, 102) ,  Vector3(155, 204, 255) };

//�R���X�g���N�^
CreditScene::CreditScene() :
	nextScene_(Scene::Title), isUseKey_(true), bgColorNum_(0), from_(0.0f), bgColor_(BgColor[0]), sceneTimer_(0.0f),
	waiting_(true), mulInit_(true)
{
	// ���[���h����
	world_ = std::make_shared<World>();
	// �C�x���g���X�i�[�o�^
	world_->AddEventMessageListener([=](EventMessage msg, void* param)
	{
		handleMessage(msg, param);
	});

	bgScreen_ = BackgroundScreen(world_.get());
}

//�f�X�g���N�^
CreditScene::~CreditScene()
{

}

//������
void CreditScene::Initialize()
{
	isEnd_ = false;

	world_->Initialize();

	player_ = std::make_shared<CreditPlayer>(world_.get());

	world_->Add(ACTOR_ID::PLAYER_ACTOR, player_);
	world_->PushStackActor(player_);

	std::string filename = "Tutorial1";
	Stage1 stage(world_.get(), filename);

	world_->InitializeInv(Vector2(player_->GetPosition().x, player_->GetPosition().y));

	world_->SetTarget(player_.get());

	bgScreen_.Init(Stage::Stage1);

	world_->Add(ACTOR_ID::LANE_ACTOR, std::make_shared<ClothesLine>(world_.get(), 1, stage.GetStageSize() + Vector2(WINDOW_WIDTH + 100, 0), Vector2(-150, 100)));

	//�J�n�n�_
	pHeadPos_ = startPos_;
	operate_ = true;
	sceneTimer_ = 0.0f;
	mulInit_ = true;
	waiting_ = true;

	//�J���X
	whitePos_ = { player_->GetCurrentPHeadPosition() + Corr + WhiteCorr,
				  Vector2(WINDOW_WIDTH / 2.0f, -300.0f) + WhiteCorr };
	dWhitePos_ = whitePos_[0];
	//�A�j���[�V�����ǂݍ���
	int crowIdNum = WHITE_ANM_01_SPRITE;
	for (int i = 0; i < 8; i++) {
		anmManager_.Add((SPRITE_ID)(crowIdNum + i));
	}
	anmManager_.SetIsRepeat(true);

	//�w�i�F
	SetBackgroundColor(bgColor_.x, bgColor_.y, bgColor_.z);
	TweenManager::GetInstance().Loop(Linear, &from_, 1.0f, 6.0f, [=]() {ChangeBackGroundColor(); });

	Sound::GetInstance().PlayBGM(BGM_ID::CREDIT_BGM, DX_PLAYTYPE_LOOP);

	world_->Add(ACTOR_ID::SAMPLE_ACTOR, std::make_shared<CreditTextGenerator>(world_.get(), Vector2(500, 100)));

	//�t�F�[�h�C��
	FadePanel::GetInstance().SetInTime(0.5f);
	FadePanel::GetInstance().FadeIn();
}

//�X�V
void CreditScene::Update()
{
	world_->Update();

	world_->SetScrollPos(Vector2::Zero);

	bgScreen_.Update();

	anmManager_.Update();

	whitePos_[0] = player_->GetCurrentPHeadPosition() + Corr + WhiteCorr;

	PlayerRestart();

	//�t�F�[�h���͈ȉ��̏������X�L�b�v
	if (!FadePanel::GetInstance().IsClearScreen()) return;

	sceneTimer_ += Time::DeltaTime;
	if (sceneTimer_ >= SceneTime) {
		//�V�[���J��
		FadePanel::GetInstance().AddCollBack([=]() { isEnd_ = true; });
		FadePanel::GetInstance().FadeOut();
	}

	//�w�i�F�Z�b�g
	SetBackgroundColor(bgColor_.x, bgColor_.y, bgColor_.z);
}

//�`��
void CreditScene::Draw() const
{
	//�w�i
	//bgScreen_.Draw();
	Sprite::GetInstance().Draw(CREDIT_BG_SPRITE, Vector2::Zero);
	Sprite::GetInstance().Draw(FENCE_SPRITE, Vector2::Zero);

	// �`��
	world_->Draw(3, world_->GetKeepDatas().playerLane_);

	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::BIRD_SPRITE);
	anmManager_.Draw(dWhitePos_, origin, Vector2::One, 1.0f);

	if (BuildMode != 1) return;
	DebugDraw::DebugDrawFormatString(0, 00, GetColor(255, 255, 255), "CreditScene");
	DebugDraw::DebugDrawFormatString(0, 20, GetColor(255, 255, 255), "FPS:[%.1f]", FPS::GetFPS);
	if (operate_) DebugDraw::DebugDrawFormatString(0, 40, GetColor(255, 255, 255), "����ł��܂��I");
	else DebugDraw::DebugDrawFormatString(0, 40, GetColor(255, 255, 255), "����ł��܂���I");
	DebugDraw::DebugDrawFormatString(0, 60, GetColor(255, 255, 255), "sceneTimer_:[%f]", sceneTimer_);
	DebugDraw::DebugDrawFormatString(0, 80, GetColor(255, 255, 255), "bgColorNum_:%d", bgColorNum_);
}

//�I�����Ă��邩�H
bool CreditScene::IsEnd() const
{
	return isEnd_;
}

//��
Scene CreditScene::Next() const
{
	return nextScene_;
}

//�I��������
void CreditScene::End()
{
	// ������
	world_->Clear();
	player_.reset();
	credit_.reset();
	bgScreen_.End();
	TweenManager::GetInstance().Cancel(&from_);
	TweenManager::GetInstance().Cancel(&bgColor_);

	Sound::GetInstance().StopBGM();
}

//���b�Z�[�W����
void CreditScene::handleMessage(EventMessage message, void * param)
{
	switch (message)
	{
	case EventMessage::PLAYER_POS_RESET:
	{
		//�X�^�[�g�n�_�ɕ��A
		RestartSet();
		break;
	}
	default:
		break;
	}
}

//��ʊO���H
bool CreditScene::ScreenOut() const
{
	if (player_->GetCurrentPHeadPosition().x < -300.0f - Corr.x || player_->GetCurrentPHeadPosition().x > WINDOW_WIDTH + 300.0f - Corr.x ||
		player_->GetCurrentPHeadPosition().y < -300.0f - Corr.y || player_->GetCurrentPHeadPosition().y > WINDOW_HEIGHT + 300.0f - Corr.y)
		return true;
	else
		return false;
}

//�v���C���[���X�^�[�g
void CreditScene::PlayerRestart()
{
	//����s�\��
	if (!operate_) {
		NotOperate();
	}

	//��ʊO�ɏo����
	if (ScreenOut() && operate_) {
		RestartSet();
	}

	//�X�^�[�g�ʒu�ɖ߂�����
	Vector2 dis = player_->GetCurrentPHeadPosition() - startPos_;
	if (dis.Length() <= 2.0f && !operate_) {
		PlayerStart();
	}

	//�ҋ@��
	if (waiting_ && (isUseKey_.KeyTriggerDown(InputChecker::Input_Key::B) || InputChecker::GetInstance().KeyTriggerDown(InputChecker::Input_Key::X))) {
		TweenManager::GetInstance().Add(EaseInQuart, &dWhitePos_, whitePos_[1], 2.0f);
		waiting_ = false;
	}
}

//���X�^�[�g�Z�b�g
void CreditScene::RestartSet()
{
	operate_ = false;
	player_->AllResurrectHead();
	player_->SetIsBiteMode(false);
	player_->CurHeadBite(pHeadPos_);
	player_->PHeadLengthReset();
	pHeadPos_ = player_->GetCurrentPHeadPosition(); //�v���C���[���W
	player_->SetOtherClothesID_(CLOTHES_ID::FLUFFY_CLOTHES);
	TweenManager::GetInstance().Add(EaseOutQuart, &pHeadPos_, startPos_, 2.0f);
	//���A���͑�����󂯕t���Ȃ�
	int flag = 0;
	world_->sendMessage(EventMessage::OPERATE_FLAG, (void*)flag);

	Sound::GetInstance().PlaySE(SE_ID::CANCEL_SE);
}

//����s�\��
void CreditScene::NotOperate()
{
	player_->SetIsBiteMode(true);
	player_->setCurPHeadSPos(pHeadPos_);
	player_->GetCurrentHead()->SetPose(Matrix::CreateTranslation(Vector3(pHeadPos_.x, pHeadPos_.y, 0)));
	dWhitePos_ = whitePos_[0];
	if (mulInit_) {
		player_->MultipleInit(110.0f, player_->GetCurrentPHeadPosition(), player_->GetRot(), 60.0f);
		mulInit_ = false;
	}
}

//�X�^�[�g��
void CreditScene::PlayerStart()
{
	player_->SetIsBiteMode(true);
	player_->GravityReset();
	operate_ = true;
	mulInit_ = true;
	waiting_ = true;
	int flag = 1;
	world_->sendMessage(EventMessage::OPERATE_FLAG, (void*)flag);
}

//�w�i�F�ύX
void CreditScene::ChangeBackGroundColor()
{
	bgColorNum_++;
	bgColorNum_ = MathHelper::Clamp(bgColorNum_, 0, 8);
	TweenManager::GetInstance().Add(Linear, &bgColor_, BgColor[bgColorNum_], 3.0f);
}
