#pragma once
#include "../../Def.h"
#include "../../math/Vector2.h"
#include <array>
#include "../../stageGenerator/Stage.h"
#include "../../math/Vector3.h"
#include <vector>
#include "../../actor/Field/Enemys/EnemyCharas/CharacterAnmManager.h"

class MenuScreen
{
public:
	//�X�e�[�W�p�l��
	struct PanelStruct
	{
		Vector2 position; //�p�l���̍��W
		bool isDraw; //�I���\��
		float alpha; //�p�l���̃A���t�@�l
		PanelStruct() :
			position(Vector2::Zero), isDraw(false), alpha(0.0f) {}
		PanelStruct(Vector2 position, bool isDraw = false, float alpha = 0.0f) :
			position(position), isDraw(isDraw), alpha(alpha) {}
	};

	//��
	struct StarStruct
	{
		Vector2 position_;
		float isAlpha_;
		float timer_;
	};


	//�R���X�g���N�^
	MenuScreen();
	//�f�X�g���N�^
	~MenuScreen();
	void Init();
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
	//��
	void Star();
	//���ꐯ
	void ShootingStar();
	//�J���X
	void Crow();
	//SE
	void SE();


	Stage GetGamePlayStage()const;

//�O���N���X�p
public:
	bool GetIsBackSelect()const {
		return backSelect;
	}
	bool GetIsTutorialSelect()const {
		return stageNum == 0;
	}
	void InputSelectStage();
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
	int bgHandle; //�w�i�̃n���h��
	int builHandle; //�r���̃n���h��
	int wwwHandle; //���̃n���h��
	Vector2 bgPos_; //�w�i���W
	Vector2 builPos_; //�r�����W
	Vector2 wwwPos_; //�����W
	float betDis_; //�ԋ���

	//�w�i�F
	std::array<Vector3, 9> bgColor_;
	Vector3 color_;

	//��
	std::array<StarStruct, 50> star_;
	int starNum_;
	float alphaValue_;
	std::array<float, 3> starAlpha_;

	//���ꐯ
	std::array<StarStruct, 10> sStar_;
	int sStarNum_;
	std::array<float, 10> waitTime_;
	std::array<Vector2, 10> prevPos_;
	std::array<float, 10> ssAlpha_;

	//�J���X
	std::array<Vector2, 3> crowPos_;
	std::array<float, 3> interval_;
	std::array<float, 3> cTimer_;
	std::array<bool, 3> spriteTurn_;
	std::array<Vector2, 3> cVelocity_;
	std::array<Vector2, 3> cFrom_;
	std::array<float, 3> cDis_;

	CharacterAnmManager anmManager_;

	std::array<Stage, 9> stageList_;
};