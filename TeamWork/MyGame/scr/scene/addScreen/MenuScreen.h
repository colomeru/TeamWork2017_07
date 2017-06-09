#pragma once
#include "../../Def.h"
#include "../../math/Vector2.h"
#include <array>
#include"../../stageGenerator/Stage.h"

class MenuScreen
{
public:
	//�X�e�[�W�p�l��
	struct PanelStruct
	{
		Vector2 position; //�p�l���̍��W
		bool isDraw; //�I���\��
		float alpha; //�p�l���̃A���t�@�l
		PanelStruct():
			position(Vector2::Zero),isDraw(false),alpha(0.0f){}
		PanelStruct(Vector2 position, bool isDraw = false, float alpha = 0.0f) :
			position(position), isDraw(isDraw), alpha(alpha) {}
	};

	//�R���X�g���N�^
	MenuScreen();
	//�f�X�g���N�^
	~MenuScreen();
	//�X�V
	void Update();
	//�`��
	void Draw() const;
	//
	void Action();
	//�O�̃X�e�[�W���N���A���Ă��邩�H
	bool CheckPreviousStage(int sNum);
	//���̃X�e�[�W�̉��
	void OpenNextStage(int sNum);
	//���̃X�e�[�W���������Ă��邩�H
	bool CheckNextStage(int sNum);
	//�p�^�[���P
	void Pattern1Update();
	void Pattern1Draw() const;
	//�p�^�[���Q
	void Pattern2Update();
	void Pattern2Draw() const;
	//"��"�����͂��ꂽ��
	bool IsInputUp() const;
	//"��"�����͂��ꂽ��
	bool IsInputDown() const;
	//"��/A"�̂����ꂩ�����͂��ꂽ��
	bool IsInputLeft() const;
	//"��/��/�E"�̂����ꂩ�����͂��ꂽ��
	bool IsInputAny() const;
	
	Stage GetGamePlayStage()const;
private:
	int stageNum = 0; //�X�e�[�W�ԍ�
	const float height = WINDOW_HEIGHT / 4.0f * 2.0f; //�p�l���P��y���W
	Vector2 backPos; //�߂�p�l���̍��W
	bool backSelect; //�߂��I�����Ă��邩�H
	Vector2 cursorPos; //�J�[�\���̍��W
	int panelNum, cursorNum; //�p�l���ԍ��A�J�[�\���ԍ�
	std::array<PanelStruct, 9> panel; //PanelStruct���X�e�[�W��������

	//
	Vector2 pos;
	Vector2 gPos;
	Vector2 dir; //�ړ�����
	float dis;
	float disN;
	Vector2 from;
	float ease;
	bool test;
	const float MoveTime = 0.5f;

	float timer_;
	Vector2 drawPos;
	Vector2 modify; //���ړ�����
	Vector2 moveDis; //�ړ�����
	Vector2 velocity; //���x
	float mag; //���x�{��

	std::array<Stage,9> stageList_;
};