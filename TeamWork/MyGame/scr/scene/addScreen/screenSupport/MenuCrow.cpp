#include "MenuCrow.h"
#include "../../../tween/TweenManager.h"
#include "../../../Def.h"
#include "../../../time/Time.h"
#include "../../../graphic/Sprite.h"
#include "../../../debugdata/DebugDraw.h"

//�R���X�g���N�^
MenuCrow::MenuCrow() :
	position_(Vector2::Zero), from_(Vector2(0.0f, 0.0f)), interval_(0.0f), timer_(0.0f), velocity_(Vector2(-5.0f, 0.0f)), dis_(0.0f), state_(State::Idle)
{
	//�A�j���[�V�����ǂݍ���
	int crowIdNum = CROW_ANM_01_SPRITE;
	for (int i = 0; i < 8; i++) {
		anmManager_.Add((SPRITE_ID)(crowIdNum + i));
	}
	anmManager_.SetIsRepeat(true);
}

//�f�X�g���N�^
MenuCrow::~MenuCrow()
{
}

//�X�V
void MenuCrow::Update(const int stageNum)
{
	//�A�j���[�V�����X�V
	anmManager_.Update();

	//��Ԃɍ��킹���X�V
	switch (state_)
	{
	case State::Move:
		Move();
		break;
	case State::Idle:
		if (stageNum >= 7) return;
		Idle();
		break;
	}
}

//�`��
void MenuCrow::Draw() const
{
	Vector2 origin = Sprite::GetInstance().GetSize(SPRITE_ID::BIRD_SPRITE);
	anmManager_.Draw(position_ + from_, origin, Vector2::One, 1.0f);
}

//�ړI�n���Z
void MenuCrow::AddDistance(float betDis)
{
	if (timer_ <= interval_) return;
	//�p�l���ɍ��킹�ďc�ړ�����
	dis_ += betDis;
	TweenManager::GetInstance().Add(EaseOutExpo, &from_, Vector2(0.0f, dis_), MoveTime);
}

//������
void MenuCrow::Initialize(Vector2 position, float interval)
{
	position_ = position;
	interval_ = interval;
}

//�ړ�
void MenuCrow::Move()
{
	position_ += velocity_;

	if (position_.x <= -OffSet) {//��ʊO�ɏo����ҋ@�ʒu�ɖ߂�
		timer_ = 0.0f;
		position_.x = WINDOW_WIDTH + OffSet;
		state_ = State::Idle;
	}
}

//�ҋ@
void MenuCrow::Idle()
{
	timer_ += Time::DeltaTime;
	from_ = Vector2(0.0f, 0.0f);
	if (timer_ >= interval_) { //�ҋ@���Ԍ�Ɉړ��J
		//�c�ړ����Z�b�g
		dis_ = 0.0f;
		from_ = 0.0f;
		state_ = State::Move;
	}
}
