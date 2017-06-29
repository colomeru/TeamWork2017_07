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
	700,//�X�e�[�W2
	500,//�X�e�[�W3
	400,//�X�e�[�W4
	450,//�X�e�[�W5
	500,//�X�e�[�W6
	400,//�X�e�[�W7
	200	//�X�e�[�W8
};
//�@���𐶐����邩�ǂ���
static const bool isSpawnTapper[8]{
	false,//1
	false,//2
	true,//3
	true,//4
	false,//5
	true,//6
	true,//7
	true//8
};
//���𐶐�����܂ł̎���
static const int spawnBirfdTime[8]{
	500,//�X�e�[�W1
	400,//�X�e�[�W2
	400,//�X�e�[�W3
	400,//�X�e�[�W4
	300,//�X�e�[�W5
	200,//�X�e�[�W6
	250,//�X�e�[�W7
	200	//�X�e�[�W8
};

//��ʓ��̕����h���m���i���j
static const int frequencyWind[8]{
	10,//�X�e�[�W1
	15,//�X�e�[�W2
	20,//�X�e�[�W3
	30,//�X�e�[�W4
	30,//�X�e�[�W5
	30,//�X�e�[�W6
	30,//�X�e�[�W7
	40 //�X�e�[�W8
};

//�ыʂ����ł���m���i���j
static const int frequencyHairBall[8]{
	0,//�X�e�[�W1
	10,//�X�e�[�W2
	15,//�X�e�[�W3
	20,//�X�e�[�W4
	20,//�X�e�[�W5
	50,//�X�e�[�W6
	100,//�X�e�[�W7
	100 //�X�e�[�W8
};

//�ыʂ����ł���p�x�i�t���[�����j
static const int hairballCnt[8]{
	100,//�X�e�[�W1
	70,//�X�e�[�W2
	60,//�X�e�[�W3
	60,//�X�e�[�W4
	60,//�X�e�[�W5
	70,//�X�e�[�W6
	60,//�X�e�[�W7
	40 //�X�e�[�W8
};

//�Q�[���J�n���̑S�̎���
static const float startEase = 4.f;
//�{�^�������Ă�Ƃ��ɐi�߂鎞��
static const float startAddTime = 0.005f;
