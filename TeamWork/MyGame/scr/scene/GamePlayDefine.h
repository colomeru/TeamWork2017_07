#pragma once
#include "../stageGenerator/Stage.h"

static const float mxmSize = 1.5f;
static const int defSinC = 90;
//���������܂ł̊�{����(�t���[����)
static const int defWindTime[8]{
800,//�X�e�[�W1
800,//�X�e�[�W2
800,//�X�e�[�W3
800,//�X�e�[�W4
800,//�X�e�[�W5
800,//�X�e�[�W6
800,//�X�e�[�W7
800	//�X�e�[�W8
};

//static const void setWindTime(Stage name) {
//	switch (name)
//	{
//	case Stage::Stage1: {
//		defWindTime_ = 800;
//	}
//	case Stage::Stage2: {
//		defWindTime_ = 1000;
//	}
//	case Stage::Stage3: {
//		defWindTime_ = 1200;
//	}
//	}
//}
//�Q�[���J�n���̑S�̎���
static const float startEase = 4.f;
//�{�^�������Ă�Ƃ��ɐi�߂鎞��
static const float startAddTime = 0.005f;
//��ʓ��̕����h���m���i���j
static const int frequencyWind = 30;
//�ыʂ����ł���m���i���j
static const int frequencyHairBall = 30;
