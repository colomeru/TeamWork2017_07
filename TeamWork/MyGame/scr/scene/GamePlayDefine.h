#pragma once
#include "../stageGenerator/Stage.h"

static const float mxmSize = 1.5f;
static const int defSinC = 90;


//��3�ɕK�v�ȃX�R�A�̃��X�g
static const int ScoreList[8]{
	1000,//�X�e�[�W1
	1500,//�X�e�[�W2
	2000,//�X�e�[�W3
	2500,//�X�e�[�W4
	3000,//�X�e�[�W5
	3500,//�X�e�[�W6
	4000,//�X�e�[�W7
	5000 //�X�e�[�W8
};

//���������܂ł̊�{����(�t���[����)
static const int defWindTime[8]{
800,//�X�e�[�W1
800,//�X�e�[�W2
500,//�X�e�[�W3
800,//�X�e�[�W4
800,//�X�e�[�W5
800,//�X�e�[�W6
800,//�X�e�[�W7
800	//�X�e�[�W8
};
//�@���𐶐����邩�ǂ���
static const bool isSpawnTapper[8]{
false,//1
false,//2
false,//3
true,//4
false,//5
false,//6
false,//7
false//8
};
//���𐶐�����܂ł̎���
static const int spawnBirfdTime[8]{
	300,//�X�e�[�W1
	300,//�X�e�[�W2
	300,//�X�e�[�W3
	300,//�X�e�[�W4
	300,//�X�e�[�W5
	300,//�X�e�[�W6
	300,//�X�e�[�W7
	300	//�X�e�[�W8
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
static const int frequencyHairBall = 10;
