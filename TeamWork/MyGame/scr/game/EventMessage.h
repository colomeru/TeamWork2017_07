#pragma once

enum class EventMessage
{
	BEGIN_WIND,			//���̐����n��
	STRONG_WIND,		//������������
	ATTENUATE_WIND,		//���̈З͂̌���
	END_WIND,			//�����I��
	START_LANE_CHANGE,
	GOAL_FLAG,			//�S�[���t���O
	GAME_CLEAR_FLAG,	//�X�e�[�W�I���t���O
	TAPPER_DEAD,	//�@�����S
	PLAY_NEXT_STAGE,
	ADD_SCORE,
	PLAYER_DEAD,
};