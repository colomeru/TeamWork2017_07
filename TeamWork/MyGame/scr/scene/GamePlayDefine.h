#pragma once
#include "../stageGenerator/Stage.h"

static const float mxmSize = 1.5f;
static const int defSinC = 90;
//���������܂ł̊�{����(�t���[����)
static int defWindTime_ = 800;
static const void setWindTime(Stage name) {
	switch (name)
	{
	case Stage::Stage1: {
		defWindTime_ = 800;
	}
	case Stage::Stage2: {
		defWindTime_ = 1000;
	}
	case Stage::Stage3: {
		defWindTime_ = 1200;
	}
	}
}
//�Q�[���J�n���̑S�̎���
static const float startEase = 4.f;
//�{�^�������Ă�Ƃ��ɐi�߂鎞��
static const float startAddTime = 0.005f;
//��ʓ��̕����h���m���i���j
static const int frequencyWind = 30;
//�ыʂ����ł���m���i���j
static const int frequencyHairBall = 30;
