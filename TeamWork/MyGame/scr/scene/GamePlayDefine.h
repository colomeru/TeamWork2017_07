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
1000,//�X�e�[�W1
600,//�X�e�[�W2
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
	25,//�X�e�[�W1
	25,//�X�e�[�W2
	25,//�X�e�[�W3
	25,//�X�e�[�W4
	25,//�X�e�[�W5
	25,//�X�e�[�W6
	25,//�X�e�[�W7
	25 //�X�e�[�W8
};

//�ыʂ����ł���m���i���j
static const int frequencyHairBall[8]{
	12,//�X�e�[�W1
	12,//�X�e�[�W2
	12,//�X�e�[�W3
	12,//�X�e�[�W4
	12,//�X�e�[�W5
	12,//�X�e�[�W6
	12,//�X�e�[�W7
	12 //�X�e�[�W8
};

//�ыʂ����ł���p�x�i�t���[�����j
static const int hairballCnt[8]{
	60,//�X�e�[�W1
	60,//�X�e�[�W2
	60,//�X�e�[�W3
	60,//�X�e�[�W4
	60,//�X�e�[�W5
	60,//�X�e�[�W6
	60,//�X�e�[�W7
	60 //�X�e�[�W8
};

//�Q�[���J�n���̑S�̎���
static const float startEase = 4.f;
//�{�^�������Ă�Ƃ��ɐi�߂鎞��
static const float startAddTime = 0.005f;
