#pragma once

///<summary>
/// �E�B���h�E���T�C�Y
///</summary>
static const int WINDOW_WIDTH = 1920;

///<summary>
/// �E�B���h�E�c�T�C�Y
///</summary>
static const int WINDOW_HEIGHT = 1080;

///<summary>
/// ��ʐF�̃r�b�g�[�x
///</summary>
static const int COLOR_BIT_DEPTH = 16;

///<summary>
/// ���t���b�V�����[�g
///</summary>
static const int REFRESH_RATE = 60;

///<summary>
/// �Q�[���^�C�g��
///</summary>
#define GAMENAME "�l����"
///<summary>
/// �f�t�H���g�̂a�f�l�{�����[��
///</summary>
static const float BGMVOLUME = 0.8f;

///<summary>
/// �f�t�H���g�̂r�d�{�����[��
///</summary>
static const float SEVOLUME = 0.8f;
#define MOVIE_FILE "�C��_PV.avi"
#define DEMO_MOVIE "�C��_PV"
//�f�o�b�O���[�h�Ȃ�1,�����[�X���[�h�Ȃ�2
#ifndef NDEBUG
#define BuildMode 1
#else
#define BuildMode 2
#endif